// FmAID.cpp: implementation of the FmAID class.
//
//////////////////////////////////////////////////////////////////////

#include "FmAID.h"

using namespace std;


const char* FmAID::STATUS_NAMES[] = {
    "ACTIVE",
    "INITIATED",
    "SUSPENDED",
    "WAITING",
    "TO DELETE",
    "DELETED"
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FmAID::FmAID(int k) {
	id = k;
	name = "";
    status = AG_INITIATED;
}

FmAID::~FmAID() {

}

int FmAID::getID() const {
	return id;
}

string FmAID::getName() const {
	return name;
}

void FmAID::setName(string n) {
	this->name = n;
}

EFmAgentStatus FmAID::getStatus() const {
	return this->status;
}

const char* FmAID::getStatusName() const {
    return STATUS_NAMES[this->status];
}

void FmAID::setStatus(EFmAgentStatus s) {
    if (s >= AG_ACTIVE && s <= AG_DELETED) {
    	this->status = s;
    }
}

bool FmAID::operator<(const FmAID& aid) const {
	return this->id < aid.id;
}

bool operator==(const FmAID& a, const FmAID& b) {
	return a.id == b.id;
}

bool operator!=(const FmAID& a, const FmAID& b) {
	return a.id != b.id;
}

ostream& operator<<(ostream& o, const FmAID& a) {
	return o << "AID(" << a.id << ")";
}

FmAID::FmAID(FmAID &a) {
    throw "FmAID should not be copied!";
}

void FmAID::operator=(FmAID &a) {
    throw "FmAID should not be copied!";
}

