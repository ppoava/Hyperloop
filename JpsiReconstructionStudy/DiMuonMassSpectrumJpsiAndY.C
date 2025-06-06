// DiMuonMassSpectrum.C
// Created: 07 NOV 2024
// Paul VEEN (paveen)

#include <iostream>
#include <string>
#include "TGraphErrors.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THashList.h"
#include "TList.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLine.h"

#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHist.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "RooChi2Var.h"
#include "RooGaussian.h"
#include "RooFormulaVar.h"
#include "RooCrystalBall.h"
#include "RooChebychev.h"
#include "RooFitResult.h"
#include "RooMsgService.h"
#include "RooWorkspace.h"


using namespace RooFit;


void Print(const char *name, Double_t value) {
    std::cout<<Form("%s = ", name)<<value<<std::endl;
}

Double_t calculateSigOverBkgRatio(RooRealVar *observable, TH1 *data, RooAbsPdf *SIG_model, RooAbsPdf *BKG_model, RooAbsPdf *full_model, 
                                  RooRealVar *sigYield, RooRealVar *bkgYield) {

    
    RooArgSet* params = SIG_model->getParameters((RooArgSet*)0);
    RooRealVar* mean = dynamic_cast<RooRealVar*>(params->find("m0"));
    RooRealVar* sigma = dynamic_cast<RooRealVar*>(params->find("sigma"));

    // Integrate over mean +/- 3 sigma range
    double rangeMin = mean->getVal() - 3 * sigma->getVal();
    double rangeMax = mean->getVal() + 3 * sigma->getVal();
    observable->setRange("signalRange", rangeMin, rangeMax);

    /*
    // This method was implemented for bug fixing
    // The method agrees with the other (old) one as long as RooFit::NormSet is used 
    // (i.e. it is proof that it is required)
    Double_t binCenter = data->GetBinCenter(data->FindBin(mean->getVal()));
    Double_t binLowEdge = data->GetBinLowEdge(data->FindBin(rangeMin));
    Double_t binHighEdge = data->GetBinLowEdge(data->FindBin(rangeMax)) + data->GetBinWidth(data->FindBin(mean->getVal()));
    observable->setVal(binCenter);          // invariant mass
    observable->setRange("binRange", binLowEdge, binHighEdge);
    std::cout<<"binLowEdge = "<<binLowEdge<<std::endl;
    std::cout<<"binHighEdge = "<<binHighEdge<<std::endl;
    Double_t sigIntegral = SIG_model->createIntegral(*observable, RooFit::NormSet(*observable),
                                                    RooFit::Range("binRange"))->getVal();
    Double_t bkgIntegral = BKG_model->createIntegral(*observable, RooFit::NormSet(*observable),
                                                    RooFit::Range("binRange"))->getVal();
    Double_t fullIntegral = full_model->createIntegral(*observable, RooFit::NormSet(*observable),
                                                    RooFit::Range("binRange"))->getVal();
    */

    Double_t sigIntegral = SIG_model->createIntegral(
        RooArgSet(*observable),
        NormSet(*observable),
        Range("signalRange")
    )->getVal();
    Double_t bkgIntegral = BKG_model->createIntegral(
        RooArgSet(*observable),
        NormSet(*observable),
        Range("signalRange")
    )->getVal();
    Double_t fullIntegral = full_model->createIntegral(
        RooArgSet(*observable),
        NormSet(*observable),
        Range("signalRange")
    )->getVal();

    Print("mean = ",mean->getVal());
    Print("sigIntegral = ",sigIntegral);
    Print("bkgIntegral = ",bkgIntegral);
    Print("fullIntegral = ",fullIntegral);
    Print("sigYield = ",sigYield->getVal());
    Print("bkgYield = ",bkgYield->getVal());

    // brackets are absolutely necessary!!
    std::cout<<"s/b from function = "<<(sigYield->getVal()*sigIntegral) / (bkgYield->getVal()*bkgIntegral)<<std::endl;
    return (sigYield->getVal()*sigIntegral) / (bkgYield->getVal()*bkgIntegral);


}


