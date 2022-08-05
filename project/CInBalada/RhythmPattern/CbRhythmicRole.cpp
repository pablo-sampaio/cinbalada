//---------------------------------------------------------------------------


#pragma hdrstop

#include "CbRhythmicRole.h"

#include "CbPercPerfHistory.h"
#include "CbPercRepertoire.h"

#include "MathUtil.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// normaliza um valor com base nos limites max e min esperados
#define NORMALIZE(x, min, max)  ( double((x) - (min)) / double((max) - (min)) )


//.................................................//
// CLASSE CbRhythmicRole
//.................................................//

CbRhythmicRole::CbRhythmicRole(const char* __name, const char* __reference, double __evalThres)
:	 name(__name),
	 evalPatternThreshold(__evalThres)
{
	if (__reference != NULL) {
		this->reference = __reference;
	}
}

const char* CbRhythmicRole::getName() const {
	return this->name.c_str();
}

const char* CbRhythmicRole::getReferenceRole() const {
	return this->reference.c_str();
}

double CbRhythmicRole::getEvaluationThreshold() const {
	return this->evalPatternThreshold;
}

void CbRhythmicRole::setEvaluationThreshold(double t) {
	this->evalPatternThreshold = t;
}

bool CbRhythmicRole::patternIsSuitable(const CbRhythmPattern& rp) const {
	double evaluation = evaluate(rp);
	return evaluation >= 0.0 && evaluation <= getEvaluationThreshold();
}

double CbRhythmicRole::evaluateMatchHistory(const CbRhythmPattern &rp, CbPercPerfHistory* history,
										CbPercRepertoire* rep, int bar) const {
	return 0.0;
}

bool CbRhythmicRole::isReferenceTo(const CbRhythmicRole& role) const {
	const char* roleReferenceName = role.getReferenceRole();
	bool result = false;

	if (roleReferenceName != NULL) {
		result = ( strcmp(this->getName(),roleReferenceName) == 0 );
	}

	return result;
}

bool CbRhythmicRole::operator==(const CbRhythmicRole& r) const {
	return this->name == r.name;
}


//.................................................//
// CLASSE CbBasicRole
//.................................................//

CbBasicRole::CbBasicRole()
	: CbRhythmicRole("base", NULL, 2.0)
{
}

double CbBasicRole::evaluate(const CbRhythmPattern &rp) const {
	double v = -1.0;

	if (rp.getInfo()->getType() == RPT_BASIC) {
		if (rp.getTatumsPerBar() == 16) {
			v = evaluateQuaternary(rp);
		} else if (rp.getTatumsPerBar() == 12) {
			v = evaluateTernary(rp);
		}
	}

	return v;
}

double CbBasicRole::evaluateQuaternary(const CbRhythmPattern &rp) const {
	double activity_n = NORMALIZE(rp.activity(), 3, 5);
	double evenness_n = NORMALIZE(rp.linearRegEvenness(), 0, 0.96);
	return activity_n + evenness_n;
}

double CbBasicRole::evaluateTernary(const CbRhythmPattern &rp) const {
	double activity_n = NORMALIZE(rp.activity(), 2, 5);
	double evenness_n = NORMALIZE(rp.linearRegEvenness(), 0, 0.60);
	return activity_n + evenness_n;
}

double CbBasicRole::evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const {
	return a.chronotonicDistance(&b);
}

double CbBasicRole::evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const {
	return 0.0;
}

double CbBasicRole::evaluateMatchHistory(const CbRhythmPattern &rp, CbPercPerfHistory* history,
										CbPercRepertoire* rep, int bar) const {
	const CbPercPerfInfo* info;
	const CbRhythmPattern* pattern;

	double sum = 0.0;
	int n = 0;

	for (int i = 0; i < 4; i ++) {
		info = history->searchBackward(getName());
		if (info != NULL && info->bar < bar) {
			pattern = rep->getRhythmPattern(info->patternIndex);
			sum += rp.chronotonicDistance(pattern);
			n ++;
		}
	}

	return (n == 0)? 0.0 : (sum / double(n));
}


//.................................................//
// CLASSE CbComplBasicRole
//.................................................//

CbComplBasicRole::CbComplBasicRole()
	: CbRhythmicRole("base-complementar", "base", 1.5)
{
}

