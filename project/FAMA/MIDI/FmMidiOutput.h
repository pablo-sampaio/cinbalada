/**
 * D'accord Music Software
 * http://www.daccord.com.br
 *
 * D'accord Violão
 *
 * Nova versão por Pablo A. Sampaio
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

	/* Inicializa o MaximimMIDI com o driver padrão.
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
     * {Parâmetros}
     *     instrumentos: Ponteiro para um array de 16 posições, referente
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
	 * {Parâmetros}:
     *     canal:  Canal a ser alterado (Deve estar entre 0 e 15).
     *     volume: Volume a ser setado (Deve estar entre 0 e 127).
     */
    void setVolume(int canal, int volume);

    /* Muda o volume geral.
     * {Parâmetros}:
     *     volume: Volume a ser setado (Deve estar entre 0 e 100).
     */
    void setGlobalVolume(int volume);

    /* Muda o volume de todos os canais simultaneamente.
     * {Parâmetros}:
     *     volumes: Array de 16 posicoes com os volumes a serem setados (de 0 a 127).
     */
    void setVolumes(const int *volumes);

    /* Retorna a lista de drivers.
     */
	void getDriversList(TStrings &result);
	int getNumDrivers();
	AnsiString getDriverName(int i);

	/* Toca a nota especificada no canal especificado.
     * {Parâmetros}:
     *     canal:  Canal onde a nota será tocada (deve estar entre 0 e 15).
	 *     volume: Volume da nota (Deve estar entre 0 e 127).
     *     nota:   Frequência (pitch) MIDI da nota a ser tocada.
     */
    void playNote(int canal, int volume, int nota);

    /* Para a nota especificada no canal especificado.
     * {Parâmetros}:
     *     canal: Canal que será parado (deve estar entre 0 e 15).
     *     nota:  Frequência MIDI da nota que será parada.
     */
    void stopNote(int canal, int nota);

    /* Para todos os sons de todos os canais.
     */
    void stopAllNotes();

    /* Set a intensidade da reverberação (reverb) em um canal.
     */
    void setReverb(int canal, int valor);

	/* Metodos para retornar/definir se o midi mapper estah disponivel.
	 */
	bool getMidiMapper();
	void setMidiMapper(bool hasMidiMapper);


private:

	/* Manda eventos para a placa de som.
	 * {Parâmetros}:
	 *     status: tipo do evento MIDI a ser enviado, mais o canal;
	 *     byte1:  parâmetro 1 do evento MIDI;
	 *     byte2:  parâmetro 2 do evento MIDI.
	 */
	inline void sendMidiEvent(int status, int byte1, int byte2);

	/* Aplica as ultimas alterações de volume (ou seja, envia os eventos MIDI
	 * correspondentes a essas alterações).
	 */
	inline void applyVolumes();

    // Driver que está sendo usado pelo sistema.
    int driver;

    // MaxMidi objects
	TMaxMidi *maxMidi;
	TMaxMidiOut *maxMidiOut;

	// Instrumentos usados nos 16 canais.
	int *instrumentos;

	// Número entre 0 e 100 que representa a porcentagem do volume de
	// cada canal
	int volumeGeral;

	// Guarda os volumes de cada canal.
	int *volumes;

	// Indica se o MIDI_MAPPER estah disponivel
	bool temMidiMapper;

};


#endif
