/// \file
/// \ingroup tutorial_webgui
///  This macro create simple testpanel, based on webgui
/// \macro_code
///
/// \author Sergey Linev


#include <ROOT/RWebWindow.hxx>

#include <vector>
#include <string>

#include "TBufferJSON.h"
#include "TROOT.h"

struct ComboBoxItem {
   std::string fId;
   std::string fName;
   ComboBoxItem() = default;
   ComboBoxItem(const std::string &id, const std::string &name) : fId(id), fName(name) {}
};

struct TestPanelModel {
   std::vector<ComboBoxItem> fDataNames;
   std::string fSelectDataId;
   std::vector<ComboBoxItem> fModelNames;
   std::string fSelectModelId;
   TestPanelModel() = default;
};


class WHandler {
private:
   std::shared_ptr<ROOT::RWebWindow> fWindow;
   unsigned fConnId{0};

public:
   WHandler() {}

   void ProcessConnection(unsigned connid)
   {
      fConnId = connid;

      printf("connection established %u\n", fConnId);
      fWindow->Send(fConnId, "INITDONE");

      TestPanelModel model;
      model.fDataNames.push_back(ComboBoxItem("1", "RootData1"));
      model.fDataNames.push_back(ComboBoxItem("2", "RootData2"));
      model.fDataNames.push_back(ComboBoxItem("3", "RootData3"));
      model.fDataNames.push_back(ComboBoxItem("4", "RootData-4"));
      model.fDataNames.push_back(ComboBoxItem("5", "RootData-5"));
      model.fDataNames.push_back(ComboBoxItem("6", "RootData-6"));
      model.fSelectDataId = "1";

      model.fModelNames.push_back(ComboBoxItem("1", "RootModel1"));
      model.fModelNames.push_back(ComboBoxItem("2", "RootModel2"));
      model.fModelNames.push_back(ComboBoxItem("3", "RootModel3"));
      model.fSelectModelId = "3";

      TString json = TBufferJSON::ConvertToJSON(&model, gROOT->GetClass("TestPanelModel"));

      fWindow->Send(fConnId, std::string("MODEL:") + json.Data());

   }

   void ProcessData(unsigned connid, const std::string &arg)
   {
      if (arg == "GET_BINARY") {
         float arr[1000];
         for (int n=0;n<1000;++n) arr[n] = n*1.11111;

         // send binary data, deep copy will be performed
         fWindow->SendBinary(fConnId, arr, sizeof(arr));
         return;
      }

      if (arg.find("LOG:") == 0) {
         printf("Msg: %s\n", arg.c_str()+4);
         return;
      }

      printf("Get msg %s \n", arg.c_str());

   }

   void popupWindow()
   {

      printf("Example does not work while sap.ui.table libs is not loaded\n");

      fWindow = ROOT::RWebWindow::Create();

      // this is very important, it defines name of openui5 widget, which
      // will run on the client side
      fWindow->SetPanelName("localapp.view.TestTables");

		fWindow->SetConnectCallBack([this](unsigned connid) { ProcessConnection(connid); });

      // this is call-back, invoked when message received via websocket
      fWindow->SetDataCallBack([this](unsigned connid, const std::string &arg) { ProcessData(connid, arg); });

      fWindow->SetGeometry(900, 500); // configure predefined geometry

      fWindow->Show();
   }
};


WHandler* handler = nullptr;

void testTables()
{
   handler = new WHandler();
   handler->popupWindow();
}
