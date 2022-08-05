//---------------------------------------------------------------------------

#ifndef RhythmPatternControlH
#define RhythmPatternControlH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>

#include "Cinbalada\RhythmPattern\CbRhythmPattern.h"
#include "Cinbalada\RhythmPattern\CbRhythmPatternSequencer.h"
#include "FAMA\MIDI\FmMidiFile.h"

//---------------------------------------------------------------------------
typedef enum {RGR_TUBS, RGR_CIRCLE, RGR_TEDAS} ERpGraphicRepr;
typedef enum {RGA_NONE, RGA_TATUM, RGA_BAR} ERpGraphicReprAlignment;

//---------------------------------------------------------------------------
class PACKAGE TRhythmPatternControl : public TGraphicControl
{
private:
    int hborder, vborder;
	ERpGraphicRepr          graphicType;
	ERpGraphicReprAlignment alignment;
    CbRhythmPattern *pattern1;
    CbRhythmPattern *pattern2;

    CbRhythmPatternSequencer *sequencer;

protected:
	// evento do sequenciador
	void OnSequencerTick(CbRhythmPatternSequencer *sequencer);

    // metodos para desenhar o padrao
    virtual void __fastcall Paint();
    void __fastcall paintTubs();
	void __fastcall paintTubs(CbRhythmPattern *rp,  int tatumsAlign, TColor clr1, TColor clr2,
											int top, int left, int height, int width, int tick);
	void __fastcall paintCircle();
	void __fastcall paintBaseCircle(int diamet, TColor clr);
	void __fastcall paintCircle(CbRhythmPattern *rp, double radius, TColor clr1, TColor clr2,
											int circleQuantization);
	void __fastcall paintTedas();
	void __fastcall paintTedas(CbRhythmPattern* rp, int tatumsAlign, int chronotAlign, TColor clr1, TColor clr2,
											int top, int left, int height, int width, bool centralize);

    // acesso a atributos
    void inline __fastcall writeHBorder(int i);
    void inline __fastcall writeVBorder(int i);
	void inline __fastcall writeGraphicRepr(ERpGraphicRepr r);
	void inline __fastcall writeGraphicReprAlignment(ERpGraphicReprAlignment al);

public:
    __fastcall TRhythmPatternControl(TComponent* Owner);
    virtual __fastcall ~TRhythmPatternControl();
	void __fastcall setRhythmPattern1(const CbRpInfo* rpi, AnsiString parentDir);
	void __fastcall setRhythmPattern2(const CbRpInfo* rpi, AnsiString parentDir);
    const CbRhythmPattern* getRhythmPattern1();
    const CbRhythmPattern* getRhythmPattern2();
    void AttachSequencer(CbRhythmPatternSequencer *sequencer); 

__published:
    __property int HBorder = {read=hborder, write=writeHBorder, default=4};
    __property int VBorder = {read=vborder, write=writeVBorder, default=4};
	__property ERpGraphicRepr          PatternRepresentation = {read=graphicType, write=writeGraphicRepr, default=RGR_TUBS};
	__property ERpGraphicReprAlignment PatternsAlignment = {read=alignment, write=writeGraphicReprAlignment, default=RGA_TATUM};
    __property Height;
    __property Width;
    __property Align;
    __property Anchors;
};
//---------------------------------------------------------------------------
#endif
