/**
 * D'accord Music Software
 * http://www.daccord.com.br
 *
 * D'accord Violão
 * Classe SaidaMIDI
 *
 * Nova versão por Pablo A. Sampaio
 */
#include <vcl.h>

#include "MaximumMidi\maxmidi.h"
#include <mmsystem.h>

#include "FmMidiOutput.h"

#pragma hdrstop
#pragma package(smart_init)
#pragma link "maxmidi"


FmMidiOutput::FmMidiOutput() {
	this->maxMidi = new TMaxMidi(NULL);
	this->maxMidiOut =  new TMaxMidiOut(NULL);

	maxMidiOut->MaxMidi = maxMidi;

	int numeroDevices = maxMidiOut->GetNumDevices();
	int i;

	// Testa um drive valido
	for (i = MIDI_MAPPER; (i < numeroDevices) && (!this->maxMidiOut->IsOpen())
																		; i++) {
		this->driver = i;
		this->maxMidiOut->Open(driver, MIDIOUT_DEFAULT);
	}

	if (this->driver == MIDI_MAPPER) {
		this->setMidiMapper(true);
	} else {
		this->setMidiMapper(false);
	}

	/* inicializa o array de instrumentos com 0 para todos os canais. Faz isto
	 * para não dar pau na hora de chamar o form de configuração "Midi". */
	this->instrumentos = new int[16];
	this->volumes = new int[16];
	this->volumeGeral = 100;
	for (int canal = 0; canal < 16; canal++) {
		this->instrumentos[canal] = 0;
		if (canal != 9) {
			// Control Change: ninguém sabe pra que serve.
			sendMidiEvent(0xB0 + canal, 0, 0);
			// Muda o instrumento, realmente (Program Change).
			sendMidiEvent(0xC0 + canal, 0, 0);
		}
		this->volumes[canal] = 90;
		sendMidiEvent(0xB0 + canal, 7, this->volumes[canal]);
	}

}

FmMidiOutput::FmMidiOutput(int driver) {
	this->maxMidi = new TMaxMidi(NULL);
	this->maxMidiOut =  new TMaxMidiOut(NULL);

	maxMidiOut->MaxMidi = maxMidi;

	// Abre o driver MIDI.

	this->driver = driver;
	if (!this->maxMidiOut->IsOpen()) {
		this->maxMidiOut->Open(driver, MIDIOUT_DEFAULT);
	}

	/* inicializa o array de instrumentos com 0 para todos os canais */
	this->instrumentos = new int[16];
	this->volumes = new int[16];
	for (int i = 0; i < 16; i++)
	{
		this->instrumentos[i] = 0;
		if (i != 9) {
			// Control Change: ninguém sabe pra que serve.
			sendMidiEvent(0xB0 + i, 0, 0);
			// Muda o instrumento, realmente (Program Change).
			sendMidiEvent(0xC0 + i, 0, 0);
		}
		this->volumes[i] = 90;
		sendMidiEvent(0xB0 + i, 7, this->volumes[i]);
	}

}

/**
 * Fecha o MaximimMIDI
 */
FmMidiOutput::~FmMidiOutput()
{
	stopPlaying();

	maxMidiOut->MaxMidi = NULL;

	delete maxMidiOut;
//	delete maxMidi; 	// DESABILITADO POR CONTA DE BUG DE RUNTIME NA APLICAÇÃO (??)
}

void FmMidiOutput::startPlaying()
{
	if (!maxMidiOut->IsOpen()) {
		maxMidiOut->Open(driver, MIDIOUT_DEFAULT);
	}
}

void FmMidiOutput::stopPlaying()
{
 if (maxMidiOut->IsOpen()) {
	 maxMidiOut->Flush();
	 maxMidiOut->Reset();
	 maxMidiOut->Close();
 }
}

void FmMidiOutput::setDriver(int driver) // exception
{
	this->driver = driver;

	if (getMidiMapper()) {
		this->driver --;
	}

	// Fecha o driver MIDI. (fechar dinamicamente...)
	if (maxMidiOut->IsOpen()) {
		maxMidiOut->Close();
	}
	// Abre o driver MIDI. (abrir dinamicamente...)
	maxMidiOut->Open(driver, MIDIOUT_DEFAULT);
}

int FmMidiOutput::getDriver()
{
	return driver;
}

void FmMidiOutput::setInstruments(const int *instrumentos)
{
    int canal;

    // Armazena o array de intrumentos e altera o valor no maximum MIDI.
    for (canal = 0; canal < 16; canal++)
    {
        if (instrumentos[canal] != 128)
        {
			setInstrument(canal, instrumentos[canal]);
        }
	}
}

