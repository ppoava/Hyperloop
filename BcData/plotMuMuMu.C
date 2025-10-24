// plotMuMuMu.C
// Created: 02-08-2025
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

TH1* GetTH1(std::string fileName, std::string histListName, std::string histName)
{
    TFile* f = TFile::Open(fileName.c_str(), "READ");
    if (!f || f->IsZombie()) {
        std::cerr << ">> Error opening file: " << fileName << std::endl;
        return nullptr;
    }

    // Go to directory
    TDirectory* dir = dynamic_cast<TDirectory*>(f->Get("analysis-dilepton-track"));
    if (!dir) {
        std::cerr << ">> Directory 'analysis-dilepton-track' not found." << std::endl;
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
    // --- matchedQualityCuts for GLOBAL MUON tracks
    // --- muonQualityCuts for MUON tracks

    TList* histList = dynamic_cast<TList*>(outputList->FindObject(histListName.c_str()));
    if (!histList) {
        std::cerr << ">> TList " << histListName << " not found." << std::endl;
    }

    // Find histogram by name
    TH1* hist = dynamic_cast<TH1*>(histList->FindObject(histName.c_str()));
    if (!hist) {
        std::cerr << ">> Histogram '" << histName << "' not found." << std::endl;
        f->Close();
        return nullptr;
    }

    // Make a clone to detach from the file before closing
    TH1* histClone = (TH1*)hist->Clone();
    histClone->SetDirectory(nullptr);

    f->Close(); // Close the file

    std::cout << "successful (?)" << std::endl;

    return histClone; // Return the cloned histogram
}

// Use this one for the dimuon spectra from Andrea/Chi's code
// When getting TH2 (mass, pT) and making the projection
TH1 *GetTH1XFromTH2(std::string fileName, std::string histListName, std::string histName, Double_t xMin = -1, Double_t xMax = -1)
{
    TFile *f = new TFile(fileName.c_str(), "READ");
    std::cout << "FUNCTION: Reading " << histName.c_str() << " from TFile" << std::endl;

    // Go to directory
    TDirectory* dir = dynamic_cast<TDirectory*>(f->Get("analysis-dilepton-track"));
    if (!dir) {
        std::cerr << ">> Directory 'analysis-dilepton-track' not found." << std::endl;
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
    // --- matchedQualityCuts for GLOBAL MUON tracks
    // --- muonQualityCuts for MUON tracks

    TList* histList = dynamic_cast<TList*>(outputList->FindObject(histListName.c_str()));
    if (!histList) {
        std::cerr << ">> TList " << histListName << " not found." << std::endl;
    }

    // Find histogram by name
    TH2 *hist = dynamic_cast<TH2*>(histList->FindObject(histName.c_str()));
    if (hist == nullptr)
    {
        std::cout << ">> error retrieving histogram" << std::endl;
    }
    else
    {
        std::cout << ">> histogram sucessfully read from TFile" << std::endl;
    }
    // TH2 *hMass_Pt = (TH2*)hist->Clone();
    if (xMin != -1 && xMax != -1) { hist->GetYaxis()->SetRangeUser(xMin, xMax); }
    std::cout << "problem with projection?" << std::endl;
    if (!hist)
    {
        std::cout << "big problem" << std::endl;
    }
    TH1 *hPt_EtaCut = hist->ProjectionX("hProjX");
    std::cout << "no problem with projection" << std::endl;
    return hPt_EtaCut;
}

TH1 *GetTH1YFromTH2(std::string fileName, std::string histListName, std::string histName, Double_t yMin = -1, Double_t yMax = -1)
{
    TFile *f = new TFile(fileName.c_str(), "READ");
    std::cout << "FUNCTION: Reading " << histName.c_str() << " from TFile" << std::endl;

    // Go to directory
    TDirectory* dir = dynamic_cast<TDirectory*>(f->Get("analysis-dilepton-track"));
    if (!dir) {
        std::cerr << ">> Directory 'analysis-dilepton-track' not found." << std::endl;
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
    // --- matchedQualityCuts for GLOBAL MUON tracks
    // --- muonQualityCuts for MUON tracks

    TList* histList = dynamic_cast<TList*>(outputList->FindObject(histListName.c_str()));
    if (!histList) {
        std::cerr << ">> TList " << histListName << " not found." << std::endl;
    }

    // Find histogram by name
    TH2 *hist = dynamic_cast<TH2*>(histList->FindObject(histName.c_str()));
    if (hist == nullptr)
    {
        std::cout << ">> error retrieving histogram" << std::endl;
    }
    else
    {
        std::cout << ">> histogram sucessfully read from TFile" << std::endl;
    }
    // TH2 *hMass_Pt = (TH2*)hist->Clone();
    if (yMin != -1 && yMax != -1) { hist->GetXaxis()->SetRangeUser(yMin, yMax); }
    std::cout << "problem with projection?" << std::endl;
    if (!hist)
    {
        std::cout << "big problem" << std::endl;
    }
    TH1 *hPt_EtaCut = hist->ProjectionY("hProjY");
    std::cout << "no problem with projection" << std::endl;
    return hPt_EtaCut;
}

// Use this one for the dimuon spectra from Andrea/Chi's code
// When getting TH3 (pT, DCA, phi, etc.) and making the projection
TH1 *GetTH1FromTH3(std::string fileName, std::string histName, Double_t minDimuonMass, Double_t maxDimuonMass, Double_t minDimuonPt, Double_t maxDimuonPt, Double_t minVar, Double_t maxVar)
{
    TFile *f = new TFile(fileName.c_str(), "READ");
    std::cout << "FUNCTION: Reading " << histName.c_str() << " from TFile" << std::endl;
    TH3 *hist = (TH3 *)f->Get(histName.c_str());

    if (hist == nullptr)
    {
        std::cout << ">> error retrieving histogram" << std::endl;
    }
    else
    {
        std::cout << ">> histogram successfully read from TFile" << std::endl;
    }

    std::cout << "Axis titles: "
              << hist->GetXaxis()->GetTitle() << ", "
              << hist->GetYaxis()->GetTitle() << ", "
              << hist->GetZaxis()->GetTitle() << std::endl;

    // Set axis ranges (mass, pT)
    // hist->GetXaxis()->SetRangeUser(minDimuonMass, maxDimuonMass); // mass
    // hist->GetYaxis()->SetRangeUser(minDimuonPt, maxDimuonPt);     // pT
    int binXMin = hist->GetXaxis()->FindBin(minDimuonMass);
    int binXMax = hist->GetXaxis()->FindBin(maxDimuonMass) - 1; // exclusive upper limit
    int binYMin = hist->GetYaxis()->FindBin(minDimuonPt);
    int binYMax = hist->GetYaxis()->FindBin(maxDimuonPt) - 1; // exclusive upper limit
    // hist->GetXaxis()->SetRange(binXMin, binXMax);
    // hist->GetYaxis()->SetRange(binYMin, binYMax);

    if (minVar != -999 && maxVar != -999)
    {
        int binZMin = hist->GetZaxis()->FindBin(minVar);
        int binZMax = hist->GetZaxis()->FindBin(maxVar);
        hist->GetZaxis()->SetRange(binZMin, binZMax);
    }

    std::cout << "problem with projection?" << std::endl;
    if (!hist)
    {
        std::cout << "big problem" << std::endl;
    }

    // Project the Z-axis
    // TH1 *hVar_MassPtCut = hist->ProjectionZ("hVar_MassPtCut");
    TH1 *hVar_MassPtCut = hist->ProjectionZ("hVar_MassPtCut", binXMin, binXMax, binYMin, binYMax);
    std::cout << "no problem with projection" << std::endl;

    return hVar_MassPtCut;
}

void drawMuMuMuTH1(std::string fAnalysisResults, std::string histListName, std::string fAnalysisResults_global, std::string histListName_global, std::vector<std::string> vHistNames) {
    for (int i = 0; i < vHistNames.size(); i++) {
        std::string histName = vHistNames[i];
        TCanvas *c = new TCanvas(Form("c_%s_%s_%s", fAnalysisResults.c_str(), histName.c_str(), histListName.c_str()), Form("c_%s_%s_%s", fAnalysisResults.c_str(), histName.c_str(), histListName.c_str()), 800, 600);
        TH1 *hMuMuMu = GetTH1(fAnalysisResults, histListName, histName);
        TH1 *hMuMuMu_global = GetTH1(fAnalysisResults_global, histListName_global, histName);
        c->cd();
        hMuMuMu->GetYaxis()->SetTitle("entries / total entries");
        hMuMuMu_global->SetLineColor(kRed);
        hMuMuMu->Scale(1/hMuMuMu->GetEntries());
        hMuMuMu_global->Scale(1/hMuMuMu_global->GetEntries());
        hMuMuMu->Draw("HIST E");
        hMuMuMu_global->Draw("SAME HIST E");
    }
}

void drawMuMuMuTH2(std::string fAnalysisResults, std::string histListName, std::string fAnalysisResults_global, std::string histListName_global, std::vector<std::string> vHistNames) {
    for (int i = 0; i < vHistNames.size(); i++) {
        std::string histName = vHistNames[i];
        TCanvas *c = new TCanvas(Form("c1_%s_%s_%s", fAnalysisResults.c_str(), histName.c_str(), histListName.c_str()), Form("c1_%s_%s_%s", fAnalysisResults.c_str(), histName.c_str(), histListName.c_str()), 800, 600);
        TH1 *hMuMuMu = GetTH1XFromTH2(fAnalysisResults, histListName, histName);
        TH1 *hMuMuMu_global = GetTH1XFromTH2(fAnalysisResults_global, histListName_global, histName);
        c->cd();
        hMuMuMu->GetYaxis()->SetTitle("entries / total entries");
        hMuMuMu_global->SetLineColor(kRed);
        hMuMuMu->Scale(1/hMuMuMu->GetEntries());
        hMuMuMu_global->Scale(1/hMuMuMu_global->GetEntries());
        hMuMuMu->Draw("HIST E");
        hMuMuMu_global->Draw("SAME HIST E");

        c = new TCanvas(Form("c2_%s_%s_%s", fAnalysisResults.c_str(), histName.c_str(), histListName.c_str()), Form("c2_%s_%s_%s", fAnalysisResults.c_str(), histName.c_str(), histListName.c_str()), 800, 600);
        hMuMuMu = GetTH1YFromTH2(fAnalysisResults, histListName, histName);
        hMuMuMu_global = GetTH1YFromTH2(fAnalysisResults_global, histListName_global, histName);
        c->cd();
        hMuMuMu->GetYaxis()->SetTitle("entries / total entries");
        hMuMuMu_global->SetLineColor(kRed);
        hMuMuMu->Scale(1/hMuMuMu->GetEntries());
        hMuMuMu_global->Scale(1/hMuMuMu_global->GetEntries());
        hMuMuMu->Draw("HIST E");
        hMuMuMu_global->Draw("SAME HIST E");
    }
}






int plotMuMuMu() {

    // 22_09_2025: correct MFT acceptance for both MUON and global
    // 14_10_2025: only save best matching candidate
    std::string fAnalysisResults = "AnalysisResults_DQ_LHC24_pass1_skimmed_DiMuon_time_assoc_MUON_Hyperloop_23_10_2025.root";
    std::string fAnalysisResults_global = "AnalysisResults_DQ_LHC24_pass1_skimmed_DiMuon_global_time_assoc_GLOBAL_Hyperloop_23_10_2025.root";
    // std::string fAnalysisResults = "AnalysisResults_muon_minbias_Hyperloop_06_10_2025.root";
    // std::string fAnalysisResults_global = "AnalysisResults_global_minbias_Hyperloop_06_10_2025.root";
    std::string histListName = "DileptonTrack_muonQualityCuts_muonQualityCuts";
    std::string histListName_global = "DileptonTrack_matchedGlobal_matchedGlobal";
    std::vector<std::string> vHistNamesBcTH1;
    vHistNamesBcTH1.push_back("Mass");
    vHistNamesBcTH1.push_back("Mass_Dilepton");
    vHistNamesBcTH1.push_back("Pt");
    vHistNamesBcTH1.push_back("Pt_Dilepton");
    vHistNamesBcTH1.push_back("Pt_Track");
    vHistNamesBcTH1.push_back("Tauz");
    drawMuMuMuTH1(fAnalysisResults, histListName, fAnalysisResults_global, histListName_global, vHistNamesBcTH1);

    std::vector<std::string> vHistNamesBcTH2;
    vHistNamesBcTH2.push_back("DeltaEta_DeltaPhi");

    drawMuMuMuTH2(fAnalysisResults, histListName, fAnalysisResults_global, histListName_global, vHistNamesBcTH2);

    // histListName = "DileptonsSelected_muonQualityCuts";
    // histListName_global = "DileptonsSelected_matchedGlobal";
    // drawMuMuMuTH1(fAnalysisResults, histListName, fAnalysisResults_global, histListName_global, vHistNames);

    return 0;
}