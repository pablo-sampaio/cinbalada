
#include "CbXmlRhythmPatternInfoRepository.h"

#include <stdlib.h>
#include <fstream>

using namespace std;

//static ofstream logFile("logs\\CbXmlRhythmPatternInfoRepository.txt", ios::out);


CbXmlRpInfoRepository::CbXmlRpInfoRepository(const char* file) :
	document(file),
	changed(false)
{
    TiXmlElement* element;
    CbRpInfo* pattern;
    CbPercInstrument* instr;

    this->nextId = 0;
    this->document.LoadFile(); //ATENCAO: pode dar problemas de parsing

    //logFile << "File : " << file << endl;

	// Carrega os padroes ritmicos
    element = document.FirstChildElement(XML_NODE_RHYTHMPATTERN);
    while (element != NULL) {
		pattern = loadRhythmPattern(element);
		pattern->setId(this->nextId);
		this->rhythms.push_back(pattern);
		this->nextId ++;

		element = element->NextSiblingElement(XML_NODE_RHYTHMPATTERN);
	}

	// Carrega os instrumentos
	element = document.FirstChildElement(XML_NODE_INSTRUMENT);
	while (element != NULL) {
		instr = loadInstrument(element);
		this->instruments[instr->getName()] = instr;

		element = element->NextSiblingElement(XML_NODE_INSTRUMENT);
	}

}

CbRpInfo* CbXmlRpInfoRepository::loadRhythmPattern(TiXmlElement* element) {
	int numTrack = 1;

	const char* name     = element->Attribute(XML_ATTR_RHYTHMPATTERN_NAME);
	const char* trackStr = element->Attribute(XML_ATTR_RHYTHMPATTERN_TRACK, &numTrack);
	const char* filepath = element->Attribute(XML_ATTR_RHYTHMPATTERN_FILE);
	const char* typeStr  = element->Attribute(XML_ATTR_RHYTHMPATTERN_TYPE);
	const char* instr    = element->Attribute(XML_ATTR_RHYTHMPATTERN_INSTR);

	ECbRpType tp = CbRpInfo::strToRhythmPatternType(typeStr);

//	logFile << "Padrao: " << name << endl;
//	logFile << "Tipo: " << typeStr << " (" << tp << " : " << CbRpInfo::rhythmPatternTypeToStr(tp) << ")" << endl << endl;

	CbMeter* meter = loadMeter(element->FirstChildElement(XML_NODE_METER));
	CbRpInfo* rhythmPattern = new CbRpInfo(-1, name, filepath, instr, tp, meter, numTrack);

	TiXmlElement* node;
	const char* style;

	// pega o no que contem todos os estilos do padrao
	node = element->FirstChildElement(XML_NODE_MUSICALSTYLES);

	if (node != NULL) {
		// pega o primeiro estilo cadastrado
		node = node->FirstChildElement(XML_NODE_MUSICALSTYLE);

		while (node != NULL) {
			style = node->Attribute(XML_ATTR_MUSICALSTYLE_NAME);
			if (style != NULL) {
				rhythmPattern->addStyle(style);
			}
			node = node->NextSiblingElement(XML_NODE_MUSICALSTYLE);
		}
	}

	return rhythmPattern;
}

CbMeter* CbXmlRpInfoRepository::loadMeter(TiXmlElement* node) {
	CbMeter *meter = NULL;

	if (node == NULL) {
		meter =  new CbMeter(CbMeter::SIMPLE_QUADR);

	} else {
		int numBeats = -1;
		int defSubdv = -1;

		node->Attribute(XML_ATTR_METER_BEATS, &numBeats);
		node->Attribute(XML_ATTR_METER_DEF_BTATUMS, &defSubdv);

		if (numBeats != -1 && defSubdv != -1) {
			meter = new CbMeter(numBeats, defSubdv);
		} else {
			// irregular...
		}
	}

	return meter;
}

CbPercInstrument* CbXmlRpInfoRepository::loadInstrument(TiXmlElement* element) {
	const char* name     = element->Attribute(XML_ATTR_INSTRUMENT_NAME);
    const char* freqStr  = element->Attribute(XML_ATTR_INSTRUMENT_FREQ_CLASS);
	ECbInstrFreqClass fc = CbPercInstrument::strToFrequencyClass(freqStr);

    CbPercInstrument *instrument = new CbPercInstrument(name, fc);

    TiXmlElement* node;
    CbPercNote pnote(-1);

    const char *midiNoteStr, *pitchStr;

    // pega o no que contem todas as notas do instrumento
    node = element->FirstChildElement(XML_NODE_NOTES);

    if (node != NULL) {
        // pega a primeira nota
        node = node->FirstChildElement(XML_NODE_NOTE);

        while (node != NULL) {
			midiNoteStr = node->Attribute(XML_ATTR_NOTE_MIDI, &pnote.midiNote);
			pitchStr = node->Attribute(XML_ATTR_NOTE_REL_PITCH);

			if (midiNoteStr != NULL && pitchStr != NULL) {
            	pnote.pitch = CbPercInstrument::strToNotePitch(pitchStr);
	            instrument->addNote(pnote);
            }

            node = node->NextSiblingElement(XML_NODE_NOTE);
        }
    }

	return instrument;
}

