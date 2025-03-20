/// \file
/// \ingroup tutorial_v7
///
/// \macro_code
///
/// \date 2015-03-22
/// \warning This is part of the ROOT 7 prototype! It will change without notice. It might trigger earthquakes. Feedback
/// is welcome!
/// \author Axel Naumann <axel@cern.ch>

/*************************************************************************
 * Copyright (C) 1995-2015, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "Rtypes.h"

R__LOAD_LIBRARY(libROOTGpadv7);

#include "ROOT/TCanvas.hxx"
// #include "ROOT/TFrame.hxx"
#include "ROOT/TLine.hxx"

void line()
{
   using namespace ROOT;
   using namespace ROOT::Experimental;

   auto canvas = ROOT::TCanvas::Create("user coordinate");

   // create user coordiantes
   canvas->SetAllAxisBounds({{50., 250.}, {-1., 1.}});

   // Please fix TLine such that {x,y} are TPadPos!
   //pad12->Draw(TLine({100._user, 0.5_normal}, {200._user, 0.5_normal}));

   canvas->Draw(ROOT::TLine({100._user + 0.1_normal - 50._px, 0.2_normal}, {200._user, 0.8_normal}));

   canvas->SaveAs("line.json");
   canvas->Show();
}
