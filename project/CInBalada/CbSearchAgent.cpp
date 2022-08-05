
#include "CbSearchAgent.h"

#include "CbOutputterAgent.h"
#include "Parameters.h"

#include "FAMA\FmMessageTemplate.h"

static ofstream logFile("logs\\CbSearchAgent.txt", ios::out);


//.......................//
// CLASSE CbSearchAgent
//.......................//

CbSearchAgent::CbSearchAgent(string name, CbAgentRepertoire *rep)
    : CbAbstractAgent(name, rep)
{
    this->setLogStream(&logFile, false);
    this->next = NULL;
    this->isFirst = false;
}

CbSearchAgent::~CbSearchAgent()
{
	// nada a deletar...
}

void CbSearchAgent::setup() {
    addBehaviour(new JoinerBehaviour(this));
}

bool CbSearchAgent::escolherProxFrase(std::string &phrasesStr) {
    bool achou = false;
    TiXmlDocument doc;
    TiXmlElement *phraseEl;
    std::vector<CbRhythmPattern*> phrases;
    FmMidiFileTrack *track;

    log() << "Escolhendo a melhor frase para casar com: " << phrasesStr;

    doc.Parse(phrasesStr.c_str());
    phraseEl = doc.FirstChildElement(XML_NODE_PHRASE);

    while (phraseEl != NULL) {
    	track = readXmlPhrase(*phraseEl);
        phrases.push_back(new CbRhythmPattern(*track, CbMeter::SIMPLE_QUADR));
        delete track;
        phraseEl = phraseEl->NextSiblingElement(XML_NODE_PHRASE);
    }

    //compara os padroes do agente com os padroes recebidos
    const CbRhythmPattern *atual;
    int currPtrn = repertoire->getIteratorCurrPattern();
    int aval;

    while (!achou && currPtrn != -1) {
        atual = repertoire->getRhythmPattern(currPtrn);
        log() << "Comparacoes do (meu) padrao [" << currPtrn << "]: ";
        for (int j = 0; j < phrases.size(); j ++) {
            aval = phrases[j]->geberFunction(atual);
            log() << "  - c/ padrao do agente <" << j << "> - " << aval;
            achou = aval < CB_PAR_MAX_GEBER_FUNC;
            if (achou == false) {
                break;
            }
        }
        if (!achou) {
	        currPtrn = repertoire->nextPattern();
        }
    }

    if (achou) {
        log() << "Frase encontrada: <" << currPtrn << ">";
    }

    for (int i = 0; i < phrases.size(); i ++) {
        delete phrases[i];
    }

    return achou;
}


//.......................................//
// CLASSE CbSearchAgent::JoinerBehaviour
//.......................................//

CbSearchAgent::JoinerBehaviour::JoinerBehaviour(CbSearchAgent *a) {
    agent = a;
    estado = WANNA_PLAY_0;
}

