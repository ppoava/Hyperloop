// JPsiQACheck.C

#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TH2F.h"
#include "THashList.h"
#include "TList.h"

TH1D* getTree(const char* fileName, Double_t massMin) {

    // **********************************************
    // G  E  T    D  A  T  A
    // **********************************************

    THashList *listDiMuon;
    TList *subListDiMuon;
    TH2F *hMass_Pt;

    // Open the ROOT file
    TFile* fDiMuon = new TFile(fileName, "READ");
    if (!fDiMuon || fDiMuon->IsZombie()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return nullptr;
    }

    // Retrieve the histogram
    listDiMuon = (THashList*)fDiMuon->Get("analysis-same-event-pairing/output");
    if (!listDiMuon) {
        std::cerr << "Error retrieving list from file." << std::endl;
        return nullptr;
    }

    subListDiMuon = (TList*)listDiMuon->FindObject("PairsMuonSEPM_muonQualityCuts");
    if (!subListDiMuon) {
        std::cerr << "Error retrieving sublist from list." << std::endl;
        return nullptr;
    }

    hMass_Pt = (TH2F*)subListDiMuon->FindObject("Mass_Pt");
    if (!hMass_Pt) {
        std::cerr << "Error retrieving histogram from sublist." << std::endl;
        return nullptr;
    }

    // **********************************************
    // S  E  T    M  A  S  S    C  U  T
    // **********************************************

    TH1D *hDiMuonPt_MassCut;

    // Clone the histogram to avoid modifying the original
    hMass_Pt = (TH2F*)hMass_Pt->Clone();

    // Set the X-axis range to only include masses greater than massMin
    hMass_Pt->GetXaxis()->SetRangeUser(massMin, hMass_Pt->GetXaxis()->GetXmax());

    // Project the histogram onto the Y-axis (pT)
    hDiMuonPt_MassCut = hMass_Pt->ProjectionY("hDiMuonPt_MassCut");

    return hDiMuonPt_MassCut;
}

void JPsiQACheck() {
    // Get the projection with a mass cut of > 2
    TH1D* projection = getTree("complete_AnalysisResults_TableReader_DiMuon_LHC24aq_small.root", 2);

    if (projection) {
        // Draw the projection
        TCanvas* canvas = new TCanvas();
        projection->Draw();
        canvas->Update();
    } else {
        std::cerr << "Failed to retrieve projection." << std::endl;
    }
}