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
#include "TCanvas.h"
#include "TLegend.h"
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


void getTree(const char* fileName, Double_t ptMin, Double_t ptMax);
void defSigModels(RooWorkspace &ws);
void defBkgModels(RooWorkspace &ws, std::string BKG_model);
void doJpsiFit(RooWorkspace &ws);
void drawPlots(RooWorkspace &ws);

int DiMuonMassSpectrum() {

    RooWorkspace wspace{"myWS"};

    // getTree("AnalysisResultsDiMuons.root",0.,2.);
    // getTree("AnalysisResultsDiMuons.root",3.,4.);
    getTree("AnalysisResultsDiMuons.root",5.,30.);

    return 0;
}

void getTree(const char* fileName, Double_t ptMin, Double_t ptMax) {


    // **********************************************
    // G  E  T    D  A  T  A
    // **********************************************


    THashList *listDiMuon;
    TList *subListDiMuon;
    TH2F *hMass_Pt;
    //
    TFile* fDiMuon = new TFile(fileName, "READ");
    listDiMuon = (THashList*)fDiMuon->Get("analysis-same-event-pairing/output");
    subListDiMuon = (TList*)listDiMuon->FindObject("PairsMuonSEPM_muonLowPt510SigmaPDCA");
    hMass_Pt = (TH2F*)subListDiMuon->FindObject("Mass_Pt");


    THashList *listDiMuon;
    TList *subListDiMuon;
    //
    TFile* fDiMuon = new TFile("AnalysisResultsDiMuons.root", "READ");
    listDiMuon = (THashList*)fDiMuon->Get("analysis-same-event-pairing/output");
    subListDiMuon = (TList*)listDiMuon->FindObject("PairsMuonSEPM_muonLowPt510SigmaPDCA");
    //
    TH2F *hMass_Pt;
    hMass_Pt = (TH2F*)subListDiMuon->FindObject("Mass_Pt");


    // **********************************************
    // S  E  T    P  _  T    C  U  T
    // **********************************************


    TH1D *hDiMuonMass_PtCut;
    //
    hMass_Pt = (TH2F*)hMass_Pt->Clone();
    hMass_Pt->GetYaxis()->SetRangeUser(ptMin, ptMax);
    hDiMuonMass_PtCut = hMass_Pt->ProjectionX("hDiMuonMass_PtCut");


} // void getTree()


void defSigModel(RooWorkspace &ws) {


    RooRealVar *m = new RooRealVar("fMass","M_{#mu^{+}#mu^{-}}",2.,5.,"GeV/c2");
    // m.setRange("fitRange",2.,5.);

    // Crystal Ball for mass signal
    RooRealVar m0("m0", "Mean", 3.097, 3.05, 3.13);
    RooRealVar sigma("sigma", "Sigma of Gaussian", 0.08, 0.05, 0.12);
    RooRealVar alphaL("alphaL", "Alpha Left", 0.883, 0.5, 3.0);
    alphaL.setConstant();
    RooRealVar nL("nL", "Exponent Left", 9.940, 5.0, 20.0); 
    nL.setConstant();
    RooRealVar alphaR("alphaR", "Alpha Right", 1.832, 1.0, 3.0);
    alphaR.setConstant();
    RooRealVar nR("nR", "Exponent Right", 15.323, 5.0, 25.0);  
    nR.setConstant();
    
    // Becomes important when signal and background model are combined
    RooRealVar* sigYield;
    
    RooCrystalBall* doubleSidedCB = new RooCrystalBall("doubleSidedCB","Double Sided Crystal Ball",
                                                       *m,m0,sigma,alphaL,nL,alphaR,nR);
    ws.import(*m);
    ws.import(*sigYield);
    ws.import(*doubleSidedCB);
    

} // void defSigModel()


