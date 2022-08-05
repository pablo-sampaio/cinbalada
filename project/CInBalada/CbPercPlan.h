//---------------------------------------------------------------------------

#ifndef __CB_PERC_PLAN_H______
#define __CB_PERC_PLAN_H______

#include <map>
#include "RhythmPattern\CbRhythmicRole.h"
//---------------------------------------------------------------------------

/**
 * Agent who plays a percussion instrument. Agents may assume different
 * rhtyhmic roles in each bar.
 */
class CbPercPlan
{
private:
	typedef std::map<int,const CbRhythmicRole*> MapRoles;

	MapRoles roles;                      // roles for each bar
	const CbRhythmicRole* defaultRole;   // role assumed for positions not in the map

public:
	CbPercPlan(const CbRhythmicRole* defaultRole);

	/* Returns the rhythmic role that the agent plays in the given bar.
	 */
	const CbRhythmicRole* getRole(int bar);
	void setRole(int bar, const CbRhythmicRole* role);

	/* Returns the default rhythmic role for bars with no role set.
	 */
	const CbRhythmicRole* getDefaultRole();
	void setDefaultRole(const CbRhythmicRole* role);

	/* Clears all the roles set after the given bar (they receive the
	 * default value).
	 */
	void truncate(int bar);

};


//---------------------------------------------------------------------------
#endif /* __CB_PERC_PLAN_H______ */


 