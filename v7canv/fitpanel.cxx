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
#include "ROOT/TFitPanel.hxx"
#include "ROOT/TDirectory.hxx"


void fitpanel0() {
  using namespace ROOT;

  // Create the histogram.
  TAxisConfig xaxis(10, 0., 10.);
  auto pHist = std::make_shared<TH1D>(xaxis);

  // Fill a few points.
  pHist->Fill(1);
  pHist->Fill(2);
  pHist->Fill(2);
  pHist->Fill(3);


  auto panel = std::make_shared<ROOT::TFitPanel>("FitPanel Title");
  panel->Show();

  // Register the histogram with ROOT: now it lives even after draw() ends.
  TDirectory::Heap().Add("fitpanel", panel);


  // Create a canvas to be displayed.
  // auto canvas = TCanvas::Create("Canvas Title");
  // canvas->Draw(pHist)->SetLineColor(TColor::kRed);
  // canvas->Draw(pHist2)->SetLineColor(TColor::kBlue);

  // canvas->Show();
}

void fitpanel() {

   using namespace ROOT;

   // TODO - also keep axis correctly in the help
   auto xaxis = std::make_shared<TAxisConfig>(10, 0., 10.);
   // Create the histogram.
   auto pHist = std::make_shared<TH1D>(*xaxis.get());

   // Fill a few points.
   pHist->Fill(1);
   pHist->Fill(2);
   pHist->Fill(2);
   pHist->Fill(3);

   auto canvas = TCanvas::Create("Canvas Title");
   canvas->Draw(pHist)->SetLineColor(TColor::kRed);

   canvas->Show();
   canvas->Update(); // need to ensure canvas is drawn

   auto panel = std::make_shared<ROOT::TFitPanel>("FitPanel Title");

   TDirectory::Heap().Add("fitpanel", panel);
   TDirectory::Heap().Add("firsthisto", pHist);
   TDirectory::Heap().Add("firstaxis", xaxis);

   // TODO: how combine there methods together
   // here std::shread_ptr<> on both sides

   panel->UseCanvas(canvas);

   canvas->AddPanel(panel);
}