void defBkgModel(RooWorkspace &ws, std::string BKG_model, Double_t ptMin, Double_t ptMax) {


    RooAbsPdf *BKG;
    RooRealVar *m = ws.var("m");

    // Exponential for background
    // RooRealVar a0("a0","a_{0}",-1,-5,0.000001);
	// RooExponential* BKG = new RooExponential("BKG","Power background", m, a0);

    // Initiallise Chebychev
    RooRealVar a0("a0", "a_{0}", -0.01, -2.0, 2.0);
    RooRealVar a1("a1", "a_{1}", -0.01, -1.0, 1.0);
    RooRealVar a2("a2", "a_{2}", -0.01, -0.5, 0.5);
    RooRealVar a3("a3", "a_{3}", 0.0, -0.5, 0.5);
    RooRealVar a4("a4", "a_{4}", 0.0, -0.3, 0.3);
    RooRealVar a5("a5", "a_{5}", 0.0, -0.3, 0.3);
    RooRealVar a6("a6", "a_{6}", 0.0, -0.1, 0.1);
    RooRealVar a7("a7", "a_{7}", 0.0, -0.05, 0.05);

    // Initially set all parameters to constant (kTRUE)
    // Iterative fitting releases parameters one-by-one
    std::vector<RooRealVar*> parameters = {&a0, &a1, &a2, &a3, &a4, &a5, &a6, &a7};
    for (auto& param : parameters) {
        param->setConstant(kTRUE);
    }

    // Initialise a Gaussian for background
    RooRealVar BKG_mean("BKG_mean", "Mean of background Gaussian", 2.5, 0., 10);
    RooRealVar BKG_sigma("BKG_sigma", "Sigma of background Gaussian", 1.7, 0., 2.5);
    
    // Initialise an exponential for background
    RooRealVar exp0("exp0","exp_{0}",-1,-5,0.001);

    // Select background model (all are initialised, one is picked)
    RooRealVar *bkgYield;
    RooRealVar *sigYield = ws.var("sigYield");

    if (BKG_model == "Chebychev") {
        BKG = new RooChebychev(Form("BKG_Pt_%.0f_%.0f", ptMin, ptMax),"Chebyshev background", *m, {a0,a1,a2,a3,a4,a5,a6,a7});
        sigYield = new RooRealVar("sigYieldCheb", "N_{sig}", 2000, 0., 10000000);
        bkgYield = new RooRealVar("bkgYieldCheb", "N_{bkg}", 150000, 0., 100000000);
    }

    if (BKG_model == "Gaussian") {
        BKG = new RooGaussian("gauss", "Background gaussian", *m, BKG_mean, BKG_sigma);
        sigYield = new RooRealVar("sigYieldGauss", "N_{sig}", 50000, 0., 100000000);
        bkgYield = new RooRealVar("bkgYieldGauss", "N_{bkg}", 1000000, 0., 1000000000);
    }

    if (BKG_model == "Exponential") {
        BKG = new RooExponential("BKG","Power background", *m, exp0);
        sigYield = new RooRealVar("sigYieldExp", "N_{sig}", 5000, 0., 100000000);
        bkgYield = new RooRealVar("bkgYieldExp", "N_{bkg}", 10000000, 0., 1000000000);
    }


    ws.import(*BKG);
    ws.import(*bkgYield);


} // void defBkgModel()


void defCombinedModel(RooWorkspace &ws, Double_t ptMin, Double_t ptMax) {


    // Add signal and background models
    RooAbsPdf *model = new RooAddPdf(Form("model_Pt_%.0f_%.0f", ptMin, ptMax), "combined mass model", 
                                {ws.pdf("doubleSidedCB"), ws.pdf("BKG")}, {ws.var("sigYield"), ws.var("bkgYield")});
    ws.import(*model);


} // defCombinedModel


