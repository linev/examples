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

#include "ROOT/TFitPanel.hxx"
#include "ROOT/TDirectory.hxx"

void fitpanel0() 
{
  auto panel = std::make_shared<ROOT::Experimental::TFitPanel>("FitPanel Title");
  panel->Show();

  // Register the histogram with ROOT: now it lives even after draw() ends.
  ROOT::Experimental::TDirectory::Heap().Add("fitpanel", panel);
}


