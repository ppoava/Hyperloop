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
#include "RooPlot.h"
#include "RooGaussian.h"
#include "RooCrystalBall.h"
#include "RooFitResult.h"

using namespace RooFit;

void fitJpsiGauss(TH1D* hist) {
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

void fitJpsiCB(TH1D* hist) {
    // Initialise
    RooRealVar m("m", "invariant mass", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    m.setRange("fitRange", 2.9, 3.25);
    RooDataHist* data = new RooDataHist("data", "Di-muon spectrum", m, Import(*hist));

    // Crystal Ball
    RooRealVar m0("m0", "Mean", 3.097, 3.05, 3.13);
    RooRealVar sigma("sigma", "Sigma of Gaussian", 0.08, 0.05, 0.12);
    RooRealVar alphaL("alphaL", "Alpha Left", 0.883, 0.5, 3.0);
    RooRealVar nL("nL", "Exponent Left", 9.940, 5.0, 20.0); 
    RooRealVar alphaR("alphaR", "Alpha Right", 1.832, 1.0, 3.0);
    RooRealVar nR("nR", "Exponent Right", 15.323, 5.0, 25.0);  

    RooCrystalBall doubleSidedCB("doubleSidedCB", "Double Sided Crystal Ball", m, m0, sigma, alphaL, nL, alphaR, nR);
    doubleSidedCB.fitTo(*data, Range("fitRange"));

    // Plot
    RooPlot* frame = m.frame();
    data->plotOn(frame, MarkerSize(0.4));
    doubleSidedCB.plotOn(frame);
    TCanvas* canvas = new TCanvas("canvas", "Double Sided Crystal Ball Fit", 800, 600);
    canvas->cd();
    frame->Draw();

    double chi2 = frame->chiSquare();
    TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->AddEntry(frame->getObject(0), "Data", "point");
    legend->AddEntry(frame->getObject(1), Form("#chi^{2} = %.2f", chi2), "l");
    legend->Draw();

    m0.Print();
    sigma.Print();
    alphaL.Print();
    nL.Print();
    alphaR.Print();
    nR.Print();

} // fitJpsiCB

void drawHist(TH2F* hMass_Pt_PM)
 {
    TH1D *hDiMuonMass_PM;
    hMass_Pt_PM->GetYaxis()->SetRangeUser(5, 30);
    hDiMuonMass_PM = hMass_Pt_PM->ProjectionX("hDiMuonMass_PM");
    // hMass_Pt_PM->Scale(1/(hGlobalMuonMass->Integral()));   

    // Plot the results
    TCanvas* c = new TCanvas("c", "Signal Extraction", 800, 600);
    hDiMuonMass_PM->SetLineColor(kBlack);  
    // hDiMuonMass_PM->SetLineStyle(2);    
    hDiMuonMass_PM->Draw("hist E");

    // fitJpsiGauss(hDiMuonMass_PM);
    fitJpsiCB(hDiMuonMass_PM);
    
    /*
    // Draw legend
    TLegend *leg = new TLegend(0.2, 0.6, 0.4, 0.8);
    leg->AddEntry((TObject*)0, "muonQualityCuts", "");
    leg->AddEntry(hGlobalMuonMass, "global muon pp");
    leg->Draw("same");
        */

    } // drawHist()


int DiMuonMassSpectrum() 
 {
    THashList *listDiMuon;
    TList *subListDiMuonPM, *subListDiMuonPP, *subListDiMuonMM;
    TFile* fDiMuon = new TFile("AnalysisResults.root", "READ");
    listDiMuon = (THashList*)fDiMuon->Get("analysis-same-event-pairing/output");
    subListDiMuonPM = (TList*)listDiMuon->FindObject("PairsMuonSEPM_muonLowPt510SigmaPDCA");
    subListDiMuonPP = (TList*)listDiMuon->FindObject("PairsMuonSEPP_muonLowPt510SigmaPDCA");
    subListDiMuonMM = (TList*)listDiMuon->FindObject("PairsMuonSEMM_muonLowPt510SigmaPDCA");

    TH2F *hMass_Pt_PM, *hMass_Pt_PP, *hMass_Pt_MM;
    hMass_Pt_PM = (TH2F*)subListDiMuonPM->FindObject("Mass_Pt");
    hMass_Pt_PP = (TH2F*)subListDiMuonPP->FindObject("Mass_Pt");
    hMass_Pt_MM = (TH2F*)subListDiMuonMM->FindObject("Mass_Pt");
    drawHist(hMass_Pt_PM);

    return 0;
 }