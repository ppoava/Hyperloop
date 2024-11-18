// DiMuonMassSpectrum.C
// Created: 07 NOV 2024
// Paul VEEN (paveen)

#include <iostream>
#include <string>
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

using namespace RooFit;

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


    Double_t mMin = 2;
    Double_t mMax = 5;
    RooRealVar m(Form("m_Pt_%.0f_%.0f", pTMin, pTMax), "invariant mass", mMin, mMax);
    m.setRange("fitRange", 2, 5);
    RooDataHist* data = new RooDataHist(Form("data_Pt_%.0f_%.0f", pTMin, pTMax), "Di-muon spectrum", m, Import(*hist));

    RooAddPdf* model;
    RooPlot* frame;

    // Crystal Ball
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

    // Signal model
    RooCrystalBall* doubleSidedCB = new RooCrystalBall(Form("doubleSidedCB_Pt_%.0f_%.0f", pTMin, pTMax), 
                                                            "Double Sided Crystal Ball", m, m0, sigma, alphaL, nL, alphaR, nR);


    // **********************************************
    // G  E  T    D  A  T  A
    // **********************************************


    RooAbsPdf* BKG;

    // Exponential for background
    // RooRealVar a0("a0","a_{0}",-1,-5,0.000001);
	// RooExponential* BKG = new RooExponential("BKG","Power background", m, a0);

    // Initiallise Chebychev
    RooRealVar a0("a0", "a_{0}", -0.01, -2.0, 2.0);
    RooRealVar a1("a1", "a_{1}", 0.0, -1.0, 1.0);
    RooRealVar a2("a2", "a_{2}", 0.0, -0.5, 0.5);
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

    // Initialise a variable width Gaussian for background
    // Define width = base + slope * m
        std::cout<<"hoi"<<std::endl;
        RooRealVar BKG_mean("BKG_mean", "Mean of background Gaussian", 2.2);
        RooRealVar BKG_base("BKG_base", "Base of variable width Gaussian", 1.0, 0.1, 10.0);
        RooRealVar BKG_slope("BKG_slope", "Slope of variable width Gaussian", 0.1, -1.0, 1.0);
        RooFormulaVar BKG_sigma("BKG_sigma", "Variable width of Background Gaussian", "@0 + @1*@2", 
                                RooArgList(BKG_base, BKG_slope, m));
    
    // Select background model (all are initialised, one is picked)
    if (BKG_model == "Chebychev") {
        BKG = new RooChebychev(Form("BKG_Pt_%.0f_%.0f", pTMin, pTMax),"Chebyshev background", m, {a0,a1,a2,a3,a4,a5,a6,a7});
    }

    if (BKG_model == "Gaussian") {
        BKG = new RooGaussian("gauss", "Gaussian with variable width", m, BKG_mean, BKG_sigma);
    }

    // Add them
    RooRealVar sigYield("sigYield", "N_{sig}", 50000, 0., 100000000);
    RooRealVar bkgYield("bkgYield", "N_{bkg}", 1000000, 0., 1000000000);
    model = new RooAddPdf(Form("model_Pt_%.0f_%.0f", pTMin, pTMax), "combined mass model", 
                                {*doubleSidedCB, *BKG}, {sigYield, bkgYield});

    // Start iterative fitting (only for Chebychev)
    // Release orders one-by-one as long as reduced χ2 is above desired value
    Double_t chi2M = 100000; // arbitrary starting value
    if (BKG_model == "Chebychev") {
        for (Int_t i = 0; i < parameters.size(); i++) {
            if (chi2M < 0 || chi2M > 1.4) {
                parameters[i]->setConstant(kFALSE);
                model->fitTo(*data, Range("fitRange"));
                frame = m.frame();
                data->plotOn(frame, MarkerSize(0.4));
                model->plotOn(frame, LineColor(kBlue), Name("full_Model"));
                chi2M = frame->chiSquare();
                std::cout << "chi2M = " << chi2M << std::endl;
            }
            else { break; }
            if (i == 7 && (chi2M < 0 || chi2M > 1.4)) { std::cout << "WARNING. Fitting failed. No convergence" << std::endl; }
        }
    }

    if (BKG_model != "Chebychev") {
        model->fitTo(*data, Range("fitRange"));
        frame = m.frame();
        data->plotOn(frame, MarkerSize(0.4));
        model->plotOn(frame, LineColor(kBlue), Name("full_Model"));
        chi2M = frame->chiSquare();
        std::cout << "chi2M = " << chi2M << std::endl;
    }


    // **********************************************
    // P  L  O  T  T  I  N  G
    // **********************************************


    TCanvas* canvas = new TCanvas(Form("canvas_Pt_%.0f_%.0f", pTMin, pTMax), 
                                  Form("Double Sided Crystal Ball Fit %.0f < p_{T} < %.0f", pTMin, pTMax),
                                  800, 600);
    canvas->cd();

    model->plotOn(frame, Components(*doubleSidedCB), LineStyle(kDashed), LineColor(kRed), Name("signal_Model"));
   	model->plotOn(frame, Components(*BKG), LineStyle(kDashed), LineColor(kBlue), Name("bkg_Model"));
   	model->paramOn(frame,ShowConstants(true),Format("TE",AutoPrecision(3)));

    frame->Draw();

    // RooChi2Var* chi2Var = new RooChi2Var("chi2","chi2", *doubleSidedCB, *data, true, RooAbsData::ErrorType::Poisson);
    // double chi2M = chi2Var->getVal();

    TLegend *legend = new TLegend(0.6, 0.7, 0.82, 0.8);
    legend->SetTextSize(0.03);
    legend->AddEntry("", Form("%.0f < p_{T} < %.0f [GeV]", pTMin, pTMax), "");
    legend->AddEntry(frame->getObject(0), "Data", "point");
    legend->AddEntry(frame->getObject(1), Form("#chi^{2}/ndf = %.2f", chi2M), "l");
    legend->Draw();

    m0.Print();
    sigma.Print();
    alphaL.Print();
    nL.Print();
    alphaR.Print();
    nR.Print();

    TCanvas* pullCanvas = new TCanvas(Form("pull_canvas_Pt_%.0f_%.0f", pTMin, pTMax), 
                                  Form("Pull canvas %.0f < p_{T} < %.0f", pTMin, pTMax),
                                  800, 600);
    // Construct a histogram with the pulls of the data w.r.t the curve
   RooHist *hpull = frame->pullHist();
   // Create a new frame to draw the pull distribution and add the distribution to the frame
   RooPlot *frame_pull = m.frame(Title("Pull Distribution"));
   // frame_pull->addPlotable(hpull, "P");
   pullCanvas->cd();
   hpull->Draw();
   // frame_pull->Draw();

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
    TFile* fDiMuon = new TFile("AnalysisResults.root", "READ");
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
    fitJpsiCB(hDiMuonMass_PtCut_0_2,  0, 2, "Gaussian");

    // fitJpsiGauss(hDiMuonMass_PM_Pt_2_5, 2, 5);
    fitJpsiCB(hDiMuonMass_PtCut_3_4,  3, 4, "Chebychev");

    // fitJpsiGauss(hDiMuonMass_PM_Pt_5_30, 5, 30);
    fitJpsiCB(hDiMuonMass_PtCut_5_30, 5, 30, "Chebychev");

    return 0;
 }