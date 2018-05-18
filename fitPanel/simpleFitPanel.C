#include <ROOT/TWebWindowsManager.hxx>

#include <vector>
#include <string>

#include "TBufferJSON.h"
#include "TROOT.h"
#include "TH1.h"
#include "TAxis.h"
#include "TCanvas.h"

struct ComboBoxItem {
   std::string fId;
   std::string fSet;
   ComboBoxItem() = default;
   ComboBoxItem(const std::string &id, const std::string &set) : fId(id), fSet(set) {}
};


struct FitPanelModel {
   std::vector<ComboBoxItem> fDataSet;
   std::string fSelectDataId;
   std::vector<ComboBoxItem> fTypeFunc;
   std::string fSelectTypeId;
   std::vector<ComboBoxItem> fTypeXY;
   std::string fSelectXYId;
   std::vector<ComboBoxItem> fMethod;
   std::string fSelectMethodId;
   std::string realfunc;
   std::string _slider;


   // all combo items for all methods
   std::vector<std::vector<ComboBoxItem>> fMethodMinAll;

   std::vector<ComboBoxItem> fMethodMin;
   std::string fSelectMethodMinId;

   float fMinRange{0};
   float fMaxRange{1};
   float fStep{0.1};
   float fRange[2];
   float fOperation{0};
   float fFitOptions1{0};
   bool fLinear{false};
   bool fRobust{false};
   bool fWeights{false};
   bool fBins{false};
   int fLibrary{0};

};


class FitPanel {
private:
   std::shared_ptr<ROOT::Experimental::TWebWindow> fWindow;
   unsigned fConnId{0};
   TH1 *fHist{nullptr};

public:
   FitPanel() {};

   virtual ~FitPanel() {}

   void AssignHistogram(TH1 *hist)
   {
      fHist = hist;
   }

