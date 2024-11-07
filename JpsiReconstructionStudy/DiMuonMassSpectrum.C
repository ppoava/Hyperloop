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
#include "RooFitResult.h"

using namespace RooFit;

void fitJpsi(TH1D* hist) {
    // Step 1: Define the x-axis range
    double mMin = hist->GetXaxis()->GetXmin();
    double mMax = hist->GetXaxis()->GetXmax();
    RooRealVar m("m", "m", mMin, mMax);

    // Step 2: Convert the TH1D histogram to a RooDataHist
    RooDataHist data("data", "Di-muon spectrum", m, Import(*hist));

    // Step 3: Define a Gaussian model
    RooRealVar mean("mean", "mean", hist->GetMean(), mMin, mMax);
    RooRealVar sigma("sigma", "width", hist->GetRMS(), 0, (mMax - mMin) / 2);
    RooGaussian gauss("gauss", "Gaussian PDF", m, mean, sigma);

    // Step 4: Fit the model to the data
    m.setRange("fitRange", 2.8, 3.4);  // Define the fit range
    RooFitResult* result = gauss.fitTo(data, Save(), Range("fitRange"));

    // Step 5: Plot the results
    TCanvas* c = new TCanvas("c", "Fit Result", 800, 600);
    c->cd();
    RooPlot* frame = m.frame();
    data.plotOn(frame, MarkerSize(0.5));
    // data.plotOn(frame, Binning(40));  // Adjust binning if needed
    gauss.plotOn(frame, Range("fitRange"));  // Plot model in fit range

    frame->Draw();

    // Optional: Print fit results
    result->Print(); 
} // fitJpsi

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

    fitJpsi(hDiMuonMass_PM);
    
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
    drawHist(hMass_Pt_PM, hMass_Pt_PP, hMass_Pt_MM);

    return 0;
 }