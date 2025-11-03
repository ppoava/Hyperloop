// muonQA_pT_eta_single_muons
// Created: 01-07-2025
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

TH1 *GetTH1(std::string fileName, std::string histName)
{
    TFile *f = new TFile(fileName.c_str(), "READ");
    std::cout << "FUNCTION: Reading " << histName.c_str() << " from TFile" << std::endl;
    TH1 *hist = (TH1 *)f->Get(histName.c_str());
    if (hist == nullptr)
    {
        std::cout << ">> error retrieving histogram" << std::endl;
    }
    else
    {
        std::cout << ">> histogram sucessfully read from TFile" << std::endl;
    }
    return hist;
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
    // hist->GetYaxis()->SetRangeUser(-5, -1);
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

void makePlots(std::string labelName, const char *drawHalfOpt, const char *drawOpt, std::string fAnalysisResultsRef, std::string fMuonIdRef, std::string fAnalysisResultsNew, std::string fMuonIdNew)
{
    std::vector<std::string> vTreeNamesEtaPos;
    std::vector<std::string> vTreeNamesEtaNeg;
    std::vector<std::string> vTreeNamesPtPos;
    std::vector<std::string> vTreeNamesPtNeg;
    // std::vector<const char*> vMuonIds; // trains are combined in Hyperloop with different configurations in the same output file
    std::vector<std::string> vHistNamesEtaPos;
    std::vector<std::string> vHistNamesEtaNeg;
    std::vector<std::string> vHistNamesPtPos;
    std::vector<std::string> vHistNamesPtNeg;
    std::vector<std::string> vLegendEntries;
    // std::string labelName; // saves PDF with widhts and peaks using this label name
    // std::string fAnalysisResults;
    // std::string fMuonId; // trains are combined in Hyperloop with different configurations in the same output file

    // fAnalysisResults = "AnalysisResults-muonQA_LHC24an_pass1_skimmed_small_muonID-30697_no_realignment_muonID-30255_javier_new2_muon_ID-30698_globalShiftY_Hyperloop_17_06_2025.root";
    // fAnalysisResults = "AnalysisResults_LHC24am_pass1_skimmed_muon-QA_Hyperloop_02_07_2025.root";
    // Description of muon IDs:
    // --- muon-qa_id30619: GeometryAlignedFix10Fix15ShiftCh1BNew2 (bottom only shift)
    // --- muon-qa_id31157: GeometryAlignedFix10Fix15Shift04Ch1BNew2: bottom only CH1 shift with half the amount
    // --- muon-qa_id30620: GeometryAlignedFix10Fix15ShiftCh1TBNew2 (top and bottom half shift)
    // --- muon-qa_id30697: GeometryAligned (reference)
    // --- muon-qa_id30991: GeometryAlignedShiftY (global shift Y)
    // --- muon-qa_id30992: GeometryAlignedFix110Fix15New2T5 (better javier new2)
    // --- muon-qa_id30993: GeometryAlignedFix110Fix15New2T5ShiftY (better javier new2 + shift)

    // vTreeNames.push_back("AnalysisResults_LHC24am_pass1_skimmed_no_realignment_27_04_2025_Hyperloop.root");
    // "AnalysisResults-muonQA_LHC24an_pass1_skimmed_small_muonID-30697_no_realignment_muonID-30255_javier_new2_muon_ID-30698_globalShiftY_Hyperloop_17_06_2025.root"

    // ======================[ Fill vectors with bash given information ]=====================
    // Top-Bottom

    if (strcmp(drawHalfOpt, "top-bottom") == 0 || strcmp(drawHalfOpt, "left-right") == 0)
    {
        if (strcmp(drawHalfOpt, "top-bottom") == 0)
        {
            vTreeNamesEtaPos.push_back(fAnalysisResultsRef);
            vHistNamesEtaPos.push_back(fMuonIdRef + "TrackEtaPos");
            vTreeNamesEtaPos.push_back(fAnalysisResultsRef);
            vHistNamesEtaPos.push_back(fMuonIdRef + "TrackEtaPos_T");
            vTreeNamesEtaPos.push_back(fAnalysisResultsRef);
            vHistNamesEtaPos.push_back(fMuonIdRef + "TrackEtaPos_B");
            //
            vTreeNamesEtaNeg.push_back(fAnalysisResultsRef);
            vHistNamesEtaNeg.push_back(fMuonIdRef + "TrackEtaNeg");
            vTreeNamesEtaNeg.push_back(fAnalysisResultsRef);
            vHistNamesEtaNeg.push_back(fMuonIdRef + "TrackEtaNeg_T");
            vTreeNamesEtaNeg.push_back(fAnalysisResultsRef);
            vHistNamesEtaNeg.push_back(fMuonIdRef + "TrackEtaNeg_B");
            //
            vTreeNamesPtPos.push_back(fAnalysisResultsRef);
            vHistNamesPtPos.push_back(fMuonIdRef + "TrackPt_TrackEtaPos");
            vTreeNamesPtPos.push_back(fAnalysisResultsRef);
            vHistNamesPtPos.push_back(fMuonIdRef + "TrackPt_TrackEtaPos_T");
            vTreeNamesPtPos.push_back(fAnalysisResultsRef);
            vHistNamesPtPos.push_back(fMuonIdRef + "TrackPt_TrackEtaPos_B");
            //
            vTreeNamesPtNeg.push_back(fAnalysisResultsRef);
            vHistNamesPtNeg.push_back(fMuonIdRef + "TrackPt_TrackEtaNeg");
            vTreeNamesPtNeg.push_back(fAnalysisResultsRef);
            vHistNamesPtNeg.push_back(fMuonIdRef + "TrackPt_TrackEtaNeg_T");
            vTreeNamesPtNeg.push_back(fAnalysisResultsRef);
            vHistNamesPtNeg.push_back(fMuonIdRef + "TrackPt_TrackEtaNeg_B");

            // ----- new geometry -----
            vLegendEntries = {"reference int", "reference T", "reference B", "new geometry int", "new geometry T", "new geometry B"};
            vTreeNamesEtaPos.push_back(fAnalysisResultsNew);
            vHistNamesEtaPos.push_back(fMuonIdNew + "TrackEtaPos");
            vTreeNamesEtaPos.push_back(fAnalysisResultsNew);
            vHistNamesEtaPos.push_back(fMuonIdNew + "TrackEtaPos_T");
            vTreeNamesEtaPos.push_back(fAnalysisResultsNew);
            vHistNamesEtaPos.push_back(fMuonIdNew + "TrackEtaPos_B");
            //
            vTreeNamesEtaNeg.push_back(fAnalysisResultsNew);
            vHistNamesEtaNeg.push_back(fMuonIdNew + "TrackEtaNeg");
            vTreeNamesEtaNeg.push_back(fAnalysisResultsNew);
            vHistNamesEtaNeg.push_back(fMuonIdNew + "TrackEtaNeg_T");
            vTreeNamesEtaNeg.push_back(fAnalysisResultsNew);
            vHistNamesEtaNeg.push_back(fMuonIdNew + "TrackEtaNeg_B");
            //
            vTreeNamesPtPos.push_back(fAnalysisResultsNew);
            vHistNamesPtPos.push_back(fMuonIdNew + "TrackPt_TrackEtaPos");
            vTreeNamesPtPos.push_back(fAnalysisResultsNew);
            vHistNamesPtPos.push_back(fMuonIdNew + "TrackPt_TrackEtaPos_T");
            vTreeNamesPtPos.push_back(fAnalysisResultsNew);
            vHistNamesPtPos.push_back(fMuonIdNew + "TrackPt_TrackEtaPos_B");
            //
            vTreeNamesPtNeg.push_back(fAnalysisResultsNew);
            vHistNamesPtNeg.push_back(fMuonIdNew + "TrackPt_TrackEtaNeg");
            vTreeNamesPtNeg.push_back(fAnalysisResultsNew);
            vHistNamesPtNeg.push_back(fMuonIdNew + "TrackPt_TrackEtaNeg_T");
            vTreeNamesPtNeg.push_back(fAnalysisResultsNew);
            vHistNamesPtNeg.push_back(fMuonIdNew + "TrackPt_TrackEtaNeg_B");
        }

        // Left-Right

        if (strcmp(drawHalfOpt, "left-right") == 0)
        {
            vTreeNamesEtaPos.push_back(fAnalysisResultsRef);
            vHistNamesEtaPos.push_back(fMuonIdRef + "TrackEtaPos");
            vTreeNamesEtaPos.push_back(fAnalysisResultsRef);
            vHistNamesEtaPos.push_back(fMuonIdRef + "TrackEtaPos_L");
            vTreeNamesEtaPos.push_back(fAnalysisResultsRef);
            vHistNamesEtaPos.push_back(fMuonIdRef + "TrackEtaPos_R");
            //
            vTreeNamesEtaNeg.push_back(fAnalysisResultsRef);
            vHistNamesEtaNeg.push_back(fMuonIdRef + "TrackEtaNeg");
            vTreeNamesEtaNeg.push_back(fAnalysisResultsRef);
            vHistNamesEtaNeg.push_back(fMuonIdRef + "TrackEtaNeg_L");
            vTreeNamesEtaNeg.push_back(fAnalysisResultsRef);
            vHistNamesEtaNeg.push_back(fMuonIdRef + "TrackEtaNeg_R");
            //
            vTreeNamesPtPos.push_back(fAnalysisResultsRef);
            vHistNamesPtPos.push_back(fMuonIdRef + "TrackPt_TrackEtaPos");
            vTreeNamesPtPos.push_back(fAnalysisResultsRef);
            vHistNamesPtPos.push_back(fMuonIdRef + "TrackPt_TrackEtaPos_L");
            vTreeNamesPtPos.push_back(fAnalysisResultsRef);
            vHistNamesPtPos.push_back(fMuonIdRef + "TrackPt_TrackEtaPos_R");
            //
            vTreeNamesPtNeg.push_back(fAnalysisResultsRef);
            vHistNamesPtNeg.push_back(fMuonIdRef + "TrackPt_TrackEtaNeg");
            vTreeNamesPtNeg.push_back(fAnalysisResultsRef);
            vHistNamesPtNeg.push_back(fMuonIdRef + "TrackPt_TrackEtaNeg_L");
            vTreeNamesPtNeg.push_back(fAnalysisResultsRef);
            vHistNamesPtNeg.push_back(fMuonIdRef + "TrackPt_TrackEtaNeg_R");

            // ----- new geometry -----
            vLegendEntries = {"reference int", "reference L", "reference R", "new geometry int", "new geometry L", "new geometry R"};
            vTreeNamesEtaPos.push_back(fAnalysisResultsNew);
            vHistNamesEtaPos.push_back(fMuonIdNew + "TrackEtaPos");
            vTreeNamesEtaPos.push_back(fAnalysisResultsNew);
            vHistNamesEtaPos.push_back(fMuonIdNew + "TrackEtaPos_L");
            vTreeNamesEtaPos.push_back(fAnalysisResultsNew);
            vHistNamesEtaPos.push_back(fMuonIdNew + "TrackEtaPos_R");
            //
            vTreeNamesEtaNeg.push_back(fAnalysisResultsNew);
            vHistNamesEtaNeg.push_back(fMuonIdNew + "TrackEtaNeg");
            vTreeNamesEtaNeg.push_back(fAnalysisResultsNew);
            vHistNamesEtaNeg.push_back(fMuonIdNew + "TrackEtaNeg_L");
            vTreeNamesEtaNeg.push_back(fAnalysisResultsNew);
            vHistNamesEtaNeg.push_back(fMuonIdNew + "TrackEtaNeg_R");
            //
            vTreeNamesPtPos.push_back(fAnalysisResultsNew);
            vHistNamesPtPos.push_back(fMuonIdNew + "TrackPt_TrackEtaPos");
            vTreeNamesPtPos.push_back(fAnalysisResultsNew);
            vHistNamesPtPos.push_back(fMuonIdNew + "TrackPt_TrackEtaPos_L");
            vTreeNamesPtPos.push_back(fAnalysisResultsNew);
            vHistNamesPtPos.push_back(fMuonIdNew + "TrackPt_TrackEtaPos_R");
            //
            vTreeNamesPtNeg.push_back(fAnalysisResultsNew);
            vHistNamesPtNeg.push_back(fMuonIdNew + "TrackPt_TrackEtaNeg");
            vTreeNamesPtNeg.push_back(fAnalysisResultsNew);
            vHistNamesPtNeg.push_back(fMuonIdNew + "TrackPt_TrackEtaNeg_L");
            vTreeNamesPtNeg.push_back(fAnalysisResultsNew);
            vHistNamesPtNeg.push_back(fMuonIdNew + "TrackPt_TrackEtaNeg_R");
        }

        TFile *fOutput = new TFile(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.root", fAnalysisResultsNew.c_str(), labelName.c_str()), "RECREATE");

        std::vector<Int_t> vLineStyles = {1, 2, 6, 1, 1, 1}; // in order to show reference everywhere..
        std::vector<Int_t> vLineColours = {1, 1, 1, 2, 8, 7};

        // make canvases
        TCanvas *cEtaPos = new TCanvas("cEtaPos", "cEtaPos", 800, 600);
        TCanvas *cEtaNeg = new TCanvas("cEtaNeg", "cEtaNeg", 800, 600);
        TCanvas *cEtaRatio = new TCanvas("cEtaRatio", "cEtaRatio", 800, 600);
        TCanvas *cEtaRatioPtDiff = new TCanvas("cEtaRatioPtDiff", "cEtaRatioPtDiff", 800, 600);
        TCanvas *cEtaSpecRatio = new TCanvas("cEtaMuPTop/MuMBottom", "cEtaMuPTop/MuMBottom", 800, 600);                      // mu+Top/mu-Bottom
        TCanvas *cEtaSpecRatioPtDiff = new TCanvas("cEtaMuPTop/MuMBottomPtDiff", "cEtaMuPTop/MuMBottomPtDiff", 800, 600);    // mu+Top/mu-Bottom
        TCanvas *cEtaSpecRatioInv = new TCanvas("cEtaMuMTop/MuPBottom", "cEtaMuMTop/MuPBottom", 800, 600);                   // mu-Top/mu+Bottom
        TCanvas *cEtaSpecRatioInvPtDiff = new TCanvas("cEtaMuMTop/MuPBottomPtDiff", "cEtaMuMTop/MuPBottomPtDiff", 800, 600); // mu-Top/mu+Bottom
        TCanvas *cPtPos = new TCanvas("cPtPos", "cPtPos", 800, 600);
        TCanvas *cPtNeg = new TCanvas("cPtNeg", "cPtNeg", 800, 600);
        TCanvas *cPtRatio = new TCanvas("cPtRatio", "cPtRatio", 800, 600);
        TCanvas *cPtSpecRatio = new TCanvas("cPtMuPTop/MuMBottom", "cPtMuPTop/MuMBottom", 800, 600);
        TCanvas *cPtSpecRatioInv = new TCanvas("cPtMuMTop/MuPBottom", "cPtMuMTop/MuPBottom", 800, 600);

        // for drawing the templates
        double maxEtaPos = 0, maxEtaNeg = 0, maxPtPos = 0, maxPtNeg = 0;
        for (Int_t i = 0; i < vTreeNamesEtaPos.size(); i++)
        {
            TH1 *hEtaPos = GetTH1(vTreeNamesEtaPos[i], vHistNamesEtaPos[i]);
            TH1 *hEtaNeg = GetTH1(vTreeNamesEtaNeg[i], vHistNamesEtaNeg[i]);
            TH1 *hPtPos = GetTH1FromTH2(vTreeNamesPtPos[i], vHistNamesPtPos[i], -4, 4);
            TH1 *hPtNeg = GetTH1FromTH2(vTreeNamesPtNeg[i], vHistNamesPtNeg[i], -4, 4);
            maxEtaPos = std::max(maxEtaPos, hEtaPos->GetMaximum());
            maxEtaNeg = std::max(maxEtaNeg, hEtaNeg->GetMaximum());
            maxPtPos = std::max(maxPtPos, hPtPos->GetMaximum());
            maxPtNeg = std::max(maxPtNeg, hPtNeg->GetMaximum());
        }

        // make templates
        TH1F *hTemplateEtaPos = new TH1F("hTemplateEtaPos", Form("#eta for #mu^{+}: %s", labelName.c_str()), 80, -5, -1);
        TH1F *hTemplateEtaNeg = new TH1F("hTemplateEtaNeg", Form("#eta for #mu^{-}: %s", labelName.c_str()), 80, -5, -1);
        TH1F *hTemplateEtaRatio = new TH1F("hTemplateEtaRatio", Form("#eta for #mu^{+}/#mu^{-}: %s", labelName.c_str()), 80, -5, -1);
        TH1F *hTemplateEtaRatioPtDiff = new TH1F("hTemplateEtaRatioPtDiff", Form("#eta for #mu^{+}/#mu^{-}: %s", labelName.c_str()), 80, -5, -1);
        TH1F *hTemplateEtaSpecRatio = new TH1F("hTemplateEtaSpecRatio", Form("#eta for #mu^{+}Top/#mu^{-}Bottom: %s", labelName.c_str()), 80, -5, -1);
        TH1F *hTemplateEtaSpecRatioPtDiff = new TH1F("hTemplateEtaSpecRatioPtDiff", Form("#eta for #mu^{+}Top/#mu^{-}Bottom: %s", labelName.c_str()), 80, -5, -1);
        TH1F *hTemplateEtaSpecRatioInv = new TH1F("hTemplateEtaSpecRatioInv", Form("#eta for #mu^{-}Top/#mu^{+}Bottom: %s", labelName.c_str()), 80, -5, -1);
        TH1F *hTemplateEtaSpecRatioInvPtDiff = new TH1F("hTemplateEtaSpecRatioInvPtDiff", Form("#eta for #mu^{-}Top/#mu^{+}Bottom: %s", labelName.c_str()), 80, -5, -1);
        TH1F *hTemplatePtPos = new TH1F("hTemplatePtPos", Form("p_{T} for #mu^{+}: %s", labelName.c_str()), 100, 0, 10);
        TH1F *hTemplatePtNeg = new TH1F("hTemplatePtNeg", Form("p_{T} for #mu^{-}: %s", labelName.c_str()), 100, 0, 10);
        TH1F *hTemplatePtRatio = new TH1F("hTemplatePtRatio", Form("p_{T} for #mu^{+}/#mu^{-}: %s", labelName.c_str()), 100, 0, 10);
        TH1F *hTemplatePtSpecRatio = new TH1F("hTemplatePtSpecRatio", Form("p_{T} for #mu^{+}Top/#mu^{-}Bottom: %s", labelName.c_str()), 100, 0, 10);
        TH1F *hTemplatePtSpecRatioInv = new TH1F("hTemplatePtSpecRatioInv", Form("p_{T} for #mu^{-}Top/#mu^{+}Bottom: %s", labelName.c_str()), 100, 0, 10);
        // set max y-axis range
        double margin = 1.1;
        hTemplateEtaPos->SetMaximum(margin * maxEtaPos);
        hTemplateEtaNeg->SetMaximum(margin * maxEtaNeg);
        hTemplateEtaRatio->SetMinimum(1 - 1), hTemplateEtaRatio->SetMaximum(1 + 1);
        hTemplateEtaRatioPtDiff->SetMinimum(1 - 1), hTemplateEtaRatioPtDiff->SetMaximum(1 + 1);
        hTemplateEtaSpecRatio->SetMinimum(1 - 1), hTemplateEtaSpecRatio->SetMaximum(1 + 1);
        hTemplateEtaSpecRatioPtDiff->SetMinimum(1 - 1), hTemplateEtaSpecRatioPtDiff->SetMaximum(1 + 1);
        hTemplateEtaSpecRatioInv->SetMinimum(1 - 1), hTemplateEtaSpecRatioInv->SetMaximum(1 + 1);
        hTemplateEtaSpecRatioInvPtDiff->SetMinimum(1 - 1), hTemplateEtaSpecRatioInvPtDiff->SetMaximum(1 + 1);
        hTemplatePtPos->SetMaximum(margin * maxPtPos);
        hTemplatePtNeg->SetMaximum(margin * maxPtNeg);
        hTemplatePtRatio->SetMinimum(1 - 0.5), hTemplatePtRatio->SetMaximum(1 + 0.5);
        hTemplatePtSpecRatio->SetMinimum(1 - 0.5), hTemplatePtSpecRatio->SetMaximum(1 + 0.5);
        hTemplatePtSpecRatioInv->SetMinimum(1 - 0.5), hTemplatePtSpecRatioInv->SetMaximum(1 + 0.5);

        cEtaPos->cd();
        hTemplateEtaPos->SetStats(0);
        hTemplateEtaPos->Draw();
        cEtaNeg->cd();
        hTemplateEtaNeg->SetStats(0);
        hTemplateEtaNeg->Draw();
        cEtaRatio->cd();
        hTemplateEtaRatio->SetStats(0);
        hTemplateEtaRatio->Draw();
        cEtaRatioPtDiff->cd();
        hTemplateEtaRatioPtDiff->SetStats(0);
        hTemplateEtaRatioPtDiff->Draw();
        cEtaSpecRatio->cd();
        hTemplateEtaSpecRatio->SetStats(0);
        hTemplateEtaSpecRatio->Draw();
        cEtaSpecRatioPtDiff->cd();
        hTemplateEtaSpecRatioPtDiff->SetStats(0);
        hTemplateEtaSpecRatioPtDiff->Draw();
        cEtaSpecRatioInv->cd();
        hTemplateEtaSpecRatioInv->SetStats(0);
        hTemplateEtaSpecRatioInv->Draw();
        cEtaSpecRatioInvPtDiff->cd();
        hTemplateEtaSpecRatioInvPtDiff->SetStats(0);
        hTemplateEtaSpecRatioInvPtDiff->Draw();
        //
        cPtPos->cd();
        gPad->SetLogy();
        hTemplatePtPos->SetStats(0);
        hTemplatePtPos->Draw();
        cPtNeg->cd();
        gPad->SetLogy();
        hTemplatePtNeg->SetStats(0);
        hTemplatePtNeg->Draw();
        cPtRatio->cd();
        hTemplatePtRatio->SetStats(0);
        hTemplatePtRatio->Draw();
        cPtSpecRatio->cd();
        hTemplatePtSpecRatio->SetStats(0);
        hTemplatePtSpecRatio->Draw();
        cPtSpecRatioInv->cd();
        hTemplatePtSpecRatioInv->SetStats(0);
        hTemplatePtSpecRatioInv->Draw();

        // make legends
        TLegend *l = new TLegend(0.60, 0.65, 0.80, 0.85);
        l->SetBorderSize(0);
        l->SetTextSize(0.02);

        TLegend *lSpecRatio = new TLegend(0.60, 0.65, 0.80, 0.85);
        lSpecRatio->SetBorderSize(0);
        lSpecRatio->SetTextSize(0.02);

        TLegend *lRatioPtDiff = new TLegend(0.60, 0.65, 0.80, 0.85);
        lRatioPtDiff->SetBorderSize(0);
        lRatioPtDiff->SetTextSize(0.02);

        for (Int_t i = 0; i < vTreeNamesEtaPos.size(); i++)
        {
            std::string fNameEtaPos = vTreeNamesEtaPos[i];
            std::string fNameEtaNeg = vTreeNamesEtaNeg[i];
            std::string fNamePtPos = vTreeNamesPtPos[i];
            std::string fNamePtNeg = vTreeNamesPtNeg[i];
            std::string hNameEtaPos = vHistNamesEtaPos[i];
            std::string hNameEtaNeg = vHistNamesEtaNeg[i];
            std::string hNamePtPos = vHistNamesPtPos[i];
            std::string hNamePtNeg = vHistNamesPtNeg[i];
            TH1 *hEtaPos = GetTH1(fNameEtaPos, hNameEtaPos);
            hEtaPos = (TH1 *)hEtaPos->Clone(Form("hEtaPos_clone_%d", i));
            TH1 *hEtaNeg = GetTH1(fNameEtaNeg, hNameEtaNeg);
            hEtaNeg = (TH1 *)hEtaNeg->Clone(Form("hEtaNeg_clone_%d", i));
            TH1 *hEtaRatio = (TH1 *)hEtaPos->Clone(Form("hEtaRatio_%d", i));
            hEtaRatio->Divide(hEtaNeg);
            //
            if (i == 0) // Reference: hNamePt stores the 2D eta and pT histograms..
            {
                TH1 *hEtaPosLowPt = GetEtaFromTH2(fNamePtPos, hNamePtPos, 0, 3);
                hEtaPosLowPt = (TH1 *)hEtaPosLowPt->Clone(Form("hEtaPosLowPt_clone_%d", i));
                TH1 *hEtaPosHighPt = GetEtaFromTH2(fNamePtPos, hNamePtPos, 3, 50);
                hEtaPosHighPt = (TH1 *)hEtaPosHighPt->Clone(Form("hEtaPosHighPt_clone_%d", i));
                TH1 *hEtaNegLowPt = GetEtaFromTH2(fNamePtNeg, hNamePtNeg, 0, 3);
                hEtaNegLowPt = (TH1 *)hEtaNegLowPt->Clone(Form("hEtaNegLowPt_clone_%d", i));
                TH1 *hEtaNegHighPt = GetEtaFromTH2(fNamePtNeg, hNamePtNeg, 3, 50);
                hEtaNegHighPt = (TH1 *)hEtaNegHighPt->Clone(Form("hEtaNegHighPt_clone_%d", i));
                TH1 *hEtaRatioLowPt = (TH1 *)hEtaPosLowPt->Clone(Form("hEtaLowPtRatio_%d", i));
                TH1 *hEtaRatioHighPt = (TH1 *)hEtaPosHighPt->Clone(Form("hEtaHighPtRatio_%d", i));
                hEtaRatioLowPt->Divide(hEtaNegLowPt);
                hEtaRatioHighPt->Divide(hEtaNegHighPt);
                hEtaRatioLowPt->SetLineColor(kBlack);
                hEtaRatioHighPt->SetLineColor(kBlack);
                hEtaRatioLowPt->SetLineStyle(0);
                hEtaRatioHighPt->SetLineStyle(2);
                cEtaRatioPtDiff->cd();
                hEtaRatioLowPt->Draw("SAME HIST");
                hEtaRatioHighPt->Draw("SAME HIST");
                lRatioPtDiff->AddEntry(hEtaRatioLowPt, "reference, pT < 3 GeV ", "l");
                lRatioPtDiff->AddEntry(hEtaRatioHighPt, "reference, pT > 3 GeV ", "l");
            }
            if (i == 3) // New geometry: hNamePt stores the 2D eta and pT histograms..
            {
                TH1 *hEtaPosLowPt = GetEtaFromTH2(fNamePtPos, hNamePtPos, 0, 3);
                hEtaPosLowPt = (TH1 *)hEtaPosLowPt->Clone(Form("hEtaPosLowPt_clone_%d", i));
                TH1 *hEtaPosHighPt = GetEtaFromTH2(fNamePtPos, hNamePtPos, 3, 50);
                hEtaPosHighPt = (TH1 *)hEtaPosHighPt->Clone(Form("hEtaPosHighPt_clone_%d", i));
                TH1 *hEtaNegLowPt = GetEtaFromTH2(fNamePtNeg, hNamePtNeg, 0, 3);
                hEtaNegLowPt = (TH1 *)hEtaNegLowPt->Clone(Form("hEtaNegLowPt_clone_%d", i));
                TH1 *hEtaNegHighPt = GetEtaFromTH2(fNamePtNeg, hNamePtNeg, 3, 50);
                hEtaNegHighPt = (TH1 *)hEtaNegHighPt->Clone(Form("hEtaNegHighPt_clone_%d", i));
                TH1 *hEtaRatioLowPt = (TH1 *)hEtaPosLowPt->Clone(Form("hEtaLowPtRatio_%d", i));
                TH1 *hEtaRatioHighPt = (TH1 *)hEtaPosHighPt->Clone(Form("hEtaHighPtRatio_%d", i));
                hEtaRatioLowPt->Divide(hEtaNegLowPt);
                hEtaRatioHighPt->Divide(hEtaNegHighPt);
                hEtaRatioLowPt->SetLineColor(kRed);
                hEtaRatioHighPt->SetLineColor(kRed);
                hEtaRatioLowPt->SetLineStyle(0);
                hEtaRatioHighPt->SetLineStyle(2);
                cEtaRatioPtDiff->cd();
                hEtaRatioLowPt->Draw("SAME HIST");
                hEtaRatioHighPt->Draw("SAME HIST");
                lRatioPtDiff->AddEntry(hEtaRatioLowPt, "new geometry, pT < 3 GeV ", "l");
                lRatioPtDiff->AddEntry(hEtaRatioHighPt, "new geometry, pT > 3 GeV ", "l");
            }
            //
            TH1 *hPtPos = GetTH1FromTH2(fNamePtPos, hNamePtPos, -4, 4);
            hPtPos = (TH1 *)hPtPos->Clone(Form("hPtPos_clone_%d", i));
            TH1 *hPtNeg = GetTH1FromTH2(fNamePtNeg, hNamePtNeg, -4, 4);
            hPtNeg = (TH1 *)hPtNeg->Clone(Form("hPtNeg_clone_%d", i));
            TH1 *hPtRatio = (TH1 *)hPtPos->Clone(Form("hPtRatio_%d", i));
            hPtRatio->Divide(hPtNeg);
            hEtaPos->SetLineStyle(vLineStyles[i]);
            hEtaNeg->SetLineStyle(vLineStyles[i]);
            if (vLineColours[i] == 1)
            {
                hEtaRatio->SetLineStyle(vLineStyles[i]);
            }
            hPtPos->SetLineStyle(vLineStyles[i]);
            hPtNeg->SetLineStyle(vLineStyles[i]);
            if (vLineColours[i] == 1)
            {
                hPtRatio->SetLineStyle(vLineStyles[i]);
            }
            hEtaPos->SetLineColor(vLineColours[i]);
            hEtaNeg->SetLineColor(vLineColours[i]);
            hEtaRatio->SetLineColor(vLineColours[i]);
            hPtPos->SetLineColor(vLineColours[i]);
            hPtNeg->SetLineColor(vLineColours[i]);
            hPtRatio->SetLineColor(vLineColours[i]);
            cEtaPos->cd();
            hEtaPos->Draw("SAME HIST E");
            cEtaNeg->cd();
            hEtaNeg->Draw("SAME HIST E");
            cEtaRatio->cd();
            hEtaRatio->Draw("SAME HIST");
            cPtPos->cd();
            hPtPos->Draw("SAME HIST E");
            cPtNeg->cd();
            hPtNeg->Draw("SAME HIST E");
            cPtRatio->cd();
            hPtRatio->Draw("SAME HIST");
            l->AddEntry(hEtaPos, vLegendEntries[i].c_str(), "l");
        }

        cEtaPos->cd();
        l->Draw();
        cEtaNeg->cd();
        l->Draw();
        cEtaRatio->cd();
        l->Draw();
        cEtaRatioPtDiff->cd();
        lRatioPtDiff->Draw();
        cEtaSpecRatio->cd();
        lSpecRatio->Draw();
        cEtaSpecRatioPtDiff->cd();
        lRatioPtDiff->Draw();
        cEtaSpecRatioInv->cd();
        lSpecRatio->Draw();
        cEtaSpecRatioInvPtDiff->cd();
        lRatioPtDiff->Draw();
        //
        cPtPos->cd();
        l->Draw();
        cPtNeg->cd();
        l->Draw();
        cPtRatio->cd();
        l->Draw();
        cPtSpecRatio->cd();
        lSpecRatio->Draw();
        cPtSpecRatioInv->cd();
        lSpecRatio->Draw();

        // hard-coded mu+Top/mu-Bottom
        TH1 *hEtaPosTopRef = GetTH1(vTreeNamesEtaPos[0], vHistNamesEtaPos[1]);
        hEtaPosTopRef = (TH1 *)hEtaPosTopRef->Clone("hEtaPosTopRef_clone");
        TH1 *hEtaNegBottomRef = GetTH1(vTreeNamesEtaNeg[0], vHistNamesEtaNeg[2]);
        hEtaNegBottomRef = (TH1 *)hEtaNegBottomRef->Clone("hEtaNegBottomRef_clone");
        TH1 *hEtaSpecRatioRef = (TH1 *)hEtaPosTopRef->Clone("hEtaSpecRatioRef_clone");

        TH1 *hEtaPosTop = GetTH1(vTreeNamesEtaPos[3], vHistNamesEtaPos[4]);
        hEtaPosTop = (TH1 *)hEtaPosTop->Clone("hEtaPosTop_clone");
        TH1 *hEtaNegBottom = GetTH1(vTreeNamesEtaNeg[3], vHistNamesEtaNeg[5]);
        hEtaNegBottom = (TH1 *)hEtaNegBottom->Clone("hEtaNegBottom_clone");
        TH1 *hEtaSpecRatio = (TH1 *)hEtaPosTop->Clone("hEtaSpecRatio_clone");

        hEtaSpecRatioRef->Divide(hEtaNegBottomRef);
        hEtaSpecRatio->Divide(hEtaNegBottom);

        // hard-coded mu+Top/mu-Bottom
        // pT differential
        TH1 *hEtaPosTopRefLowPt = GetEtaFromTH2(vTreeNamesPtPos[0], vHistNamesPtPos[1], 0, 3);
        hEtaPosTopRefLowPt = (TH1 *)hEtaPosTopRefLowPt->Clone("hEtaPosTopRefLowPt_clone");
        TH1 *hEtaNegBottomRefLowPt = GetEtaFromTH2(vTreeNamesPtNeg[0], vHistNamesPtNeg[2], 0, 3);
        hEtaNegBottomRefLowPt = (TH1 *)hEtaNegBottomRefLowPt->Clone("hEtaNegBottomRefLowPt_clone");
        TH1 *hEtaSpecRatioRefLowPt = (TH1 *)hEtaPosTopRefLowPt->Clone("hEtaSpecRatioRefLowPt_clone");

        TH1 *hEtaPosTopRefHighPt = GetEtaFromTH2(vTreeNamesPtPos[0], vHistNamesPtPos[1], 3, 50);
        hEtaPosTopRefHighPt = (TH1 *)hEtaPosTopRefHighPt->Clone("hEtaPosTopRefHighPt_clone");
        TH1 *hEtaNegBottomRefHighPt = GetEtaFromTH2(vTreeNamesPtNeg[0], vHistNamesPtNeg[2], 3, 50);
        hEtaNegBottomRefHighPt = (TH1 *)hEtaNegBottomRefHighPt->Clone("hEtaNegBottomRefHighPt_clone");
        TH1 *hEtaSpecRatioRefHighPt = (TH1 *)hEtaPosTopRefHighPt->Clone("hEtaSpecRatioRefHighPt_clone");

        TH1 *hEtaPosTopLowPt = GetEtaFromTH2(vTreeNamesPtPos[3], vHistNamesPtPos[4], 0, 3);
        hEtaPosTopLowPt = (TH1 *)hEtaPosTopLowPt->Clone("hEtaPosTopLowPt_clone");
        TH1 *hEtaNegBottomLowPt = GetEtaFromTH2(vTreeNamesPtNeg[3], vHistNamesPtNeg[5], 0, 3);
        hEtaNegBottomLowPt = (TH1 *)hEtaNegBottomLowPt->Clone("hEtaNegBottomLowPt_clone");
        TH1 *hEtaSpecRatioLowPt = (TH1 *)hEtaPosTopLowPt->Clone("hEtaSpecRatioLowPt_clone");

        TH1 *hEtaPosTopHighPt = GetEtaFromTH2(vTreeNamesPtPos[3], vHistNamesPtPos[4], 3, 50);
        hEtaPosTopHighPt = (TH1 *)hEtaPosTopHighPt->Clone("hEtaPosTopHighPt_clone");
        TH1 *hEtaNegBottomHighPt = GetEtaFromTH2(vTreeNamesPtNeg[3], vHistNamesPtNeg[5], 3, 50);
        hEtaNegBottomHighPt = (TH1 *)hEtaNegBottomHighPt->Clone("hEtaNegBottomHighPt_clone");
        TH1 *hEtaSpecRatioHighPt = (TH1 *)hEtaPosTopHighPt->Clone("hEtaSpecRatioHighPt_clone");

        hEtaSpecRatioRefLowPt->Divide(hEtaNegBottomRefLowPt);
        hEtaSpecRatioLowPt->Divide(hEtaNegBottomLowPt);
        hEtaSpecRatioRefHighPt->Divide(hEtaNegBottomRefHighPt);
        hEtaSpecRatioHighPt->Divide(hEtaNegBottomHighPt);
        //
        hEtaSpecRatioRefLowPt->SetLineColor(kBlack);
        hEtaSpecRatioRefHighPt->SetLineColor(kBlack);
        hEtaSpecRatioLowPt->SetLineColor(kRed);
        hEtaSpecRatioHighPt->SetLineColor(kRed);
        hEtaSpecRatioRefLowPt->SetLineStyle(0);
        hEtaSpecRatioRefHighPt->SetLineStyle(2);
        hEtaSpecRatioLowPt->SetLineStyle(0);
        hEtaSpecRatioHighPt->SetLineStyle(2);

        // hard-coded mu-Top/mu+Bottom (inv)
        TH1 *hEtaNegTopInvRef = GetTH1(vTreeNamesEtaNeg[0], vHistNamesEtaNeg[1]);
        hEtaNegTopInvRef = (TH1 *)hEtaNegTopInvRef->Clone("hEtaNegTopInvRef_clone");
        TH1 *hEtaPosBottomInvRef = GetTH1(vTreeNamesEtaPos[0], vHistNamesEtaPos[2]);
        hEtaPosBottomInvRef = (TH1 *)hEtaPosBottomInvRef->Clone("hEtaPosBottomInvRef_clone");
        TH1 *hEtaSpecRatioInvRef = (TH1 *)hEtaNegTopInvRef->Clone("hEtaSpecRatioInvRef_clone");

        TH1 *hEtaNegTopInv = GetTH1(vTreeNamesEtaNeg[3], vHistNamesEtaNeg[4]);
        hEtaNegTopInv = (TH1 *)hEtaNegTopInv->Clone("hEtaNegTopInv_clone");
        TH1 *hEtaPosBottomInv = GetTH1(vTreeNamesEtaPos[3], vHistNamesEtaPos[5]);
        hEtaPosBottomInv = (TH1 *)hEtaPosBottomInv->Clone("hEtaPosBottomInv_clone");
        TH1 *hEtaSpecRatioInv = (TH1 *)hEtaNegTopInv->Clone("hEtaSpecRatioInv_clone");

        hEtaSpecRatioInvRef->Divide(hEtaPosBottomInvRef);
        hEtaSpecRatioInv->Divide(hEtaPosBottomInv);

        // hard-coded mu-Top/mu+Bottom (inv)
        // pT differential
        TH1 *hEtaNegTopInvRefLowPt = GetEtaFromTH2(vTreeNamesPtNeg[0], vHistNamesPtNeg[1], 0, 3);
        hEtaNegTopInvRefLowPt = (TH1 *)hEtaNegTopInvRefLowPt->Clone("hEtaNegTopInvRefLowPt_clone");
        TH1 *hEtaPosBottomInvRefLowPt = GetEtaFromTH2(vTreeNamesPtPos[0], vHistNamesPtPos[2], 0, 3);
        hEtaPosBottomInvRefLowPt = (TH1 *)hEtaPosBottomInvRefLowPt->Clone("hEtaPosBottomInvRefLowPt_clone");
        TH1 *hEtaSpecRatioInvRefLowPt = (TH1 *)hEtaNegTopInvRefLowPt->Clone("hEtaSpecRatioInvRefLowPt_clone");

        TH1 *hEtaNegTopInvRefHighPt = GetEtaFromTH2(vTreeNamesPtNeg[0], vHistNamesPtNeg[1], 3, 50);
        hEtaNegTopInvRefHighPt = (TH1 *)hEtaNegTopInvRefHighPt->Clone("hEtaNegTopInvRefHighPt_clone");
        TH1 *hEtaPosBottomInvRefHighPt = GetEtaFromTH2(vTreeNamesPtPos[0], vHistNamesPtPos[2], 3, 50);
        hEtaPosBottomInvRefHighPt = (TH1 *)hEtaPosBottomInvRefHighPt->Clone("hEtaPosBottomInvRefHighPt_clone");
        TH1 *hEtaSpecRatioInvRefHighPt = (TH1 *)hEtaNegTopInvRefHighPt->Clone("hEtaSpecRatioInvRefHighPt_clone");

        TH1 *hEtaNegTopInvLowPt = GetEtaFromTH2(vTreeNamesPtNeg[3], vHistNamesPtNeg[4], 0, 3);
        hEtaNegTopInvLowPt = (TH1 *)hEtaNegTopInvLowPt->Clone("hEtaNegTopInvLowPt_clone");
        TH1 *hEtaPosBottomInvLowPt = GetEtaFromTH2(vTreeNamesPtPos[3], vHistNamesPtPos[5], 0, 3);
        hEtaPosBottomInvLowPt = (TH1 *)hEtaPosBottomInvLowPt->Clone("hEtaPosBottomInvLowPt_clone");
        TH1 *hEtaSpecRatioInvLowPt = (TH1 *)hEtaNegTopInvLowPt->Clone("hEtaSpecRatioInvLowPt_clone");

        TH1 *hEtaNegTopInvHighPt = GetEtaFromTH2(vTreeNamesPtNeg[3], vHistNamesPtNeg[4], 3, 50);
        hEtaNegTopInvHighPt = (TH1 *)hEtaNegTopInvHighPt->Clone("hEtaNegTopInvHighPt_clone");
        TH1 *hEtaPosBottomInvHighPt = GetEtaFromTH2(vTreeNamesPtPos[3], vHistNamesPtPos[5], 3, 50);
        hEtaPosBottomInvHighPt = (TH1 *)hEtaPosBottomInvHighPt->Clone("hEtaPosBottomInvHighPt_clone");
        TH1 *hEtaSpecRatioInvHighPt = (TH1 *)hEtaNegTopInvHighPt->Clone("hEtaSpecRatioInvHighPt_clone");

        hEtaSpecRatioInvRefLowPt->Divide(hEtaPosBottomInvRefLowPt);
        hEtaSpecRatioInvLowPt->Divide(hEtaPosBottomInvLowPt);
        hEtaSpecRatioInvRefHighPt->Divide(hEtaPosBottomInvRefHighPt);
        hEtaSpecRatioInvHighPt->Divide(hEtaPosBottomInvHighPt);
        //
        hEtaSpecRatioInvRefLowPt->SetLineColor(kBlack);
        hEtaSpecRatioInvRefHighPt->SetLineColor(kBlack);
        hEtaSpecRatioInvLowPt->SetLineColor(kRed);
        hEtaSpecRatioInvHighPt->SetLineColor(kRed);
        hEtaSpecRatioInvRefLowPt->SetLineStyle(0);
        hEtaSpecRatioInvRefHighPt->SetLineStyle(2);
        hEtaSpecRatioInvLowPt->SetLineStyle(0);
        hEtaSpecRatioInvHighPt->SetLineStyle(2);

        // hard-coded mu+Top/mu-Bottom
        TH1 *hPtPosTopRef = GetTH1FromTH2(vTreeNamesPtPos[0], vHistNamesPtPos[1], -4, 4);
        std::cout << "opening " << vTreeNamesPtPos[0] << " and " << vHistNamesPtPos[1] << std::endl;
        hPtPosTopRef = (TH1 *)hPtPosTopRef->Clone("hPtPosTopRef_clone");
        TH1 *hPtNegBottomRef = GetTH1FromTH2(vTreeNamesPtNeg[0], vHistNamesPtNeg[2], -4, 4);
        std::cout << "opening " << vTreeNamesPtNeg[0] << " and " << vHistNamesPtNeg[2] << std::endl;
        hPtNegBottomRef = (TH1 *)hPtNegBottomRef->Clone("hPtNegBottomRef_clone");
        TH1 *hPtSpecRatioRef = (TH1 *)hPtPosTopRef->Clone("hPtSpecRatioRef_clone");

        TH1 *hPtPosTop = GetTH1FromTH2(vTreeNamesPtPos[3], vHistNamesPtPos[4], -4, 4);
        std::cout << "opening " << vTreeNamesPtPos[3] << " and " << vHistNamesPtPos[4] << std::endl;
        hPtPosTop = (TH1 *)hPtPosTop->Clone("hPtPosTop_clone");
        TH1 *hPtNegBottom = GetTH1FromTH2(vTreeNamesPtNeg[3], vHistNamesPtNeg[5], -4, 4);
        std::cout << "opening " << vTreeNamesPtNeg[3] << " and " << vHistNamesPtNeg[5] << std::endl;
        hPtNegBottom = (TH1 *)hPtNegBottom->Clone("hPtNegBottom_clone");
        TH1 *hPtSpecRatio = (TH1 *)hPtPosTop->Clone("hPtSpecRatio_clone");

        hPtSpecRatio->Divide(hPtNegBottom);
        hPtSpecRatioRef->Divide(hPtNegBottomRef);

        // hard-coded mu-Top/mu+Bottom (inv)
        TH1 *hPtNegTopInvRef = GetTH1FromTH2(vTreeNamesPtNeg[0], vHistNamesPtNeg[1], -4, 4);
        std::cout << "opening " << vTreeNamesPtNeg[0] << " and " << vHistNamesPtNeg[1] << std::endl;
        hPtNegTopInvRef = (TH1 *)hPtNegTopInvRef->Clone("hPtNegTopInvRef_clone");
        TH1 *hPtPosBottomInvRef = GetTH1FromTH2(vTreeNamesPtPos[0], vHistNamesPtPos[2], -4, 4);
        std::cout << "opening " << vTreeNamesPtPos[0] << " and " << vHistNamesPtPos[2] << std::endl;
        hPtPosBottomInvRef = (TH1 *)hPtPosBottomInvRef->Clone("hPtPosBottomInvRef_clone");
        TH1 *hPtSpecRatioInvRef = (TH1 *)hPtNegTopInvRef->Clone("hPtSpecRatioInvRef_clone");

        TH1 *hPtNegTopInv = GetTH1FromTH2(vTreeNamesPtNeg[3], vHistNamesPtNeg[4], -4, 4);
        std::cout << "opening " << vTreeNamesPtNeg[3] << " and " << vHistNamesPtNeg[4] << std::endl;
        hPtNegTopInv = (TH1 *)hPtNegTopInv->Clone("hPtNegTopInv_clone");
        TH1 *hPtPosBottomInv = GetTH1FromTH2(vTreeNamesPtPos[3], vHistNamesPtPos[5], -4, 4);
        std::cout << "opening " << vTreeNamesPtPos[3] << " and " << vHistNamesPtPos[5] << std::endl;
        hPtPosBottomInv = (TH1 *)hPtPosBottomInv->Clone("hPtPosBottomInv_clone");
        TH1 *hPtSpecRatioInv = (TH1 *)hPtNegTopInv->Clone("hPtSpecRatioInv_clone");

        hPtSpecRatioInv->Divide(hPtPosBottomInv);
        hPtSpecRatioInvRef->Divide(hPtPosBottomInvRef);

        cEtaSpecRatio->cd();
        hEtaSpecRatio->SetLineColor(kRed);
        hEtaSpecRatioRef->SetLineColor(kBlack);
        hEtaSpecRatioRef->Draw("SAME HIST");
        hEtaSpecRatio->Draw("SAME HIST");
        cEtaSpecRatioPtDiff->cd();
        hEtaSpecRatioRefLowPt->Draw("SAME HIST");
        hEtaSpecRatioLowPt->Draw("SAME HIST");
        hEtaSpecRatioRefHighPt->Draw("SAME HIST");
        hEtaSpecRatioHighPt->Draw("SAME HIST");
        cEtaSpecRatioInv->cd();
        hEtaSpecRatioInv->SetLineColor(kRed);
        hEtaSpecRatioInvRef->SetLineColor(kBlack);
        hEtaSpecRatioInvRef->Draw("SAME HIST");
        hEtaSpecRatioInv->Draw("SAME HIST");
        cEtaSpecRatioInvPtDiff->cd();
        hEtaSpecRatioInvRefLowPt->Draw("SAME HIST");
        hEtaSpecRatioInvLowPt->Draw("SAME HIST");
        hEtaSpecRatioInvRefHighPt->Draw("SAME HIST");
        hEtaSpecRatioInvHighPt->Draw("SAME HIST");
        //
        cPtSpecRatio->cd();
        hPtSpecRatio->SetLineColor(kRed);
        hPtSpecRatioRef->SetLineColor(kBlack);
        hPtSpecRatioRef->Draw("SAME HIST");
        hPtSpecRatio->Draw("SAME HIST");
        cPtSpecRatioInv->cd();
        hPtSpecRatioInv->SetLineColor(kRed);
        hPtSpecRatioInvRef->SetLineColor(kBlack);
        hPtSpecRatioInvRef->Draw("SAME HIST");
        hPtSpecRatioInv->Draw("SAME HIST");

        lSpecRatio->AddEntry(hEtaSpecRatioRef, "reference geometry", "l");
        lSpecRatio->AddEntry(hEtaSpecRatio, "new geometry", "l");

        cEtaPos->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf(", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cEtaNeg->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cEtaRatio->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cEtaRatioPtDiff->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cEtaSpecRatio->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cEtaSpecRatioPtDiff->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cEtaSpecRatioInv->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cEtaSpecRatioInvPtDiff->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cPtPos->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cPtNeg->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cPtRatio->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cPtSpecRatio->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
        cPtSpecRatioInv->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf)", fAnalysisResultsNew.c_str(), labelName.c_str()));

        fOutput->cd();
        cEtaPos->Write();
        cEtaNeg->Write();
        cEtaRatio->Write();
        cEtaRatioPtDiff->Write();
        cEtaSpecRatio->Write();
        cEtaSpecRatioPtDiff->Write();
        cEtaSpecRatioInv->Write();
        cEtaSpecRatioInvPtDiff->Write();
        cPtPos->Write();
        cPtNeg->Write();
        cPtRatio->Write();
        cPtSpecRatio->Write();
        cPtSpecRatioInv->Write();
        fOutput->Close();
    }

    // Now do dimuon-muon correlations from 3D plots
    // dimuon/same-event/single-muon-dimuon-correlations/invariantMass_pT_MuPosPt_MuonKine_MuonCuts

    if (strcmp(drawOpt, "draw_dimuons") == 0)
    {

        std::vector<std::string> vTreeNamesRefDimuons;
        std::vector<std::string> vTreeNamesNewDimuons;
        std::vector<std::string> vHistNamesDimuonsPos;
        std::vector<std::string> vHistNamesDimuonsNeg;

        // reference geometry
        vTreeNamesRefDimuons.push_back(fAnalysisResultsRef);
        vHistNamesDimuonsPos.push_back(fMuonIdRef + "invariantMass_pT_MuPosPt_MuonKine_MuonCuts");
        vTreeNamesRefDimuons.push_back(fAnalysisResultsRef);
        vHistNamesDimuonsPos.push_back(fMuonIdRef + "invariantMass_pT_MuPosEta_MuonKine_MuonCuts");
        vTreeNamesRefDimuons.push_back(fAnalysisResultsRef);
        vHistNamesDimuonsPos.push_back(fMuonIdRef + "invariantMass_pT_MuPosRabs_MuonKine_MuonCuts");
        vTreeNamesRefDimuons.push_back(fAnalysisResultsRef);
        vHistNamesDimuonsPos.push_back(fMuonIdRef + "invariantMass_pT_MuPosDca_MuonKine_MuonCuts");
        vTreeNamesRefDimuons.push_back(fAnalysisResultsRef);
        vHistNamesDimuonsPos.push_back(fMuonIdRef + "invariantMass_pT_MuPosPhi_MuonKine_MuonCuts");

        vHistNamesDimuonsNeg.push_back(fMuonIdRef + "invariantMass_pT_MuNegPt_MuonKine_MuonCuts");
        vHistNamesDimuonsNeg.push_back(fMuonIdRef + "invariantMass_pT_MuNegEta_MuonKine_MuonCuts");
        vHistNamesDimuonsNeg.push_back(fMuonIdRef + "invariantMass_pT_MuNegRabs_MuonKine_MuonCuts");
        vHistNamesDimuonsNeg.push_back(fMuonIdRef + "invariantMass_pT_MuNegDca_MuonKine_MuonCuts");
        vHistNamesDimuonsNeg.push_back(fMuonIdRef + "invariantMass_pT_MuNegPhi_MuonKine_MuonCuts");

        // new geometry
        vTreeNamesNewDimuons.push_back(fAnalysisResultsNew);
        vHistNamesDimuonsPos.push_back(fMuonIdNew + "invariantMass_pT_MuPosPt_MuonKine_MuonCuts");
        vTreeNamesNewDimuons.push_back(fAnalysisResultsNew);
        vHistNamesDimuonsPos.push_back(fMuonIdNew + "invariantMass_pT_MuPosEta_MuonKine_MuonCuts");
        vTreeNamesNewDimuons.push_back(fAnalysisResultsNew);
        vHistNamesDimuonsPos.push_back(fMuonIdNew + "invariantMass_pT_MuPosRabs_MuonKine_MuonCuts");
        vTreeNamesNewDimuons.push_back(fAnalysisResultsNew);
        vHistNamesDimuonsPos.push_back(fMuonIdNew + "invariantMass_pT_MuPosDca_MuonKine_MuonCuts");
        vTreeNamesNewDimuons.push_back(fAnalysisResultsNew);
        vHistNamesDimuonsPos.push_back(fMuonIdNew + "invariantMass_pT_MuPosPhi_MuonKine_MuonCuts");

        vHistNamesDimuonsNeg.push_back(fMuonIdNew + "invariantMass_pT_MuNegPt_MuonKine_MuonCuts");
        vHistNamesDimuonsNeg.push_back(fMuonIdNew + "invariantMass_pT_MuNegEta_MuonKine_MuonCuts");
        vHistNamesDimuonsNeg.push_back(fMuonIdNew + "invariantMass_pT_MuNegRabs_MuonKine_MuonCuts");
        vHistNamesDimuonsNeg.push_back(fMuonIdNew + "invariantMass_pT_MuNegDca_MuonKine_MuonCuts");
        vHistNamesDimuonsNeg.push_back(fMuonIdNew + "invariantMass_pT_MuNegPhi_MuonKine_MuonCuts");

        // TODO:
        // - add ratio plots for mu+ mu- (in the same style as the Jpsi peaks and widths plots)
        // - add larger simulation output from Hyperloop
        // - add new geometry plots
        // - do checks whether pT bins are correctly plotted
        // - add legend with cuts in projections (dimuon invariant mass) and reference/new

        TFile *fDimuonOutput = new TFile(Form("Plots_MCH_Quadrants/dimuonSingleMuonCorrelations_%s_%s.root", fAnalysisResultsNew.c_str(), labelName.c_str()), "RECREATE");
        TCanvas c_dummy("dummy", "dummy", 800, 600);
        c_dummy.SaveAs(Form("Plots_MCH_Quadrants/dimuonSingleMuonMuonCorrelations_%s_%s.pdf(", fAnalysisResultsNew.c_str(), labelName.c_str()));

        TLegend *lOverview = new TLegend();
        TLegend *lRatio = new TLegend();

        std::vector<std::pair<double, double>> ptBinsFull = {
            {0, 30}, {0, 2}, {2, 4}, {4, 6}, {6, 8}, {8, 10}, {10, 12}, {12, 20}};

        for (int i = 0; i < vTreeNamesRefDimuons.size(); i++)
        {
            std::string fNameRef = vTreeNamesRefDimuons[i];
            std::string fNameNew = vTreeNamesNewDimuons[i];
            std::string histNamePos = vHistNamesDimuonsPos[i];
            std::string histNameNeg = vHistNamesDimuonsNeg[i];
            Double_t mMinJpsi = 2.5;
            Double_t mMaxJpsi = 4.0;
            TCanvas *c_overview_1 = new TCanvas(Form("c_overview_1_%s", histNamePos.c_str()), Form("c_overview_1_%s", histNamePos.c_str()), 800, 600);
            TCanvas *c_overview_2 = new TCanvas(Form("c_overview_2_%s", histNamePos.c_str()), Form("c_overview_2_%s", histNamePos.c_str()), 800, 600);
            TCanvas *c_overview_ratio_1 = new TCanvas(Form("c_overview_ratio_1_%s", histNamePos.c_str()), Form("c_overview_ratio_1_%s", histNamePos.c_str()), 800, 600);
            TCanvas *c_overview_ratio_2 = new TCanvas(Form("c_overview_ratio_2_%s", histNamePos.c_str()), Form("c_overview_ratio_2_%s", histNamePos.c_str()), 800, 600);
            c_overview_1->Divide(2, 2);
            c_overview_2->Divide(2, 2);
            c_overview_ratio_1->Divide(2, 2);
            c_overview_ratio_2->Divide(2, 2);

            // Optional cut on z-axis (DCA, Rabs, phi, etc.)
            Double_t minVar;
            Double_t maxVar;
            if (i == 3) // DCA cut (DCA a bit useless in these plots...)
            {
                minVar = 0.;
                maxVar = 1.;
            }
            else
            {
                minVar = -999;
                maxVar = -999;
            }

            for (int j = 0; j < ptBinsFull.size(); j++)
            {
                std::pair<double, double> ptBinFull = ptBinsFull[j];
                TH1 *histRefPos = GetTH1FromTH3(fNameRef, histNamePos, mMinJpsi, mMaxJpsi, ptBinFull.first, ptBinFull.second, minVar, maxVar);
                histRefPos = (TH1 *)histRefPos->Clone(Form("histRefPos_clone_%s_%f_%f", histNamePos.c_str(), ptBinFull.first, ptBinFull.second));
                TH1 *histRefNeg = GetTH1FromTH3(fNameRef, histNameNeg, mMinJpsi, mMaxJpsi, ptBinFull.first, ptBinFull.second, minVar, maxVar);
                histRefNeg = (TH1 *)histRefNeg->Clone(Form("histRefNeg_clone_%s_%f_%f", histNameNeg.c_str(), ptBinFull.first, ptBinFull.second));
                TH1 *histRefRatio = (TH1 *)histRefPos->Clone(Form("histRefRatio_clone_%s_%f_%f", histNamePos.c_str(), ptBinFull.first, ptBinFull.second));
                histRefRatio->Divide(histRefNeg);
                //
                TH1 *histNewPos = GetTH1FromTH3(fNameNew, histNamePos, mMinJpsi, mMaxJpsi, ptBinFull.first, ptBinFull.second, minVar, maxVar);
                histNewPos = (TH1 *)histNewPos->Clone(Form("histNewPos_clone_%s_%f_%f", histNamePos.c_str(), ptBinFull.first, ptBinFull.second));
                TH1 *histNewNeg = GetTH1FromTH3(fNameNew, histNameNeg, mMinJpsi, mMaxJpsi, ptBinFull.first, ptBinFull.second, minVar, maxVar);
                histNewNeg = (TH1 *)histNewNeg->Clone(Form("histNewNeg_clone_%s_%f_%f", histNameNeg.c_str(), ptBinFull.first, ptBinFull.second));
                TH1 *histNewRatio = (TH1 *)histNewPos->Clone(Form("histNewRatio_clone_%s_%f_%f", histNamePos.c_str(), ptBinFull.first, ptBinFull.second));
                histNewRatio->Divide(histNewNeg);

                if (j < 4)
                {
                    c_overview_1->cd(j + 1);
                }
                else if (j > 3)
                {
                    c_overview_2->cd(j - 4 + 1);
                }
                histRefPos->Draw("HIST E");
                histRefNeg->Draw("SAME HIST E");
                histNewPos->Draw("SAME HIST E");
                histNewNeg->Draw("SAME HIST E");
                histRefPos->SetLineColor(kBlack);
                histRefNeg->SetLineColor(kBlack);
                histNewPos->SetLineColor(kRed);
                histNewNeg->SetLineColor(kRed);
                histRefPos->SetLineStyle(0);
                histRefNeg->SetLineStyle(2);
                histNewPos->SetLineStyle(0);
                histNewNeg->SetLineStyle(2);
                histRefPos->SetTitle(Form("p_{T}^{#mu^{+}#mu^{-}} in [%.1f, %.1f]", ptBinFull.first, ptBinFull.second));
                if (i == 0 && j == 0)
                {
                    lOverview->AddEntry(histRefPos, "reference #mu^{+}", "l");
                    lOverview->AddEntry(histRefNeg, "reference #mu^{-}", "l");
                    lOverview->AddEntry(histNewPos, "new #mu^{+}", "l");
                    lOverview->AddEntry(histNewNeg, "new #mu^{-}", "l");
                    lRatio->AddEntry(histRefRatio, "reference", "l");
                    lRatio->AddEntry(histNewRatio, "new", "l");
                }
                /*
                TCanvas *c_binned = new TCanvas(Form("c_binned_%s_%f_%f", histNamePos.c_str(), ptBinFull.first, ptBinFull.second), Form("c_binned_%s_%f_%f", histNamePos.c_str(), ptBinFull.first, ptBinFull.second), 800, 600);
                c_binned->cd();
                histRefPos->Draw("HIST");
                histRefNeg->Draw("SAME HIST");
                histRefPos->SetLineColor(kBlack);
                histRefNeg->SetLineColor(kBlack);
                histRefPos->SetLineStyle(0);
                histRefNeg->SetLineStyle(2);
                histRefPos->SetTitle(Form("dimuon p_{T} in [%.1f, %.1f]", ptBinFull.first, ptBinFull.second));
                */
                if (j < 4)
                {
                    c_overview_ratio_1->cd(j + 1);
                }
                else if (j > 3)
                {
                    c_overview_ratio_2->cd(j - 4 + 1);
                }
                histRefRatio->Draw("HIST");
                histNewRatio->Draw("SAME HIST");
                histRefRatio->SetLineColor(kBlack);
                histNewRatio->SetLineColor(kRed);
                histRefRatio->SetTitle(Form("#mu^{+} / #mu^{-} for p_{T}^{#mu^{+}#mu^{-}} in [%.1f, %.1f]", ptBinFull.first, ptBinFull.second));

                if (j == 3)
                {
                    c_overview_1->cd(1);
                    lOverview->Draw();
                    c_overview_ratio_1->cd(1);
                    lRatio->Draw();
                    c_overview_1->SaveAs(Form("Plots_MCH_Quadrants/dimuonSingleMuonMuonCorrelations_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
                    c_overview_ratio_1->SaveAs(Form("Plots_MCH_Quadrants/dimuonSingleMuonMuonCorrelations_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
                    fDimuonOutput->cd();
                    c_overview_1->Write();
                    c_overview_ratio_1->Write();
                }
                if (j == 7)
                {
                    c_overview_2->cd(1);
                    lOverview->Draw();
                    c_overview_ratio_2->cd(1);
                    lRatio->Draw();
                    c_overview_2->SaveAs(Form("Plots_MCH_Quadrants/dimuonSingleMuonMuonCorrelations_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
                    c_overview_ratio_2->SaveAs(Form("Plots_MCH_Quadrants/dimuonSingleMuonMuonCorrelations_%s_%s.pdf", fAnalysisResultsNew.c_str(), labelName.c_str()));
                    fDimuonOutput->cd();
                    c_overview_2->Write();
                    c_overview_ratio_2->Write();
                }
            }
        }

        c_dummy.SaveAs(Form("Plots_MCH_Quadrants/dimuonSingleMuonMuonCorrelations_%s_%s.pdf)", fAnalysisResultsNew.c_str(), labelName.c_str()));
    }

    // Draw the DCA distributions for dimuons within the Jpsi mass range
    // The DCAs are given as DCA(mu+) - DCA(mu-)
    // This is done for 'physical' events and for mixed events. Physical is subtracted by mixed to reduce background.
    // Everything is done for:
    // --- all given geometries
    // --- pT binning that can be specified by the user
    // --- top/bottom and left/right combinations of the CH1 of the MCH detector
    // In addition, the comparison is made with the 'reference' geometry.
    if (strcmp(drawOpt, "draw_dimuonDCA") == 0)
    {
        // AnalysisResults files
        std::vector<std::string> vTreeNamesRefDimuons;
        std::vector<std::string> vTreeNamesNewDimuons;

        // 'Same' events
        std::vector<std::string> vSameHistNamesRefDimuonDCAx;
        std::vector<std::string> vSameHistNamesNewDimuonDCAx;
        std::vector<std::string> vSameHistNamesRefDimuonDCAy;
        std::vector<std::string> vSameHistNamesNewDimuonDCAy;

        // 'Same' events with mu+mu+ background
        std::vector<std::string> vSamePPHistNamesRefDimuonDCAx;
        std::vector<std::string> vSamePPHistNamesNewDimuonDCAx;
        std::vector<std::string> vSamePPHistNamesRefDimuonDCAy;
        std::vector<std::string> vSamePPHistNamesNewDimuonDCAy;

        // 'Same' events with mu-mu- background
        std::vector<std::string> vSameMMHistNamesRefDimuonDCAx;
        std::vector<std::string> vSameMMHistNamesNewDimuonDCAx;
        std::vector<std::string> vSameMMHistNamesRefDimuonDCAy;
        std::vector<std::string> vSameMMHistNamesNewDimuonDCAy;

        // 'Mixed' events (for background reduction)
        std::vector<std::string> vMixedHistNamesRefDimuonDCAx;
        std::vector<std::string> vMixedHistNamesNewDimuonDCAx;
        std::vector<std::string> vMixedHistNamesRefDimuonDCAy;
        std::vector<std::string> vMixedHistNamesNewDimuonDCAy;

        // same events
        // opposite sign +- or -+
        // --- reference geometry
        vTreeNamesRefDimuons.push_back(fAnalysisResultsRef);
        vSameHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/opposite-sign/DCA/pT_MuPosTDCAx_minus_MuNegTDCAx_MuonKine_MuonCuts");
        vTreeNamesRefDimuons.push_back(fAnalysisResultsRef);
        vSameHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/opposite-sign/DCA/pT_MuPosTDCAx_minus_MuNegBDCAx_MuonKine_MuonCuts");
        vTreeNamesRefDimuons.push_back(fAnalysisResultsRef);
        vSameHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/opposite-sign/DCA/pT_MuPosBDCAx_minus_MuNegTDCAx_MuonKine_MuonCuts");
        vTreeNamesRefDimuons.push_back(fAnalysisResultsRef);
        vSameHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/opposite-sign/DCA/pT_MuPosBDCAx_minus_MuNegBDCAx_MuonKine_MuonCuts");

        vSameHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/opposite-sign/DCA/pT_MuPosTDCAy_minus_MuNegTDCAy_MuonKine_MuonCuts");
        vSameHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/opposite-sign/DCA/pT_MuPosTDCAy_minus_MuNegBDCAy_MuonKine_MuonCuts");
        vSameHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/opposite-sign/DCA/pT_MuPosBDCAy_minus_MuNegTDCAy_MuonKine_MuonCuts");
        vSameHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/opposite-sign/DCA/pT_MuPosBDCAy_minus_MuNegBDCAy_MuonKine_MuonCuts");

        // --- new geometry
        vTreeNamesNewDimuons.push_back(fAnalysisResultsNew);
        vSameHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/opposite-sign/DCA/pT_MuPosTDCAx_minus_MuNegTDCAx_MuonKine_MuonCuts");
        vTreeNamesNewDimuons.push_back(fAnalysisResultsNew);
        vSameHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/opposite-sign/DCA/pT_MuPosTDCAx_minus_MuNegBDCAx_MuonKine_MuonCuts");
        vTreeNamesNewDimuons.push_back(fAnalysisResultsNew);
        vSameHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/opposite-sign/DCA/pT_MuPosBDCAx_minus_MuNegTDCAx_MuonKine_MuonCuts");
        vTreeNamesNewDimuons.push_back(fAnalysisResultsNew);
        vSameHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/opposite-sign/DCA/pT_MuPosBDCAx_minus_MuNegBDCAx_MuonKine_MuonCuts");

        vSameHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/opposite-sign/DCA/pT_MuPosTDCAy_minus_MuNegTDCAy_MuonKine_MuonCuts");
        vSameHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/opposite-sign/DCA/pT_MuPosTDCAy_minus_MuNegBDCAy_MuonKine_MuonCuts");
        vSameHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/opposite-sign/DCA/pT_MuPosBDCAy_minus_MuNegTDCAy_MuonKine_MuonCuts");
        vSameHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/opposite-sign/DCA/pT_MuPosBDCAy_minus_MuNegBDCAy_MuonKine_MuonCuts");

        // same-sign ++
        // --- reference geometry
        vSamePPHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/same-sign-PP/DCA/pT_Mu1TDCAx_minus_Mu2TDCAx_MuonKine_MuonCuts");
        vSamePPHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/same-sign-PP/DCA/pT_MuTDCAx_minus_MuBDCAx_MuonKine_MuonCuts");
        // vSamePPHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/same-sign-PP/DCA/pT_Mu1BDCAx_minus_Mu2TDCAx_MuonKine_MuonCuts");
        vSamePPHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/same-sign-PP/DCA/pT_Mu1BDCAx_minus_Mu2BDCAx_MuonKine_MuonCuts");

        vSamePPHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/same-sign-PP/DCA/pT_Mu1TDCAy_minus_Mu2TDCAy_MuonKine_MuonCuts");
        vSamePPHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/same-sign-PP/DCA/pT_MuTDCAy_minus_MuBDCAy_MuonKine_MuonCuts");
        // vSamePPHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/same-sign-PP/DCA/pT_Mu1BDCAy_minus_Mu2TDCAy_MuonKine_MuonCuts");
        vSamePPHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/same-sign-PP/DCA/pT_Mu1BDCAy_minus_Mu2BDCAy_MuonKine_MuonCuts");

        // --- new geometry
        vSamePPHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/same-sign-PP/DCA/pT_Mu1TDCAx_minus_Mu2TDCAx_MuonKine_MuonCuts");
        vSamePPHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/same-sign-PP/DCA/pT_MuTDCAx_minus_MuBDCAx_MuonKine_MuonCuts");
        // vSamePPHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/same-sign-PP/DCA/pT_Mu1BDCAx_minus_Mu2TDCAx_MuonKine_MuonCuts");
        vSamePPHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/same-sign-PP/DCA/pT_Mu1BDCAx_minus_Mu2BDCAx_MuonKine_MuonCuts");

        vSamePPHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/same-sign-PP/DCA/pT_Mu1TDCAy_minus_Mu2TDCAy_MuonKine_MuonCuts");
        vSamePPHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/same-sign-PP/DCA/pT_MuTDCAy_minus_MuBDCAy_MuonKine_MuonCuts");
        // vSamePPHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/same-sign-PP/DCA/pT_Mu1BDCAy_minus_Mu2TDCAy_MuonKine_MuonCuts");
        vSamePPHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/same-sign-PP/DCA/pT_Mu1BDCAy_minus_Mu2BDCAy_MuonKine_MuonCuts");

        // same-sign --
        // --- reference geometry
        vSameMMHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/same-sign-MM/DCA/pT_Mu1TDCAx_minus_Mu2TDCAx_MuonKine_MuonCuts");
        vSameMMHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/same-sign-MM/DCA/pT_MuTDCAx_minus_MuBDCAx_MuonKine_MuonCuts");
        // vSameMMHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/same-sign-MM/DCA/pT_Mu1BDCAx_minus_Mu2TDCAx_MuonKine_MuonCuts");
        vSameMMHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "same-event/same-sign-MM/DCA/pT_Mu1BDCAx_minus_Mu2BDCAx_MuonKine_MuonCuts");

        vSameMMHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/same-sign-MM/DCA/pT_Mu1TDCAy_minus_Mu2TDCAy_MuonKine_MuonCuts");
        vSameMMHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/same-sign-MM/DCA/pT_MuTDCAy_minus_MuBDCAy_MuonKine_MuonCuts");
        // vSameMMHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/same-sign-MM/DCA/pT_Mu1BDCAy_minus_Mu2TDCAy_MuonKine_MuonCuts");
        vSameMMHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "same-event/same-sign-MM/DCA/pT_Mu1BDCAy_minus_Mu2BDCAy_MuonKine_MuonCuts");

        // --- new geometry
        vSameMMHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/same-sign-MM/DCA/pT_Mu1TDCAx_minus_Mu2TDCAx_MuonKine_MuonCuts");
        vSameMMHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/same-sign-MM/DCA/pT_MuTDCAx_minus_MuBDCAx_MuonKine_MuonCuts");
        // vSameMMHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/same-sign-MM/DCA/pT_Mu1BDCAx_minus_Mu2TDCAx_MuonKine_MuonCuts");
        vSameMMHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "same-event/same-sign-MM/DCA/pT_Mu1BDCAx_minus_Mu2BDCAx_MuonKine_MuonCuts");

        vSameMMHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/same-sign-MM/DCA/pT_Mu1TDCAy_minus_Mu2TDCAy_MuonKine_MuonCuts");
        vSameMMHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/same-sign-MM/DCA/pT_MuTDCAy_minus_MuBDCAy_MuonKine_MuonCuts");
        // vSameMMHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/same-sign-MM/DCA/pT_Mu1BDCAy_minus_Mu2TDCAy_MuonKine_MuonCuts");
        vSameMMHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "same-event/same-sign-MM/DCA/pT_Mu1BDCAy_minus_Mu2BDCAy_MuonKine_MuonCuts");

        // mixed events
        // --- reference geometry
        vMixedHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "mixed-event/DCA/pT_MuPosTDCAx_minus_MuNegTDCAx_MuonKine_MuonCuts");
        vMixedHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "mixed-event/DCA/pT_MuPosTDCAx_minus_MuNegBDCAx_MuonKine_MuonCuts");
        vMixedHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "mixed-event/DCA/pT_MuPosBDCAx_minus_MuNegTDCAx_MuonKine_MuonCuts");
        vMixedHistNamesRefDimuonDCAx.push_back(fMuonIdRef + "mixed-event/DCA/pT_MuPosBDCAx_minus_MuNegBDCAx_MuonKine_MuonCuts");

        vMixedHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "mixed-event/DCA/pT_MuPosTDCAy_minus_MuNegTDCAy_MuonKine_MuonCuts");
        vMixedHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "mixed-event/DCA/pT_MuPosTDCAy_minus_MuNegBDCAy_MuonKine_MuonCuts");
        vMixedHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "mixed-event/DCA/pT_MuPosBDCAy_minus_MuNegTDCAy_MuonKine_MuonCuts");
        vMixedHistNamesRefDimuonDCAy.push_back(fMuonIdRef + "mixed-event/DCA/pT_MuPosBDCAy_minus_MuNegBDCAy_MuonKine_MuonCuts");

        // --- new geometry
        vMixedHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "mixed-event/DCA/pT_MuPosTDCAx_minus_MuNegTDCAx_MuonKine_MuonCuts");
        vMixedHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "mixed-event/DCA/pT_MuPosTDCAx_minus_MuNegBDCAx_MuonKine_MuonCuts");
        vMixedHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "mixed-event/DCA/pT_MuPosBDCAx_minus_MuNegTDCAx_MuonKine_MuonCuts");
        vMixedHistNamesNewDimuonDCAx.push_back(fMuonIdNew + "mixed-event/DCA/pT_MuPosBDCAx_minus_MuNegBDCAx_MuonKine_MuonCuts");

        vMixedHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "mixed-event/DCA/pT_MuPosTDCAy_minus_MuNegTDCAy_MuonKine_MuonCuts");
        vMixedHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "mixed-event/DCA/pT_MuPosTDCAy_minus_MuNegBDCAy_MuonKine_MuonCuts");
        vMixedHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "mixed-event/DCA/pT_MuPosBDCAy_minus_MuNegTDCAy_MuonKine_MuonCuts");
        vMixedHistNamesNewDimuonDCAy.push_back(fMuonIdNew + "mixed-event/DCA/pT_MuPosBDCAy_minus_MuNegBDCAy_MuonKine_MuonCuts");

        // Just so that everything is drawn in this order: x -> all ptBins, then y -> all ptBins
        //std::vector<std::vector<std::string>> vSameHistNamesRefDimuonDCAxy = {vSameHistNamesRefDimuonDCAx, vSameHistNamesRefDimuonDCAy};
        //std::vector<std::vector<std::string>> vSameHistNamesNewDimuonDCAxy = {vSameHistNamesNewDimuonDCAx, vSameHistNamesNewDimuonDCAy};
        const char *vxy[] = {"x", "y"};
        std::vector<std::string> vDCAspecs = {"DCA(mu+T) - DCA(mu-T)", "DCA(mu+T) - DCA(mu-B)", "DCA(mu+B) - DCA(mu-T)", "DCA(mu+B) - DCA(mu-B)"};

        // Dummy canvas for labelling (not to overcrowd the plots)
        TCanvas *c_dummy_label = new TCanvas(Form("c_dummy_label_%s", fAnalysisResultsNew.c_str()), Form("c_dummy_label_%s", fAnalysisResultsNew.c_str()), 800, 600);

        // Drawing settings (to be consistent with previous plots)
        std::vector<int> vLineStylesRef = {2, 3, 4, 6};
        std::vector<int> vLineColoursRef = {1, 1, 1, 1};
        std::vector<int> vLineStylesNew = {1, 1, 1, 1};
        std::vector<int> vLineColoursNew = {8, 4, 6, 7};
        std::vector<double> vYuppValues = {0.004, 0.004, 0.005, 0.006, 0.008, 0.02, 0.02, 0.02};

        std::vector<std::pair<double, double>> ptBinsFull = {
            {0, 30}, {0, 2}, {2, 4}, {4, 6}, {6, 8}, {8, 10}, {10, 12}, {12, 20}};

        // For average DCA plot
        TH1D *hTemplatePeaks = new TH1D("hTemplatePeaks", "hTemplatePeaks", ptBinsFull.size(), 0, ptBinsFull.size());
        hTemplatePeaks->SetStats(0);
        hTemplatePeaks->SetTitle("average DCA");
        hTemplatePeaks->GetYaxis()->SetRangeUser(-3, 3);
        for (int ibin = 0; ibin < ptBinsFull.size(); ibin++)
        {
            hTemplatePeaks->GetXaxis()->SetBinLabel(ibin + 1, Form("[%.1f, %.1f]", ptBinsFull[ibin].first, ptBinsFull[ibin].second));
        }

        std::vector<std::string> eventTypes = {"same", "mixed", "plus-plus", "minus-minus"};
        // BE CAREFUL: SAME SIGN PAIRS DO NOT HAVE TB AND BT COMBINATIONS. ALL MIXED COMBINATIONS ARE T - B ALWAYS

        for (const auto &eventType : eventTypes)
        {
            // Bash structure gives muon-qa_id30697/dimuon/

            bool isSame = (eventType == "same");
            bool isMixed = (eventType == "mixed");
            bool isSamePP = (eventType == "plus-plus");
            bool isSameMM = (eventType == "minus-minus");

            std::vector<std::vector<std::string>> vHistNamesRefDimuonDCAxy;
            std::vector<std::vector<std::string>> vHistNamesNewDimuonDCAxy;

            if (isSame)
            {
                hTemplatePeaks->GetYaxis()->SetTitle("DCA(mu+) - DCA(mu-)");
                vHistNamesRefDimuonDCAxy = {vSameHistNamesRefDimuonDCAx, vSameHistNamesRefDimuonDCAy};
                vHistNamesNewDimuonDCAxy = {vSameHistNamesNewDimuonDCAx, vSameHistNamesNewDimuonDCAy};
            }
            else if (isMixed)
            {
                hTemplatePeaks->GetYaxis()->SetTitle("DCA(mu+) - DCA(mu-)");
                vHistNamesRefDimuonDCAxy = {vMixedHistNamesRefDimuonDCAx, vMixedHistNamesRefDimuonDCAy};
                vHistNamesNewDimuonDCAxy = {vMixedHistNamesNewDimuonDCAx, vMixedHistNamesNewDimuonDCAy};
            }
            else if (isSamePP)
            {
                hTemplatePeaks->GetYaxis()->SetTitle("DCA(mu_1) - DCA(mu_2)");
                vHistNamesRefDimuonDCAxy = {vSamePPHistNamesRefDimuonDCAx, vSamePPHistNamesRefDimuonDCAy};
                vHistNamesNewDimuonDCAxy = {vSamePPHistNamesNewDimuonDCAx, vSamePPHistNamesNewDimuonDCAy};
            }
            else if (isSameMM)
            {
                hTemplatePeaks->GetYaxis()->SetTitle("DCA(mu_1) - DCA(mu_2)");
                vHistNamesRefDimuonDCAxy = {vSameMMHistNamesRefDimuonDCAx, vSameMMHistNamesRefDimuonDCAy};
                vHistNamesNewDimuonDCAxy = {vSameMMHistNamesNewDimuonDCAx, vSameMMHistNamesNewDimuonDCAy};
            }

            // Print which event type is processed
            std::cout << "Processing " << eventType << "-event dimuon DCA plots..." << std::endl;

            TCanvas c_dummy_save("dummy_save", "dummy_save", 800, 600);
            c_dummy_save.SaveAs(Form("Plots_MCH_Quadrants/dimuonDCAs_%s_%s_%s.pdf(", eventType.c_str(), fAnalysisResultsNew.c_str(), labelName.c_str()));

            for (int k = 0; k < vHistNamesRefDimuonDCAxy.size(); k++)
            {
                // DCAx or DCAy
                std::cout << "j = " << k << std::endl;
                std::vector<std::string> vHistNamesRefDimuonDCA = vHistNamesRefDimuonDCAxy[k];
                std::vector<std::string> vHistNamesNewDimuonDCA = vHistNamesNewDimuonDCAxy[k];

                TCanvas *c_overview_1 = new TCanvas(Form("c_overview_1_%s", eventType.c_str()), Form("c_overview_1_%s", eventType.c_str()), 800, 600);
                TCanvas *c_overview_2 = new TCanvas(Form("c_overview_2_%s", eventType.c_str()), Form("c_overview_2_%s", eventType.c_str()), 800, 600);
                c_overview_1->Divide(2, 2);
                c_overview_2->Divide(2, 2);

                // Create TLatex for text drawing
                c_dummy_label->cd();
                TLegend *l_dummy = new TLegend(0.1, 0.1, 0.9, 0.9);
                l_dummy->SetBorderSize(0);
                l_dummy->SetFillStyle(0);
                l_dummy->SetTextSize(0.025);

                // Plot with average DCAs
                TCanvas *cPeaks = new TCanvas(Form("cPeaks_%s", eventType.c_str()), Form("cPeaks_%s", eventType.c_str()), 800, 600);
                cPeaks->cd();
                hTemplatePeaks->Draw();

                for (int i = 0; i < vHistNamesRefDimuonDCA.size(); i++)
                {
                    std::cout << "i = " << i << std::endl;
                    std::string fNameRef = vTreeNamesRefDimuons[i];
                    std::string fNameNew = vTreeNamesNewDimuons[i];
                    std::string histNameRefDCAspec = vHistNamesRefDimuonDCA[i];
                    std::string histNameNewDCAspec = vHistNamesNewDimuonDCA[i];

                    // Create dummy histogram to set DCA peak position values (for x and y seperately)
                    TH1D *hRefPeaks = new TH1D(Form("hRefPeaks_%i_%i", i, k), "ref peak per pT bin", ptBinsFull.size(), 0, ptBinsFull.size());
                    TH1D *hNewPeaks = new TH1D(Form("hNewPeaks_%i_%i", i, k), "new peak per pT bin", ptBinsFull.size(), 0, ptBinsFull.size());

                    for (int j = 0; j < ptBinsFull.size(); j++)
                    {
                        std::pair<double, double> ptBinFull = ptBinsFull[j];
                        // Look at function definition, doesn't give "eta" per se..
                        TH1 *histRefDCAspec = GetEtaFromTH2(fNameRef, histNameRefDCAspec, ptBinFull.first, ptBinFull.second);
                        histRefDCAspec = (TH1 *)histRefDCAspec->Clone(Form("histRefDCAspec_clone_%s_%f_%f", histNameRefDCAspec.c_str(), ptBinFull.first, ptBinFull.second));
                        histRefDCAspec->Scale(1. / histRefDCAspec->GetEntries());
                        //
                        TH1 *histNewDCAspec = GetEtaFromTH2(fNameNew, histNameNewDCAspec, ptBinFull.first, ptBinFull.second);
                        histNewDCAspec = (TH1 *)histNewDCAspec->Clone(Form("histNewDCAspec_clone_%s_%f_%f", histNameNewDCAspec.c_str(), ptBinFull.first, ptBinFull.second));
                        histNewDCAspec->Scale(1. / histNewDCAspec->GetEntries());

                        hRefPeaks->SetBinContent(j + 1, histRefDCAspec->GetMean());
                        hNewPeaks->SetBinContent(j + 1, histNewDCAspec->GetMean());
                        hRefPeaks->SetBinError(j + 1, histRefDCAspec->GetMeanError());
                        hNewPeaks->SetBinError(j + 1, histNewDCAspec->GetMeanError());

                        if (j < 4)
                        {
                            c_overview_1->cd(j + 1);
                        }
                        else if (j > 3)
                        {
                            c_overview_2->cd(j - 4 + 1);
                        }

                        if (i == 0)
                        {
                            histNewDCAspec->GetYaxis()->SetTitle("count/total counts");
                            histNewDCAspec->Draw("HIST");
                            if (isSamePP || isSameMM) {
                                histNewDCAspec->GetYaxis()->SetRangeUser(0, vYuppValues[j]);
                            }
                        }
                        else
                        {
                            histNewDCAspec->Draw("SAME HIST");
                        }
                        histRefDCAspec->Draw("SAME HIST");
                        histRefDCAspec->SetLineColor(vLineColoursRef[i]);
                        histNewDCAspec->SetLineColor(vLineColoursNew[i]);
                        histRefDCAspec->SetLineStyle(vLineStylesRef[i]);
                        histNewDCAspec->SetLineStyle(vLineStylesNew[i]);

                        // ADD: if same-sign AND if i == 1 (T-B hists)
                        // --- put histNewDCAspec->GetYaxis()->SetRangeUser(1.2 * histNewDCAspec->GetMaximum());
                        
                        if (i == 0)
                        {
                            std::cout << "j = " << j << std::endl;
                            std::cout << "DEBUG: setting title for minipads" << std::endl;
                            histNewDCAspec->SetTitle(Form("DCA_%s for p_{T}^{#mu^{+}#mu^{-}} in [%.1f, %.1f]", vxy[k], ptBinFull.first, ptBinFull.second));
                        }

                        if (j == 0 && k == 0)
                        {
                            std::cout << "i = " << i << std::endl;
                            std::cout << "hist name = " << histNameNewDCAspec.c_str() << std::endl;
                            c_dummy_label->cd();
                            if (i == 0)
                            {
                                l_dummy->AddEntry((TObject *)0, Form("%s", labelName.c_str()), "");
                            }
                            l_dummy->AddEntry(histRefDCAspec, Form("REFERENCE: %s", vDCAspecs[i].c_str()), "l");
                            l_dummy->AddEntry(histNewDCAspec, Form("NEW: %s", vDCAspecs[i].c_str()), "l");
                            if (i == vHistNamesRefDimuonDCA.size() - 1)
                            {
                                l_dummy->Draw();
                                c_dummy_label->SaveAs(Form("Plots_MCH_Quadrants/dimuonDCAs_%s_%s_%s.pdf", eventType.c_str(), fAnalysisResultsNew.c_str(), labelName.c_str()));
                            }
                        }

                        if (i == vHistNamesRefDimuonDCA.size() - 1 && j == ptBinsFull.size() - 1)
                        {
                            std::cout << "SAVING CANVASES TO PDF" << std::endl;
                            c_overview_1->SaveAs(Form("Plots_MCH_Quadrants/dimuonDCAs_%s_%s_%s.pdf", eventType.c_str(), fAnalysisResultsNew.c_str(), labelName.c_str()));
                            c_overview_2->SaveAs(Form("Plots_MCH_Quadrants/dimuonDCAs_%s_%s_%s.pdf", eventType.c_str(), fAnalysisResultsNew.c_str(), labelName.c_str()));
                        }
                    }
                    cPeaks->cd();
                    hRefPeaks->Draw("SAME");
                    hNewPeaks->Draw("SAME");
                    hRefPeaks->SetLineColor(vLineColoursRef[i]);
                    hNewPeaks->SetLineColor(vLineColoursNew[i]);
                    hRefPeaks->SetLineStyle(vLineStylesRef[i]);
                    hNewPeaks->SetLineStyle(vLineStylesNew[i]);
                    if (i == vHistNamesRefDimuonDCA.size() - 1)
                    {
                        cPeaks->SaveAs(Form("Plots_MCH_Quadrants/dimuonDCAs_%s_%s_%s.pdf", eventType.c_str(), fAnalysisResultsNew.c_str(), labelName.c_str()));
                    }
                }
            }
            c_dummy_save.SaveAs(Form("Plots_MCH_Quadrants/dimuonDCAs_%s_%s_%s.pdf)", eventType.c_str(), fAnalysisResultsNew.c_str(), labelName.c_str()));
        }
    }
}

int muonQA_pT_eta_single_muons(std::string labelName, const char *drawHalfOpt, const char *drawOpt, std::string fAnalysisResultsRef, std::string fMuonIdRef, std::string fAnalysisResultsNew, std::string fMuonIdNew)
{
    makePlots(labelName, drawHalfOpt, drawOpt, fAnalysisResultsRef, fMuonIdRef, fAnalysisResultsNew, fMuonIdNew);
    return 0;
}