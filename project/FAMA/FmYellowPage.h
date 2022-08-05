

#ifndef __YELLOWPAGE_H__
#define __YELLOWPAGE_H__

#include "util.h"
#include "FmServiceDescription.h"


/**
 * Implementation of the yellow page service of the FAMA plataform.
 */

class FmYellowPage {

private:
	Vector_pServiceDescription services;

public:
	FmYellowPage();
	~FmYellowPage();

    /* Returns the identifiers (FmAID*) of agents that offer the service
     * with the given name.
     */
    void searchByServiceName(const std::string &name, Vector_AID &result);

    /* Returns the identifiers (FmAID*) of agents that offer services
     * of the given type.
     */
	void searchByServiceType(const std::string &type, Vector_AID &result);

    /* Returns the identifiers (FmAID*) of agents that offer services
     * of the given type and with the given name.
     */
    void searchByServiceNameAndType(const std::string &name,
                            const std::string &type, Vector_AID &result);

	bool addService(const FmServiceDescription &sd);

	void deleteService(const FmServiceDescription &sd);

	void deleteAgentServices(AID aid);
};

#endif