   void ProcessData(unsigned connid, const std::string &arg)
   {

      if (arg == "CONN_READY") {
         printf("Start here\n");

         fConnId = connid;
         //printf("connection established %u\n", fConnId);
         fWindow->Send(fConnId, "INITDONE");

         FitPanelModel model;
         model.fDataSet.push_back(ComboBoxItem("1", "No Selection"));
         model.fDataSet.push_back(ComboBoxItem("2", "TH1F::hpx"));
         model.fDataSet.push_back(ComboBoxItem("3", "TH2F::hpxhpy"));
         model.fDataSet.push_back(ComboBoxItem("4", "TProfile::hprof"));
         model.fDataSet.push_back(ComboBoxItem("5", "TNtuple::ntuple"));
         model.fSelectDataId = "2";

         model.fTypeFunc.push_back(ComboBoxItem("1", "User Func"));
         model.fTypeFunc.push_back(ComboBoxItem("2", "Predef-1D"));
         model.fSelectTypeId = "1";

         model.fTypeXY.push_back(ComboBoxItem("1", "gaus"));
         model.fTypeXY.push_back(ComboBoxItem("2", "expo"));
         model.fTypeXY.push_back(ComboBoxItem("3", "landau"));
         model.fTypeXY.push_back(ComboBoxItem("4", "pol1"));
         model.fSelectXYId = "1";

         model.fMethod.push_back(ComboBoxItem("1", "Chi-square"));
         model.fMethod.push_back(ComboBoxItem("2", "Binned Likelihood"));
         model.fSelectMethodId = "1";

         model.fSelectMethodMinId = "1";
         // corresponds to library == 0
         model.fMethodMinAll.emplace_back();
         std::vector<ComboBoxItem> &vect0 = model.fMethodMinAll.back();
         vect0.push_back(ComboBoxItem("1", "MIGRAD"));
         vect0.push_back(ComboBoxItem("2", "SIMPLEX"));
         vect0.push_back(ComboBoxItem("3", "SCAN"));
         vect0.push_back(ComboBoxItem("4", "Combination"));

         // corresponds to library == 1
         model.fMethodMinAll.emplace_back();
         std::vector<ComboBoxItem> &vect1 = model.fMethodMinAll.back();
         vect1.push_back(ComboBoxItem("1", "Lib1_1"));
         vect1.push_back(ComboBoxItem("2", "Lib1_2"));

         // corresponds to library == 2
         model.fMethodMinAll.emplace_back();
         std::vector<ComboBoxItem> &vect2 = model.fMethodMinAll.back();
         vect2.push_back(ComboBoxItem("1", "FUMILI"));

         // corresponds to library == 3
         model.fMethodMinAll.emplace_back();
         std::vector<ComboBoxItem> &vect3 = model.fMethodMinAll.back();
         // vect3.push_back(ComboBoxItem("1", "Lib3_1"));
         // vect3.push_back(ComboBoxItem("2", "Lib3_2"));

         // corresponds to library == 4
         model.fMethodMinAll.emplace_back();
         std::vector<ComboBoxItem> &vect4 = model.fMethodMinAll.back();
         vect4.push_back(ComboBoxItem("1", "TMVA Genetic Algorithm"));

         // select items list for initial display
         model.fMethodMin = model.fMethodMinAll[model.fLibrary];

         

         model.fMaxRange = 4;
         if (fHist) {
            model.fMinRange = fHist->GetXaxis()->GetXmin();
            model.fMaxRange = fHist->GetXaxis()->GetXmax();
         }

         model.fStep = (model.fMaxRange - model.fMinRange) / 100;
         model.fRange[0] = model.fMinRange;
         model.fRange[1] = model.fMaxRange;
         model.fOperation = 0;
         model.fFitOptions1 = 3;
         model.fLinear = false;
         model.fRobust = false;
         model.fWeights = false;
         model.fBins = false;
         model.fLibrary = 0;
        


         TString json = TBufferJSON::ConvertToJSON(&model, gROOT->GetClass("FitPanelModel"));
         fWindow->Send(fConnId, std::string("MODEL:") + json.Data());

         //printf("fLibrary is (%d)\n", model.fLibrary);
         printf("Slider %s\n", model._slider.c_str() );

         return;
      }
      if (arg.find("DOFIT:") == 0) {
         std::string arg1 = arg;
         arg1.erase(0,6);
         FitPanelModel *obj = nullptr;
         TBufferJSON::FromJSON(obj, arg1.c_str());
         if (obj) {
            printf("DOFIT: range %f %f select %s function %s\n ", obj->fRange[0], obj->fRange[1], obj->fSelectDataId.c_str(), obj->fSelectXYId.c_str());

            if(obj->fSelectXYId == "1"){
               obj->realfunc = "gaus";
               printf("%s\n", obj->realfunc.c_str());
            }
            else if (obj->fSelectXYId == "2"){
               obj->realfunc = "expo";
               printf("%s\n", obj->realfunc.c_str());
            }
            else if (obj->fSelectXYId == "3"){
               obj->realfunc = "landau";
               printf("%s\n", obj->realfunc.c_str());
            }
            else if (obj->fSelectXYId == "4"){
               obj->realfunc = "pol1";
               printf("%s\n", obj->realfunc.c_str());
            }
            else {
               obj->realfunc = "gaus";
               printf("%s\n", obj->realfunc.c_str());
            }
            
            if (fHist)
               fHist->Fit(obj->realfunc.c_str(), "", "", obj->fRange[0], obj->fRange[1]);
            delete obj;
         }

      }


   }

   void Show(const std::string &where = "")
   {

      fWindow = ROOT::Experimental::TWebWindowsManager::Instance()->CreateWindow(false);

      // this is very important, it defines name of openui5 widget, which
      // will run on the client side
      fWindow->SetPanelName("localapp.view.SimpleFitPanel");

      // fWindow->SetDefaultPage("file:fclWithRouting.html");

      // this is call-back, invoked when message received via websocket
      fWindow->SetDataCallBack([this](unsigned connid, const std::string &arg) { ProcessData(connid, arg); });

      fWindow->SetGeometry(450, 550); // configure predefined geometry

      fWindow->Show(where);

      // instead showing of window just generate URL, which can be copied into the browser
      std::string url = fWindow->GetUrl(true);
      printf("Example: %s\n", url.c_str());
   }
};


void simpleFitPanel()
{
   auto panel = new FitPanel();

   TH1F *hpx = new TH1F("hpx","This is the px distribution",100,-4,4);
   hpx->FillRandom("gaus", 10000);
   hpx->Draw();

   panel->AssignHistogram(hpx);

   panel->Show();
}