CbXmlRpInfoRepository::~CbXmlRpInfoRepository() {
    CbRpInfo* pattern;

    //logFile << "Destrutor" << endl;

    if (changed) {
        //logFile << "Salvando..." << endl;
        saveXmlFile();
    }

    // Desaloca os padroes ritmicos
    for (int i = 0; i < this->rhythms.size(); i ++) {
        pattern = this->rhythms[i];
        this->rhythms[i] = NULL;
        delete pattern;
    }

    // Desaloca os instrumentos
	MapInstruments::iterator it;
    for (it = instruments.begin(); it != instruments.end(); it ++) {
		delete it->second;
        it->second = NULL;
    }

    // Obs: nao precisa desalocar os nos XML, pq eles foram alocados por 'document'
    // (da classe TiXmlDocument) e serao desalocados automaticamente
}

void CbXmlRpInfoRepository::saveXmlFile() {
	TiXmlNode* element;

	document.Clear();

    // Salva os padroes ritmicos
    for (int i = 0; i < this->rhythms.size(); i ++) {
        if (rhythms[i] != NULL) {
            element = loadXmlNode(rhythms[i]);
            document.InsertEndChild(*element);
            delete element;
        }
    }

    // Salva os instrumentos
	MapInstruments::iterator it;
    for (it = instruments.begin(); it != instruments.end(); it ++) {
		element = loadXmlNode(it->second);
		document.InsertEndChild(*element);
		delete element;
    }

    document.SaveFile();
}

TiXmlElement* CbXmlRpInfoRepository::loadXmlNode(CbRpInfo* rhythm) {
    TiXmlElement* rhythmPatternEl = new TiXmlElement(XML_NODE_RHYTHMPATTERN);

    /* attributes (node 'RhythmPattern') */
	rhythmPatternEl->SetAttribute(XML_ATTR_RHYTHMPATTERN_NAME, rhythm->getName().c_str());
	rhythmPatternEl->SetAttribute(XML_ATTR_RHYTHMPATTERN_TYPE, CbRpInfo::rhythmPatternTypeToStr(rhythm->getType()));
    rhythmPatternEl->SetAttribute(XML_ATTR_RHYTHMPATTERN_INSTR, rhythm->getInstrument().c_str());
    rhythmPatternEl->SetAttribute(XML_ATTR_RHYTHMPATTERN_FILE, rhythm->getFileName().c_str());
    rhythmPatternEl->SetAttribute(XML_ATTR_RHYTHMPATTERN_TRACK, rhythm->getTrack());

    /* child-node 'MusicalStyles' */
    TiXmlElement musicalStylesEl (XML_NODE_MUSICALSTYLES);
    TiXmlElement musicStyleEl (XML_NODE_MUSICALSTYLE);

    for (int i = 0; i < rhythm->getNumStyles(); i ++) {
		musicStyleEl.SetAttribute(XML_ATTR_MUSICALSTYLE_NAME, rhythm->getStyle(i).c_str());
        musicalStylesEl.InsertEndChild(musicStyleEl);
    }

	rhythmPatternEl->InsertEndChild(musicalStylesEl);

	/* child-node 'Meter' */
	TiXmlElement *meterEl = loadXmlNode(rhythm->getMeter());
	rhythmPatternEl->InsertEndChild(*meterEl);
	delete meterEl;

	return rhythmPatternEl;
}

TiXmlElement* CbXmlRpInfoRepository::loadXmlNode(const CbMeter* meter) {
	TiXmlElement* meterEl = new TiXmlElement(XML_NODE_METER);

	if (meter->isRegular()) {
		meterEl->SetAttribute(XML_ATTR_METER_BEATS, meter->getNumBeats());
		meterEl->SetAttribute(XML_ATTR_METER_DEF_BTATUMS, meter->getTatumsInBeat(0));
	} else {
		// irregular...
	}

	return meterEl;
}