void fitModelToData(RooWorkspace &ws, TH1 *data, std::string BKG_model) {


    // Start iterative fitting (only for Chebychev)
    // Release orders one-by-one as long as reduced χ2 is above desired value
    Double_t chi2M = -1.; // arbitrary starting value
    if (BKG_model == "Chebychev") {
        for (Int_t i = 0; i < parameters.size(); i++) {
            if (chi2M < 0. || chi2M > 2. || std::isnan(chi2M)) {
                frame = nullptr;
                parameters[i]->setConstant(kFALSE);
                model->fitTo(*data, Range("fitRange"),Extended(kTRUE),Verbose(kFALSE));
                frame = m->frame();
                data->plotOn(frame, MarkerSize(0.4),Range("fitRange"));
                model->plotOn(frame, LineColor(kBlue), Name("full_Model"),Range("fitRange"));
                chi2M = frame->chiSquare();
                std::cout << "chi2M = " << chi2M << std::endl;
            }
            else { break; }
            if (i == 7 && (chi2M < 0. || chi2M > 2.)) { std::cout << "WARNING. Fitting failed. No convergence" << std::endl; }
        }
    }


    RooHist *hpull = frame->pullHist();


} // void fitModelToData()





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
        RooFit::NormSet(*observable),
        RooFit::Range("signalRange")
    )->getVal();
    Double_t bkgIntegral = BKG_model->createIntegral(
        RooArgSet(*observable),
        RooFit::NormSet(*observable),
        RooFit::Range("signalRange")
    )->getVal();
    Double_t fullIntegral = full_model->createIntegral(
        RooArgSet(*observable),
        RooFit::NormSet(*observable),
        RooFit::Range("signalRange")
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
    std::cout<<"integration range = ["<<rangeMin<<","<<rangeMax<<"]"<<std::endl;
    observable->setRange("signalRange", rangeMin, rangeMax);

    // Should it be normalised to the whole 'model' in the range?
    Double_t sigIntegral = SIG_model->createIntegral(
        RooArgSet(*observable),
        RooFit::NormSet(*observable),
        RooFit::Range("signalRange")
    )->getVal();
    Double_t bkgIntegral = BKG_model->createIntegral(
        RooArgSet(*observable),
        RooFit::NormSet(*observable),
        RooFit::Range("signalRange")
    )->getVal();
    Double_t fullIntegral = full_model->createIntegral(
        RooArgSet(*observable),
        RooFit::NormSet(*observable),
        RooFit::Range("signalRange")
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

    // Number of bins in the input histogram
    // Int_t nBins = data->GetNbinsX();
    Int_t nBinsOld = data->GetNbinsX();
    Double_t xMin = data->GetXaxis()->GetXmin();
    Double_t xMax = data->GetXaxis()->GetXmax();
    Double_t binWidth = (xMax - xMin) / nBinsOld;
    Int_t nBins = static_cast<Int_t>((mMax - mMin) / binWidth);

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


void fitJpsiGauss(TH1D* hist, Double_t pTMin, Double_t pTMax) {
    // Initialise
    Double_t mMin = hist->GetXaxis()->GetXmin();
    Double_t mMax = hist->GetXaxis()->GetXmax();
    RooRealVar m("m", "invariant mass", mMin, mMax);
    RooDataHist* data = new RooDataHist("data", "Di-muon spectrum", m, Import(*hist));

    // Gaussian
    RooRealVar mean("mean", "mean", mMin, mMax);
    RooRealVar sigma("sigma", "width", hist->GetRMS(), 0, (mMax - mMin) / 2);
    RooGaussian gauss("gauss", "Gaussian PDF", m, mean, sigma);

    // Fit
    m.setRange("fitRange", 2.8, 3.4);  // Define the fit range
    RooFitResult* result = gauss.fitTo(*data, Save(), Range("fitRange"));

    // Plot
    TCanvas* c = new TCanvas("c", "Fit Result", 800, 600);
    c->cd();
    RooPlot* frame = m.frame();
    data->plotOn(frame, MarkerSize(0.5));
    // data.plotOn(frame, Binning(40)); 
    gauss.plotOn(frame, Range("fitRange")); 

    frame->Draw();

    // Optional: Print fit results
    result->Print(); 
} // fitJpsiGauss

void fitJpsiCB(TH1D* hist, Double_t pTMin, Double_t pTMax, std::string BKG_model) {


    // **********************************************
    // I  N  I  T  I  A  L  I  S  A  T  I  O  N
    // **********************************************


    // hist->Rebin(2);
    Double_t mMin = 2.5;
    Double_t mMax = 4.5;
    hist->GetXaxis()->SetRangeUser(mMin, mMax);
    RooRealVar* m = new RooRealVar(Form("m_Pt_%.0f_%.0f", pTMin, pTMax), "invariant mass", mMin, mMax);
    RooDataHist* data = new RooDataHist(Form("data_Pt_%.0f_%.0f", pTMin, pTMax), "Di-muon spectrum", *m, Import(*hist));
    m->setRange("fitRange", mMin, mMax);
    data->Print("v");

    /*
    // Print hist information
    for (int i = 1; i <= hist->GetNbinsX(); i++) {
        double binLowEdge = hist->GetXaxis()->GetBinLowEdge(i);
        double binUpEdge = hist->GetXaxis()->GetBinUpEdge(i);
        std::cout << "Bin " << i << ": [" << binLowEdge << ", " << binUpEdge << "]" << std::endl;
    }

    // Print bin information for the RooDataHist
    const RooArgSet* obsSet = data->get(0); // Get observables (assumes single observable)
    if (obsSet) {
        RooRealVar* m_var = (RooRealVar*)obsSet->find("m_Pt_5_30");
        if (m_var) {
            int nBins = data->numEntries(); // Number of bins in RooDataHist
            double rangeMin = m_var->getMin();
            double rangeMax = m_var->getMax();
            double binWidth = (rangeMax - rangeMin) / nBins;

            std::cout << "Number of bins: " << nBins << std::endl;
            std::cout << "Observable range: [" << rangeMin << ", " << rangeMax << "]" << std::endl;
            std::cout << "Bin width: " << binWidth << std::endl;

            for (int i = 0; i < nBins; ++i) {
                double binLowEdge = rangeMin + i * binWidth;
                double binHighEdge = binLowEdge + binWidth;
                const RooArgSet* binContent = data->get(i); // Get the i-th bin
                double binCenter = m_var->getVal();
                double binWeight = data->weight(); // Bin content (weight)

                std::cout << "Bin " << i + 1
                        << " Edges: [" << binLowEdge << ", " << binHighEdge << "]"
                        << ", Center: " << binCenter
                        << ", Weight: " << binWeight
                        << std::endl;
            }
        } else {
            std::cerr << "Observable 'm_Pt_5_30' not found in RooDataHist." << std::endl;
        }
    } else {
        std::cerr << "No observables found in the RooDataHist." << std::endl;
    }
    */



    RooAddPdf *model;
    RooPlot *frame;

    // Crystal Ball
    RooRealVar m0("m0", "#mu", 3.097, 3.05, 3.13);
    RooRealVar sigma("sigma", "#sigma", 0.08, 0.05, 0.12);
    RooRealVar alphaL("alphaL", "Alpha Left", 0.883, 0.5, 3.0);
    alphaL.setConstant();
    RooRealVar nL("nL", "Exponent Left", 9.940, 5.0, 20.0); 
    nL.setConstant();
    RooRealVar alphaR("alphaR", "Alpha Right", 1.832, 1.0, 3.0);
    alphaR.setConstant();
    RooRealVar nR("nR", "Exponent Right", 15.323, 5.0, 25.0);  
    nR.setConstant();

    // Signal model
    RooCrystalBall* doubleSidedCB = new RooCrystalBall(Form("doubleSidedCB_Pt_%.0f_%.0f", pTMin, pTMax), 
                                                            "Double Sided Crystal Ball", *m, m0, sigma, alphaL, nL, alphaR, nR);
    

    // **********************************************
    // G  E  T    D  A  T  A
    // **********************************************


    RooAbsPdf* BKG;

    // Exponential for background
    // RooRealVar a0("a0","a_{0}",-1,-5,0.000001);
	// RooExponential* BKG = new RooExponential("BKG","Power background", m, a0);

    // Initiallise Chebychev
    RooRealVar a0("a0", "a_{0}", -0.01, -2.0, 2.0);
    RooRealVar a1("a1", "a_{1}", -0.01, -1.0, 1.0);
    RooRealVar a2("a2", "a_{2}", -0.01, -0.5, 0.5);
    RooRealVar a3("a3", "a_{3}", 0.0, -0.5, 0.5);
    RooRealVar a4("a4", "a_{4}", 0.0, -0.3, 0.3);
    RooRealVar a5("a5", "a_{5}", 0.0, -0.3, 0.3);
    RooRealVar a6("a6", "a_{6}", 0.0, -0.1, 0.1);
    RooRealVar a7("a7", "a_{7}", 0.0, -0.05, 0.05);

    // Initially set all parameters to constant (kTRUE)
    // Iterative fitting releases parameters one-by-one
    std::vector<RooRealVar*> parameters = {&a0, &a1, &a2, &a3, &a4, &a5, &a6, &a7};
    for (auto& param : parameters) {
        param->setConstant(kTRUE);
    }

    // Initialise a Gaussian for background
    RooRealVar BKG_mean("BKG_mean", "Mean of background Gaussian", 2.5, 0., 10);
    RooRealVar BKG_sigma("BKG_sigma", "Sigma of background Gaussian", 1.7, 0., 2.5);
    
    // Initialise an exponential for background
    RooRealVar exp0("exp0","exp_{0}",-1,-5,0.001);

    // Select background model (all are initialised, one is picked)
    RooRealVar* sigYield;
    RooRealVar* bkgYield;

    if (BKG_model == "Chebychev") {
        BKG = new RooChebychev(Form("BKG_Pt_%.0f_%.0f", pTMin, pTMax),"Chebyshev background", *m, {a0,a1,a2,a3,a4,a5,a6,a7});
        sigYield = new RooRealVar("sigYieldCheb", "N_{sig}", 2000, 0., 10000000);
        bkgYield = new RooRealVar("bkgYieldCheb", "N_{bkg}", 150000, 0., 100000000);
    }

    if (BKG_model == "Gaussian") {
        BKG = new RooGaussian("gauss", "Background gaussian", *m, BKG_mean, BKG_sigma);
        sigYield = new RooRealVar("sigYieldGauss", "N_{sig}", 50000, 0., 100000000);
        bkgYield = new RooRealVar("bkgYieldGauss", "N_{bkg}", 1000000, 0., 1000000000);
    }

    if (BKG_model == "Exponential") {
        BKG = new RooExponential("BKG","Power background", *m, exp0);
        sigYield = new RooRealVar("sigYieldExp", "N_{sig}", 5000, 0., 100000000);
        bkgYield = new RooRealVar("bkgYieldExp", "N_{bkg}", 10000000, 0., 1000000000);
    }

    // Add signal and background models
    model = new RooAddPdf(Form("model_Pt_%.0f_%.0f", pTMin, pTMax), "combined mass model", 
                                {*doubleSidedCB, *BKG}, {*sigYield, *bkgYield});

    // Start iterative fitting (only for Chebychev)
    // Release orders one-by-one as long as reduced χ2 is above desired value
    Double_t chi2M = -1.; // arbitrary starting value
    if (BKG_model == "Chebychev") {
        for (Int_t i = 0; i < parameters.size(); i++) {
            if (chi2M < 0. || chi2M > 2. || std::isnan(chi2M)) {
                frame = nullptr;
                parameters[i]->setConstant(kFALSE);
                model->fitTo(*data, Range("fitRange"),Extended(kTRUE),Verbose(kFALSE));
                frame = m->frame();
                data->plotOn(frame, MarkerSize(0.4),Range("fitRange"));
                model->plotOn(frame, LineColor(kBlue), Name("full_Model"),Range("fitRange"));
                chi2M = frame->chiSquare();
                std::cout << "chi2M = " << chi2M << std::endl;
            }
            else { break; }
            if (i == 7 && (chi2M < 0. || chi2M > 2.)) { std::cout << "WARNING. Fitting failed. No convergence" << std::endl; }
        }
    }


    RooHist *hpull = frame->pullHist();


    /*
    // Construct a histogram with the pulls of the data w.r.t the curve
    // Conclusion: it doesn't

    if (BKG_model != "Chebychev") {
        model->fitTo(*data, Range("fitRange"),Extended(kTRUE));
        frame = m->frame();
        data->plotOn(frame, MarkerSize(0.4));
        model->plotOn(frame, LineColor(kBlue), Name("full_Model"),Range("fitRange"));
        chi2M = frame->chiSquare();
        std::cout << "chi2M = " << chi2M << std::endl;
    }

    // Test to see if integral calculation depends on getting arguments through a function or not
    // Integrate over mean +/- 3 sigma range
    double rangeMin = m0.getVal() - 3 * sigma.getVal();
    double rangeMax = m0.getVal() + 3 * sigma.getVal();
    m->setRange("signalRange", rangeMin, rangeMax);

    RooAbsReal* sigIntegral = doubleSidedCB->createIntegral(
        RooArgSet(*m),
        // NormSet(*m),
        RooFit::Range("signalRange")
    );
    RooAbsReal* bkgIntegral = BKG->createIntegral(
        RooArgSet(*m),
        // NormSet(*m),
        RooFit::Range("signalRange")
    );
    Print("sigIntegralLocal = ",sigIntegral->getVal());
    Print("bkgIntegralLocal = ",bkgIntegral->getVal());
    Print("sigYieldLocal = ",sigYield->getVal());
    Print("bkgYieldLocal = ",bkgYield->getVal());
    std::cout<<"s/b local = "<<sigYield->getVal()*sigIntegral->getVal() / bkgYield->getVal()*bkgIntegral->getVal()<<std::endl;
    */


    // **********************************************
    // P  L  O  T  T  I  N  G
    // **********************************************


    TCanvas* canvas = new TCanvas(Form("canvas_Pt_%.0f_%.0f", pTMin, pTMax), 
                                  Form("Double Sided Crystal Ball Fit %.0f < p_{T} < %.0f", pTMin, pTMax),
                                  800, 600);
    canvas->cd();

    model->plotOn(frame,Components(*doubleSidedCB),LineStyle(kDashed),LineColor(kRed),Name("signal_Model"),Range("fitRange"));
   	model->plotOn(frame,Components(*BKG),LineStyle(kDashed),LineColor(kBlue),Name("bkg_Model"),Range("fitRange"));
   	model->paramOn(frame,ShowConstants(true),Format("TE",AutoPrecision(3)));

    frame->Draw();

    // RooChi2Var* chi2Var = new RooChi2Var("chi2","chi2", *doubleSidedCB, *data, true, RooAbsData::ErrorType::Poisson);
    // double chi2M = chi2Var->getVal();

    TLegend *legend = new TLegend(0.6, 0.7, 0.82, 0.8);
    legend->SetTextSize(0.03);
    legend->AddEntry("", Form("%.0f < p_{T} < %.0f [GeV]", pTMin, pTMax), "");
    legend->AddEntry(frame->getObject(0), "Data", "point");
    legend->AddEntry(frame->getObject(1), Form("#chi^{2}/ndf = %.2f", chi2M), "l");
    legend->AddEntry("", Form("signal/background = %.3f", calculateSigOverBkgRatio(m,hist,doubleSidedCB,BKG,model,sigYield,bkgYield)), "");
    legend->AddEntry("", Form("significance = %.2f", calculateSignificance(m, doubleSidedCB,BKG,model,sigYield,bkgYield)), "");
    Int_t ndf = (hist->FindBin(mMax)-hist->FindBin(mMin))-8;
    legend->AddEntry("", Form("hand-made #chi^{2}/ndf = %.2f", calculateChi2(m,hist,model,sigYield,bkgYield)/ndf), "");
    legend->Draw();

    // check output
    std::cout<<"chi2/ndf by machine = "<<chi2M<<std::endl;

    m0.Print();
    sigma.Print();
    alphaL.Print();
    nL.Print();
    alphaR.Print();
    nR.Print();

    TCanvas* pullCanvas = new TCanvas(Form("pull_canvas_Pt_%.0f_%.0f", pTMin, pTMax), 
                                  Form("Pull canvas %.0f < p_{T} < %.0f", pTMin, pTMax),
                                  800, 600);
    // Create a new frame to draw the pull distribution and add the distribution to the frame
    RooPlot *frame_pull = m->frame(Title("Pull Distribution"));
    frame_pull->addPlotable(hpull, "P");
    pullCanvas->cd();
    hpull->Draw();
    frame_pull->Draw();

    /*
    TCanvas *hand_made_pullCanvas = new TCanvas(Form("hand-made_pull_%.0f_%.0f",pTMin,pTMax),
                                                Form("hand-made pull_%.0f_%.0f",pTMin,pTMax));
    hand_made_pullCanvas->cd();
    TGraphErrors* pullGraph = calculatePullHist(m,hist,model,sigYield,bkgYield);
    pullGraph->SetTitle("pull plot by hand (equal to machine?)");
    pullGraph->GetXaxis()->SetRangeUser(mMin,mMax);
    pullGraph->GetXaxis()->SetTitle("invariant mass");
    pullGraph->Draw();
    */

} // fitJpsiCB

int DiMuonMassSpectrum() 
 {
    // Set RooFit to minimal messages
    RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);


    // **********************************************
    // G  E  T    D  A  T  A
    // **********************************************


    THashList *listDiMuon;
    TList *subListDiMuon;
    TFile* fDiMuon = new TFile("AnalysisResultsDiMuons.root", "READ");
    listDiMuon = (THashList*)fDiMuon->Get("analysis-same-event-pairing/output");
    subListDiMuon = (TList*)listDiMuon->FindObject("PairsMuonSEPM_muonLowPt510SigmaPDCA");

    TH2F *hMass_Pt;
    hMass_Pt = (TH2F*)subListDiMuon->FindObject("Mass_Pt");


    // **********************************************
    // S  E  T    P  _  T    C  U  T  S
    // **********************************************


    TH2F *hMass_Pt_PtCut_0_2,    *hMass_Pt_PtCut_3_4,    *hMass_Pt_PtCut_5_30;
    TH1D *hDiMuonMass_PtCut_0_2, *hDiMuonMass_PtCut_3_4, *hDiMuonMass_PtCut_5_30;

    hMass_Pt_PtCut_0_2 = (TH2F*)hMass_Pt->Clone();
    hMass_Pt_PtCut_0_2->GetYaxis()->SetRangeUser(0, 2);
    hDiMuonMass_PtCut_0_2 = hMass_Pt_PtCut_0_2->ProjectionX("hDiMuonMass_PtCut_0_2");

    hMass_Pt_PtCut_3_4 = (TH2F*)hMass_Pt->Clone();
    hMass_Pt_PtCut_3_4->GetYaxis()->SetRangeUser(3, 4);
    hDiMuonMass_PtCut_3_4 = hMass_Pt_PtCut_3_4->ProjectionX("hDiMuonMass_PtCut_3_4");

    hMass_Pt_PtCut_5_30 = (TH2F*)hMass_Pt->Clone();
    hMass_Pt_PtCut_5_30->GetYaxis()->SetRangeUser(5, 30);
    hDiMuonMass_PtCut_5_30 = hMass_Pt_PtCut_5_30->ProjectionX("hDiMuonMass_PtCut_5_30");


    // **********************************************
    // F  I  T  T  I  N  G
    // **********************************************


    // fitJpsiGauss(hDiMuonMass_PM_Pt_0_2, 0, 2);
    fitJpsiCB(hDiMuonMass_PtCut_0_2,  0, 2, "Chebychev");

    // fitJpsiGauss(hDiMuonMass_PM_Pt_2_5, 2, 5);
    fitJpsiCB(hDiMuonMass_PtCut_3_4,  3, 4, "Chebychev");

    // fitJpsiGauss(hDiMuonMass_PM_Pt_5_30, 5, 30);
    fitJpsiCB(hDiMuonMass_PtCut_5_30, 5, 30, "Chebychev");

    return 0;


 }