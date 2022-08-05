
#ifndef _____CB_ROLE_AGENT_H____
#define _____CB_ROLE_AGENT_H____

class Role;

/**
 * Agent with roles.
 */
class CbRoleAgent : public CbAbstractAgent
{
private:
    Role* roles[];    // role in each measure
    int currentBar;  // current bar (measure)

public:
    CbRoleAgent(std::string name, CbAgentRepertoire *rep,
                                Role *roles[], int cycleLength);
    virtual ~CbRoleAgent();


};


#define