Double_t calculateSignificance(RooRealVar *observable, RooAbsPdf *SIG_model, RooAbsPdf *BKG_model, RooAbsPdf *full_model,
                               RooRealVar *sigYield, RooRealVar *bkgYield) {

    
    RooArgSet* params = SIG_model->getParameters((RooArgSet*)0);
    RooRealVar* mean = dynamic_cast<RooRealVar*>(params->find("m0"));
    RooRealVar* sigma = dynamic_cast<RooRealVar*>(params->find("sigma"));

    // Integrate over mean +/- 3 sigma range
    double rangeMin = mean->getVal() - 3 * sigma->getVal();
    double rangeMax = mean->getVal() + 3 * sigma->getVal();
    // double rangeFullMin = observable->getMin();
    // double rangeFullMax = observable->getMax();
    std::cout<<"integration range = ["<<rangeMin<<","<<rangeMax<<"]"<<std::endl;
    observable->setRange("signalRange", rangeMin, rangeMax);
    // observable->setRange("fullRange",rangeFullMin,rangeFullMax);
    

    // Should it be normalised to the whole 'model' in the range?
    Double_t sigIntegral = SIG_model->createIntegral(
        RooArgSet(*observable),
        NormSet(*observable),
        Range("signalRange")
    )->getVal();
    Double_t bkgIntegral = BKG_model->createIntegral(
        RooArgSet(*observable),
        NormSet(*observable),
        Range("signalRange")
    )->getVal();
    Double_t fullIntegral = full_model->createIntegral(
        RooArgSet(*observable),
        NormSet(*observable),
        Range("signalRange")
    )->getVal();
    Print("sigIntegralSignificance = ",sigIntegral);
    Print("bkgIntegralSignificance = ",bkgIntegral);
    Print("fullIntegral = ",fullIntegral);
    Print("sigYieldSignificance = ",sigYield->getVal());
    Print("bkgYieldSignificance = ",bkgYield->getVal());

    std::cout<<"significance from function = "<<(sigYield->getVal()*sigIntegral) 
           /
           (sqrt((sigYield->getVal()*sigIntegral)+(bkgYield->getVal()*bkgIntegral)))<<std::endl;


    return (sigYield->getVal()*sigIntegral) 
           /
           (sqrt((sigYield->getVal()*sigIntegral)+(bkgYield->getVal()*bkgIntegral)));


}


Double_t calculateChi2(RooRealVar *observable, TH1 *data, RooAbsPdf *model,
                       RooRealVar *sigYield, RooRealVar *bkgYield) {

    Double_t mMin = observable->getMin();
    Double_t mMax = observable->getMax();
    data->GetXaxis()->SetRangeUser(mMin, mMax);

    // Number of bins in the input histogram
    // Int_t nBins = data->GetNbinsX();
    Int_t nBinsOld = data->GetNbinsX();
    Double_t xMin = data->GetXaxis()->GetXmin();
    Double_t xMax = data->GetXaxis()->GetXmax();
    Double_t binWidth = (xMax - xMin) / nBinsOld;
    Int_t nBins = static_cast<Int_t>((mMax - mMin) / binWidth);

    // Calculate chi2 by hand
    Double_t chi2 = -1.;
    // Loop over the bins of the data
    for (Int_t i = data->FindBin(mMin); i <= data->FindBin(mMax); ++i) {
        Double_t dataValue = data->GetBinContent(i);
        Double_t dataError = data->GetBinError(i);
        Double_t binCenter = data->GetBinCenter(i);
        Double_t binLowEdge = data->GetBinLowEdge(i);
        Double_t binHighEdge = binLowEdge + data->GetBinWidth(i);

        // Integrate the model over the bin range
        observable->setVal(binCenter);          // invariant mass
        observable->setRange("binRange", binLowEdge, binHighEdge);
        Double_t modelValue = model->createIntegral(*observable, RooFit::NormSet(*observable),
                                                        RooFit::Range("binRange"))->getVal();
        // Multiply model value by combined yield to equalise to data values
        modelValue *= sigYield->getVal()+bkgYield->getVal();
        // Print("modelValueChi2",modelValue);
        // Print("dataValueChi2",dataValue);

        // Avoid division by zero and compute chi-squared
        if (dataError != 0) {
            chi2 += std::pow((dataValue-modelValue),2)/dataValue;
        } else {
            std::cerr << "Warning: Zero error in bin " << i << ", skipping." << std::endl;
        }
    }

    // Return the calculated chi-squared value
    // for debugging print statement
    Int_t ndf = (data->FindBin(mMax)-data->FindBin(mMin))-8;
    std::cout<<"chi2/ndf by hand = "<<chi2/ndf<<std::endl;
    return chi2;


}


