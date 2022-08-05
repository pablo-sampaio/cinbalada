//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#include <vcl/vcl.h>
#pragma hdrstop

#include "MaxMidiDLL.cpp"
#include "MaxMidiIn.cpp"
#include "MaxMidiOut.cpp"
#include "MaxMidiSync.cpp"
#include "MaxMidiTrack.cpp"
#include "MaxMidiScore.cpp"
#include "MaxMidiSMF.cpp"

#pragma package(smart_init)

namespace Maxmidi {
  void __fastcall PACKAGE Register()
  {
    TComponentClass classes[] = { __classid(TMaxMidi),
                                  __classid(TMaxMidiIn),
                                  __classid(TMaxMidiOut),
                                  __classid(TMaxMidiSync),
                                  __classid(TMaxMidiSMF)
                                };

    RegisterComponents("MaxMidi",
                        classes,
                        (sizeof(classes)/sizeof(classes[0])) - 1);

  } // Register
}   // namespace Maxmidi
