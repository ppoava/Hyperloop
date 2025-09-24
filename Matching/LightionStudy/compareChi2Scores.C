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

// TODO: put arguments for the TDirectory and TList and stuff
TH1 *GetTH1(std::string fileName, std::string histName, bool doMC)
{
    TFile *f = TFile::Open(fileName.c_str(), "READ");
    if (!f || f->IsZombie())
    {
        std::cerr << ">> Error opening file: " << fileName << std::endl;
        return nullptr;
    }

    // Go to directory
    TDirectory *dir;
    if (doMC)
    {
        dir = dynamic_cast<TDirectory *>(f->Get("table-maker-m-c"));
        if (!dir)
        {
            std::cerr << ">> Directory 'table-maker-m-c' not found." << std::endl;
            f->Close();
            return nullptr;
        }
    }
    else
    {
        dir = dynamic_cast<TDirectory *>(f->Get("table-maker"));
        if (!dir)
        {
            std::cerr << ">> Directory 'table-maker' not found." << std::endl;
            f->Close();
            return nullptr;
        }
    }

    // Get the THashList output
    THashList *outputList = dynamic_cast<THashList *>(dir->Get("output"));
    if (!outputList)
    {
        std::cerr << ">> THashList 'output' not found." << std::endl;
        f->Close();
        return nullptr;
    }

    // Get the TList with histograms
    TList *histList = dynamic_cast<TList *>(outputList->FindObject("Muons_muonQualityCuts"));
    if (!histList)
    {
        histList = dynamic_cast<TList *>(outputList->FindObject("Muons_matchedQualityCuts"));
        std::cout << ">> Problem with histList?" << std::endl;
    }

    // Find histogram by name
    TH1 *hist = dynamic_cast<TH1 *>(histList->FindObject(histName.c_str()));
    if (!hist)
    {
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

void doCompare(const char *fpp, const char *fpO, const char *fOO, const char *fNeNe,
               bool doMC, const char *fppMC, const char *fpOMC, const char *fOOMC, const char *fNeNeMC)
{
    TH1 *hChi2MCHMFT_pp = GetTH1(fpp, "Chi2MCHMFT", false);
    TH1 *hChi2MCHMFT_pO = GetTH1(fpO, "Chi2MCHMFT", false);
    TH1 *hChi2MCHMFT_OO = GetTH1(fOO, "Chi2MCHMFT", false);
    TH1 *hChi2MCHMFT_NeNe = GetTH1(fNeNe, "Chi2MCHMFT", false);

    //TH1 *hChi2MCHMFT_pp_MC;
    TH1 *hChi2MCHMFT_pO_MC;
    TH1 *hChi2MCHMFT_OO_MC;
    TH1 *hChi2MCHMFT_NeNe_MC;
    if (doMC)
    {
        //hChi2MCHMFT_pp_MC = GetTH1(fppMC, "Chi2MCHMFT", true);
        hChi2MCHMFT_pO_MC = GetTH1(fpOMC, "Chi2MCHMFT", true);
        hChi2MCHMFT_OO_MC = GetTH1(fOOMC, "Chi2MCHMFT", true);
        hChi2MCHMFT_NeNe_MC = GetTH1(fNeNeMC, "Chi2MCHMFT", true);
    }

    hChi2MCHMFT_pp->Scale(1.0 / hChi2MCHMFT_pp->GetEntries());
    hChi2MCHMFT_pO->Scale(1.0 / hChi2MCHMFT_pO->GetEntries());
    hChi2MCHMFT_OO->Scale(1.0 / hChi2MCHMFT_OO->GetEntries());
    hChi2MCHMFT_NeNe->Scale(1.0 / hChi2MCHMFT_NeNe->GetEntries());

    if (doMC)
    {
        //hChi2MCHMFT_pp_MC->Scale(1.0 / hChi2MCHMFT_pp_MC->GetEntries());
        hChi2MCHMFT_pO_MC->Scale(1.0 / hChi2MCHMFT_pO_MC->GetEntries());
        hChi2MCHMFT_OO_MC->Scale(1.0 / hChi2MCHMFT_OO_MC->GetEntries());
        hChi2MCHMFT_NeNe_MC->Scale(1.0 / hChi2MCHMFT_NeNe_MC->GetEntries());
    }

    hChi2MCHMFT_pp->GetYaxis()->SetTitle("entries / total entries");
    hChi2MCHMFT_pp->GetYaxis()->SetRangeUser(0, 0.15);
    hChi2MCHMFT_pp->SetStats(0);
    hChi2MCHMFT_pp->SetLineColor(kBlack);
    hChi2MCHMFT_pO->SetLineColor(kRed);
    hChi2MCHMFT_OO->SetLineColor(kGreen);
    hChi2MCHMFT_NeNe->SetLineColor(kBlue);

    if (doMC)
    {
        //hChi2MCHMFT_pp_MC->SetLineColor(kBlack);
        hChi2MCHMFT_pO_MC->SetLineColor(kRed);
        hChi2MCHMFT_OO_MC->SetLineColor(kGreen);
        hChi2MCHMFT_NeNe_MC->SetLineColor(kBlue);
        //hChi2MCHMFT_pp_MC->SetLineStyle(kDashed);
        hChi2MCHMFT_pO_MC->SetLineStyle(kDashed);
        hChi2MCHMFT_OO_MC->SetLineStyle(kDashed);
        hChi2MCHMFT_NeNe_MC->SetLineStyle(kDashed);
    }

    TCanvas *c = new TCanvas("c", "c");
    c->cd();
    hChi2MCHMFT_pp->Draw("HIST");
    hChi2MCHMFT_pO->Draw("SAME HIST");
    hChi2MCHMFT_OO->Draw("SAME HIST");
    hChi2MCHMFT_NeNe->Draw("SAME HIST");

    if (doMC)
    {
        //hChi2MCHMFT_pp_MC->Draw("HIST");
        hChi2MCHMFT_pO_MC->Draw("SAME HIST");
        hChi2MCHMFT_OO_MC->Draw("SAME HIST");
        hChi2MCHMFT_NeNe_MC->Draw("SAME HIST");
    }

    TLegend *l = new TLegend(0.5, 0.6, 0.8, 0.8);
    ;
    l->AddEntry(hChi2MCHMFT_pp, "pp", "l");
    l->AddEntry(hChi2MCHMFT_pO, "pO", "l");
    l->AddEntry(hChi2MCHMFT_OO, "OO", "l");
    l->AddEntry(hChi2MCHMFT_NeNe, "NeNe", "l");
    if (doMC)
    {
        //l->AddEntry(hChi2MCHMFT_pp_MC, "pp mc", "l");
        l->AddEntry(hChi2MCHMFT_pO_MC, "pO mc", "l");
        l->AddEntry(hChi2MCHMFT_OO_MC, "OO mc", "l");
        l->AddEntry(hChi2MCHMFT_NeNe_MC, "NeNe mc", "l");
    }
    l->SetBorderSize(0);
    l->Draw();
}

int compareChi2Scores()
{
    const char *fpp = "AnalysisResults_LHC24ap_pass1_small_pp_globalmuon_Hyperloop_12_09_2025.root";
    const char *fpO = "AnalysisResults_pO_Hyperloop_18_09_2025.root";
    const char *fOO = "AnalysisResults_OO_Hyperloop_18_09_2025.root";
    const char *fNeNe = "AnalysisResults_NeNe_Hyperloop_18_09_2025.root";
    const char *fppMC;
    const char *fpOMC = "AnalysisResults_mc_pO_Hyperloop_18_09_2025.root";
    const char *fOOMC = "AnalysisResults_mc_OO_Hyperloop_18_09_2025.root";
    const char *fNeNeMC = "AnalysisResults_mc_NeNe_Hyperloop_18_09_2025.root";

    doCompare(fpp, fpO, fOO, fNeNe, true, fppMC, fpOMC, fOOMC, fNeNeMC);

    return 0;
}