TGraphErrors* calculatePullHist(RooRealVar *observable, TH1 *data, RooAbsPdf *model,
                                RooRealVar *sigYield, RooRealVar *bkgYield) {


    Double_t mMin = observable->getMin();
    Double_t mMax = observable->getMax();
    data->GetXaxis()->SetRangeUser(mMin,mMax);
    std::cout<<"pullHist range = "<<"["<<mMin<<","<<mMax<<"]"<<std::endl;

    // Number of bins in the input histogram
    // Int_t nBins = data->GetNbinsX();
    Int_t nBinsOld = data->GetNbinsX();
    Double_t xMin = data->GetXaxis()->GetXmin();
    Double_t xMax = data->GetXaxis()->GetXmax();
    Double_t binWidth = (xMax-xMin)/nBinsOld;
    Int_t nBins = static_cast<Int_t>((mMax-mMin)/binWidth);

    // data->Scale(1.0 / data->Integral(data->FindBin(mMin),data->FindBin(mMax)));

    std::vector<Double_t> xVals;
    std::vector<Double_t> yVals;
    std::vector<Double_t> xErrors(data->GetNbinsX(),0);
    std::vector<Double_t> yErrors(data->GetNbinsX(),0);

    for (Int_t i = data->FindBin(mMin); i <= data->FindBin(mMax); ++i) {
        Double_t dataValue = data->GetBinContent(i);
        Double_t dataError = data->GetBinError(i);
        Double_t binCenter = data->GetBinCenter(i);
        Double_t binLowEdge = data->GetBinLowEdge(i);
        Double_t binHighEdge = binLowEdge + data->GetBinWidth(i);

        // Integrate the model over the bin range
        observable->setVal(binCenter);          // invariant mass
        observable->setRange("binRange", binLowEdge, binHighEdge);
        Double_t modelValue = model->createIntegral(*observable, RooFit::NormSet(*observable),
                                                        RooFit::Range("binRange"))->getVal();
        // Multiply model value by combined yield to equalise to data values
        modelValue *= sigYield->getVal()+bkgYield->getVal();
        // Print("modelValueChi2",modelValue);
        // Print("dataValueChi2",dataValue);
        // Print("modelValue",modelValue);
        // Print("dataValue",dataValue);

        

        if (dataError != 0) {
            // Compute the pull value and store
            Double_t pull = (dataValue - modelValue) / dataError;
            xVals.push_back(binCenter);
            yVals.push_back(pull);
            yErrors.push_back(0);
        } else {
            std::cerr << "Warning: Zero error in bin " << i << ", skipping." << std::endl;
        }
    }

    // Create a TGraphErrors object for the pull plot
    auto *pullPlot = new TGraphErrors(xVals.size(), xVals.data(), yVals.data(), xErrors.data(), yErrors.data());
    pullPlot->SetTitle("Pull Plot;Observable;Pull Value (data - model) / sigma");
    pullPlot->SetMarkerStyle(20); // Optional: Style of markers
    pullPlot->SetMarkerSize(0.8); // Optional: Size of markers

    Print("mMin",mMin);
    Print("mMax",mMax);
    Print("nBinsOld",nBinsOld);
    Print("nBins",nBins);
    return pullPlot;


}


TH1D* getTree(const char* fileName, Double_t ptMin, Double_t ptMax);
void defSigModel(RooWorkspace &ws);
void defBkgModel(RooWorkspace &ws, std::string BKG_model, Double_t ptMin, Double_t ptMax);
void defCombinedModel(RooWorkspace &ws, Double_t ptMin, Double_t ptMax);
void fitModelToData(RooWorkspace &ws, TH1 *hist, std::string BKG_model, Double_t ptMin, Double_t ptMax);
void drawPlots(RooWorkspace &ws, TH1 *hist, Double_t ptMin, Double_t ptMax);


