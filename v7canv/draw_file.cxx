/// \file
/// \ingroup tutorial_v7
///
/// This macro generates a small V7 TH1D, fills it and draw it in a V7 canvas.
/// The canvas is display in the web browser and the corresponding png picture
/// is generated.
///
/// \macro_code
///
/// \date 2015-03-22
/// \warning This is part of the ROOT 7 prototype! It will change without notice. It might trigger earthquakes. Feedback is welcome!
/// \author Axel Naumann <axel@cern.ch>

/*************************************************************************
 * Copyright (C) 1995-2015, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

R__LOAD_LIBRARY(libROOTGpadv7);
R__LOAD_LIBRARY(libROOTGraphicsPrimitives);
R__LOAD_LIBRARY(libROOTHistDraw);

#include "ROOT/THist.hxx"
#include "ROOT/RCanvas.hxx"

#include "TRandom3.h"

void draw_file() {
   using namespace ROOT::Experimental;

   // Create histograms
   TAxisConfig xaxis(100, -10., 10.);
   auto pHist = std::make_shared<ROOT::TH1D>(xaxis);
   auto pHist2 = std::make_shared<ROOT::TH1D>(xaxis);

   TRandom3 random;
   Float_t px, py;

   for(int n=0;n<10000;++n) {
      random.Rannor(px,py);
      pHist->Fill(px-2);
      pHist2->Fill(py+2);
   }

   // Create a canvas to be displayed.
   auto canvas = RCanvas::Create("Canvas Title");
   canvas->Draw(pHist)->SetLineColor(RColor::kRed);
   canvas->Draw(pHist2)->SetLineColor(RColor::kBlue);

//   canvas->Show();

   canvas->SaveAs("th1.jpg");
   canvas->SaveAs("th1.svg");
   canvas->SaveAs("th1.png");
}
