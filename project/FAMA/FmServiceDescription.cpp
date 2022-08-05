
#include "FmServiceDescription.h"

#include "FmAID.h"

using namespace std;


FmServiceDescription::FmServiceDescription() {
	this->checkName = false;
	this->checkType = false;
}

const string& FmServiceDescription::getName() const {
	return this->name;
}

const string& FmServiceDescription::getType() const {
	return this->type;
}

AID FmServiceDescription::getAID() const {
	return this->aid;
}

void FmServiceDescription::addLanguage(const string& language) {
	languages.push_back(language);
}

void FmServiceDescription::addOntology(const string& ontology) {
	ontologies.push_back(ontology);
}

void FmServiceDescription::setName(const string& n) {
	this->checkName = true;
	this->name = n;
}

void FmServiceDescription::setType(const string& t) {
	this->checkType = true;
	this->type = t;
}

void FmServiceDescription::setAID(AID aid) {
	this->aid = aid;
}

int FmServiceDescription::getNumLanguages() const {
	return this->languages.size();
}

const std::string& FmServiceDescription::getLanguage(int index) {
    return this->languages[index];
}

int FmServiceDescription::getNumOntologies() const {
	return this->ontologies.size();
}

const std::string& FmServiceDescription::getOntology(int index) {
	return this->ontologies[index];
}

bool FmServiceDescription::matchPartialDescription(const FmServiceDescription& partDesc) {
	bool namesMatch = (partDesc.checkName == false) || (this->name == partDesc.name);
	bool typesMatch = (partDesc.checkType == false) || (this->type == partDesc.type);
	bool checkIds = (this->aid == NULL && partDesc.aid == NULL)
			|| ( this->aid != NULL && partDesc.aid != NULL && *(this->aid) == *(partDesc.aid) );

	return namesMatch && typesMatch && checkIds;
}

bool operator==(const FmServiceDescription& s1, const FmServiceDescription& s2) {
	bool namesMatch = (s1.name == s2.name);
	bool typesMatch = (s1.type == s2.type);
	bool checkIds = (s1.aid == NULL && s2.aid == NULL)
			|| ( s1.aid != NULL && s2.aid != NULL && *(s1.aid) == *(s2.aid) );

	return namesMatch && typesMatch && checkIds;
}
