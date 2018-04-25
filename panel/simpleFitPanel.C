#include <ROOT/TWebWindowsManager.hxx>

#include <vector>
#include <string>

#include "TBufferJSON.h"
#include "TROOT.h"

struct ComboBoxItem {
   std::string fId;
   std::string fSet;
   ComboBoxItem() = default;
   ComboBoxItem(const std::string &id, const std::string &set) : fId(id), fSet(set) {}  
};


struct SimpleFitPanel {
   std::vector<ComboBoxItem> fDataSet;
   std::string fSelectDataId;
   std::vector<ComboBoxItem> fTypeFunc;
   std::string fSelectTypeId;
   std::vector<ComboBoxItem> fTypeXY;
   std::string fSelectXYId;
   float fMinRange;
   float fMaxRange;
   float fStep;
   float fRange[2];

   SimpleFitPanel() = default;
};


class WHandler {
private:
   std::shared_ptr<ROOT::Experimental::TWebWindow> fWindow;
   unsigned fConnId{0};

public:
   WHandler() {};

   virtual ~WHandler() { printf("Destructor!!!!\n"); }

   void ProcessData(unsigned connid, const std::string &arg)
   {
    	printf("Start here\n");
   		 
          fConnId = connid;
          //printf("connection established %u\n", fConnId);
          fWindow->Send(fConnId, "INITDONE");

	         SimpleFitPanel model;
	         model.fDataSet.push_back(ComboBoxItem("1", "No Selection"));
	         model.fDataSet.push_back(ComboBoxItem("2", "TH1F::hpx"));
	         model.fDataSet.push_back(ComboBoxItem("3", "TH2F::hpxhpy"));
	         model.fDataSet.push_back(ComboBoxItem("4", "TProfile::hprof"));
	         model.fDataSet.push_back(ComboBoxItem("5", "TNtuple::ntuple"));
           model.fSelectDataId = "2";

           model.fTypeFunc.push_back(ComboBoxItem("1", "User Func"));
           model.fTypeFunc.push_back(ComboBoxItem("2", "Predef-1D"));
           model.fSelectTypeId = "1";

           model.fTypeXY.push_back(ComboBoxItem("1", "gauson"));
           model.fTypeXY.push_back(ComboBoxItem("2", "expo"));
           model.fTypeXY.push_back(ComboBoxItem("3", "landau"));
           model.fTypeXY.push_back(ComboBoxItem("4", "pol1"));
           model.fSelectXYId = "1";

           model.fMinRange = -10;
           model.fMaxRange = 10;
           model.fStep = 1.5;
           model.fRange[0]  = -7;
           model.fRange[1] = 7;



	         TString json = TBufferJSON::ConvertToJSON(&model, gROOT->GetClass("SimpleFitPanel"));
	         fWindow->Send(fConnId, std::string("MODEL:") + json.Data());
	        
	         return;
	     
    	
    }

    void popupWindow(const std::string &where = "")
   {

      fWindow = ROOT::Experimental::TWebWindowsManager::Instance()->CreateWindow(false);

      // this is very important, it defines name of openui5 widget, which
      // will run on the client side
      fWindow->SetPanelName("localapp.view.SimpleFitPanel");

      // fWindow->SetDefaultPage("file:fclWithRouting.html");

      // this is call-back, invoked when message received via websocket
      fWindow->SetDataCallBack([this](unsigned connid, const std::string &arg) { ProcessData(connid, arg); });

      fWindow->SetGeometry(400, 400); // configure predefined geometry

      fWindow->Show(where);

      // instead showing of window just generate URL, which can be copied into the browser
      std::string url = fWindow->GetUrl(true);
      printf("Example: %s\n", url.c_str());
   }
};


WHandler* handler = nullptr;
void simpleFitPanel()
{
   handler = new WHandler();
   handler->popupWindow();
}



