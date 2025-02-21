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
#include "THnSparse.h"



TH1D* getHistFromTree(const char* fileName, Double_t ptMin, Double_t ptMax) {


THashList *listDiMuon;
TList *subListDiMuon;
TFile *fDiMuon = new TFile(fileName,"READ");
listDiMuon = (THashList*)fDiMuon->Get("analysis-same-event-pairing/output");
subListDiMuon = (TList*)listDiMuon->FindObject("PairsMuonSEPM_muonQualityCuts");


// **********************************************
// S  E  T    P  _  T    C  U  T
// **********************************************


// Approach with "dimuon" histogram option in same-event-pairing
/*
TH2F *hMass_Pt;
hMass_Pt = (TH2F*)subListDiMuon->FindObject("Mass_Pt");
TH1D *hDiMuonMass_PtCut;
hMass_Pt = (TH2F*)hMass_Pt->Clone();
hMass_Pt->GetYaxis()->SetRangeUser(ptMin,ptMax);
hDiMuonMass_PtCut = hMass_Pt->ProjectionX("hDiMuonMass_PtCut");
hDiMuonMass_PtCut->GetXaxis()->SetRangeUser(0,8); // plot only relevant mass range
*/


// Approach with "dimuon-rap" histogram option in same-event-pairing
THnSparse *hMass_Pt_Cent_Rap;
hMass_Pt_Cent_Rap = (THnSparse*)subListDiMuon->FindObject("Mass_Pt_Cent_Rap");
THnSparse* hSparse = (THnSparse*)hMass_Pt_Cent_Rap->Clone();

// Define the axis indices
const int kMassAxis = 0;  // Mass axis index
const int kPtAxis = 1;    // pT axis index
const int kCentAxis = 2;  // Centrality axis index
const int kRapAxis = 3;   // Rapidity axis index

// Set axis ranges
// double massMin = 0., massMax = 8.;  // Example mass range
double ptCutMin = 0., ptCutMax = 15.;        // Example pT range
// double rapMin = -0.8, rapMax = 0.8; // Example rapidity range
// double centMin = 0, centMax = 10;   // Example centrality range
// Apply axis range cuts
// hSparse->GetAxis(kMassAxis)->SetRangeUser(massMin, massMax);
hSparse->GetAxis(kPtAxis)->SetRangeUser(ptMin, ptMax);
// hSparse->GetAxis(kRapAxis)->SetRangeUser(rapMin, rapMax);
// hSparse->GetAxis(kCentAxis)->SetRangeUser(centMin, centMax);

// Project onto the pT axis
TH1D *hist;
hist = (TH1D*)hSparse->Projection(kPtAxis);
// hist = (TH1D*)hSparse->Projection(kMassAxis);
// hist = (TH1D*)hSparse->Projection(kRapAxis);


return hist;


} // getHistFromTree()


// VtxZ taken from analysis-event-selection is the PV -- doesn't tell you anything about prompt/non-prompt
// Why does the 'vertexing' option on hyperloop not give you the dimuon production vertex?
TH1D* getVertexHistFromTree(const char* fileName, Double_t ptMin, Double_t ptMax) {


THashList *list;
TList *subList;
TFile *fDiMuon = new TFile(fileName,"READ");
list = (THashList*)fDiMuon->Get("analysis-event-selection/output");
subList = (TList*)list->FindObject("Event_AfterCuts");

TH1D *hist = (TH1D*)subList->FindObject("VtxZ");


return hist;


} // getVertexHistFromTree()


TCanvas* plotHist(const char* canvasName, TH1D *hNonPrompt, TH1D *hPrompt) {


    hNonPrompt->Scale(1/hNonPrompt->GetEntries());
    hNonPrompt->SetLineColor(kRed);
    hPrompt->Scale(1/hPrompt->GetEntries());


    TCanvas *canvas = new TCanvas(Form("%s",canvasName), Form("%s",canvasName), 800, 600);
    canvas->cd();
    hPrompt->Draw("HIST E");
    hNonPrompt->Draw("SAME HIST E");
    

    TLegend *legend = new TLegend(0.80,0.60,0.90,0.75);
    legend->AddEntry(hNonPrompt, "non-prompt", "l");
    legend->AddEntry(hPrompt, "prompt", "l");
    legend->Draw();


    return canvas;


} // plotHist()


int compareKinematics() {


    std::cout<<"running compareKinematics macro"<<std::endl;


    TH1D *hNonPrompt = getHistFromTree("/Users/pv280546/alice/nonPromptJpsiTest_AnalysisResults-3.root", 0, 30);
    TH1D *hPrompt = getHistFromTree("/Users/pv280546/alice/promptJpsiTest_AnalysisResults-3.root", 0, 30);
    TCanvas *canvasKinematics = plotHist("kinematics", hNonPrompt, hPrompt);
    canvasKinematics->Draw();

    TH1D *hVtxZNonPrompt = getVertexHistFromTree("/Users/pv280546/alice/nonPromptJpsiTest_AnalysisResults-3.root", 0, 30);
    TH1D *hVtxZPrompt = getVertexHistFromTree("/Users/pv280546/alice/promptJpsiTest_AnalysisResults-3.root", 0, 30);
    TCanvas *canvasVtxZ = plotHist("vtxz", hVtxZNonPrompt, hVtxZPrompt);
    canvasVtxZ->Draw();


    return 0;


} // compareKinematics()