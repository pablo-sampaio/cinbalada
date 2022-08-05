

/**
 * Esse arquivo contem definicoes comuns usadas pelas classes do framework.
 * Serve para evitar problemas de dependencia circular.
 */

#ifndef __UTIL_H__
#define __UTIL_H__


#include <map>
#include <string>
#include <vector>
#include <iostream>

class FmAgent;
class FmAID;
class FmTContainer;

class FmBehaviour;
class FmCyclicBehaviour;
class FmOneShotBehaviour;

class FmAclMessage;
class FmMessageQueue;
class FmMessageTemplate;

class FmServiceDescription;
class FmWhitePage;
class FmYellowPage;

typedef const FmAID* AID;

typedef std::vector<AID> Vector_AID;
typedef std::vector<FmAclMessage*> Vector_pAclMessage;
typedef std::vector<FmAgent*> Vector_pAgent;
typedef std::vector<FmBehaviour*> Vector_pBehaviour;
typedef std::vector<FmServiceDescription*> Vector_pServiceDescription;

#endif