void CbSearchAgent::JoinerBehaviour::action() {
    FmAclMessage *rec;
    FmAclMessage *snd;
    Vector_AID banda;
    int currPhrase;

    switch (estado) {
    case WANNA_PLAY_0:
        agent->getBandIds(banda);
		agent->repertoire->resetPatternIterator();  // tem q ter ao menos um padrao !!!
        if (banda.size() == 0) {
            log() << "Ninguem tocando...";
			agent->setCurrRhythmPattern(agent->repertoire->getIteratorCurrPattern());
            agent->isFirst = true;
            agent->addBehaviour(new PlayerBehaviour(agent));
            estado = QUIT;
        } else {
            log() << "ENVIANDO: REQUEST - join band";
            snd = new FmAclMessage(FP_REQUEST);
            snd->setContent(MSG_REQ_JOINBAND);
            agent->sendBanda(snd);
            delete snd;
            estado = SHOWING_BEAT_0;
        }
        break;

    case WANNA_PLAY_1:
        rec = agent->receive();
        //se chegar a autorizacao para mostrar batida...
        if (rec != NULL) {
            log() << "Recebido: " << rec->getPerformativeName() << " - " << rec->getContent();
            if (rec->getPerformative() == FP_AGREE) {
                estado = SHOWING_BEAT_0;
            } else {
                throw EXCP_UNEXPECTED_MSG;
            }
            delete rec;
        }
        break;

    case SHOWING_BEAT_0:
        log() << "ENVIANDO: PROPOSE - beat " << agent->repertoire->getIteratorCurrPattern();
        currPhrase = agent->repertoire->getIteratorCurrPattern();
        snd = agent->createMsgXmlPhrase(agent->repertoire->getRhythmPattern(currPhrase));
        snd->setPerformative(FP_PROPOSE);
        agent->sendBanda(snd);
        delete snd;
        estado = SHOWING_BEAT_1;
        break;

    case SHOWING_BEAT_1:
        rec = agent->receive();
        if (rec != NULL) {
            log() << "Recebido: " << rec->getPerformativeName();

            //se a batida nao for aceita...
            if (rec->getPerformative() == FP_REJECT_PROPOSAL) {

                //se houver mais padroes, volta pro estado SHOWING_BEAT_0
                if (agent->repertoire->hasNextPattern()) {
                    log() << "Ainda tem batidas para enviar...";
                    agent->repertoire->nextPattern();
                    estado = SHOWING_BEAT_0;
                //senão, encerra o behaviour e mata o agente
                } else {
                    log() << "ENVIANDO: CANCEL (nao tem mais padroes)";
                    snd = new FmAclMessage(FP_CANCEL);
                    snd->setContent(MSG_CAN_NOMOREPATTERNS);
                    agent->sendBanda(snd);
                    agent->doDelete();
                    delete snd;
                    estado = QUIT;
                }
            }
            //se a batida for aceita, encerra esse behaviour (QUIT) e adiciona um PlayerBehaviour
            else if (rec->getPerformative() == FP_ACCEPT_PROPOSAL) {
				agent->setCurrRhythmPattern(agent->repertoire->getIteratorCurrPattern());
                agent->addBehaviour(new PlayerBehaviour(agent));
                estado = QUIT;
            }

            delete rec;
        }
        break;

    case QUIT:
        //faz nada
        break;
    }
}

bool CbSearchAgent::JoinerBehaviour::done() {
    if (estado == QUIT) log() << "Fim do Behaviour...";
    return (estado == QUIT);
}

ofstream& CbSearchAgent::JoinerBehaviour::log(bool prompt) {
	ofstream* ofs = agent->getLogStream(prompt);
    if (prompt) {
        (*ofs) << "{JOINER, " << estado << "} - ";
    }
    return (*ofs);
}


//.......................................//
// CLASSE CbSearchAgent::PlayerBehaviour
//.......................................//

CbSearchAgent::PlayerBehaviour::PlayerBehaviour(CbSearchAgent *a) {
    agent = a;
    prev = NULL; //so sera inicializado e usado durante uma rodada negociacao

    templNotOutputter = FmMessageTemplate::matchSender(agent->outputter);
    templNotOutputter = FmMessageTemplate::not(templNotOutputter);

    estado = SETUP;
}

CbSearchAgent::PlayerBehaviour::~PlayerBehaviour() {
    delete templNotOutputter;
}

void CbSearchAgent::PlayerBehaviour::onStart() {
    //cadastra na banda
    agent->registerInBand();

    //cadastra no outputter
	agent->registerInOutputter();
}

void CbSearchAgent::PlayerBehaviour::action() {
    FmAclMessage *rec, *snd;

    switch (estado) {
    case SETUP:
        setupState();
        break;

    case PLAYING:
        playingState();
        break;

    case RECEIVE_BEAT:
        receiveBeatState();
        break;

    case ANALYSING_REQ_0:
        analysingReq0State();
        break;

    case ANALYSING_REQ_1:
        analysingReq1State();
        break;

    }

}

