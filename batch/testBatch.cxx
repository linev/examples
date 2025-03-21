/// \file
/// \ingroup tutorial_http
///  This program show usage of batch JSROOT graphics inside RWebWindow
///
/// \macro_code
///
/// \author Sergey Linev


#include <ROOT/RWebWindow.hxx>

#include "TFile.h"
#include "TH2.h"
#include "TEnv.h"

#include "TBufferJSON.h"
#include "TBase64.h"

#include <fstream>

class WHandler {
private:
   std::shared_ptr<ROOT::RWebWindow>  fWindow;
   unsigned fConnId = 0;
   int fImgCnt = 0;

public:
   WHandler() {};

   virtual ~WHandler() { printf("Destructor!!!!\n"); }

   void ProcessConnection(unsigned connid)
   {
      fConnId = connid;
      fImgCnt = 0;
      printf("connection established %u\n", fConnId);
   }

   void ProcessData(unsigned connid, const std::string &arg)
   {
      if (arg == "Init") {
         TFile *f = TFile::Open("https://root.cern/js/files/hsimple.root");
         if (!f) return;

         TH2 *h = (TH2 *) f->Get("hpxpy");

         TString json = TBufferJSON::ToJSON(h);

         fWindow->Send(fConnId, json.Data()); // send histogram object

         return;
      }

      if (arg.substr(0,4) == "PNG:") {
         printf("GET PNG IMAGE len = %lu\n", arg.length() - 4);

         auto pos = arg.find("base64,");

         TString binary = TBase64::Decode(arg.c_str() + pos + 7);

         std::ofstream ofs("file.png", std::ios::binary);
         ofs.write(binary.Data(), binary.Length());
         ofs.close();

         printf("Create file.png size %u\n", (unsigned) binary.Length());

         if (++fImgCnt == 2) {
            fWindow->CloseConnection(connid);
            fWindow->TerminateROOT();
         }

         return;
      }

      if (arg.substr(0,4) == "SVG:") {
         printf("GET SVG IMAGE len = %lu\n", arg.length()-4);

         std::ofstream ofs("file.svg", std::ios::binary);
         ofs.write(arg.c_str()+4, arg.length()-4);
         ofs.close();

         printf("Create file.png size %u\n", (unsigned) arg.length()-4);


         if (++fImgCnt == 2) {
            fWindow->CloseConnection(connid);
            fWindow->TerminateROOT();
         }

         return;
      }

      if (arg.substr(0,4) == "DBG:") {
         printf("Debug: %s\n", arg.c_str() + 4);
         return;
      }

      printf("msg -> %s\n", arg.c_str());
   }

   void popupTest()
   {

      fWindow =  ROOT::RWebWindow::Create();

      // this is very important, it defines name of openui5 widget, which
      // will run on the client side
      // fWindow->SetPanelName("TextTest");

      fWindow->SetDefaultPage("file:testBatch.html");

      fWindow->SetConnectCallBack([this](unsigned connid) { ProcessConnection(connid); });

      // this is call-back, invoked when message received via websocket
      fWindow->SetDataCallBack([this](unsigned connid, const std::string &arg) { ProcessData(connid, arg); });

      fWindow->SetGeometry(300, 300); // configure predefined geometry  900x700

      ROOT::RWebDisplayArgs args;
      // special browser mode when window is not shown, but all functionality can be used
      // works only with --web=chrome and --web=firefox
      if (gROOT->IsBatch()) {
         printf("Start browser in headless mode and just wait for connection\n");
         args.SetHeadless(true);
      }

      fWindow->Show(args);
   }

};

WHandler* handler = nullptr;

void testBatch()
{
   handler = new WHandler();

   handler->popupTest();
}
