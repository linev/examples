/// \file
/// \ingroup tutorial_http
///  This program show usage of JSROOT graphics inside batch RWebWindow
///
/// \macro_code
///
/// \author Sergey Linev


#include <ROOT/TWebWindowsManager.hxx>

#include "TFile.h"
#include "TH2.h"

#include "TBufferJSON.h"

class WHandler {
private:
   std::shared_ptr<ROOT::Experimental::TWebWindow>  fWindow;
   unsigned fConnId{0};

public:
   WHandler() {};

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

      if (arg != "Init") {
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

         return;
      }

      // example of reading canvas from other file

      TFile *f = TFile::Open("https://root.cern/js/files/hsimple.root");
      if (!f) return;

      TH2 *h = (TH2 *) f->Get("hpxpy");

      TString json = TBufferJSON::ToJSON(h);

      fWindow->Send(fConnId, json.Data()); // send histogram object
   }

   void popupTest(const std::string &where = "")
   {

      fWindow =  ROOT::Experimental::TWebWindowsManager::Instance()->CreateWindow();

      // this is very important, it defines name of openui5 widget, which
      // will run on the client side
      // fWindow->SetPanelName("TextTest");

      fWindow->SetDefaultPage("file:testBatch.html");

      // this is call-back, invoked when message received via websocket
      fWindow->SetDataCallBack([this](unsigned connid, const std::string &arg) { ProcessData(connid, arg); });

      fWindow->SetGeometry(800, 500); // configure predefined geometry

      fWindow->Show(where);
   }

};

WHandler* handler = nullptr;

void testBatch()
{
   handler = new WHandler();

   handler->popupTest();
}
