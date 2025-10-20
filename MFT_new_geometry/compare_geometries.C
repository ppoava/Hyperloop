// compare_geometries.C

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

TH1D *getTH1(std::string fileName)
{
    THashList *listDiMuon;
    TList *subListDiMuon;

    TFile *fDiMuon = new TFile(fileName.c_str(), "READ");
    listDiMuon = (THashList *)fDiMuon->Get("analysis-event-selection/output");
    subListDiMuon = (TList *)listDiMuon->FindObject("Event_BeforeCuts");
    TH1D *hVtxZ = (TH1D *)subListDiMuon->FindObject("VtxZ");

    return hVtxZ;

} // void getTree()

int compare_geometries() {
    TH1D *hVtxZ_ref = getTH1("AnalysisResults_MUON_ref_MFT_one_run_Hyperloop_07_10_2025.root");
    TH1D *hVtxZ_new = getTH1("AnalysisResults_MUON_new_MFT_one_run_Hyperloop_07_10_2025.root");
    TCanvas *c = new TCanvas("canvas", "canvas", 800, 600);
    c->cd();
    hVtxZ_ref->Draw("HIST");
    hVtxZ_new->Draw("HIST SAME");
    //hVtxZ_ref->Scale(1./hVtxZ_ref->GetEntries());
    //hVtxZ_new->Scale(1./hVtxZ_new->GetEntries());
    hVtxZ_ref->SetLineColor(kBlack);
    hVtxZ_new->SetLineColor(kRed);

    return 0;
}