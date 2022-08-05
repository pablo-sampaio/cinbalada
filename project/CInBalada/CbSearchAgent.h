
#ifndef __CBSEARCHAGENT_H__
#define __CBSEARCHAGENT_H__

#include "CbAbstractAgent.h"

#define MSG_REQ_JOINBAND       "join band"
#define MSG_CAN_NOMOREPATTERNS "no more patterns"
#define MSG_AGR_SHOWBEAT       "show beat"
#define MSG_INF_EXITING        "exiting"


/**
 * Esta classe representa um musico capaz de tocar algumas
 * frases num instrumento musical (de percussão).
 * Esse agente se cadastra no container com o seguinte servico
 *    - tipo: "Musician Service"
 *    - nome: "percussionist"
 */
class CbSearchAgent : public CbAbstractAgent
{
private:

    AID next;
    bool isFirst;

    /* Metodos que indicam a posição (ordem) do agente na banda.
     */
    bool first() { return isFirst; }
    bool last()  { return next == NULL; }

    /* Parte da posição atual setada no iterador do repertorio e busca encontrar
     * a primeira frase que case com todas as frases passadas em formato XML
     * pelo parametro 'phrases'. Retorna booleano para indicar se conseguiu achar
     * uma frase.
     * Atencao: antes de chamar esse metodo o iterator do repertorio deve ja
     * ter sido iniciado. Ao sair desse metodo, o iterator estara na posicao
     * da primeira frase valida encontrada, se o metodo tiver retornado 'true'.
     */
    bool escolherProxFrase(std::string &phrases);

public:

	CbSearchAgent(std::string name, CbAgentRepertoire *rep);
	virtual ~CbSearchAgent();

    /* Implementação do metodo da superclasse. Inicia o agente com
     * o behaviour JoinerBehaviour.
	 */
	void setup();


    /**
     * Este é o Behaviour que implementa o comportamento de um agente que
     * deseja entrar na banda. Se conseguir, o JoinerBehaviour é trocado por
     * um PlayerBehaviour.
     */
    class JoinerBehaviour : public FmBehaviour {
	private:
        CbSearchAgent *agent;
        typedef enum {WANNA_PLAY_0, WANNA_PLAY_1, SHOWING_BEAT_0, SHOWING_BEAT_1, QUIT} STATE;
        STATE estado;
	public:
		JoinerBehaviour(CbSearchAgent *a);
		void action();
		bool done();
        std::ofstream& log(bool prompt=true);
    };


    /**
     * Este é o Behaviour de um agente que já foi aceito na banda.
     * Ele analisa requisicoes de outros agentes que desejem entrar na banda.
     */
    class PlayerBehaviour : public FmBehaviour {
	private:
        CbSearchAgent *agent;
        typedef enum {SETUP, PLAYING, RECEIVE_BEAT, ANALYSING_REQ_0,
        				 ANALYSING_REQ_1, UNJOINING_BAND, EXIT} STATE;

        STATE estado;
        AID prev;       //o agente anterior, durante negociação (pode ser o novato)
        AID novato;
        FmMessageTemplate *templNotOutputter;
        std::string previousPhrases;  // frases dos agentes "anteriores"

	public:
		PlayerBehaviour(CbSearchAgent *a);
		virtual ~PlayerBehaviour();

        void onStart();
		void action();

        void setupState();
        void playingState();
        void receiveBeatState();
        void analysingReq0State();
        void analysingReq1State();

		bool done();
		void onEnd();

        std::ofstream& log(bool prompt=true);
    };

    friend class JoinerBehaviour;
    friend class PlayerBehaviour;

};


#endif
