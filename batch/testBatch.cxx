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
#include "TBase64.h"

#include <fstream>

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

      if (arg == "Init") {
         TFile *f = TFile::Open("https://root.cern/js/files/hsimple.root");
         if (!f) return;

         TH2 *h = (TH2 *) f->Get("hpxpy");

         TString json = TBufferJSON::ToJSON(h);

         fWindow->Send(fConnId, json.Data()); // send histogram object

         return;
      }

      if (arg.substr(0,3) == "IMG") {
         printf("GET IMAGE len = %lu\n", arg.length()-3);

         TString binary = TBase64::Decode(arg.c_str()+3);

         std::ofstream ofs("file.png", std::ios::binary);
         ofs.write(binary.Data(), binary.Length());
         ofs.close();

         return;
      }

      printf("Get msg:\n%s\n", arg.c_str());
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

      fWindow->SetGeometry(600, 400); // configure predefined geometry  900x700

      fWindow->Show(where);
   }

};

WHandler* handler = nullptr;

void testBatch()
{
   handler = new WHandler();

   handler->popupTest();
}
