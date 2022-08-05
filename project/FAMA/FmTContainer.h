//---------------------------------------------------------------------------

#ifndef __FMTContainer_H__
#define __FMTContainer_H__
//---------------------------------------------------------------------------
#include <Classes.hpp>

#include "util.h"
#include "FmAID.h"
#include "FmWhitePage.h"
#include "FmYellowPage.h"

//---------------------------------------------------------------------------

/**
 * Classe responsavel por oferecer os diversos serviços da plataforma
 * multiagentes FAMA. A maioria das suas funcionalidades eh de uso interno
 * das classes que formam o framework, porem esta classe tambem oferece
 * alguns serviços para os desenvolvedores de agentes, tais como: paginas
 * branca, paginas amarelas. Outros serviços podem ser acessados atraves
 * da classe FmAgent (envio e recebimento de mensagens, metodos de alteraçao
 * do status do agente, etc.).
 */
class FmTContainer : public TThread
{
private:
	std::map<AID,FmAgent*> aidAgents;
    Vector_pAgent agents;
	FmWhitePage whitePage;
	FmYellowPage yellowPage;

    int nextID;      // next agent identifier
    int nextIndex;   // next index of the vector 'agents' to be executed

    bool executedLastAgent;

    void __fastcall runNextAgent();
    bool __fastcall runAgent(int indexAgent);

    void removeAgent(int indexAgent);

protected:

    /* Construtor. Cria o container e inicia a sua execucao (multi-threaded)
     * imediatamente.
     */
    __fastcall FmTContainer();

    /* Implementation of TThread function.
     */
    void __fastcall Execute();

    /* Cleanup function.
     */
    void __fastcall cleanup(TObject* Sender);

public:

	/* Retorna a instancia de Container. E importante que seja chamado
     * o metodo 'deleteContainer' quando o container nao estiver mais sendo
     * utilizado, ou seja, quando os agentes tiverem encerrado ou quando
     * os resultados esperados ja tiverem sido alcancados.
	 */
	static FmTContainer* getInstance();

    /* Encerra e desaloca a Thread. Chama o metodo shutdown para encerrar
     * todos os agentes. Esse metodo deve ser chamado de dentro da thread
     * VCL principal (de dentro de uma rotina de tratamento de evento de um
     * componente grafico, por exemplo).
     */
    static void deleteInstance();

	/* Adiciona um agente ao container. Automaticamente o agente eh
     * registrado no serviço de paginas brancas.
     * Retorna 'true' se o agente foi registrado com sucesso e
     * 'false', caso o agente já esteja cadastrado.
	 */
	bool addAgent(FmAgent *a, std::string name);

    void setStatus(FmAgent *a, EFmAgentStatus st);
    void setName(FmAgent *a, std::string name);

	/* Realiza busca pelo ID no serviço de paginas brancas.
	 */
	AID searchAgent(int id);

    /* Indica se o container tem um agente com o dado nome.
     */
    bool hasAgentWithName(const char *);

	/* Realiza busca pelo nome no serviço de paginas brancas.
	 */
	AID searchAgent(const std::string &name);

	/* Adiciona um serviço de um agente nas pagina amarelas.
	 */
	void addService(FmAgent* agent, FmServiceDescription& sd);

	/* Remove um serviço de um agente das pagina amarelas.
     * {deallocation of parameter}: caller
	 */
	void deleteService(FmAgent* agent, FmServiceDescription& sd);

	/* Retorna os AID dos agentes que oferecem o serviço especificado.
     * Retorna NULL se não houver servico com a descrição dada.
     * {deallocation of parameter}: caller
	 */
	void searchService(const FmServiceDescription& sd, Vector_AID& result);

    /* Envia uma mensagem para um agente do container.
     * {deallocation of parameter}: caller
     */
   	void sendMessage(FmAclMessage *message);


private:

    /* Classe para efetuar a desalocação automática do container.
     * Necessária porque threads têm que ser alocadas dinamicamente e
     * o container eh um singleton (tem uma unica instancia global).
     */
	class SmartCleaner {
	private:
		FmTContainer* object;
	public:
		SmartCleaner(FmTContainer* pObject = NULL) : object( pObject ) { }
		virtual ~SmartCleaner() {
			deleteInstance();
        }
        void setObject(FmTContainer* pObject) { this->object = pObject; }
        FmTContainer* getObject() { return object; }
    };

	static SmartCleaner smartCleaner;

};

//---------------------------------------------------------------------------
#endif
