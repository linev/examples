/// \file
/// \ingroup tutorial_v7
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

#include "ROOT/THist.hxx"
#include "ROOT/TCanvas.hxx"

void draw_two() {
  using namespace ROOT;

  // Create the histogram.
  TAxisConfig xaxis(10, 0., 10.);
  auto pHist = std::make_shared<TH1D>(xaxis);
  auto pHist2 = std::make_shared<TH1D>(xaxis);

  // Fill a few points.
  pHist->Fill(1);
  pHist->Fill(2);
  pHist->Fill(2);
  pHist->Fill(3);

  pHist2->Fill(5);
  pHist2->Fill(6);
  pHist2->Fill(6);
  pHist2->Fill(7);

  // Create a canvas to be displayed.
  auto canvas = TCanvas::Create("Canvas Title");

  auto draw1 = canvas->Draw(pHist);
  draw1->SetLineColor(TColor::kRed);

  canvas->Draw(pHist2)->SetLineColor(TColor::kBlue);

  canvas->Show();
 
}
