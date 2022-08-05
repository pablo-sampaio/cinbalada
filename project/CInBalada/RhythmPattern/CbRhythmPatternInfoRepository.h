
#ifndef __RHYTHMPATTERNINFO_REP_H__
#define __RHYTHMPATTERNINFO_REP_H__


#include "CbRhythmPatternInfo.h"
#include "CbPercussionInstrument.h"

/**
 * This is an interface for repositories of RpInfo (rhythm pattern info) objects.
 * Each RpInfo object is given a unique identifier by the repository
 * when the object is first instantiated. The identifier is valid only during
 * the session (i.e, after creation and before destruction of the repository).
 * If the repositoy is reinstantiated, the identifiers are reassigned freely,
 * and may differ from previously assigned identifiers.
 */
class ICbRpInfoRepository {
public:

	/* Returns the number of PatternInfo objects stored in the repository.
	 */
	virtual int getNumRpInfos() const = 0;

	/* Returns all RpInfo objects stored.
	 * Atention: the objects returned should never be changed directly. Use
	 * method 'changeRpInfo' instead.
	 * {Deallocation of results}: done by this class
	 */
	virtual void getAllRpInfos(Vector_pRpInfo&) const = 0;

	/* Returns all RpInfo objects whith the given instrument.
	 * The rhythm patterns are returned through the parameter 'result'.
	 * Atention: the objects returned should never be changed directly. Use
	 * method 'changeRpInfo' instead.
	 */
    virtual void getRpInfosByInstrument(const std::string &instrument,
    												 Vector_pRpInfo& result) const = 0;
    virtual int getNumPatternsOfInstrument(std::string& instr) const = 0;

    /* Returns the (only) RpInfo with the given identifier.
     * The result is returned through the parameter 'result' which must be
	 * pre-allocated.
     * {Deallocation of result}: done by this class
     */
    virtual const CbRpInfo* getRpInfoById(int id) const = 0;

    /* Adds a new RpInfo to the repository. The value of attribute 'id'
     * is ignored and a new value is assigned to it by the repository.
     */
    virtual void addRpInfo(const CbRpInfo& info) = 0;

    /* Replaces all the information of previous RpInfo with the
     * new information given. If there's no object with the given id, this
     * method does nothing.
     */
    virtual void changeRpInfo(int id, const CbRpInfo& info) = 0;

    /* The object with the given identifier is deleted from repository and
     * deallocated from memory.
	 */
    virtual void deleteRpInfo(int id) = 0;

    /* Returns all instruments stored (and probably played in some of the rhythm
     * patterns stored).
     */
    virtual void getAllInstrumentNames(std::vector<std::string>& result) const = 0;
	virtual void getAllInstruments(Vector_pPercInstrument& result) const = 0;
    virtual int getNumInstruments() const = 0;

    /* Returns a specific instrument (by its name).
     */
    virtual const CbPercInstrument* getInstrument(std::string instrName) const = 0;

    /* Adds a new instrument to the repository.
     */
	virtual void addInstrument(const CbPercInstrument &instr) = 0;

    /* Changes information associated to an instrument (by its name).
     */
    virtual void changeInstrument(const CbPercInstrument &instr) = 0;

    /* The instrument with the given name is deleted from repository and
     * deallocated from memory.
     */
	virtual void deleteInstrument(std::string name) = 0;

    /* Destructor.
     */
	virtual ~ICbRpInfoRepository() {}
};

#endif