// run macro with root -l 'DiMuonMassSpectrum.C(Double_t ptMin, Double_t ptMax)'
int DiMuonMassSpectrumJpsiAndY(Double_t ptMin, Double_t ptMax) {


    // From tutorial to make output less crowded: why doesn't it work?
    RooMsgService::instance().getStream(1).removeTopic(Minimization);
    RooMsgService::instance().getStream(1).removeTopic(Fitting);
    RooMsgService::instance().getStream(1).removeTopic(Generation);


    RooWorkspace wspace{"myWS"};


    TH1D *hist = getTree("AnalysisResults_merged_LHC24aq_LHC24ap_pass1_medium_no_realignment_PbPb_values_22_04_2025_Hyperloop.root",ptMin,ptMax);


    Double_t mMin = 8; // 2.5
    Double_t mMax = 12; // 10.5
    RooRealVar *m = new RooRealVar("m","M_{#mu^{+}#mu^{-}}",mMin,mMax,"GeV/c2");
    wspace.import(*m);


    defSigModel(wspace);
    defBkgModel(wspace,"Chebychev",ptMin,ptMax);
    defCombinedModel(wspace,ptMin,ptMax);
    fitModelToData(wspace,hist,"Chebychev",ptMin,ptMax);
    drawPlots(wspace,hist,ptMin,ptMax);


    return 0;


} // int DiMuonMassSpectrumJpsiAndY()


TH1D* getTree(const char* fileName, Double_t ptMin, Double_t ptMax) {


    // **********************************************
    // G  E  T    D  A  T  A
    // **********************************************


    THashList *listDiMuon;
    TList *subListDiMuon;
    TH2F *hMass_Pt;
    
    TFile* fDiMuon = new TFile(fileName,"READ");
    listDiMuon = (THashList*)fDiMuon->Get("analysis-same-event-pairing/output");
    // NAME DEPENDS ON CUT USED
    // PairsMuonSEPM_muonLowPt510SigmaPDCA
    // PairsMuonSEPM_muonQualityCuts
    // For Jpsi: PairsMuonSEPM_muonLowPt510SigmaPDCA
    // For Y(1S): PairsMuonSEPM_muonLowPt610SigmaPDCA
    subListDiMuon = (TList*)listDiMuon->FindObject("PairsMuonSEPM_muonLowPt510SigmaPDCA");
    hMass_Pt = (TH2F*)subListDiMuon->FindObject("Mass_Pt");


    // **********************************************
    // S  E  T    P  _  T    C  U  T
    // **********************************************


    TH1D *hDiMuonMass_PtCut;
    
    hMass_Pt = (TH2F*)hMass_Pt->Clone();
    hMass_Pt->GetYaxis()->SetRangeUser(ptMin,ptMax);
    hDiMuonMass_PtCut = hMass_Pt->ProjectionX("hDiMuonMass_PtCut");

    TCanvas *canvasPlot = new TCanvas("plot","plot",800,600);
    canvasPlot->cd();
    hDiMuonMass_PtCut->Draw("hist");


    return hDiMuonMass_PtCut;


} // void getTree()


void defSigModel(RooWorkspace &ws) {


    RooRealVar *m = ws.var("m");
    Double_t mMin = m->getMin("fitRange");
    Double_t mMax = m->getMax("fitRange");
    m->setRange("fitRange",mMin,mMax);


    // Crystal Ball for mass signal
    RooRealVar m0("m0","#mu",9.46,9.40,9.52);
    RooRealVar sigma("sigma","#sigma",0.08,0.05,0.12);
    RooRealVar alphaL("alphaL","Alpha Left",0.883,0.5,3.0);
    alphaL.setConstant();
    RooRealVar nL("nL","Exponent Left",9.940,5.0,20.0); 
    nL.setConstant();
    RooRealVar alphaR("alphaR","Alpha Right",1.832,1.0,3.0);
    alphaR.setConstant();
    RooRealVar nR("nR","Exponent Right",15.323,5.0,25.0);  
    nR.setConstant();

    
    RooCrystalBall* doubleSidedCB = new RooCrystalBall("doubleSidedCB","Double Sided Crystal Ball",
                                                       *m,m0,sigma,alphaL,nL,alphaR,nR);

    ws.import(*m);
    ws.import(*doubleSidedCB);
    

} // void defSigModel()


