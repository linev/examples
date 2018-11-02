
{
    //Define constant
    Int_t nbSteps = 1000;
    Double_t xMin = -10.0;
    Double_t xMax = 10.0;

    //Create the function
    TF1 *myFunction = new TF1("function", "x*sin(x)",xMin,xMax);
    // Create a tree in which results will be stored
    TNtupleD *myTree = new TNtupleD("tree", "Example", "x:y:index");

    //Fill the tree
    for(Int_t i = 0; i < nbSteps; i++)
    {
        Double_t v1 = xMin + i * (xMax-xMin)/nbSteps;
        Double_t v2 = myFunction->Eval(v1) + gRandom->Gaus(0.0, 0.1);
        myTree->Fill(v1,v2,(Double_t)(i+1));
    }

    TCanvas *Can = new TCanvas();
    myTree->Draw("x:y:index","","para");
    TParallelCoord* para = (TParallelCoord*)gPad->GetListOfPrimitives()->FindObject("ParaCoord");

    para->SetCandleChart(false);

    TParallelCoordVar* axis = (TParallelCoordVar*)para->GetVarList()->FindObject("y");
    axis->AddRange(new TParallelCoordRange(axis,3.,5.));
    para->AddSelection("blue");
    para->GetCurrentSelection()->SetLineColor(kBlue);

    gPad->Modified();

    gPad->GetListOfPrimitives()->ls();

    int sz = TWebCanvas::StoreCanvasJSON(Can, "StandAloneaPara.json.gz", "3");

    TString res = TWebCanvas::CreateCanvasJSON(Can);
    printf("JSON plain:%d compressed:%d \n", res.Length(), sz);
}