// SETUP
// espera aceitacao do condutor
void CbSearchAgent::PlayerBehaviour::setupState() {
    FmAclMessage *rec = agent->receive();

    if (rec!= NULL) {
        if (rec->getPerformative() == FP_AGREE) {
            estado = PLAYING;
        } else {
        	log() << "Erro!!! Mensagem recebida: " << *rec;
            throw EXCP_UNEXPECTED_MSG;
        }
        delete rec;
    }

}

// PLAYING
// o agente toca repetidamente a batida escolhida
void CbSearchAgent::PlayerBehaviour::playingState() {
    FmAclMessage *rec, *snd;

    log() << "Tocando...";
    rec = agent->receive();   //pode ser do outputter

    if (rec == NULL) {
        block();
        return;
    }

    log() << "Recebido: " << rec->getPerformativeName() << " - " << rec->getContent();

    //mensagem de request pra entrar na banda,
    if (rec->getContent() == MSG_REQ_JOINBAND) {
        if (agent->first()) {
            log() << "ENVIANDO: AGREE - show beat";
            snd = rec->createReply();
            snd->setPerformative(FP_AGREE);
            snd->setContent(MSG_AGR_SHOWBEAT);
            agent->send(*snd);
            delete snd;
        }
        this->novato = rec->getSender();
        estado = RECEIVE_BEAT;
    }
    //mensagem do agente condutor autorizando mandar frases
    else if (rec->getContent() == MSG_REQ_SEND_NEXTPHRASE) {
        // envia a batida atual
        log() << "Enviando frase atual...";
        int cb = agent->getCurrRhythmPattern();
        snd = agent->createMsgXmlPhrase(agent->repertoire->getRhythmPattern(cb));
        snd->setPerformative(FP_INFORM);
        snd->addReceiver(agent->outputter);
        agent->send(*snd);
        delete snd;
    }
    else if (rec->getContent() == MSG_INF_EXITING) {
    	log() << "Recebida notificação de saida do agente...";
		if (*(agent->next) == *(rec->getSender())) {
        	agent->next = NULL;
        }
    }
    else {
        throw EXCP_UNEXPECTED_MSG;
    }

    delete rec;

}

// RECEIVE_BEAT
// o primeiro agente da banda espera uma batida do novato para ser analisada;
// os demais agentes esperam pepor mensagens dos agentes anteriores a ele, que
// enviarão suas batidas a medida que forem selecionando padrões que casem com a banda
void CbSearchAgent::PlayerBehaviour::receiveBeatState() {
    FmAclMessage *rec = agent->receive(*templNotOutputter);

    if (rec == NULL) {
        return;
    }

    // espera chegar uma batida (do novato ou de outro musico da banda),
    // se chegar vai pro proximo estado
    if (rec->getPerformative() == FP_PROPOSE) {
        agent->repertoire->resetPatternIterator(); //prepara para analisar todo o
										           //repertorio a partir do padrao atual
        if (rec->getSender() == novato) {
            if (agent->first()) {
                log() << "Recebido: PROPOSE (novato)";
                prev = novato;  //endereco do novato
                this->previousPhrases = rec->getContent();
                estado = ANALYSING_REQ_0;
            }
            //else: ignora
        }
        else {
            log() << "Recebido: PROPOSE - " << rec->getContent();
            prev = rec->getSender(); //endereco do agente anterior na banda
            this->previousPhrases = rec->getContent();
            estado = ANALYSING_REQ_0;
        }

    }
    //se o novato desistir, volta a tocar
    else if (rec->getPerformative() == FP_CANCEL) {
        log() << "Recebido: CANCEL >> Permanece o antigo padrao...";
        estado = PLAYING;
    }

    delete rec;
}

