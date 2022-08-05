//---------------------------------------------------------------------------

#pragma hdrstop

#include "CbPercPlan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------


//......................//
// CLASSE CbPercPlan
//......................//

CbPercPlan::CbPercPlan(const CbRhythmicRole* defRole) {
	this->defaultRole = defRole;
}

const CbRhythmicRole* CbPercPlan::getRole(int bar) {
	MapRoles::iterator it = roles.find(bar);
	return (it == roles.end() ? this->defaultRole : it->second);
}

void CbPercPlan::setRole(int bar, const CbRhythmicRole* role) {
	if (role == this->defaultRole) {
		MapRoles::iterator it = roles.find(bar);
		if (it != roles.end()) {
			roles.erase(it);
		}
	} else {
		this->roles[bar] = role;
	}
}

const CbRhythmicRole* CbPercPlan::getDefaultRole() {
	return this->defaultRole;
}

void CbPercPlan::setDefaultRole(const CbRhythmicRole* role) {
	this->defaultRole = role;
}

void CbPercPlan::truncate(int bar) {
	MapRoles::iterator it;

	it = roles.lower_bound(bar);
	while (it != roles.end()) {
		roles.erase(it);
		it = roles.lower_bound(bar);
	}
}


 