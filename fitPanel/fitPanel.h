#include <ROOT/TWebWindowsManager.hxx>

#include <vector>
#include <string>

#include "TBufferJSON.h"
#include "TROOT.h"
#include "TH1.h"
#include "TAxis.h"
#include "TCanvas.h"

// Structure for the ComboBox and Tree models

struct ComboBoxItem {
   std::string fId;
   std::string fSet;
   ComboBoxItem() = default;
   ComboBoxItem(const std::string &id, const std::string &set) : fId(id), fSet(set) {}
};

//Structure for the main fit panel model
struct FitPanelModel {
   std::vector<ComboBoxItem> fDataSet;
   std::string fSelectDataId;
   std::vector<ComboBoxItem> fTypeFunc;
   std::string fSelectXYId;
   std::vector<ComboBoxItem> fMethod;
   std::string fSelectMethodId;
   std::string fRealFunc;
   std::string fOption;
   std::string fFuncChange;

   // all combo items for all methods

   //Minimization Tab
   std::vector<std::vector<ComboBoxItem>> fMethodMinAll;
   //Fit Function --- Type
   std::vector<std::vector<ComboBoxItem>> fTypeXYAll;

   std::vector<ComboBoxItem> fMethodMin;
   std::vector<ComboBoxItem> fTypeXY;

   std::string fSelectTypeId;
   std::string fSelectMethodMinId;

   float fMinRange{0};
   float fMaxRange{1};
   float fStep{0.1};
   float fRange[2];
   float fOperation{0};
   float fFitOptions{0};
   bool fLinear{false};
   bool fRobust{false};
   int fLibrary{0};
   int fPrint{0};

   //convert fSelectTypeID from string to int
   int fTypeId = atoi(fSelectTypeId.c_str());
   int fFuncChangeInt = atoi(fFuncChange.c_str());

   //Checkboxes Options
   bool fIntegral{false};
   bool fBestErrors {false};
   bool fWeights{false};
   bool fBins{false};
   bool fUseRange {false};
   bool fImproveFit {false};
   bool fAddList {false};
   bool fUseGradient {false};
   bool fSame {false};
   bool fNoDrawing {};
   bool fNoStore {false};

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
         vec0.push_back(ComboBoxItem("2", "gausn"));
         vec0.push_back(ComboBoxItem("3", "expo"));
         vec0.push_back(ComboBoxItem("4", "landau"));
         vec0.push_back(ComboBoxItem("5", "landaun"));
         vec0.push_back(ComboBoxItem("6", "pol0"));
         vec0.push_back(ComboBoxItem("7", "pol1"));
         vec0.push_back(ComboBoxItem("8", "pol2"));
         vec0.push_back(ComboBoxItem("9", "pol3"));
         vec0.push_back(ComboBoxItem("10", "pol4"));
         vec0.push_back(ComboBoxItem("11", "pol5"));
         vec0.push_back(ComboBoxItem("12", "pol6"));
         vec0.push_back(ComboBoxItem("13", "pol7"));
         vec0.push_back(ComboBoxItem("14", "pol8"));
         vec0.push_back(ComboBoxItem("15", "pol9"));
         vec0.push_back(ComboBoxItem("16", "cheb0"));
         vec0.push_back(ComboBoxItem("17", "cheb1"));
         vec0.push_back(ComboBoxItem("18", "cheb2"));
         vec0.push_back(ComboBoxItem("19", "cheb3"));
         vec0.push_back(ComboBoxItem("20", "cheb4"));
         vec0.push_back(ComboBoxItem("21", "cheb5"));
         vec0.push_back(ComboBoxItem("22", "cheb6"));
         vec0.push_back(ComboBoxItem("23", "cheb7"));
         vec0.push_back(ComboBoxItem("24", "cheb8"));
         vec0.push_back(ComboBoxItem("25", "cheb9"));
         vec0.push_back(ComboBoxItem("26", "user"));


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
         model.fFitOptions = 3;
         model.fRobust = false;
         model.fLibrary = 0;
         model.fPrint = 0;