void defBkgModel(RooWorkspace &ws, std::string BKG_model, Double_t ptMin, Double_t ptMax) {


    RooAbsPdf *BKG;
    RooRealVar *m = ws.var("m");

    // Exponential for background
    // RooRealVar a0("a0","a_{0}",-1,-5,0.000001);
	// RooExponential* BKG = new RooExponential("BKG","Power background", m, a0);

    // Initiallise Chebychev
    RooRealVar a0("a0","a_{0}",-0.01,-2.0,2.0);
    RooRealVar a1("a1","a_{1}",-0.01,-1.0,1.0);
    RooRealVar a2("a2","a_{2}",-0.01,-0.5,0.5);
    RooRealVar a3("a3","a_{3}",0.0,-0.5,0.5);
    RooRealVar a4("a4","a_{4}",0.0,-0.3,0.3);
    RooRealVar a5("a5","a_{5}",0.0,-0.3,0.3);
    RooRealVar a6("a6","a_{6}",0.0,-0.1,0.1);
    RooRealVar a7("a7","a_{7}",0.0,-0.05,0.05);

    // Initialise a Gaussian for background
    RooRealVar BKG_mean("BKG_mean","Mean of background Gaussian",2.5,0.,10);
    RooRealVar BKG_sigma("BKG_sigma","Sigma of background Gaussian",1.7,0.,2.5);
    
    // Initialise an exponential for background
    RooRealVar exp0("exp0","exp_{0}",-1,-5,0.001);

    // Select background model (all are initialised, one is picked)
    RooRealVar *bkgYield;
    RooRealVar *sigYield;


    if (BKG_model == "Chebychev") {
        BKG = new RooChebychev("BKG","Chebyshev background",*m,{a0,a1,a2,a3,a4,a5,a6,a7});
        sigYield = new RooRealVar("sigYield","N_{sig}",20000,0.,10000000);
        bkgYield = new RooRealVar("bkgYield","N_{bkg}",150000,0.,100000000);
    }

    if (BKG_model == "Gaussian") {
        BKG = new RooGaussian("BKG","Background gaussian",*m,BKG_mean,BKG_sigma);
        sigYield = new RooRealVar("sigYield","N_{sig}",50000,0.,100000000);
        bkgYield = new RooRealVar("bkgYield","N_{bkg}",1000000,0.,1000000000);
    }

    if (BKG_model == "Exponential") {
        BKG = new RooExponential("BKG","Power background",*m,exp0);
        sigYield = new RooRealVar("sigYield","N_{sig}",5000,0.,100000000);
        bkgYield = new RooRealVar("bkgYield","N_{bkg}",10000000,0.,1000000000);
    }


    // Add parameters to the workspace
    // This step is necessary for the iterative fitting later
    ws.import(a0);
    ws.import(a1);
    ws.import(a2);
    ws.import(a3);
    ws.import(a4);
    ws.import(a5);
    ws.import(a6);
    ws.import(a7);
    // Add models
    ws.import(*BKG);
    ws.import(*sigYield);
    ws.import(*bkgYield);


} // void defBkgModel()


void defCombinedModel(RooWorkspace &ws, Double_t ptMin, Double_t ptMax) {


    // Add signal and background models
    RooAbsPdf *model = new RooAddPdf("model", "combined mass model", 
                                {*ws.pdf("doubleSidedCB"),*ws.pdf("BKG")},{*ws.var("sigYield"),*ws.var("bkgYield")});


    ws.import(*model);


} // defCombinedModel


