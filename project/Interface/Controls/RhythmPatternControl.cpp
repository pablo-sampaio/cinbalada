//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RhythmPatternControl.h"
#pragma package(smart_init)

#include "Util\MathUtil.h"

#include <fstream>
using namespace std;
static ofstream logFile("logs\\TRhythmPatternControl.txt", ios::out);


//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TRhythmPatternControl *)
{
    new TRhythmPatternControl(NULL);
}
//---------------------------------------------------------------------------
__fastcall TRhythmPatternControl::TRhythmPatternControl(TComponent* Owner)
	: TGraphicControl(Owner)
{
    Width = 65;
    Height = 65;
    this->hborder = 4;
    this->vborder = 4;
    this->pattern1 = NULL;
	this->pattern2 = NULL;
	this->graphicType = RGR_TUBS;
	this->alignment = RGA_TATUM;
    this->sequencer = NULL;
}
//---------------------------------------------------------------------------
__fastcall TRhythmPatternControl::~TRhythmPatternControl()
{
    if (pattern1 != NULL) {
        delete pattern1;
    }
    if (pattern2 != NULL) {
        delete pattern2;
    }
}
//---------------------------------------------------------------------------
void inline __fastcall TRhythmPatternControl::writeHBorder(int i) {
	if (i >= 4) {
		this->hborder = i;
	}
	Invalidate();
}
//---------------------------------------------------------------------------
void inline __fastcall TRhythmPatternControl::writeVBorder(int i) {
	if (i >= 4) {
        this->vborder = i;
	}
    Invalidate();
}
//---------------------------------------------------------------------------
void inline __fastcall TRhythmPatternControl::writeGraphicRepr(ERpGraphicRepr r) {
	this->graphicType = r;
	Invalidate();
}
//---------------------------------------------------------------------------
void inline __fastcall TRhythmPatternControl::writeGraphicReprAlignment(ERpGraphicReprAlignment al) {
	this->alignment = al;
    Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRhythmPatternControl::setRhythmPattern1(const CbRpInfo* rpi, AnsiString parentDir) {
    if (rpi != NULL) {

		if (pattern1 != NULL) {
			delete pattern1;
		}
		pattern1 = new CbRhythmPattern(*rpi, parentDir.c_str());

	} else {
		if (pattern1 != NULL) {
			delete pattern1;
		}
		pattern1 = pattern2;
		pattern2 = NULL;
	}

	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TRhythmPatternControl::setRhythmPattern2(const CbRpInfo* rpi, AnsiString parentDir) {
	if (rpi != NULL) {

		if (pattern2 != NULL) {
			delete pattern2;
		}
		pattern2 = new CbRhythmPattern(*rpi, parentDir.c_str());

	} else {
		if (pattern2 != NULL) {
			delete pattern2;
		}
		pattern2 = NULL;
	}

	Invalidate();
}

//---------------------------------------------------------------------------
const CbRhythmPattern* TRhythmPatternControl::getRhythmPattern1() {
	return this->pattern1;
}

//---------------------------------------------------------------------------
const CbRhythmPattern* TRhythmPatternControl::getRhythmPattern2() {
	return this->pattern2;
}

//---------------------------------------------------------------------------
void TRhythmPatternControl::AttachSequencer(CbRhythmPatternSequencer *__sequencer) {
	this->sequencer = __sequencer;
	if (this->sequencer != NULL) {
		this->sequencer->OnTick = this->OnSequencerTick;
    }
}

//---------------------------------------------------------------------------
void TRhythmPatternControl::OnSequencerTick(CbRhythmPatternSequencer *sequencer) {
	if ((sequencer->getCurrentTick() % 2) == 0) {
	    Invalidate();
    }
}

//---------------------------------------------------------------------------
void __fastcall TRhythmPatternControl::Paint()
{
    switch (graphicType) {
	case RGR_TUBS:
		paintTubs();
		break;
	case RGR_CIRCLE:
		paintCircle();
		break;
	case RGR_TEDAS:
		paintTedas();
		break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TRhythmPatternControl::paintTubs() {
	int tatumsAlign1 = 0, tatumsAlign2 = 0;
	int tick = -1;

	if (pattern1 == NULL) {
		tatumsAlign1 = 0;
		tatumsAlign2 = 0;

	} else if (pattern2 == NULL) {
		tatumsAlign1 = pattern1->getNumTatums();
		tatumsAlign2 = 0;

	} else {
		int maxTatums, maxBars;

		switch (this->alignment) {
		case RGA_TATUM:
			maxTatums = MAX(pattern1->getNumTatums(), pattern2->getNumTatums());
			tatumsAlign1 = maxTatums;
			tatumsAlign2 = maxTatums;
			break;
		case RGA_BAR:
			maxBars = MAX(pattern1->getNumBars(),pattern2->getNumBars());
			tatumsAlign1 = maxBars * pattern1->getTatumsPerBar();
			tatumsAlign2 = maxBars * pattern2->getTatumsPerBar();
			break;
		case RGA_NONE:
		default:
			tatumsAlign1 = pattern1->getNumTatums();
			tatumsAlign2 = pattern2->getNumTatums();
			break;
		};

	}

	/*
	if (sequencer != NULL) {
		int tick = this->sequencer->getCurrentTick();
	} //*/

	Canvas->Pen->Style = psSolid;
	Canvas->Pen->Width = 1;

	//divisão horizontal da tela
	paintTubs(pattern1, tatumsAlign1, clBlue, clSkyBlue, 0, 0, Height/2, Width, tick);
	paintTubs(pattern2, tatumsAlign2, clYellow, clMoneyGreen, Height/2, 0, Height/2, Width, tick);

}
//---------------------------------------------------------------------------
void __fastcall TRhythmPatternControl::paintTubs(CbRhythmPattern *rp,  int tatumsAlign, TColor clr1, TColor clr2,
													int top, int left, int height, int width, int tick)
{
	Canvas->Pen->Color = clBlack;
	Canvas->Brush->Color = clBlack;
	Canvas->Brush->Style = bsSolid;
	Canvas->Rectangle(left + hborder-1, top + vborder, left + width-hborder+1, top + height-vborder);

	if (rp != NULL && tatumsAlign != 0) {
		int sizeTubs = rp->getTubs()->getSize();
		int resolution = rp->getTubs()->getTatumsPerBar();
		int sizeCanvas = (width - 2*hborder);

		int onsetWidth = MIN(6, CONVERTTICK(2,tatumsAlign,sizeCanvas)
									- CONVERTTICK(1,tatumsAlign,sizeCanvas) - 2);

		int innerVBorder = height / 8;
		int y1 = top + vborder + innerVBorder + 1;
		int y2 = top + height - vborder - innerVBorder;
		int xbase = left + hborder + 1;
		int x1, x2;

		// obs: sem repeticao !
		for (int i = 0; i < sizeTubs; i++) {
			x1 = CONVERTTICK(i,tatumsAlign,sizeCanvas);
			x2 = x1 + onsetWidth;
			if ((i % resolution) == 0) {
				Canvas->Pen->Color = clWhite;
				Canvas->Rectangle(xbase + x1, top + vborder, xbase + x1 + 1, top + vborder + innerVBorder - 5);
			}
			if (rp->getTubs()->hasOnset(i%sizeTubs)) {
				if ((i / sizeTubs) == 0) {
					Canvas->Pen->Color = clr1;
					Canvas->Brush->Color = clr1;
				} else {
					Canvas->Pen->Color = clr2;
					Canvas->Brush->Color = clr2;
				}
				Canvas->Rectangle(xbase + x1, y1, xbase + x2, y2);
			}
		}

		/*if (tick != -1) {
			x1 = CONVERTTICK(tick, beatsAlign*this->sequencer->getResolution(), size);
			Canvas->Pen->Color = clWhite;
			Canvas->Rectangle(xbase + x1, top + vborder, xbase + x1 + 1, top + height - vborder);
		}*/
	}
}
//---------------------------------------------------------------------------
void __fastcall TRhythmPatternControl::paintCircle() {
	int vsize = (Height - 2*vborder);
	int hsize = (Width - 2*hborder);
	int diamet1 = MIN(vsize,hsize);
	int diamet2 = diamet1 - 40;

	// imprime os dois circulos base
	paintBaseCircle(diamet1, clLtGray);
	paintBaseCircle(diamet2, clGray);

	int tatumsAlign1 = 0, tatumsAlign2 = 0;

	if (pattern1 == NULL) {
		tatumsAlign1 = 0;
		tatumsAlign2 = 0;

	} else if (pattern2 == NULL) {
		tatumsAlign1 = pattern1->getNumTatums();
		tatumsAlign2 = 0;

	} else {
		int maxTatums, maxBars;

		switch (this->alignment) {
		case RGA_TATUM:
			maxTatums = MAX(pattern1->getNumTatums(), pattern2->getNumTatums());
			tatumsAlign1 = maxTatums;
			tatumsAlign2 = maxTatums;
			break;
		case RGA_BAR:
			maxBars = MAX(pattern1->getNumBars(),pattern2->getNumBars());
			tatumsAlign1 = maxBars * pattern1->getTatumsPerBar();
			tatumsAlign2 = maxBars * pattern2->getTatumsPerBar();
			break;
		case RGA_NONE:
		default:
			tatumsAlign1 = pattern1->getNumTatums();
			tatumsAlign2 = pattern2->getNumTatums();
			break;
		};

	}

	// imprime os padroes dentro dos circulos base
	if (pattern1 != NULL) {
		paintCircle(pattern1, diamet1/2.0 - 1, clBlue, clSkyBlue, tatumsAlign1);
	}
	if (pattern2 != NULL) {
		paintCircle(pattern2, diamet2/2.0 - 1, clYellow, clMoneyGreen, tatumsAlign2);
	}

}
//---------------------------------------------------------------------------
void __fastcall TRhythmPatternControl::paintBaseCircle(int diamet, TColor clr) {
	int vsize = (Height - 2*vborder);
	int hsize = (Width - 2*hborder);

	Canvas->Pen->Width = 1;
	Canvas->Brush->Style = bsSolid;

	Canvas->Pen->Color = clr;
	Canvas->Brush->Color = clr;
	Canvas->Ellipse(hborder + (hsize-diamet)/2, vborder + (vsize-diamet)/2,
				Width - hborder - (hsize-diamet)/2, Height - vborder - (vsize-diamet)/2);
}
//---------------------------------------------------------------------------
void __fastcall TRhythmPatternControl::paintCircle(CbRhythmPattern *rp, double radius, TColor clr1,
											TColor clr2, int circleQuantization) {
	int sizeTubs = rp->getTubs()->getSize();
	double incRad = (2.0 * M_PI) / double(circleQuantization);

	std::vector<TPoint*> vpoints;
	int x, y;
	int i;

	Canvas->Pen->Width = 2;
	Canvas->Pen->Color = clr1;

	// obs: sem repetição!
	for (i = 0; i < sizeTubs; i++) {
		if (rp->getTubs()->hasOnset(i % sizeTubs)) {
			x = (Width/2)  + radius * sin((i % circleQuantization) * incRad);
			y = (Height/2) - radius * cos((i % circleQuantization) * incRad);
			vpoints.push_back(new TPoint(x,y));
		}
	}
	vpoints.push_back(new TPoint(vpoints[0]->x, vpoints[0]->y));

	TPoint* arrayPts = new TPoint[vpoints.size()];
	for (i = 0; i < vpoints.size(); i ++) {
		arrayPts[i] = *(vpoints[i]);
	}

	// PROBLEMA: nao da pra setar a segunda cor !!!
	Canvas->Polyline(arrayPts, vpoints.size()-1);

	Canvas->Pen->Width = 4;
	Canvas->Pen->Color = clBlack;

	TPoint* aux;
	//marca os pontos e desaloca as estruturas da memoria
	for (i = vpoints.size() - 1; i >= 0; i --) {
		aux = vpoints[i];
		Canvas->PenPos = *aux;
		Canvas->LineTo(aux->x, aux->y);
		vpoints.erase(vpoints.begin() + i);
		delete aux;
	}

	delete arrayPts;
}
//---------------------------------------------------------------------------
void __fastcall TRhythmPatternControl::paintTedas() {
	int tatumsAlign1 = 0, tatumsAlign2 = 0;
	int chronotAlign1 = -1, chronotAlign2 = -1;
	int centralize = true;

	if (pattern1 == NULL) {
		tatumsAlign1 = 0;
		tatumsAlign2 = 0;

	} else if (pattern2 == NULL) {
		tatumsAlign1 = pattern1->getNumTatums();
		tatumsAlign2 = 0;
		chronotAlign1 = pattern1->getChronotonicVector()->getMax();
		centralize = true;

	} else {
		int maxTatums, maxBars;

		switch (this->alignment) {
		case RGA_TATUM:
			maxTatums = MAX(pattern1->getNumTatums(), pattern2->getNumTatums());
			tatumsAlign1 = maxTatums;
			tatumsAlign2 = maxTatums;
			chronotAlign1 = chronotAlign2 = MAX(pattern1->getChronotonicVector()->getMax(), pattern2->getChronotonicVector()->getMax());
			centralize = false;
			break;
		case RGA_BAR:
			maxBars = MAX(pattern1->getNumBars(),pattern2->getNumBars());
			tatumsAlign1 = maxBars * pattern1->getTatumsPerBar();
			tatumsAlign2 = maxBars * pattern2->getTatumsPerBar();
			chronotAlign1 = chronotAlign2 = MAX(pattern1->getChronotonicVector()->getMax(), pattern2->getChronotonicVector()->getMax());
			centralize = false;
			break;
		case RGA_NONE:
		default:
			tatumsAlign1 = pattern1->getNumTatums();
			tatumsAlign2 = pattern2->getNumTatums();
			chronotAlign1 = pattern1->getChronotonicVector()->getMax();
			chronotAlign2 = pattern2->getChronotonicVector()->getMax();
			centralize = true;
			break;
		};

	}

	if (pattern1 != NULL && tatumsAlign1 != 0) {
		paintTedas(pattern1, tatumsAlign1, chronotAlign1, clBlue, clSkyBlue, 0, 0, Height/2, Width, centralize);
	}
	if (pattern2 != NULL && tatumsAlign2 != 0) {
		paintTedas(pattern2, tatumsAlign2, chronotAlign2, clRed, clMoneyGreen, Height/2, 0, Height/2, Width, centralize);
	}

}
//---------------------------------------------------------------------------
void __fastcall TRhythmPatternControl::paintTedas(CbRhythmPattern* rp, int tatumsAlign, int chronotAlign,
											TColor clr1, TColor clr2, int top, int left, int height,
											int width, bool centralize) {
	const CbInterOnsetVector* interonset = rp->getInterOnsetVector();
	const CbChronotonicVector* chronot = rp->getChronotonicVector();

	top += vborder;
	height -= 2*vborder + 4;   // para as marcacoes no eixo
	left += hborder + 4;
	width -= 2*hborder + 8;    // para o eixo horizontal

	logFile << " --- TEDAS --- " << endl;
	logFile << "dimensoes - (top: " << top << ", left: " << left << ", height: ";
	logFile << height << ", " << " width: " << width << ")" << endl;
	logFile << "tatums-alignment: " << tatumsAlign << endl;

	int i;
	int unitH = width / tatumsAlign;
	int unitV = height / chronotAlign; //chronot->getMax();

	// escolhe a menor das unidades permitidas para representar um box do TUBS
	// (a mesma unidade eh usada nas duas dimensoes)
	/* if (unitV < unitH) {
		unitH = unitV;
	} else {
		unitV = unitH;
	}*/

	logFile << "unitV=" << unitV << ", unitH=" << unitH << endl;

	if (centralize == true) {
		left += (width - unitH*tatumsAlign) / 2;
	}

	// desenha o grafico tedas

	Canvas->Pen->Color = clr1;
	Canvas->Pen->Width = 1;
	Canvas->Brush->Color = clr2;
	Canvas->Brush->Style = bsBDiagonal; //bsSolid;

	int b, h;    // base e altura dos retangulos do histograma
	int x1, y2;
	int chronotPos = 0;
	int interonsetSize = interonset->getSize();
	int chronotSize = chronot->getSize();

	y2 = top + height;

	for (i = 0; i < interonsetSize; i ++) {
		b = interonset->get(i % interonsetSize);
		h = chronot->get(chronotPos % chronotSize);

		if (i == (interonsetSize - 1)) {
			b -= interonset->get(0);
		}

		x1 = left + chronotPos*unitH;
		Canvas->Rectangle(x1, y2 - h*unitV, x1 + b*unitH + 1, y2);

		logFile << "<> rectangle: (" << x1 << ", " << (y2 - h*unitV) << ") - ";
		logFile << "(" << (x1 + b*unitH + 1) << ", " << y2 << ")" << endl;

		chronotPos += b;
	}

	// desenha os eixos

	Canvas->Pen->Color = clBlack;
	Canvas->Pen->Width = 1;
	Canvas->Font->Height = 8;

	Canvas->PenPos = TPoint (left-3, top+height);
	Canvas->LineTo(left + (tatumsAlign*unitH) + 4, top+height);

	int lft = 0;
	for (int i = 0; i < tatumsAlign + 1; i ++) {
		Canvas->PenPos = TPoint (left + lft, top+height-1);
		Canvas->LineTo(left + lft, top+height+2);
		lft += unitH;
	}

	//Canvas->PenPos = TPoint(left, top+height-(maxChronot*unitV) - 4);
	//Canvas->LineTo(left, top+height+2);
}
//---------------------------------------------------------------------------
namespace Rhythmpatterncontrol
{
	void __fastcall PACKAGE Register()
	{
		 TComponentClass classes[1] = {__classid(TRhythmPatternControl)};
		 RegisterComponents("Samples", classes, 0);
	}
}
//---------------------------------------------------------------------------
