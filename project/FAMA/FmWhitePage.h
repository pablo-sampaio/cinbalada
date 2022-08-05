
#ifndef __FMWHITEPAGE_H__
#define __FMWHITEPAGE_H__

#include "util.h"


/**
 * Implementation of the white-page service of the FAMA plataform.
 */

class FmWhitePage {

private:
	Vector_AID agents;

protected:

public:
	FmWhitePage();
	virtual ~FmWhitePage();

    /* Search by the number of the agent.
     */
	AID search(int id);

    /* Search by agent name.
     */
	AID search(std::string name);

	bool deleteAID(int id);
	bool deleteAID(std::string name);
	void addAID(AID a);
};

#endif