void fitModelToData(RooWorkspace &ws, TH1 *hist, std::string BKG_model, Double_t ptMin, Double_t ptMax) {


    RooRealVar *m = ws.var("m");
    Double_t mMin = m->getMin("fitRange");
    Double_t mMax = m->getMax("fitRange");
    m->setRange("fitRange",mMin,mMax);
    RooDataHist* data = new RooDataHist(Form("data_Pt_%.0f_%.0f",ptMin,ptMax),"Di-muon spectrum",*m,Import(*hist));
    RooAbsPdf *model = ws.pdf("model");
    RooRealVar *sigYield = ws.var("sigYield");
    RooRealVar *bkgYield = ws.var("bkgYield");


    // Retreive Chebyshev parameters
    std::vector<RooRealVar*> parameters = {
        ws.var("a0"),
        ws.var("a1"),
        ws.var("a2"),
        ws.var("a3"),
        ws.var("a4"),
        ws.var("a5"),
        ws.var("a6"),
        ws.var("a7")
    };
    // Initially set all parameters to constant (kTRUE)
    // Iterative fitting releases parameters one-by-one
    for (auto& param : parameters) { param->setConstant(kTRUE); }


    /*
    // Start iterative fitting (only for Chebychev)
    // Release orders one-by-one as long as reduced χ2 is above desired value
    Double_t chi2M = -1.; // arbitrary starting value
    if (BKG_model == "Chebychev") {
        for (Int_t i = 0; i < parameters.size(); i++) {
            if (chi2M < 0. || chi2M > 2. || std::isnan(chi2M)) {
                frame = nullptr;
                parameters[i]->setConstant(kFALSE);
                // IntegrateBins(1e-3): integrate per bin?
                model->fitTo(*data,Range("fitRange"),PrintLevel(-1),Extended(kTRUE),Verbose(kFALSE));
                frame = m->frame();
                data->plotOn(frame,MarkerSize(0.4),Range("fitRange"));
                model->plotOn(frame,LineColor(kBlue),Name("full_Model"),Range("fitRange"));
                chi2M = frame->chiSquare();
                std::cout<<"chi2M = "<<chi2M<<std::endl;
            }
            else { break; }
            if (i == 7 && (chi2M < 0. || chi2M > 2.)) { std::cout<<"WARNING. Fitting failed. No convergence"<<std::endl; }
        }
    }
    */


   // Alternative approach that prevents drawing frames every iteration
   Double_t chi2M = -1.; // arbitrary starting value
   Int_t ndf = (hist->FindBin(mMax)-hist->FindBin(mMin))-8;
    if (BKG_model == "Chebychev") {
        for (Int_t i = 0; i < parameters.size(); i++) {
            if (chi2M < 0. || chi2M > 2. || std::isnan(chi2M)) {
                parameters[i]->setConstant(kFALSE);
                // IntegrateBins(1e-3): integrate per bin?
                model->fitTo(*data,Range("fitRange"),PrintLevel(-1),Extended(kTRUE),Verbose(kFALSE));
                chi2M = calculateChi2(m,hist,model,sigYield,bkgYield)/ndf;
                std::cout<<"chi2New = "<<chi2M<<std::endl;
            }
            else { break; }
            if (i == 7 && (chi2M < 0. || chi2M > 2.)) { std::cout<<"WARNING. Fitting failed. No convergence"<<std::endl; }
        }
    }

    // Pull needs to be calculated before more fitting is done
    // (so that it is calculated with respect to the correct model)
    // RooHist *hpull = frame->pullHist();
    RooRealVar chi2Var("chi2M", "Chi-squared value",chi2M,-1.,1e6);


    // ws.import(*hpull);
    ws.import(chi2Var);
    // ws.import(*frame);


} // void fitModelToData()


