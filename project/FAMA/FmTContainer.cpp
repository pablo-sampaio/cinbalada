//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FmTContainer.h"

#include "FmAgent.h"
#include "FmAclMessage.h"
#include "FmBehaviour.h"
#include "FmMessageQueue.h"
#include "FmServiceDescription.h"

// mudar aqui para habilitar/desabilitar log
#define LOGGING

// definicao do arquivo de LOG
#ifdef LOGGING
	#include <fstream>
	static std::ofstream logFile("logs\\FmTContainer.txt", std::ios::out);
	#define LOG logFile
#else
	#define LOG cout
#endif


//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;

FmTContainer::SmartCleaner FmTContainer::smartCleaner;

//---------------------------------------------------------------------------
__fastcall FmTContainer::FmTContainer()
    : TThread(false)
{
    LOG << "Container criado" << endl;
	this->FreeOnTerminate = true;

	this->nextIndex = 0;
	this->nextID = 0;
}

//---------------------------------------------------------------------------
FmTContainer* FmTContainer::getInstance() {
	if (smartCleaner.getObject() == NULL) {
		smartCleaner.setObject(new FmTContainer());
	}
	return smartCleaner.getObject();
}

//---------------------------------------------------------------------------
void FmTContainer::deleteInstance() {
	FmTContainer* theContainer = smartCleaner.getObject();
	if (theContainer != NULL) {
		theContainer->Terminate();

		// deveria ser chamado automaticamente pela classe TThread apos chamada a Terminate
		// estou chamando diretamente pq o evento OnTerminate nao tem sido chamado
		// quando a chamada a 'Terminate()' acontece no encerramento do programa
		// obs: nao tem problema se acontecer de 'cleanup' ser chamado novamente
		theContainer->cleanup(NULL);

		smartCleaner.setObject(NULL);

		// obs: a desalocação da instancia fica a cargo da class TThread (FreeOnTerminate eh true)
		//delete theContainer
	}

}

//---------------------------------------------------------------------------
void __fastcall FmTContainer::cleanup(TObject* Sender) {
	for (int i = agents.size() - 1; i >= 0; i --) {
		agents[i]->setStatus(AG_TO_DELETE);
		this->removeAgent(i);
	}

	// deleta os AIDs dos agentes
	map<AID,FmAgent*>::iterator it;
	for (it = aidAgents.begin(); it != aidAgents.end(); it ++) {
		delete it->first;
	}
	aidAgents.clear();

	LOG << "Container destruido!" << endl;
}

//---------------------------------------------------------------------------
bool FmTContainer::addAgent(FmAgent *ag, string name) {

	// o AID de um novo agente será sempre NULL
	if (ag->getID() != NULL) {
        LOG << "ERRO: agente nao pode ser adicionado !" << endl;
		return false;
	}
	else {
        FmAID* aid = new FmAID(this->nextID);
        this->nextID ++;

        aid->setStatus(AG_INITIATED);
        if (name != "") {
            aid->setName(name);
        } else {
            aid->setName("agent" + nextID);
        }
        ag->setID(aid);

		this->aidAgents[aid] = ag;
        this->agents.push_back(ag);
		whitePage.addAID(aid);

        LOG << "Agente adicionado: " << aid->getName() << endl;
		return true;
	}
}

//---------------------------------------------------------------------------
void FmTContainer::setStatus(FmAgent *a, EFmAgentStatus st) {
	FmAID* aid = const_cast<FmAID*>(a->getID());

	if (aid->getStatus() != AG_DELETED) {
		aid->setStatus(st);
	}
}

//---------------------------------------------------------------------------
void FmTContainer::setName(FmAgent *a, std::string name) {
    FmAID* aid = const_cast<FmAID*>(a->getID());
    if (name != "") {
        aid->setName(name);
    } else {
        aid->setName("agent" + nextID);
    }
}

//---------------------------------------------------------------------------
// Esse eh o metodo principal da thread, que fica executando os
// behaviours dos agentes.
void __fastcall FmTContainer::Execute()
{
    LOG << "Container entrando em execucao..." << endl;

    executedLastAgent = true;

	while (true) {

        if (this->Terminated == true) {
            LOG << "Container serah finalizado..." << endl;
            break;
        }

        // so reagenda apos executar o agendamento anterior...
        if (executedLastAgent == true) {
            executedLastAgent = false;
            Synchronize(runNextAgent);
        }

        SwitchToThread();
	}
}

//---------------------------------------------------------------------------
void __fastcall FmTContainer::runNextAgent() {

    // caso esse metodo tenha ficado agendado antes da
    // chamada ao metodo Terminate (de TThread)
    if (!Terminated) {

        if (this->agents.size() > 0) {
            bool deleted = runAgent(nextIndex);

            if (deleted) {
                if (this->agents.size() != 0) {
                    this->nextIndex = nextIndex % agents.size();
                }
            } else {
                this->nextIndex = (nextIndex + 1) % agents.size();
            }
        }

        executedLastAgent = true;
    }

}