double CbComplBasicRole::evaluate(const CbRhythmPattern &rp) const {
	double v = -1.0;

	if (rp.getInfo()->getType() == RPT_BASIC) {
		if (rp.getTatumsPerBar() == 16) {
			v = evaluateQuaternary(rp);
		} else if (rp.getTatumsPerBar() == 12) {
			v = evaluateTernary(rp);
		}
	}

	return v;
}

double CbComplBasicRole::evaluateQuaternary(const CbRhythmPattern &rp) const {
	double activity_n = 2 * NORMALIZE(rp.activity(), 8, 16);
	double evenness_n = NORMALIZE(rp.linearRegEvenness(), 0, 1.80);

	// desejado: atividade=12 e evenness=0
	double desired_acti = 2 * NORMALIZE(12, 8, 16);
	double desired_even = NORMALIZE(0, 0, 1.80);

	double diff_activity = (activity_n - desired_acti)*(activity_n - desired_acti);
	double diff_evenness = (evenness_n - desired_even)*(evenness_n - desired_even);

	return sqrt(diff_activity  + diff_evenness);
}

double CbComplBasicRole::evaluateTernary(const CbRhythmPattern &rp) const {
	double activity_n = NORMALIZE(rp.activity(), 7, 12);
	double evenness_n = NORMALIZE(rp.linearRegEvenness(), 0, 0.5);

	// desejado: atividade=10 e evenness=0.20 (~bemba)
	double desired_acti = NORMALIZE(10, 7, 12);
	double desired_even = NORMALIZE(0.2, 0, 0.5); // escolhido por tentativa e erro

	double diff_activity = (activity_n - desired_acti)*(activity_n - desired_acti);
	double diff_evenness = (evenness_n - desired_even)*(evenness_n - desired_even);

	return sqrt(diff_activity  + diff_evenness);
}

double CbComplBasicRole::evaluateMatch(const CbRhythmPattern &rp, const CbRhythmPattern &peer) const {
	return rp.geberFunction(&peer) / rp.getTubs()->getSize();
}

double CbComplBasicRole::evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const {
	return 1 - rp.derivationMeasure(&ref); // para ficar [0;2]
}

double CbComplBasicRole::evaluateMatchHistory(const CbRhythmPattern &rp, CbPercPerfHistory* history,
										CbPercRepertoire* rep, int bar) const {
	const CbPercPerfInfo* info;
	const CbRhythmPattern* pattern;

	double sum = 0.0;
	int n = 0;

	for (int i = 0; i < 2; i ++) {
		info = history->searchBackward(getName());
		if (info != NULL && info->bar < bar) {
			pattern = rep->getRhythmPattern(info->patternIndex);
			sum += rp.chronotonicDistance(pattern);
			n ++;
		}
	}

	return (n == 0)? 0.0 : (sum / double(n));
}


//.................................................//
// CLASSE CbSoloRole
//.................................................//

CbSoloRole::CbSoloRole()
	: CbRhythmicRole("solo", NULL /* "base" */, 1.5)
{
}

double CbSoloRole::evaluate(const CbRhythmPattern &rp) const {
	double v = -1.0;

	if (rp.getInfo()->getType() == RPT_BASIC) {
		if (rp.getTatumsPerBar() == 16) {
			v = evaluateQuaternary(rp);
		} else if (rp.getTatumsPerBar() == 12) {
			v = evaluateTernary(rp);
		}
	}

	return v;
}

double CbSoloRole::evaluateQuaternary(const CbRhythmPattern &rp) const {
	double v = 0.0;

	double activity_n = 2 * NORMALIZE(rp.activity(), 8, 16);
	double evenness_n = NORMALIZE(rp.linearRegEvenness(), 0.52, 3.74);

	// desejado: atividade=10 e evenness=2.0
	double desired_acti = 2 * NORMALIZE(10, 8, 16);
	double desired_even = NORMALIZE(2.0, 0.52, 3.74);

	v = ABS(activity_n - desired_acti) + ABS(evenness_n - desired_even);

	if (rp.offbeatness() > 3.0) {
		v -= 0.2;
	}

	if (rp.averageIntensity() < 90.0) {
		v += 1.0;
	}

	return (v < 0.0? 0.0 : v);
}