         //Checkboxes Values
         model.fIntegral = false;
         model.fWeights = false;
         model.fBins = false;
         model.fUseRange = false;
         model.fAddList = false;
         model.fUseGradient = false;
         model.fSame = false;
         model.fNoStore = false;
         model.fBestErrors = false;
         model.fImproveFit = false;

         if(model.fNoStore){
            model.fNoDrawing = true;
         }
         else{
            model.fNoDrawing = false;
         }

         if((model.fFuncChangeInt >= 6) && (model.fFuncChangeInt <= 15)){
            model.fLinear = true;

         }
         else {
            model.fLinear = false;

         }

         //Communication with the JSONModel in JS
         // TString json = TBufferJSON::ConvertToJSON(&model, gROOT->GetClass("FitPanelModel"));
         TString json = TBufferJSON::ToJSON(&model);

         fWindow->Send(fConnId, std::string("MODEL:") + json.Data());

         return;
      }
      //FITTING
      if (arg.find("DOFIT:") == 0) {
         std::string arg1 = arg;
         arg1.erase(0,6);
         FitPanelModel *obj = nullptr;
         TBufferJSON::FromJSON(obj, arg1.c_str());
         //Fitting Options
         if (obj) {
            printf("DOFIT: range %f %f select %s function %s\n ", obj->fRange[0], obj->fRange[1], obj->fSelectDataId.c_str(), obj->fSelectXYId.c_str());

            if (!obj->fRealFunc.empty()) {
               printf("GOT fRealFunc: %s\n", obj->fRealFunc.c_str());
            }
            else {
               obj->fRealFunc = "gaus";
               printf("%s\n", obj->fRealFunc.c_str());
            }

            if(obj->fIntegral){
               obj->fOption = "I";
            }
            else if(obj->fBestErrors){
               obj->fOption = "E";
            }
            else if(obj->fWeights){
               obj->fOption = "W";
            }
            else if(obj->fUseRange){
               obj->fOption = "R";
            }
            else if(obj->fImproveFit){
               obj->fOption = "M";
            }
            else if(obj->fNoDrawing){
               obj->fOption = "O";
            }
            else if(obj->fNoStore){
               obj->fOption = "N";
            }
            else if((obj->fWeights) && (obj->fBins)){
               obj->fOption = "WW";
            }
            else if(obj->fAddList){
               obj->fOption = "+";
            }
            else if(obj->fUseGradient){
               obj->fOption = "G";
            }
            else if(obj->fSelectMethodId == "1"){
               obj->fOption = "P";
            }
            else if(obj->fSelectMethodId == "2"){
               obj->fOption = "L";
            }
            else {
               obj->fOption = "";
            }

            //Assign the options to Fitting function
            if (fHist) {
               fHist->Fit(obj->fRealFunc.c_str(), obj->fOption.c_str(), "*", obj->fRange[0], obj->fRange[1]);
               gPad->Update();
            }
            delete obj;
         }

      }


   }

   //Create the window
   void Show(const std::string &where = "")
   {

      fWindow = ROOT::Experimental::TWebWindowsManager::Instance()->CreateWindow();

      // this is very important, it defines name of openui5 widget, which
      // will run on the client side
      fWindow->SetPanelName("localapp.view.SimpleFitPanel");

      //fWindow->SetDefaultPage("file:fclWithRouting.html");

      // this is call-back, invoked when message received via websocket
      fWindow->SetDataCallBack([this](unsigned connid, const std::string &arg) { ProcessData(connid, arg); });

      fWindow->SetGeometry(450, 550); // configure predefined geometry

      fWindow->Show(where);

      // instead showing of window just generate URL, which can be copied into the browser
      std::string url = fWindow->GetUrl(true);
      printf("Example: %s\n", url.c_str());
   }
};

