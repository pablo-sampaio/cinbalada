
#include "FmWhitePage.h"
#include "FmAID.h"

using namespace std;


FmWhitePage::FmWhitePage() {

}

FmWhitePage::~FmWhitePage() {

}

AID FmWhitePage::search(int id) {
	Vector_AID::iterator it;

	for (it = agents.begin(); it != agents.end(); it ++) {
		if ((*it)->getID() == id) {
			return (*it);
		}
	}

	return NULL;
}

AID FmWhitePage::search(string name) {
	Vector_AID::iterator it;

	for (it = agents.begin(); it != agents.end(); it ++) {
		if ((*it)->getName() == name) {
			return (*it);
		}
	}

	return NULL;
}

bool FmWhitePage::deleteAID(int id) {
	Vector_AID::iterator it;

	for (it = agents.begin(); it != agents.end(); it ++) {
		if ((*it)->getID() == id) {
			agents.erase(it);
			return true;
		}
	}

	return false;
}

bool FmWhitePage::deleteAID(string name) {
	Vector_AID::iterator it;

	for (it = agents.begin(); it != agents.end(); it ++) {
		if ((*it)->getName() == name) {
			agents.erase(it);
			return true;
		}
	}

	return false;
}

void FmWhitePage::addAID(const FmAID* a) {
	agents.push_back(a);
}
