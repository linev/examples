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
   std::string fSelectXYId;
   std::vector<ComboBoxItem> fMethod;
   std::string fSelectMethodId;
   std::string realfunc;


   // all combo items for all methods

   //Minimization Tab
   std::vector<std::vector<ComboBoxItem>> fMethodMinAll;
   //Fit Function --- Type
   std::vector<std::vector<ComboBoxItem>> fTypeXYAll;;

   std::vector<ComboBoxItem> fMethodMin;
   std::vector<ComboBoxItem> fTypeXY;

   std::string fSelectTypeId;
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
   //convert fSelectTypeID from string to int
   int fTypeId = atoi(fSelectTypeId.c_str());

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
         //ComboBox for Data Set
         model.fDataSet.push_back(ComboBoxItem("1", "No Selection"));
         model.fDataSet.push_back(ComboBoxItem("2", "TH1F::hpx"));
         model.fDataSet.push_back(ComboBoxItem("3", "TH2F::hpxhpy"));
         model.fDataSet.push_back(ComboBoxItem("4", "TProfile::hprof"));
         model.fDataSet.push_back(ComboBoxItem("5", "TNtuple::ntuple"));
         model.fSelectDataId = "2";

         //ComboBox for Fit Function --- Type
         model.fTypeFunc.push_back(ComboBoxItem("0", "Predef-1D"));
         model.fTypeFunc.push_back(ComboBoxItem("1", "User Func"));
         model.fSelectTypeId = "0";
         
         //Sub ComboBox for Type Function
         model.fSelectXYId = "1";

         //corresponds when Type == Predef-1D (fSelectedTypeID == 0)
         model.fTypeXYAll.emplace_back();
         std::vector<ComboBoxItem> &vec0 = model.fTypeXYAll.back();
         vec0.push_back(ComboBoxItem("1", "gaus"));
         vec0.push_back(ComboBoxItem("2", "expo"));
         vec0.push_back(ComboBoxItem("3", "landau"));
         vec0.push_back(ComboBoxItem("4", "pol1"));


         //corresponds when Type == User Func (fSelectedTypeID == 1)
         model.fTypeXYAll.emplace_back();
         std::vector<ComboBoxItem> &vec1 = model.fTypeXYAll.back();
         vec1.push_back(ComboBoxItem("1", "chebyshev0"));
         vec1.push_back(ComboBoxItem("2", "chebyshev1"));
         vec1.push_back(ComboBoxItem("3", "chebyshev2"));
         vec1.push_back(ComboBoxItem("4", "chebyshev3"));
         vec1.push_back(ComboBoxItem("5", "chebyshev4"));
         vec1.push_back(ComboBoxItem("6", "chebyshev5"));
         vec1.push_back(ComboBoxItem("7", "chebyshev6"));
         vec1.push_back(ComboBoxItem("8", "chebyshev7"));
         vec1.push_back(ComboBoxItem("9", "chebyshev8"));
         vec1.push_back(ComboBoxItem("10", "chebyshev9"));

         //ComboBox for General Tab --- Method
         model.fMethod.push_back(ComboBoxItem("1", "Chi-square"));
         model.fMethod.push_back(ComboBoxItem("2", "Binned Likelihood"));
         model.fSelectMethodId = "1";

         //Sub ComboBox for Minimization Tab --- Method
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
         vect1.push_back(ComboBoxItem("1", "MIGRAD"));
         vect1.push_back(ComboBoxItem("2", "SIMPLEX"));
         vect1.push_back(ComboBoxItem("3", "SCAN"));
         vect1.push_back(ComboBoxItem("4", "Combination"));

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
         model.fTypeXY = model.fTypeXYAll[model.fTypeId];

         model.fMinRange = -4;
         model.fMaxRange = 4;
         if (fHist) {
            model.fMinRange = fHist->GetXaxis()->GetXmin();
            model.fMaxRange = fHist->GetXaxis()->GetXmax();
         }

         //defined values
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
         //printf("fTypeId is %d\n", model.fTypeId );


         return;
      }
      //FITTING
      if (arg.find("DOFIT:") == 0) {
         std::string arg1 = arg;
         arg1.erase(0,6);
         FitPanelModel *obj = nullptr;
         TBufferJSON::FromJSON(obj, arg1.c_str());
         if (obj) {
            printf("DOFIT: range %f %f select %s function %s\n ", obj->fRange[0], obj->fRange[1], obj->fSelectDataId.c_str(), obj->fSelectXYId.c_str());

            if (!obj->realfunc.empty()) {
               printf("GOT realfunc: %s\n", obj->realfunc.c_str());
            } 
            else {
               obj->realfunc = "gaus";
               printf("%s\n", obj->realfunc.c_str());
            }

            if (fHist)
               fHist->Fit(obj->realfunc.c_str(), "I", "", obj->fRange[0], obj->fRange[1]);
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
   hpx->FillRandom("expo", 10000);
   hpx->Draw();

   panel->AssignHistogram(hpx);

   panel->Show();
}

