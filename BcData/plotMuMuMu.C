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

TH1* GetTH1(std::string fileName, std::string histName)
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
    TList* histList = dynamic_cast<TList*>(outputList->FindObject("DileptonTrack_muonQualityCuts_muonQualityCuts"));
    if (!histList) {
        std::cerr << ">> TList 'DileptonTrack_muonQualityCuts_muonQualityCuts' not found." << std::endl;
        f->Close();
        return nullptr;
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

    return histClone; // Return the cloned histogram
}

// Use this one for the dimuon spectra from Andrea/Chi's code
// When getting TH2 (mass, pT) and making the projection
TH1 *GetTH1FromTH2(std::string fileName, std::string histName, Double_t etaMin, Double_t etaMax)
{
    TFile *f = new TFile(fileName.c_str(), "READ");
    std::cout << "FUNCTION: Reading " << histName.c_str() << " from TFile" << std::endl;
    TH2 *hist = (TH2 *)f->Get(histName.c_str());
    if (hist == nullptr)
    {
        std::cout << ">> error retrieving histogram" << std::endl;
    }
    else
    {
        std::cout << ">> histogram sucessfully read from TFile" << std::endl;
    }
    // TH2 *hMass_Pt = (TH2*)hist->Clone();
    hist->GetYaxis()->SetRangeUser(etaMin, etaMax);
    hist->GetXaxis()->SetRangeUser(0, 10);
    std::cout << "problem with projection?" << std::endl;
    if (!hist)
    {
        std::cout << "big problem" << std::endl;
    }
    TH1 *hPt_EtaCut = hist->ProjectionX("hPt_EtaCut");
    std::cout << "no problem with projection" << std::endl;
    return hPt_EtaCut;
}

// Same as above but projection in other coordinate
TH1 *GetEtaFromTH2(std::string fileName, std::string histName, Double_t ptMin, Double_t ptMax)
{
    TFile *f = new TFile(fileName.c_str(), "READ");
    std::cout << "FUNCTION: Reading " << histName.c_str() << " from TFile" << std::endl;
    TH2 *hist = (TH2 *)f->Get(histName.c_str());
    if (hist == nullptr)
    {
        std::cout << ">> error retrieving histogram" << std::endl;
    }
    else
    {
        std::cout << ">> histogram sucessfully read from TFile" << std::endl;
    }
    // TH2 *hMass_Pt = (TH2*)hist->Clone();
    hist->GetXaxis()->SetRangeUser(ptMin, ptMax);
    hist->GetYaxis()->SetRangeUser(-5, -1);
    std::cout << "problem with projection?" << std::endl;
    if (!hist)
    {
        std::cout << "big problem" << std::endl;
    }
    TH1 *hEta_PtCut = hist->ProjectionY("hEta_PtCut");
    std::cout << "no problem with projection" << std::endl;
    return hEta_PtCut;
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

void drawMuMuMuTH1(std::string fAnalysisResults, std::string histName) {
    TCanvas *c = new TCanvas(Form("c_%s_%s", fAnalysisResults.c_str(), histName.c_str()), Form("c_%s_%s", fAnalysisResults.c_str(), histName.c_str()), 800, 600);
    TH1 *hMuMuMu = GetTH1(fAnalysisResults, histName);
    c->cd();
    hMuMuMu->Draw("HIST E");
}






int plotMuMuMu() {

    std::string fAnalysisResults = "AnalysisResults_LHC24an_pass1_skimmed_Bc_pp_Hyperloop_02_08_2025.root";
    std::string hMassName = "Mass";
    drawMuMuMuTH1(fAnalysisResults, hMassName);

    return 0;
}