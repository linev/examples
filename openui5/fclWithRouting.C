///  This demo starts openui5 widget inside RWebWindow
///
///
/// \author Sergey Linev


#include <ROOT/RWebWindow.hxx>

class WHandler {
private:
   std::shared_ptr<ROOT::RWebWindow>  fWindow;
   unsigned fConnId = 0;

public:
   WHandler() {}

   void ProcessConnection(unsigned connid)
   {
      fConnId = connid;
      printf("connection established %u\n", fConnId);
      fWindow->Send(fConnId, "INITDONE");
   }

   void ProcessData(unsigned connid, const std::string &arg)
   {
      printf("Get msg %s \n", arg.c_str());

      if (arg == "QUIT_ROOT")
         fWindow->TerminateROOT();
   }

   void popupWindow()
   {
      fWindow = ROOT::RWebWindow::Create();

      fWindow->SetDefaultPage("file:fclWithRouting.html");

      fWindow->SetUseCurrentDir(true);

      fWindow->SetConnectCallBack([this](unsigned connid) { ProcessConnection(connid); });

      // this is call-back, invoked when message received via websocket
      fWindow->SetDataCallBack([this](unsigned connid, const std::string &arg) { ProcessData(connid, arg); });

      fWindow->SetGeometry(800, 600); // configure predefined geometry

      fWindow->Show();
   }

};

WHandler* handler = nullptr;

void fclWithRouting()
{
   handler = new WHandler();

   handler->popupWindow();
}
