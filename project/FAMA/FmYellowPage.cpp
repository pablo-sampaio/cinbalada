
#include "FmYellowPage.h"

#include "FmAID.h"

using namespace std;


FmYellowPage::FmYellowPage() {

}

FmYellowPage::~FmYellowPage() {

}

void FmYellowPage::searchByServiceName(const string &name, Vector_AID &ret) {
	Vector_pServiceDescription::iterator it;
	for (it = services.begin(); it != services.end(); it++) {
		if ((*it)->getName() == name)
			ret.push_back((*it)->getAID());
	}
}

void FmYellowPage::searchByServiceNameAndType(const string& name, const string& type, Vector_AID &ret) {
    Vector_pServiceDescription::iterator it;
    for (it = services.begin(); it != services.end(); it++) {
        if ((*it)->getName() == name && (*it)->getType() == type) {
            ret.push_back((*it)->getAID());
        }
    }
}

void FmYellowPage::searchByServiceType(const string& type, Vector_AID &ret) {
	Vector_pServiceDescription::iterator it;
	for (it = services.begin(); it != services.end(); it++) {
		if ((*it)->getType() == type) {
			ret.push_back((*it)->getAID());
        }
	}
}

void FmYellowPage::deleteService(const FmServiceDescription &sd) {
    bool found = false;

	Vector_pServiceDescription::iterator it, toDelete;
	for (it = services.begin(); it != services.end(); it++) {
		if ((*it)->matchPartialDescription(sd)) {
			toDelete = it;
            found = true;
            break;
        }
	}

    if (found) {
        delete *toDelete;
    	services.erase(toDelete);
    }
}

bool FmYellowPage::addService(const FmServiceDescription &sd) {
	if (sd.getName() != "" && sd.getType() != "" && sd.getAID() != NULL) {
		services.push_back(new FmServiceDescription(sd));
		return true;
	} else {
		return false;
    }
}

void FmYellowPage::deleteAgentServices(AID aid) {
    for (int i = services.size()-1; i >= 0; i --) {
        if (*(services[i]->getAID()) == *aid) {
            services.erase(services.begin() + i);
        }
    }
}