TiXmlElement* CbXmlRpInfoRepository::loadXmlNode(CbPercInstrument* instr) {
	TiXmlElement* rhythmPatternEl = new TiXmlElement(XML_NODE_INSTRUMENT);

	/* attributes (node 'PercussionInstrument') */
	rhythmPatternEl->SetAttribute(XML_ATTR_INSTRUMENT_NAME, instr->getName());
	rhythmPatternEl->SetAttribute(XML_ATTR_INSTRUMENT_FREQ_CLASS,
						CbPercInstrument::frequencyClassToStr(instr->getFrequencyClass()));

    /* child-node 'Notes' */
    TiXmlElement midiNotesEl (XML_NODE_NOTES);
    TiXmlElement singleNote (XML_NODE_NOTE);

    CbPercNote aux (-1);
    for (int i = 0; i < instr->getNumNotes(); i ++) {
		aux = instr->getNote(i);
		singleNote.SetAttribute(XML_ATTR_NOTE_MIDI, aux.midiNote);
		singleNote.SetAttribute(XML_ATTR_NOTE_REL_PITCH, CbPercInstrument::notePitchToStr(aux.pitch));
        midiNotesEl.InsertEndChild(singleNote);
    }

    rhythmPatternEl->InsertEndChild(midiNotesEl);

    return rhythmPatternEl;
}

int CbXmlRpInfoRepository::getNumRpInfos() const {
    int num = 0;

    for (int i = 0; i < this->rhythms.size(); i ++) {
        if (rhythms[i] != NULL) {
            num ++;
        }
    }

    return num;
}

void CbXmlRpInfoRepository::getAllRpInfos(Vector_pRpInfo& res) const {
    int i;

    res.clear();
    for (i = 0; i < this->rhythms.size(); i ++) {
        if (rhythms[i] != NULL) {
            res.push_back(rhythms[i]);
        }
    }
}

void CbXmlRpInfoRepository::getRpInfosByInstrument(const string &instrument,
												 Vector_pRpInfo& result) const {
    Vector_pRpInfo::const_iterator it;

    result.clear();
    for (it = rhythms.begin(); it != rhythms.end(); it++) {
		if ((*it) != NULL && (*it)->getInstrument() == instrument) {
            result.push_back(*it);
        }
	}
}

const CbRpInfo* CbXmlRpInfoRepository::getRpInfoById(int id) const {
    return rhythms[id];
}

int CbXmlRpInfoRepository::getNumPatternsOfInstrument(string& instr) const {
    Vector_pRpInfo::const_iterator it;
    int result = 0;

    for (it = rhythms.begin(); it != rhythms.end(); it++) {
		if ((*it) != NULL && (*it)->getInstrument() == instr) {
            result ++;
        }
    }

    return result;
}

void CbXmlRpInfoRepository::addRpInfo(const CbRpInfo& info) {
    CbRpInfo* novo = new CbRpInfo(info);

    novo->setId(this->nextId);
    this->rhythms.push_back(novo);
    this->nextId ++;
    changed = true;
}

void CbXmlRpInfoRepository::changeRpInfo(int id, const CbRpInfo& info) {
    // se o id for valido...
    if (id < rhythms.size()) {
        CbRpInfo* novo = new CbRpInfo(info);
        CbRpInfo* velho = rhythms[id];

        novo->setId(id);
        rhythms[id] = novo;
        changed = true;

        delete velho;
    }
}

void CbXmlRpInfoRepository::deleteRpInfo(int id) {
    CbRpInfo* pattern = rhythms[id];

    if (pattern != NULL) {
        rhythms[id] = NULL;
        changed = true;

        delete pattern;
    }
}

void CbXmlRpInfoRepository::getAllInstrumentNames(vector<string> &results) const {
	MapInstruments::const_iterator it;

    results.clear();
    for (it = instruments.begin(); it != instruments.end(); it ++) {
		results.push_back(it->first);
    }
}

void CbXmlRpInfoRepository::getAllInstruments(Vector_pPercInstrument& results) const {
	MapInstruments::const_iterator it;

    results.clear();
    for (it = instruments.begin(); it != instruments.end(); it ++) {
		results.push_back(it->second);
    }
}

int CbXmlRpInfoRepository::getNumInstruments() const {
	return instruments.size();
}

const CbPercInstrument* CbXmlRpInfoRepository::getInstrument(string instrName) const {
	MapInstruments::const_iterator it = this->instruments.find(instrName);

	if (it != this->instruments.end()) {
		return it->second;
    }

    return NULL;
}

void CbXmlRpInfoRepository::addInstrument(const CbPercInstrument &instr) {
	// so insere se ainda nao existir instrumento com o dado nome
	if (instruments.find(instr.getName()) == instruments.end()) {
		instruments[instr.getName()] = new CbPercInstrument(instr);
		changed = true;
	}
}

void CbXmlRpInfoRepository::changeInstrument(const CbPercInstrument &instr) {
	string instrName = instr.getName();
	// testa se ja existe o instrumento
	if (instruments.find(instrName) != instruments.end()) {
		delete instruments[instrName];
		instruments[instrName] = new CbPercInstrument(instr);
		changed = true;
	}
}

void CbXmlRpInfoRepository::deleteInstrument(string name) {
	MapInstruments::iterator it = instruments.find(name);

	if (it != instruments.end()) {
    	delete instruments[name];
    	instruments.erase(it);
		changed = true;
    }
}

