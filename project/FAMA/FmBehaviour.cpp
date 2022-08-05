
#include "FmBehaviour.h"


FmBehaviour::FmBehaviour(FmAgent *a) {
	myAgent = a;
	status = BHV_NEW;
}

FmBehaviour::~FmBehaviour() {

}

void FmBehaviour::block() {
	status = BHV_BLOCKED;
}

void FmBehaviour::onEnd() {
		//user defined
}

void FmBehaviour::onStart() {
		//user defined
}

void FmBehaviour::setAgent(FmAgent* a) {
	myAgent = a;
}

EFmBehaviourStatus FmBehaviour::getStatus() {
	return this->status;
}

void FmBehaviour::setStatus(EFmBehaviourStatus st) {
	this->status = st;
}



bool FmOneShotBehaviour::done() {
	return true;
}



bool FmCyclicBehaviour::done() {
	return false;
}