// ANALYSING_REQ0
// negociacao interna, escolhe frase e manda
// nesse estado, as analises das batidas estão sendo encaminhadas pra frente
void CbSearchAgent::PlayerBehaviour::analysingReq0State() {
    FmAclMessage *snd;
    bool foundBeat;

    log() << "Escolhendo uma batida...";
    foundBeat = agent->escolherProxFrase(previousPhrases);

    if (foundBeat) {
        if (agent->last()) {
            //envia para o anterior (que, por sua vez, estará no estado ANALYSING_REQ_1)
            log() << "ENVIANDO: ACCEPT-PROPOSAL >> Novato aceito !!!";
            agent->next = this->novato;
            agent->setCurrRhythmPattern(agent->repertoire->getIteratorCurrPattern());
            snd = new FmAclMessage(FP_ACCEPT_PROPOSAL);
            snd->addReceiver(prev);
            agent->send(*snd);
            delete snd;
            estado = PLAYING;
        } else {
            //envia todos os padroes escolhidos ate o momento
            //para o proximo agente da cadeia (que estara no estado RECEIVE_BEAT)
            log() << "ENVIANDO: PROPOSE (internal)";
            int crp = agent->repertoire->getIteratorCurrPattern();
            snd = agent->createMsgXmlPhrase(agent->repertoire->getRhythmPattern(crp));
            snd->getContent() += previousPhrases;
            snd->setPerformative(FP_PROPOSE);
            snd->addReceiver(agent->next);
            agent->send(*snd);
            delete snd;
            estado = ANALYSING_REQ_1;
        }
    } else {
        //envia um REJECT para o anterior (que estará no estado ANALYSING_REQ1)
        //o restante (os seguintes) ainda estão no estado RECEIVE_BEAT
        log() << "Nao encontrou >> ENVIANDO: REJECT-PROPOSAL";
        snd = new FmAclMessage(FP_REJECT_PROPOSAL);
        snd->addReceiver(prev);
        agent->send(*snd);
        delete snd;
        estado = RECEIVE_BEAT;
    }

    return;
}

// ANALYSING_REQ_1
// ja escolheu uma batida e agora espera resposta da banda
// nesse estado, o feedback é sucessivamente repassado (para os agentes anteriores)
void CbSearchAgent::PlayerBehaviour::analysingReq1State() {
    FmAclMessage *rec = agent->receive(*templNotOutputter);
    FmAclMessage *snd;

    if (rec == NULL) {
        return;
    }

    log() << "Recebido: " << rec->getPerformativeName();
    if (rec->getPerformative() == FP_ACCEPT_PROPOSAL) {
        agent->setCurrRhythmPattern(agent->repertoire->getIteratorCurrPattern());
        log() << "REPASSANDO: ACCEPT-PROPOSAL";
        snd = new FmAclMessage(FP_ACCEPT_PROPOSAL);
        snd->addReceiver(prev);
        agent->send(*snd);
        delete snd;
        estado = PLAYING;
    } else if (rec->getPerformative() == FP_REJECT_PROPOSAL) {
        agent->repertoire->nextPattern();
        //volta para tentar encontrar outra batida
        estado = ANALYSING_REQ_0;
    }

    delete rec;
}

bool CbSearchAgent::PlayerBehaviour::done() {
    return false;
}

void CbSearchAgent::PlayerBehaviour::onEnd() {
    log() << "PlayerBehaviour encerrado => descadastrando da banda...";

    agent->unregisterFromOutputter();
    agent->registerInBand();

    FmAclMessage acl (FP_INFORM);
    acl.setContent(MSG_INF_EXITING);
    agent->sendBanda(&acl);
}

ofstream& CbSearchAgent::PlayerBehaviour::log(bool prompt) {
	ofstream* ofs = agent->getLogStream(prompt);
    if (prompt) {
        (*ofs) << "{PLAYER, " << estado << "} - ";
    }
    return *ofs;
}

