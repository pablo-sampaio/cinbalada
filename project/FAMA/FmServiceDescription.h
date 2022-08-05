

#ifndef __FMSERVICEDESCRIPTION_H__
#define __FMSERVICEDESCRIPTION_H__

#include "util.h"

typedef std::vector<std::string> Vector_String;

/**
 * Description of a service offered by FAMA agents. This service description
 * may be published (and search for) through the yellow-page service.
 */
class FmServiceDescription {
private:

	Vector_String languages;
	Vector_String ontologies;
	std::string name;
	bool checkName;
	std::string type;
	bool checkType;
	AID aid;

public:

	/* Constructor.
	 */
	FmServiceDescription();

    /* Name of the service.
     */
	const std::string& getName() const;

    /* Type of the service.
     */
	const std::string& getType() const;

    /* Identifier of the agent that offer the service.
     */
	AID getAID() const;

	void setName(const std::string& n);
	void setType(const std::string& t);
	void setAID(AID aid);

    /* Methods to access the set of comunication languages used by
     * service providers (agents).
     */
	int getNumLanguages() const;
    const std::string& getLanguage(int index);

    /* Methods to access the set of ontologies related to the service.
     */
	int getNumOntologies() const;
    const std::string& getOntology(int index);

	void addLanguage(const std::string& language);
	void addOntology(const std::string& ontology);

	bool matchPartialDescription(const FmServiceDescription& partDesc);

	friend bool operator==(const FmServiceDescription& s1, const FmServiceDescription& s2);

};

#endif