

#ifndef __FMMESSAGEQUEUE_H__
#define __FMMESSAGEQUEUE_H__

#include "util.h"
#include "FmAclMessage.h"
#include "FmAgent.h"
#include "FmMessageTemplate.h"


/**
 * Fila de mensagens associada a cada agente. Esta classe poder ser
 * considerada uma classe interna da biblioteca e não deve ser usada
 * diretamente pelos desenvolvedores de agentes. Desenvolvedores devem
 * usar os metodos 'send' e 'receive' da classe FmAgent.
 */
class FmMessageQueue {

private:
	Vector_pAclMessage msgBox;
	FmAgent *myAgent;

public:
	FmMessageQueue();
	virtual ~FmMessageQueue();

	/* Inclui uma mensagem.
	 */
	void includeMessage(FmAclMessage *msg);

    /* Number of 'unread' messages queued.
     */
    int getNumMessages();

	/* Recebe a proxima mensagem
	 */
	FmAclMessage* getNextMessage();

	/* Recebe a proxima mensagem de acordo o template especificado
	 */
	FmAclMessage* getNextMessage(const FmMessageTemplate &mt);
};

#endif