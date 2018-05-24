#include "fitPanel.h"


void simpleFitPanel()
{
   auto panel = new FitPanel();

   TH1F *hpx = new TH1F("hpx","This is the px distribution",100,-4,4);
   hpx->FillRandom("gaus", 10000);
   hpx->Draw();

   panel->AssignHistogram(hpx);

   panel->Show();
}