void FmMidiOutput::setInstrument(int canal, int instrumento)
{
    if (instrumento != 128)
    {
        this->instrumentos[canal] = instrumento;
		// Control Change: ninguém sabe pra que serve.
		sendMidiEvent(0xB0 + canal, 0, 0);
        // Muda o instrumento, realmente (Program Change).
        sendMidiEvent(0xC0 + canal, instrumento, 0);
    }

}

int FmMidiOutput::getInstrument(int canal) {
    return instrumentos[canal];
}

void FmMidiOutput::setVolume(int canal, int volume)
{
    if (volume < 128)
	{
        if (volume < 0)
		{
			this->volumes[canal] = 0;
        }

		sendMidiEvent(0xB0 + canal, 7, (int)(volumes[canal]*(volumeGeral/100.0)));
        //sendMidiEvent(0xB0 + canal, 10, 63); // tentativa
    }
}

int FmMidiOutput::getVolume(int canal) {
    return volumes[canal];
}

void FmMidiOutput::setGlobalVolume(int volume)
{
    this->volumeGeral = volume;
    this->applyVolumes();
}

void FmMidiOutput::applyVolumes() {

    for (int canal = 0; canal < 16; canal++) {
        if (volumes[canal] < 0) {
			sendMidiEvent(0xB0 + canal, 7, 0);
        }
		else {
			sendMidiEvent(0xB0 + canal, 7, (int)(volumes[canal]*(volumeGeral/100.0)));
        }
    }
}

int FmMidiOutput::getGlobalVolume() {
    return volumeGeral;
}

void FmMidiOutput::setVolumes(const int *volumes)
{
    for (int i = 0; i < 16; i++) {
		if(volumes[i] != 128 )
        {
            setVolume(i, volumes[i]);
        }
    }
}

void FmMidiOutput::getDriversList(TStrings &retorno)
{
	int i;
	int count;

	// Estrutura de driver.
	MIDIOUTCAPSA midicaps;

	// Verifica se midi mapper estah disponivel
	if (midiOutGetDevCapsA(MIDI_MAPPER, &midicaps, sizeof(midicaps)) == 0)
	{
		// insere o primeiro driver
		retorno.Add(AnsiString(midicaps.szPname));
		this->setMidiMapper(true);

	} else {
		this->setMidiMapper(false);
	}

	// insere todos os drivers
	i = 0;

	count = this->maxMidiOut->GetNumDevices();

	// Pega os nomes dos drivers.
	for ( i = 0; i < count; i++)
	{   // Checa se o driver existe.
		if (midiOutGetDevCapsA(i, &midicaps, sizeof(midicaps)) == 0)
		{
			retorno.Add(AnsiString(midicaps.szPname));
		}
	}

}

int FmMidiOutput::getNumDrivers() {
	int add = 0;

	// Verifica se midi mapper estah disponivel
	if (getMidiMapper()) {
		add = 1;
	}

	return this->maxMidiOut->GetNumDevices() + add;
}

AnsiString FmMidiOutput::getDriverName(int i) {
	MIDIOUTCAPSA midicaps;
	AnsiString res = "undefined";

	if (getMidiMapper()) {
		i --;
	}

	if (i == -1) {
		if (midiOutGetDevCapsA(MIDI_MAPPER, &midicaps, sizeof(midicaps)) == 0) {
			res = AnsiString(midicaps.szPname);
		}
	} else {
		if (midiOutGetDevCapsA(i, &midicaps, sizeof(midicaps)) == 0) {
			res = AnsiString(midicaps.szPname);
		}
	}

	return res;
}

void FmMidiOutput::playNote(int canal, int volume, int nota)
{
	sendMidiEvent(0x90 + canal, nota, volume);
}

void FmMidiOutput::stopNote(int canal, int nota)
{
    sendMidiEvent(0x90 + canal, nota, 0);
}

void FmMidiOutput::stopAllNotes()
{
    if (maxMidiOut->IsOpen()) {
        maxMidiOut->Reset();
	}
}

void FmMidiOutput::setReverb(int canal, int valor) {
    sendMidiEvent(0xB0 + canal, 91, valor);
}

bool FmMidiOutput::getMidiMapper() {
    return this->temMidiMapper;
}

void FmMidiOutput::setMidiMapper(bool temMidiMapper) {
	this->temMidiMapper = temMidiMapper;
}

inline void FmMidiOutput::sendMidiEvent(int status, int byte1, int byte2)
{
    MidiEvent *evento = new MidiEvent;

    evento->status = status;
    evento->data1 = byte1;
    evento->data2 = byte2;

    maxMidiOut->Put(evento);

    delete evento;
}

