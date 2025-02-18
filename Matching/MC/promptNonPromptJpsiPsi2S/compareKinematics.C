// compareKinematics.C
// Simple macro to compare prompt and non-prompt MC Jpsi + Psi (2S) results


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


TH1D* getTree(const char* fileName, Double_t ptMin, Double_t ptMax) {


THashList *listDiMuon;
TList *subListDiMuon;
TH2F *hMass_Pt;

TFile *fDiMuon = new TFile(fileName,"READ");
listDiMuon = (THashList*)fDiMuon->Get("analysis-same-event-pairing/output");
subListDiMuon = (TList*)listDiMuon->FindObject("PairsMuonSEPM_muonQualityCuts");
hMass_Pt = (TH2F*)subListDiMuon->FindObject("Mass_Pt");


// **********************************************
// S  E  T    P  _  T    C  U  T
// **********************************************


TH1D *hDiMuonMass_PtCut;

hMass_Pt = (TH2F*)hMass_Pt->Clone();
hMass_Pt->GetYaxis()->SetRangeUser(ptMin,ptMax);
hDiMuonMass_PtCut = hMass_Pt->ProjectionX("hDiMuonMass_PtCut");


return hDiMuonMass_PtCut;


} // getTree()


TCanvas* plotHist(TH1D *hNonPrompt, TH1D *hPrompt) {


    TCanvas *canvas;
    canvas->cd();
    hNonPrompt->Draw("HIST E");


    return canvas;


} // plotHist()


int compareKinematics() {


    std::cout<<"running compareKinematics macro"<<std::endl;


    TH1D *hist = getTree("/Users/pv280546/alice/nonPromptJpsiTest_AnalysisResults-3.root", 0, 30);
    TCanvas *canvas = plotHist(hist, hist);
    canvas->Draw();


    return 0;


} // compareKinematics()