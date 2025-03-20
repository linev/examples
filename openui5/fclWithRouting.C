/// \file
/// \ingroup tutorial_http
///  This program creates and fills one and two dimensional histogram
///  Macro used to demonstrate usage of custom HTML page in custom.htm
///  One can use plain JavaScript to assign different actions with HTML buttons
///
/// \macro_code
///
/// \author Sergey Linev


#include <ROOT/RWebWindowsManager.hxx>

class WHandler {
private:
   std::shared_ptr<ROOT::RWebWindow>  fWindow;
   unsigned fConnId{0};

public:
   WHandler() {};

   virtual ~WHandler() { printf("Destructor!!!!\n"); }

   void ProcessData(unsigned connid, const std::string &arg)
   {
      if (arg == "CONN_READY") {
         fConnId = connid;
         printf("connection established %u\n", fConnId);
         fWindow->Send(fConnId, "INITDONE");

         return;
      }

      if (arg == "CONN_CLOSED") {
         printf("connection closed\n");
         fConnId = 0;
         return;
      }

      printf("Get msg %s \n", arg.c_str());

   }

   void popupTextTest(const std::string &where = "")
   {

      fWindow =  ROOT::RWebWindowsManager::Instance()->CreateWindow();

      // this is very important, it defines name of openui5 widget, which
      // will run on the client side
      // fWindow->SetPanelName("TextTest");

      fWindow->SetDefaultPage("file:fclWithRouting.html");

      // this is call-back, invoked when message received via websocket
      fWindow->SetDataCallBack([this](unsigned connid, const std::string &arg) { ProcessData(connid, arg); });

      fWindow->SetGeometry(300, 500); // configure predefined geometry

      fWindow->Show(where);
   }

};

WHandler* handler = nullptr;

void fclWithRouting()
{
   handler = new WHandler();

   handler->popupTextTest();
}
