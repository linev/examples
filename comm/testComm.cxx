/// \file
/// \ingroup tutorial_http
///  This program creates and fills one and two dimensional histogram
///  Macro used to demonstrate usage of custom HTML page in custom.htm
///  One can use plain JavaScript to assign different actions with HTML buttons
///
/// \macro_code
///
/// \author Sergey Linev


#include <ROOT/RWebWindow.hxx>

class WHandler {
private:
   std::shared_ptr<ROOT::RWebWindow>  fWindow;
   unsigned fConnId = 0;

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

      printf("Get msg %s \n", arg.c_str());

      float arr[10000];
      for (int n=0;n<10000;++n)
         arr[n] = n;

      for (int n=0;n<8;n++) {
         fWindow->Send(fConnId, TString::Format("Message%d",n).Data());
         arr[0] = n;
         fWindow->SendBinary(fConnId, arr, (n%2 == 0) ? sizeof(arr) : 40);
      }
   }

   void popupTest()
   {
      fWindow = ROOT::RWebWindow::Create();

      fWindow->SetDefaultPage("file:testComm.html");

      // this is call-back, invoked when message received via websocket
      fWindow->SetDataCallBack([this](unsigned connid, const std::string &arg) { ProcessData(connid, arg); });

      fWindow->SetGeometry(300, 700); // configure predefined geometry

      fWindow->Show();
   }

};

WHandler* handler = nullptr;

void testComm()
{
   handler = new WHandler();

   handler->popupTest();
}