void drawPlots(RooWorkspace &ws, TH1 *hist, Double_t ptMin, Double_t ptMax) {


    // Collect everything from workspace  
    RooRealVar *m = ws.var("m");
    Double_t mMin = m->getMin("fitRange");
    Double_t mMax = m->getMax("fitRange");
    m->setRange("fitRange",mMin,mMax);
    
    RooAbsPdf *doubleSidedCB = ws.pdf("doubleSidedCB");
    RooAbsPdf *BKG = ws.pdf("BKG");
    RooAbsPdf *model = ws.pdf("model");
    RooRealVar *sigYield = ws.var("sigYield");
    RooRealVar *bkgYield = ws.var("bkgYield");
    RooDataHist* data = new RooDataHist(Form("data_Pt_%.0f_%.0f",ptMin,ptMax),"Di-muon spectrum",*m,Import(*hist));


    // RooPlot *frame = dynamic_cast<RooPlot*>(ws.obj("frame"));
    RooPlot *frame = m->frame();
    // RooHist *hpull = dynamic_cast<RooHist*>(ws.obj("hpull"));
    TGraphErrors *hpull = calculatePullHist(m,hist,model,sigYield,bkgYield);
    

    TCanvas* canvas = new TCanvas(Form("canvas_Pt_%.0f_%.0f",ptMin,ptMax), 
                                  Form("Double Sided Crystal Ball Fit %.0f < p_{T} < %.0f",ptMin,ptMax),
                                  800,600);
    canvas->SetBottomMargin(0);
    canvas->cd();

    TPad* megaPad = new TPad("megaPad", "megaPad", 0.0, 0.4, 1.0, 1.0); // 0.3
    megaPad->SetRightMargin(0.05);
    megaPad->SetBottomMargin(0.10); // 1e-3
    megaPad->SetLeftMargin(0.10);
    megaPad->SetTicks(1,1);
    megaPad->Draw();
    TPad* miniPad = new TPad("miniPad", "miniPad", 0.0, 0.0, 1.0, 0.3);
    miniPad->SetTopMargin(1e-3);
    miniPad->SetRightMargin(0.05);
    miniPad->SetBottomMargin(0.40);
    miniPad->SetLeftMargin(0.10);
    miniPad->SetTicks(1,1);
    miniPad->Draw();
    
    megaPad->cd();
    gPad->SetLogy();
    data->plotOn(frame,MarkerSize(0.4),Range("fitRange"));
    model->plotOn(frame,LineColor(kBlue),Name("full_Model"),Range("fitRange"));
    model->plotOn(frame,Components(*doubleSidedCB),LineStyle(kDashed),LineColor(kRed),Name("signal_Model"),Range("fitRange"));
   	model->plotOn(frame,Components(*BKG),LineStyle(kDashed),LineColor(kBlue),Name("bkg_Model"),Range("fitRange"));
   	model->paramOn(frame,ShowConstants(false),Format("TE",AutoPrecision(3)));
    

    // Beautify the box displaying the parameters
    TPaveText* paramsBox = (TPaveText*)frame->findObject("model_paramBox");
    paramsBox->SetTextSize(0.04);
    paramsBox->SetTextFont(42);
    // This part below doesn't work?
    // ** //
    paramsBox->SetX1NDC(0.70);
    paramsBox->SetY1NDC(0.45);
    paramsBox->SetX2NDC(0.95);
    paramsBox->SetY2NDC(0.90);
    // ** //
    std::cout << "TPaveText found with coordinates: "
              << paramsBox->GetX1NDC() << ", "
              << paramsBox->GetY1NDC() << ", "
              << paramsBox->GetX2NDC() << ", "
              << paramsBox->GetY2NDC() << std::endl;
    paramsBox->Draw();


    // Beautify the canvas and frame
    frame->SetTitle("J/#psi invariant mass plot");
    frame->SetMinimum(1e-1);
    frame->SetMaximum(1.5*frame->GetMaximum());
    frame->GetXaxis()->SetTitle("");
    frame->GetXaxis()->SetLabelSize(0.035); // 0


    frame->Draw();


    // alternative chi2 method
    /*
    RooChi2Var* chi2Var = new RooChi2Var("chi2","chi2", *doubleSidedCB, *data, true, RooAbsData::ErrorType::Poisson);
    double chi2M = chi2Var->getVal();
    */


    TLegend *legend = new TLegend(0.13,0.16,0.37,0.44);
    legend->SetBorderSize(0);
    legend->SetTextSize(0.04);
    legend->AddEntry("",Form("%.0f < p_{T} < %.0f [GeV]", ptMin,ptMax),"");
    legend->AddEntry(frame->getObject(0),"Data","point");
    legend->AddEntry(frame->getObject(1),"Full model","l");
    legend->AddEntry("",Form("#chi^{2}/ndf = %.2f",ws.var("chi2M")->getVal()),"");
    Int_t ndf = (hist->FindBin(mMax)-hist->FindBin(mMin))-8;
    legend->AddEntry("",Form("hand-made #chi^{2}/ndf = %.2f",calculateChi2(m,hist,model,sigYield,bkgYield)/ndf),"");
    legend->AddEntry("",Form("S/B (3#sigma) = %.3f",calculateSigOverBkgRatio(m,hist,doubleSidedCB,BKG,model,sigYield,bkgYield)),"");
    legend->AddEntry("",Form("S/#sqrt{S+B} (3#sigma) = %.2f",calculateSignificance(m,doubleSidedCB,BKG,model,sigYield,bkgYield)),"");
    legend->Draw();
    // check output
    std::cout<<"chi2/ndf by machine = "<<ws.var("chi2M")->getVal()<<std::endl;


    /*
    m0.Print();
    sigma.Print();
    alphaL.Print();
    nL.Print();
    alphaR.Print();
    nR.Print();
    */

   
    miniPad->cd();
    Double_t factor = 3.33;
    hpull->SetTitle("");
    hpull->GetYaxis()->SetTitle("pull");
    hpull->GetYaxis()->SetTitleSize(factor*frame->GetYaxis()->GetTitleSize());
    hpull->GetYaxis()->SetTitleOffset(0.35);
    hpull->GetYaxis()->SetLabelSize(0.8*factor*frame->GetYaxis()->GetLabelSize());
    hpull->GetYaxis()->SetTickLength(frame->GetYaxis()->GetTickLength());
    //
    hpull->GetXaxis()->SetTitle("mass [GeV/c^2]");
    hpull->GetXaxis()->SetTitleSize(factor*frame->GetXaxis()->GetTitleSize());
    hpull->GetXaxis()->SetTitleOffset(1.5);
    hpull->GetXaxis()->SetLabelOffset(0.08);
    // hpull->GetXaxis()->SetTickLength(factor*frame->GetXaxis()->GetTickLength());
    hpull->GetXaxis()->SetLabelSize(factor*0.035);
    hpull->GetXaxis()->SetRangeUser(frame->GetXaxis()->GetXmin(),frame->GetXaxis()->GetXmax());
    hpull->GetXaxis()->SetTickLength(frame->GetXaxis()->GetTickLength());
    hpull->Draw();

    // FIX THIS
    TLine *line1 = new TLine(frame->GetXaxis()->GetXmin(),-1,frame->GetXaxis()->GetXmax(),-1);
    TLine *line2 = new TLine(frame->GetXaxis()->GetXmin(),1,frame->GetXaxis()->GetXmax(),1);
    line1->SetLineWidth(3);
    line1->SetLineColor(kRed);
    //
    line2->SetLineWidth(3);
    line2->SetLineColor(kRed);
    line1->Draw();
    line2->Draw();
    // miniPad->Update();
    

    // canvas->SaveAs(Form("Plots/SimpleJpsiFitting/pTRange_[%.0f,%.0f]_JpsiSingleMuonCut1GeV.pdf",ptMin,ptMax));
    // canvas->SaveAs(Form("Plots/SimpleJpsiFitting/pTRange_[%.0f,%.0f]_JpsiSingleMuonCut1GeV.png",ptMin,ptMax));


    // Calculate in mass range the entries for histogram
    int bin_min = hist->FindBin(mMin);
    int bin_max = hist->FindBin(mMax);
    double sumEntries = 0;
    for (int bin = bin_min; bin <= bin_max; ++bin) {
        sumEntries += hist->GetBinContent(bin);
    }
    std::cout << "Entries in range [" << mMin << ", " << mMax << "]: " << sumEntries << std::endl;
    std::cout<<"sigYield + bkgYield = "<<sigYield->getVal()+bkgYield->getVal()<<std::endl;


} // void drawPlots()