double CbSoloRole::evaluateTernary(const CbRhythmPattern &rp) const {
	double v = 0.0;

	double activity_n = NORMALIZE(rp.activity(), 3, 8);
	double evenness_n = NORMALIZE(rp.linearRegEvenness(), 0.37, 0.83);

	// desejado: atividade=6 (media) e evenness=0.6 (media)
	double desired_acti = NORMALIZE(7, 4, 8);
	double desired_even = NORMALIZE(0.7, 0.37, 0.83);

	v = ABS(activity_n - desired_acti) + ABS(evenness_n - desired_even);

	if (rp.offbeatness() < 3.0) {
		v += 0.26;
	}
	if (rp.oddity() >= 3.0) {
		v += 0.26;
	}
	if (rp.metricity() > 2.5) {
		v += 0.26;
	}
	if (rp.wnbdSyncopation() <= 2.0) {
		v += 0.26;
	}

	return v;
}

double CbSoloRole::evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const {
	return a.geberFunction(&b);
}

double CbSoloRole::evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const {
	return 0.0; //rp.geberFunction(&ref);
}

double CbSoloRole::evaluateMatchHistory(const CbRhythmPattern &rp, CbPercPerfHistory* history,
										CbPercRepertoire* rep, int bar) const {
	const CbPercPerfInfo* info;
	const CbRhythmPattern* pattern;

	double sum = 0.0;
	int n = 0;

	for (int i = 0; i < 2; i ++) {
		info = history->searchBackward(getName());
		if (info != NULL && info->bar < bar) {
			pattern = rep->getRhythmPattern(info->patternIndex);
			sum += rp.chronotonicDistance(pattern);
			n ++;
		}
	}

	return (n == 0)? 0.0 : (sum / double(n));
}


//.................................................//
// CLASSE CbFillRole
//.................................................//

CbFillRole::CbFillRole()
	: CbRhythmicRole("virada", NULL, 0.5)
{
}

double CbFillRole::evaluate(const CbRhythmPattern &rp) const {
	int type = rp.getInfo()->getType();
	return (type == RPT_ROLL || type == RPT_FILL) ? 0.0 : 1.0;
}

double CbFillRole::evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const {
	return a.chronotonicDistance(&b);
}

double CbFillRole::evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const {
	return 0.0;
}


//.................................................//
// CLASSE CbMuteRole
//.................................................//

CbMuteRole::CbMuteRole()
	: CbRhythmicRole("mudo", NULL, 0.5)
{
}

double CbMuteRole::evaluate(const CbRhythmPattern &rp) const {
	return rp.getOnsetsVector()->getSize();
}

double CbMuteRole::evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const {
	return 0.0;  // todos sao validos
}

double CbMuteRole::evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const {
	return 0.0;
}


//.................................................//
// CLASSE CbFreeRole
//.................................................//

CbFreeRole::CbFreeRole()
	: CbRhythmicRole("livre", NULL, 1.0)
{
}

double CbFreeRole::evaluate(const CbRhythmPattern &rp) const {
	double eval;

	if (rp.getOnsetsVector()->getSize() > 0) {
		eval = 0.0;
	} else {
		eval = -1.0;
	}

	return eval;
}

double CbFreeRole::evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const {
	return 0.0;
}

double CbFreeRole::evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const {
	return 0.0;
}


//.................................................//
// CLASSE CbUniformRole
//.................................................//

CbUniformRole::CbUniformRole()
	: CbRhythmicRole("uniforme", NULL, 1.0)
{
}

double CbUniformRole::evaluate(const CbRhythmPattern &rp) const {
	double eval;

	if (rp.getOnsetsVector()->getSize() > 0) {
		eval = 0.0;
	} else {
		eval = -1.0;
	}

	return eval;
}

double CbUniformRole::evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const {
	return a.chronotonicDistance(&b); // a.geberFunction(&b);
}

double CbUniformRole::evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const {
	return 0.0;
}

double CbUniformRole::evaluateMatchHistory(const CbRhythmPattern &rp, CbPercPerfHistory* history,
										CbPercRepertoire* rep, int bar) const {
	const CbPercPerfInfo* info;
	const CbRhythmPattern* pattern;

	double sum = 0.0;

	info = history->searchBackward(getName());
	if (info != NULL && info->bar < bar) {
		pattern = rep->getRhythmPattern(info->patternIndex);
		sum = rp.chronotonicDistance(pattern);
	}

	return 2.0 * sum;
}

