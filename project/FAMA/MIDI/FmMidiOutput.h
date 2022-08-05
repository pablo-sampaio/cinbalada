/**
 * D'accord Music Software
 * http://www.daccord.com.br
 *
 * D'accord Viol�o
 *
 * Nova vers�o por Pablo A. Sampaio
 */
#ifndef __FM_MIDIOUTPUT_H__
#define __FM_MIDIOUTPUT_H__

#include "MaximumMidi\maxmidi.h"

/**
 * This class encpasulates a MIDI output.
 */
class FmMidiOutput {
public:

    /* Inicializa o MaximimMIDI com o driver indicado.
	 * {deallocation of parameters}: done by this class
     */
	FmMidiOutput(int driver);

	/* Inicializa o MaximimMIDI com o driver padr�o.
	 * {deallocation of parameters}: done by this class
	 */
	FmMidiOutput();

    /* Fecha o MaximimMIDI
     */
    virtual ~FmMidiOutput();

	/* Toca/Para de tocar (abre/fecha o maxmidiout).
	 */
	void startPlaying();
	void stopPlaying();

	/* Muda o driver.
	 */
	void setDriver(int driver);

	/* Retorna o driver atual.
	 */
	int getDriver();

    /* Muda os intrumentos dos 16 canais para o do array recebido.
     * {Par�metros}
     *     instrumentos: Ponteiro para um array de 16 posi��es, referente
     *                   aos intrumentos dos 16 canais.
     */
    void setInstruments(const int *instrumentos);

    /* Muda o instrumento de um canal.
     */
    void setInstrument(int canal, int instrumento);

    /* Retorna o instrumento setado no canal.
     */
    int getInstrument(int canal);

	/* Retorna o volume de um canal especifico.
     */
    int getVolume(int channel);

    /* Retorna o volume geral.
     */
	int getGlobalVolume();

    /* Muda o volume do canal recebido.
	 * {Par�metros}:
     *     canal:  Canal a ser alterado (Deve estar entre 0 e 15).
     *     volume: Volume a ser setado (Deve estar entre 0 e 127).
     */
    void setVolume(int canal, int volume);

    /* Muda o volume geral.
     * {Par�metros}:
     *     volume: Volume a ser setado (Deve estar entre 0 e 100).
     */
    void setGlobalVolume(int volume);

    /* Muda o volume de todos os canais simultaneamente.
     * {Par�metros}:
     *     volumes: Array de 16 posicoes com os volumes a serem setados (de 0 a 127).
     */
    void setVolumes(const int *volumes);

    /* Retorna a lista de drivers.
     */
	void getDriversList(TStrings &result);
	int getNumDrivers();
	AnsiString getDriverName(int i);

	/* Toca a nota especificada no canal especificado.
     * {Par�metros}:
     *     canal:  Canal onde a nota ser� tocada (deve estar entre 0 e 15).
	 *     volume: Volume da nota (Deve estar entre 0 e 127).
     *     nota:   Frequ�ncia (pitch) MIDI da nota a ser tocada.
     */
    void playNote(int canal, int volume, int nota);

    /* Para a nota especificada no canal especificado.
     * {Par�metros}:
     *     canal: Canal que ser� parado (deve estar entre 0 e 15).
     *     nota:  Frequ�ncia MIDI da nota que ser� parada.
     */
    void stopNote(int canal, int nota);

    /* Para todos os sons de todos os canais.
     */
    void stopAllNotes();

    /* Set a intensidade da reverbera��o (reverb) em um canal.
     */
    void setReverb(int canal, int valor);

	/* Metodos para retornar/definir se o midi mapper estah disponivel.
	 */
	bool getMidiMapper();
	void setMidiMapper(bool hasMidiMapper);


private:

	/* Manda eventos para a placa de som.
	 * {Par�metros}:
	 *     status: tipo do evento MIDI a ser enviado, mais o canal;
	 *     byte1:  par�metro 1 do evento MIDI;
	 *     byte2:  par�metro 2 do evento MIDI.
	 */
	inline void sendMidiEvent(int status, int byte1, int byte2);

	/* Aplica as ultimas altera��es de volume (ou seja, envia os eventos MIDI
	 * correspondentes a essas altera��es).
	 */
	inline void applyVolumes();

    // Driver que est� sendo usado pelo sistema.
    int driver;

    // MaxMidi objects
	TMaxMidi *maxMidi;
	TMaxMidiOut *maxMidiOut;

	// Instrumentos usados nos 16 canais.
	int *instrumentos;

	// N�mero entre 0 e 100 que representa a porcentagem do volume de
	// cada canal
	int volumeGeral;

	// Guarda os volumes de cada canal.
	int *volumes;

	// Indica se o MIDI_MAPPER estah disponivel
	bool temMidiMapper;

};


#endif
