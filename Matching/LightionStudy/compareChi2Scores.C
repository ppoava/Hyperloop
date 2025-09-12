// compareChi2Scores.C
// simple macro to check the matching Chi2 scores accross different collision systems (pp, pO, OO, NeNe)

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

TH1* GetTH1(std::string fileName, std::string histName)
{
    TFile* f = TFile::Open(fileName.c_str(), "READ");
    if (!f || f->IsZombie()) {
        std::cerr << ">> Error opening file: " << fileName << std::endl;
        return nullptr;
    }

    // Go to directory
    TDirectory* dir = dynamic_cast<TDirectory*>(f->Get("table-maker"));
    if (!dir) {
        std::cerr << ">> Directory 'table-maker' not found." << std::endl;
        f->Close();
        return nullptr;
    }

    // Get the THashList output
    THashList* outputList = dynamic_cast<THashList*>(dir->Get("output"));
    if (!outputList) {
        std::cerr << ">> THashList 'output' not found." << std::endl;
        f->Close();
        return nullptr;
    }

    // Get the TList with histograms
    TList* histList = dynamic_cast<TList*>(outputList->FindObject("Muons_muonQualityCuts"));
    if (!histList) {
        histList = dynamic_cast<TList*>(outputList->FindObject("Muons_matchedQualityCuts"));
        std::cout << ">> Problem with histList?" << std::endl;
    }

    // Find histogram by name
    TH1* hist = dynamic_cast<TH1*>(histList->FindObject(histName.c_str()));
    if (!hist) {
        std::cerr << ">> Histogram '" << histName << "' not found." << std::endl;
        f->Close();
        return nullptr;
    }

    return hist;

    /*
    // Make a clone to detach from the file before closing
    TH1* histClone = (TH1*)hist->Clone();
    histClone->SetDirectory(nullptr);

    f->Close(); // Close the file

    return histClone; // Return the cloned histogram
    */
}

void doCompare(const char *fpp, const char *fpO, const char *fOO, const char* fNeNe) {
    TH1 *hChi2MCHMFT_pp = GetTH1(fpp, "Chi2MCHMFT");
    TH1 *hChi2MCHMFT_pO = GetTH1(fpO, "Chi2MCHMFT");
    TH1 *hChi2MCHMFT_OO = GetTH1(fOO, "Chi2MCHMFT");
    TH1 *hChi2MCHMFT_NeNe = GetTH1(fNeNe, "Chi2MCHMFT");

    hChi2MCHMFT_pp->Scale(1.0 / hChi2MCHMFT_pp->GetEntries());
    hChi2MCHMFT_pO->Scale(1.0 / hChi2MCHMFT_pO->GetEntries());
    hChi2MCHMFT_OO->Scale(1.0 / hChi2MCHMFT_OO->GetEntries());
    hChi2MCHMFT_NeNe->Scale(1.0 / hChi2MCHMFT_NeNe->GetEntries());

    hChi2MCHMFT_pp->GetYaxis()->SetTitle("entries / total entries");
    hChi2MCHMFT_pp->SetStats(0);
    hChi2MCHMFT_pp->SetLineColor(kBlack);
    hChi2MCHMFT_pO->SetLineColor(kRed);
    hChi2MCHMFT_OO->SetLineColor(kGreen);
    hChi2MCHMFT_NeNe->SetLineColor(kBlue);

    TCanvas *c = new TCanvas("c", "c");
    c->cd();
    hChi2MCHMFT_pp->Draw("HIST");
    hChi2MCHMFT_pO->Draw("SAME HIST");
    hChi2MCHMFT_OO->Draw("SAME HIST");
    hChi2MCHMFT_NeNe->Draw("SAME HIST");
    
    
    TLegend *l = new TLegend(0.5, 0.6, 0.8, 0.8);;
    l->AddEntry(hChi2MCHMFT_pp, "pp", "l");
    l->AddEntry(hChi2MCHMFT_pO, "pO", "l");
    l->AddEntry(hChi2MCHMFT_OO, "OO", "l");
    l->AddEntry(hChi2MCHMFT_NeNe, "NeNe", "l");
    l->SetBorderSize(0);
    l->Draw();
}

int compareChi2Scores() {
    const char *fpp = "AnalysisResults_LHC24ap_pass1_small_pp_globalmuon_Hyperloop_12_09_2025.root";
    const char *fpO = "AnalysisResults_LHC24ad_pass2_pO_Hyperloop_12_09_2025.root";
    const char *fOO = "AnalysisResults_LHC24ae_pass2_small_OO_Hyperloop_12_09_2025.root";
    const char *fNeNe = "AnalysisResults_LHC24af_pass2_small_NeNe_Hyperloop_12_09_2025.root";

    doCompare(fpp, fpO, fOO, fNeNe);

    return 0;
}