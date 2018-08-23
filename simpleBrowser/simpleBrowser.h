#include <ROOT/TWebWindowsManager.hxx>

#include <vector>
#include <string>

#include "TBufferJSON.h"
#include "TROOT.h"
#include "TH1.h"
#include "TAxis.h"
#include "TCanvas.h"

struct TreeListItem {
   std::string text;
   std::vector<TreeListItem> treelist;
   TreeListItem() = default;
   TreeListItem(const std::string &text) : text(text) {}  
};


struct BrowserModel{
	std::vector<TreeListItem> ftree;
};


class SimpleBrowser {
private:
	std::shared_ptr<ROOT::Experimental::TWebWindow> fWindow;
	unsigned fConnId{0};
	//TH1 *fHist{nullptr};

public:
	SimpleBrowser() {};

	virtual ~SimpleBrowser() {};


	void ProcessData(unsigned connid, const std::string &arg){

		if (arg == "CONN_READY"){
			printf("Start Here\n");

			fConnId = connid;
			fWindow->Send(fConnId, "INITDONE");

			BrowserModel model;

			//Tree Data
	         model.ftree.push_back(TreeListItem("Node 1"));
	         model.ftree.push_back(TreeListItem("Node 2"));
	         model.ftree.back().treelist.push_back(TreeListItem("Node 2-1"));
	         model.ftree.back().treelist.push_back(TreeListItem("Node 2-2"));
	         model.ftree.push_back(TreeListItem("Node3"));

         TString json = TBufferJSON::ConvertToJSON(&model, gROOT->GetClass("BrowserModel"));
         fWindow->Send(fConnId, std::string("MODEL:") + json.Data());

		}
	}

	void Show(const std::string	&where = ""){
		fWindow = ROOT::Experimental::TWebWindowsManager::Instance()->CreateWindow(false);

		fWindow->SetPanelName("localapp.view.SimpleBrowser");

		fWindow->SetDataCallBack([this](unsigned connid, const std::string &arg) {ProcessData(connid, arg); });
		fWindow->SetGeometry(450, 550);

		fWindow->Show(where);

		std::string url = fWindow->GetUrl(true);
		printf("Example: %s\n",url.c_str() );
	}

};