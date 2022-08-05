// AID.h: interface for the AID class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __FMAID_H__
#define __FMAID_H__

#include "util.h"

typedef enum {AG_ACTIVE, AG_INITIATED,
              AG_SUSPENDED, AG_WAITING,
              AG_TO_DELETE, AG_DELETED} EFmAgentStatus;

/**
 * An agent identifier (Agent ID). When an agent is created, it receives an
 * FmAid that identifies him uniquely in the container.
 */
class FmAID
{
private:
    static const char* STATUS_NAMES[];

	int id;
	std::string name;
	EFmAgentStatus status;

protected:
    friend class FmTContainer;

	FmAID(int k);

	void setName(std::string n);
	void setStatus(EFmAgentStatus s);

    // prohibited operations !
   	FmAID(FmAID &a);
    void operator=(FmAID &a);

public:

	virtual ~FmAID();

	int getID() const;

	std::string getName() const;
	EFmAgentStatus getStatus() const;
    const char* getStatusName() const;

	/* Relational operator, returns 'true' when (*this) < aid.
     * This operator must be defined to allow objects FmAID to be
     * used as keys in STL maps.
     */
	bool operator<(const FmAID& aid) const;

	/* Comparation operators.
	 */
	friend bool operator==(const FmAID& a, const FmAID& b);
	friend bool operator!=(const FmAID& a, const FmAID& b);

    /* Overloads the operator << to print FmAID objects.
     */
	friend std::ostream& operator<<(std::ostream& o, const FmAID& a);

};

#endif

