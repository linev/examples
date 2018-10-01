/// \file
/// \ingroup tutorial_http
///  This program show simple usage of JSROOT graphics inside RWebWindow
///
/// \macro_code
///
/// \author Sergey Linev


#include <ROOT/TWebWindowsManager.hxx>

#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"


#include "TBufferJSON.h"

class WHandler {
private:
   std::shared_ptr<ROOT::Experimental::TWebWindow>  fWindow;
   unsigned fConnId{0};
   bool fSendCanvas{false};

public:
   WHandler(bool send_canv = true) : fSendCanvas(send_canv) {};

   virtual ~WHandler() { printf("Destructor!!!!\n"); }

   void ProcessData(unsigned connid, const std::string &arg)
   {
      if (arg == "CONN_READY") {
         fConnId = connid;
         printf("connection established %u\n", fConnId);
         //fWindow->Send(fConnId, "INITDONE");
         return;
      }

      if (arg == "CONN_CLOSED") {
         printf("connection closed\n");
         fConnId = 0;
         return;
      }

      if (arg!="Init") {
         printf("Get msg:\n%s\n", arg.c_str());
         // printf("Get msg %s\n", arg.substr(0,200).c_str());

         /* TGraphAsymmErrors *gr3 = nullptr;
         TBufferJSON::FromJSON(gr3, arg.c_str());
         if (gr3) {
            printf("Graph class %s\n", gr3->ClassName());
            TCanvas *c3 = new TCanvas("c3","title3", 3);
            gr3->Draw();
         }
         */

         TCanvas *c3 = nullptr;
         TBufferJSON::FromJSON(c3, arg.c_str());
         if (c3) { printf("Get class %s\n", c3->ClassName());  c3->SetName("c3"); c3->Draw(); }
            else { printf("Fail to parse canvas from JSROOT\n"); }

         return;
      }

      // example of reading canvas from other file

      /*
      TFile *f = TFile::Open("canvas_h1.root");
      if (!f) return;

      TCanvas *c1 = (TCanvas *) f->Get("c1");

      TString json = TBufferJSON::ToJSON(c1);

      // TCanvas *c1 = new TCanvas("c1","title", 3);
      c1->Draw();

      fWindow->Send(fConnId, json.Data()); // send graph object
      */



      TFile *f = TFile::Open("cosmic-rays.root");
      if (!f) return;

      TGraph *gr = (TGraph *) f->Get("graph1");

      if (fSendCanvas) {

         // if graphs does not draw, it does not have histogram object
         TCanvas *c1 = new TCanvas("c1","title", 3);
         gr->Draw();

         TString json = TBufferJSON::ToJSON(c1,3);
         fWindow->Send(fConnId, json.Data());  // send complete canvas

         TCanvas *c2 = nullptr;
         TBufferJSON::FromJSON(c2, json.Data());
         if (c2) { printf("Canvas with TGraph is %s\n", c2->ClassName()); c2->SetName("c2"); }
            else printf("Fail to parse JSON with TCanvas and TGraphAsymmErrors\n");
      } else {
         gr->SetHistogram(nullptr); // be sure - no histograms send

         TString json = TBufferJSON::ToJSON(gr);
         fWindow->Send(fConnId, json.Data()); // send graph object
         TGraphAsymmErrors *gr2 = nullptr;
         TBufferJSON::FromJSON(gr2, json.Data());
         if (gr2) printf("Graph class %s\n", gr2->ClassName());
             else printf("Fail to parse JSON with TGraphAsymmErrors\n");
      }
   }

   void popupTest(const std::string &where = "")
   {

      fWindow =  ROOT::Experimental::TWebWindowsManager::Instance()->CreateWindow();

      // this is very important, it defines name of openui5 widget, which
      // will run on the client side
      // fWindow->SetPanelName("TextTest");

      fWindow->SetDefaultPage("file:testGraphics.html");

      // this is call-back, invoked when message received via websocket
      fWindow->SetDataCallBack([this](unsigned connid, const std::string &arg) { ProcessData(connid, arg); });

      fWindow->SetGeometry(800, 500); // configure predefined geometry

      fWindow->Show(where);
   }

};

WHandler* handler = nullptr;

void testGraphics(bool send_canvas = false)
{
   handler = new WHandler(send_canvas);

   handler->popupTest();
}