//---------------------------------------------------------------------------
bool __fastcall FmTContainer::runAgent(int indexAgent) {
    FmAgent *ag = agents[indexAgent];
	FmAID *aid = const_cast<FmAID*>(ag->getID());
	FmBehaviour *b;
	bool executed = false;
    bool deleted = false;

    ag->resetBehaviourIterator();
    //Sera q toda vez vai executar o primeiro behaviour ?
    // => Nao ! Porque o indice do ultimo behaviour acessado é mantido
    //    e a iteraçao começa do indice seguinte.

	if ((aid->getStatus() == AG_TO_DELETE)) {
		//LOG << "Removendo o agente \"" << aid->getName() << "\"..." << endl;
		this->removeAgent(indexAgent);
		deleted = true;

	} else if (aid->getStatus() == AG_INITIATED) {
		//LOG << "Agente \"" << aid->getName() << "\" iniciado !" << endl;
		ag->setup();

		if (ag->createSuspended()) {
			ag->doSuspend();
		} else {
			ag->doActivate();
		}

	} else if (aid->getStatus() == AG_WAITING) {
		//LOG << "Agente \"" << aid->getName() << "\" em espera..." << endl;
		if (ag->msgQueue->getNumMessages() > 0) {
			ag->doWake();
		}

	} else if (aid->getStatus() == AG_ACTIVE && ! ag->hasNextBehaviour()) {
		//LOG << "Agente \"" << aid->getName() << "\" sem behaviours..." << endl;
		ag->doDelete();

	} else if (aid->getStatus() == AG_ACTIVE) {

		while (ag->hasNextBehaviour() && !executed) {
			b = ag->getNextBehaviour();

			switch (b->getStatus()) {
			case BHV_NEW:
				b->setStatus(BHV_ACTIVE); //tem que ser antes do 'onStart' !
				b->onStart();
				executed = true;
				break;

			case BHV_ACTIVE:
				//Executa ação correspondente ao behaviour
				b->action();
				executed = true;

				//Se o behaviour tiver concluido (done() = true), remove-o
				if (b->done()) {
					//da proxima vez que for executado,
					//o behaviour sera removido
					b->setStatus(BHV_DELETED);
				}
				break;

			case BHV_DELETED:
				b->onEnd();
				executed = true;
				ag->removeBehaviour(b);
				break;

			case BHV_BLOCKED:
				break;
			}

		} //fim while

		// se nenhum behaviour tiver sido executado
		// o agente ficará esperando a chegada de uma mensagem
		if (! executed) {
			ag->doWait();
		}

	} //fim if

	return deleted;
}

//---------------------------------------------------------------------------
void FmTContainer::removeAgent(int indexAgent) {
	FmAgent *ag = agents[indexAgent];
	FmBehaviour *b;

    ag->resetBehaviourIterator();

    // remove os behaviours
	while (ag->hasNextBehaviour()) {
		b = ag->getNextBehaviour();
        if (b->getStatus() != BHV_NEW) {
	        b->setStatus(BHV_DELETED);
			b->onEnd();
        }
    }

	//chama a rotina de execucao final do agente (definida pelo usuario)
	ag->takeDown();

	//descadastra das paginas brancas/amarelas
	whitePage.deleteAID(ag->getID()->getID());
	yellowPage.deleteAgentServices(ag->getID());

	//deleta o agente do container (sem desalocar)
	//this->aidAgents.erase(aidAgents.find(aid)); // o AID não eh mais deletado !
	this->agents.erase(agents.begin() + indexAgent);

	ag->setStatus(AG_DELETED);

	if (ag->autoDelete() == true) {
		LOG << "Desalocando o agente..." << endl;
		//desaloca o agente (obs: o AID nao eh deletado)
		delete ag;
	} else {
		//desalocação fica a cargo do usuário
	}

}

//---------------------------------------------------------------------------
void FmTContainer::sendMessage(FmAclMessage *message) {
	int numReceivers = message->getNumReceivers();
    FmAID *rec;

    map<AID,FmAgent*>::iterator it;

	for (int i = 0; i < numReceivers; i++) {
        rec = const_cast<FmAID*>(message->getReceiver(i));
        it = aidAgents.find(rec);

        if (it != aidAgents.end()) {
            if (it->first->status != AG_DELETED) {
                this->aidAgents[rec]->includeMessage(new FmAclMessage(*message));
            } else {
                LOG << "Agente já foi deletado: " << rec << endl;
            }
        } else {
            LOG << "Endereco receiver invalido: " << rec << endl;
        }
	}

    LOG << (*message);
}

//---------------------------------------------------------------------------
AID FmTContainer::searchAgent(int id) {
	return whitePage.search(id);
}

//---------------------------------------------------------------------------
AID FmTContainer::searchAgent(const string &name) {
	return whitePage.search(name);
}

//---------------------------------------------------------------------------
bool FmTContainer::hasAgentWithName(const char *name) {
	string nm = name;
	return searchAgent(nm) != NULL;
}

//---------------------------------------------------------------------------
void FmTContainer::addService(FmAgent* agent, FmServiceDescription &sd) {
    sd.setAID(agent->getID());
   	yellowPage.addService(sd);
}

//---------------------------------------------------------------------------
void FmTContainer::searchService(const FmServiceDescription &sd, Vector_AID &result) {
    result.clear();

    if (sd.getName() != "" && sd.getType() == "") {
        return yellowPage.searchByServiceName(sd.getName(), result);
    } else if (sd.getName() == "" && sd.getType() != "") {
        return yellowPage.searchByServiceType(sd.getType(), result);
    } else if (sd.getName() != "" && sd.getType() != "") {
        yellowPage.searchByServiceNameAndType(sd.getName(), sd.getType(), result);
    }
}

//---------------------------------------------------------------------------
void FmTContainer::deleteService(FmAgent* agent, FmServiceDescription &sd) {
    sd.setAID(agent->getID());
	yellowPage.deleteService(sd);
}

