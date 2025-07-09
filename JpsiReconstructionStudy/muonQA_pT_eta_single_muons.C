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

TH1 *GetTH1(const char *fileName, std::string histName)
{
    TFile *f = new TFile(fileName, "READ");
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
TH1 *GetTH1FromTH2(const char *fileName, std::string histName, Double_t etaMin, Double_t etaMax)
{
    TFile *f = new TFile(fileName, "READ");
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
    TH1 *hPt_EtaCut = hist->ProjectionX("hPt_EtaCut");
    std::cout << "no problem with projection" << std::endl;
    return hPt_EtaCut;
}

void makePlots()
{
    std::vector<const char *> vTreeNamesEtaPos;
    std::vector<const char *> vTreeNamesEtaNeg;
    std::vector<const char *> vTreeNamesPtPos;
    std::vector<const char *> vTreeNamesPtNeg;
    // std::vector<const char*> vMuonIds; // trains are combined in Hyperloop with different configurations in the same output file
    std::vector<std::string> vHistNamesEtaPos;
    std::vector<std::string> vHistNamesEtaNeg;
    std::vector<std::string> vHistNamesPtPos;
    std::vector<std::string> vHistNamesPtNeg;
    const char *labelName; // saves PDF with widhts and peaks using this label name
    const char *fAnalysisResults;
    std::string fMuonId; // trains are combined in Hyperloop with different configurations in the same output file

    // fAnalysisResults = "AnalysisResults-muonQA_LHC24an_pass1_skimmed_small_muonID-30697_no_realignment_muonID-30255_javier_new2_muon_ID-30698_globalShiftY_Hyperloop_17_06_2025.root";
    fAnalysisResults = "AnalysisResults_LHC24am_pass1_skimmed_muon-QA_Hyperloop_02_07_2025.root";
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

    // ======================[ GeometryAligned: Reference (no realignment) ]=====================
    // Top-Bottom

    // labelName = "LHC24an_pass1_skimmed_no_realignment_top-bottom";
    // std::vector<const char *> vLegendEntries = {"integrated", "top", "bottom"};
    fMuonId = "muon-qa_id30697/muons/";
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_T");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_B");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_T");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_B");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_T");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_B");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_T");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_B");

    // Left-Right
    /*
    // labelName = "LHC24an_pass1_skimmed_no_realignment_left-right";
    // std::vector<const char*> vLegendEntries = {"integrated", "left", "right"};
    fMuonId = "muon-qa_id30697/muons/";
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_L");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_R");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_L");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_R");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_L");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_R");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_L");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_R");
    */

    // ======================[ GeometryAlignedFix110Fix15New2T5: Javier new2 ]=====================
    // Top-Bottom

    labelName = "GeometryAlignedFix110Fix15New2T5_top-bottom";
    fMuonId = "muon-qa_id30992/muons/";
    std::vector<const char *> vLegendEntries = {"reference int", "reference T", "reference B", "new geometry int", "new geometry T", "new geometry B"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_T");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_B");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_T");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_B");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_T");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_B");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_T");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_B");

    // Left-Right

    /*
    labelName = "GeometryAlignedFix110Fix15New2T5_left-right";
    fMuonId = "muon-qa_id30992/muons/";
    std::vector<const char*> vLegendEntries = {"reference int", "reference L", "reference R", "new geometry int", "new geometry L","new geometry R"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_L");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_R");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_L");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_R");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_L");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_R");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_L");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_R");
    */

    // ======================[ GeometryAlignedShiftY: globalShiftY ]=====================
    // Top-Bottom
    /*
    labelName = "GeometryAlignedShiftY_top-bottom";
    fMuonId = "muon-qa_id30991/muons/";
    std::vector<const char*> vLegendEntries = {"reference int", "reference T", "reference B", "new geometry int", "new geometry T","new geometry B"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_T");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_B");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_T");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_B");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_T");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_B");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_T");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_B");
    */

    // Left-Right

    /*
    labelName = "GeometryAlignedShiftY_left-right";
    fMuonId = "muon-qa_id30991/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference L", "reference R", "new geometry int", "new geometry L","new geometry R"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_L");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_R");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_L");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_R");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_L");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_R");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_L");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_R");
    */

    // ======================[ GeometryAlignedFix110Fix15New2T5ShiftY: globalShiftY + javier new2 ]=====================
    // Top-Bottom
    /*
    labelName = "GeometryAlignedFix110Fix15New2T5ShiftY_top-bottom";
    fMuonId = "muon-qa_id30993/muons/";
    std::vector<const char*> vLegendEntries = {"reference int", "reference T", "reference B", "new geometry int", "new geometry T","new geometry B"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_T");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_B");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_T");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_B");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_T");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_B");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_T");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_B");
    */

    // Left-Right

    /*
    labelName = "GeometryAlignedFix110Fix15New2T5ShiftY_left-right";
    fMuonId = "muon-qa_id30993/muons/";
    std::vector<const char*> vLegendEntries = {"reference int", "reference L", "reference R", "new geometry int", "new geometry L","new geometry R"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_L");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_R");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_L");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_R");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_L");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_R");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_L");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_R");
    */

    // ======================[ GeometryAlignedFix10Fix15ShiftCh1BNew2: bottom only CH1 shift ]=====================
    // Top-Bottom

    /*
    labelName = "GeometryAlignedFix10Fix15ShiftCh1BNew2_top-bottom";
    fMuonId = "muon-qa_id30619/muons/";
    std::vector<const char*> vLegendEntries = {"reference int", "reference T", "reference B", "new geometry int", "new geometry T","new geometry B"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_T");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_B");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_T");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_B");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_T");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_B");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_T");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_B");
    */

    // Left-Right

    /*
    labelName = "GeometryAlignedFix10Fix15ShiftCh1BNew2_left-right";
    fMuonId = "muon-qa_id30619/muons/";
    std::vector<const char*> vLegendEntries = {"reference int", "reference L", "reference R", "new geometry int", "new geometry L","new geometry R"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_L");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_R");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_L");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_R");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_L");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_R");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_L");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_R");
    */

    // ======================[ GeometryAlignedFix10Fix15Shift04Ch1BNew2: bottom only CH1 shift with half the amount ]=====================
    // Top-Bottom

    /*
    labelName = "GeometryAlignedFix10Fix15Shift04Ch1BNew2_top-bottom";
    // fMuonId = "muon-qa/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    fMuonId = "muon-qa_id31157/muons/";
    std::vector<const char*> vLegendEntries = {"reference int", "reference T", "reference B", "new geometry int", "new geometry T","new geometry B"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_T");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_B");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_T");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_B");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_T");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_B");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_T");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_B");
    */

    // Left-Right

    /*
    labelName = "GeometryAlignedFix10Fix15Shift04Ch1BNew2_left-right";
    fMuonId = "muon-qa_id31157/muons/";
    std::vector<const char*> vLegendEntries = {"reference int", "reference L", "reference R", "new geometry int", "new geometry L","new geometry R"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_L");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_R");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_L");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_R");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_L");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_R");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_L");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_R");
    */

    // ======================[ GeometryAlignedFix10Fix15ShiftCh1TBNew2: bottom and top CH1 1/2 shift ]=====================
    // Top-Bottom

    /*
    labelName = "GeometryAlignedFix10Fix15ShiftCh1TBNew2_top-bottom";
    fMuonId = "muon-qa_id30620/muons/";
    std::vector<const char*> vLegendEntries = {"reference int", "reference T", "reference B", "new geometry int", "new geometry T","new geometry B"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_T");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_B");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_T");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_B");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_T");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_B");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_T");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_B");
    */

    // Left-Right

    /*
    labelName = "GeometryAlignedFix10Fix15ShiftCh1TBNew2_left-right";
    fMuonId = "muon-qa_id30620/muons/";
    std::vector<const char*> vLegendEntries = {"reference int", "reference L", "reference R", "new geometry int", "new geometry L"","new geometry R"};
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_T");
    vTreeNamesEtaPos.push_back(fAnalysisResults);
    vHistNamesEtaPos.push_back(fMuonId + "TrackEtaPos_B");
    //
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_T");
    vTreeNamesEtaNeg.push_back(fAnalysisResults);
    vHistNamesEtaNeg.push_back(fMuonId + "TrackEtaNeg_B");
    //
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_T");
    vTreeNamesPtPos.push_back(fAnalysisResults);
    vHistNamesPtPos.push_back(fMuonId + "TrackPt_TrackEtaPos_B");
    //
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_T");
    vTreeNamesPtNeg.push_back(fAnalysisResults);
    vHistNamesPtNeg.push_back(fMuonId + "TrackPt_TrackEtaNeg_B");
    */

   // ======================[ GeometryAlignedFix110Fix19NewIST7: changes? ]=====================
    // Top-Bottom
    
    /*
    fAnalysisResults = "AnalysisResults-muonQA_LHC24am_pass1_skimmed_GeometryAlignedFix110Fix19NewIST7_Hyperloop_08_07_2025.root";
    labelName = "GeometryAlignedFix110Fix19NewIST7_top-bottom";
    fMuonId = "muon-qa/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference TT","reference TB or BT", "reference BB", "new geometry int", "new geometry TT","new geometry TB or BT","new geometry BB"};
    */

    // Left-Right

    /*
    fAnalysisResults = "AnalysisResults-muonQA_LHC24am_pass1_skimmed_GeometryAlignedFix110Fix19NewIST7_Hyperloop_08_07_2025.root";
    labelName = "GeometryAlignedFix110Fix19NewIST7_left-right";
    fMuonId = "muon-qa/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference LL","reference LR or RR", "reference RR", "new geometry int", "new geometry LL","new geometry LR or RL","new geometry RR"};
    */

    TFile *fOutput = new TFile(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.root", fAnalysisResults, labelName), "RECREATE");

    std::vector<Int_t> vLineStyles = {1, 2, 6, 1, 1, 1}; // in order to show reference everywhere..
    std::vector<Int_t> vLineColours = {1, 1, 1, 2, 8, 7};

    // make canvases
    TCanvas *cEtaPos = new TCanvas("cEtaPos", "cEtaPos", 800, 600);
    TCanvas *cEtaNeg = new TCanvas("cEtaNeg", "cEtaNeg", 800, 600);
    TCanvas *cEtaRatio = new TCanvas("cEtaRatio", "cEtaRatio", 800, 600);
    TCanvas *cEtaSpecRatio = new TCanvas("cEtaMuPTop/MuMBottom", "cEtaMuPTop/MuMBottom", 800, 600); // mu+Top/mu-Bottom
    TCanvas *cEtaSpecRatioInv = new TCanvas("cEtaMuMTop/MuPBottom", "cEtaMuMTop/MuPBottom", 800, 600); // mu-Top/mu+Bottom
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
    TH1F *hTemplateEtaPos = new TH1F("hTemplateEtaPos", Form("#eta for #mu^{+}: %s", labelName), 80, -5, -1);
    TH1F *hTemplateEtaNeg = new TH1F("hTemplateEtaNeg", Form("#eta for #mu^{-}: %s", labelName), 80, -5, -1);
    TH1F *hTemplateEtaRatio = new TH1F("hTemplateEtaRatio", Form("#eta for #mu^{+}/#mu^{-}: %s", labelName), 80, -5, -1);
    TH1F *hTemplateEtaSpecRatio = new TH1F("hTemplateEtaSpecRatio", Form("#eta for #mu^{+}Top/#mu^{-}Bottom: %s", labelName), 80, -5, -1);
    TH1F *hTemplateEtaSpecRatioInv = new TH1F("hTemplateEtaSpecRatioInv", Form("#eta for #mu^{-}Top/#mu^{+}Bottom: %s", labelName), 80, -5, -1);
    TH1F *hTemplatePtPos = new TH1F("hTemplatePtPos", Form("p_{T} for #mu^{+}: %s", labelName), 100, 0, 10);
    TH1F *hTemplatePtNeg = new TH1F("hTemplatePtNeg", Form("p_{T} for #mu^{-}: %s", labelName), 100, 0, 10);
    TH1F *hTemplatePtRatio = new TH1F("hTemplatePtRatio", Form("p_{T} for #mu^{+}/#mu^{-}: %s", labelName), 100, 0, 10);
    TH1F *hTemplatePtSpecRatio = new TH1F("hTemplatePtSpecRatio", Form("p_{T} for #mu^{+}Top/#mu^{-}Bottom: %s", labelName), 100, 0, 10);
    TH1F *hTemplatePtSpecRatioInv = new TH1F("hTemplatePtSpecRatioInv", Form("p_{T} for #mu^{-}Top/#mu^{+}Bottom: %s", labelName), 100, 0, 10);
    // set max y-axis range
    double margin = 1.1;
    hTemplateEtaPos->SetMaximum(margin * maxEtaPos);
    hTemplateEtaNeg->SetMaximum(margin * maxEtaNeg);
    hTemplateEtaRatio->SetMinimum(1 - 1), hTemplateEtaRatio->SetMaximum(1 + 1);
    hTemplateEtaSpecRatio->SetMinimum(1 - 1), hTemplateEtaSpecRatio->SetMaximum(1 + 5);
    hTemplateEtaSpecRatioInv->SetMinimum(1 - 1), hTemplateEtaSpecRatioInv->SetMaximum(1 + 5);
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
    cEtaSpecRatio->cd();
    hTemplateEtaSpecRatio->SetStats(0);
    hTemplateEtaSpecRatio->Draw();
    cEtaSpecRatioInv->cd();
    hTemplateEtaSpecRatioInv->SetStats(0);
    hTemplateEtaSpecRatioInv->Draw();
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

    for (Int_t i = 0; i < vTreeNamesEtaPos.size(); i++)
    {
        const char *fNameEtaPos = vTreeNamesEtaPos[i];
        const char *fNameEtaNeg = vTreeNamesEtaNeg[i];
        const char *fNamePtPos = vTreeNamesPtPos[i];
        const char *fNamePtNeg = vTreeNamesPtNeg[i];
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
        l->AddEntry(hEtaPos, vLegendEntries[i], "l");
    }

    cEtaPos->cd();
    l->Draw();
    cEtaNeg->cd();
    l->Draw();
    cEtaRatio->cd();
    l->Draw();
    cEtaSpecRatio->cd();
    lSpecRatio->Draw();
    cEtaSpecRatioInv->cd();
    lSpecRatio->Draw();
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
    std::cout << "opening " << vTreeNamesEtaPos[0] << " and " << vHistNamesEtaPos[1] << std::endl;
    hEtaPosTopRef = (TH1 *)hEtaPosTopRef->Clone("hEtaPosTopRef_clone");
    TH1 *hEtaNegBottomRef = GetTH1(vTreeNamesEtaNeg[0], vHistNamesEtaNeg[2]);
    std::cout << "opening " << vTreeNamesEtaNeg[0] << " and " << vHistNamesEtaNeg[2] << std::endl;
    hEtaNegBottomRef = (TH1 *)hEtaNegBottomRef->Clone("hEtaNegBottomRef_clone");
    TH1 *hEtaSpecRatioRef = (TH1 *)hEtaPosTopRef->Clone("hEtaSpecRatioRef_clone");

    TH1 *hEtaPosTop = GetTH1(vTreeNamesEtaPos[3], vHistNamesEtaPos[4]);
    std::cout << "opening " << vTreeNamesEtaPos[3] << " and " << vHistNamesEtaPos[4] << std::endl;
    hEtaPosTop = (TH1 *)hEtaPosTop->Clone("hEtaPosTop_clone");
    TH1 *hEtaNegBottom = GetTH1(vTreeNamesEtaNeg[3], vHistNamesEtaNeg[5]);
    std::cout << "opening " << vTreeNamesEtaNeg[3] << " and " << vHistNamesEtaNeg[5] << std::endl;
    hEtaNegBottom = (TH1 *)hEtaNegBottom->Clone("hEtaNegBottom_clone");
    TH1 *hEtaSpecRatio = (TH1 *)hEtaPosTop->Clone("hEtaSpecRatio_clone");

    hEtaSpecRatioRef->Divide(hEtaNegBottomRef);
    hEtaSpecRatio->Divide(hEtaNegBottom);

    // hard-coded mu-Top/mu+Bottom (inv)
    TH1 *hEtaNegTopInvRef = GetTH1(vTreeNamesEtaNeg[0], vHistNamesEtaNeg[1]);
    std::cout << "opening " << vTreeNamesEtaNeg[0] << " and " << vHistNamesEtaNeg[1] << std::endl;
    hEtaNegTopInvRef = (TH1 *)hEtaNegTopInvRef->Clone("hEtaNegTopInvRef_clone");
    TH1 *hEtaPosBottomInvRef = GetTH1(vTreeNamesEtaPos[0], vHistNamesEtaPos[2]);
    std::cout << "opening " << vTreeNamesEtaPos[0] << " and " << vHistNamesEtaPos[2] << std::endl;
    hEtaPosBottomInvRef = (TH1 *)hEtaPosBottomInvRef->Clone("hEtaPosBottomInvRef_clone");
    TH1 *hEtaSpecRatioInvRef = (TH1 *)hEtaNegTopInvRef->Clone("hEtaSpecRatioInvRef_clone");

    TH1 *hEtaNegTopInv = GetTH1(vTreeNamesEtaNeg[3], vHistNamesEtaNeg[4]);
    std::cout << "opening " << vTreeNamesEtaNeg[3] << " and " << vHistNamesEtaNeg[4] << std::endl;
    hEtaNegTopInv = (TH1 *)hEtaNegTopInv->Clone("hEtaNegTopInv_clone");
    TH1 *hEtaPosBottomInv = GetTH1(vTreeNamesEtaPos[3], vHistNamesEtaPos[5]);
    std::cout << "opening " << vTreeNamesEtaPos[3] << " and " << vHistNamesEtaPos[5] << std::endl;
    hEtaPosBottomInv = (TH1 *)hEtaPosBottomInv->Clone("hEtaPosBottomInv_clone");
    TH1 *hEtaSpecRatioInv = (TH1 *)hEtaNegTopInv->Clone("hEtaSpecRatioInv_clone");

    hEtaSpecRatioInvRef->Divide(hEtaPosBottomInvRef);
    hEtaSpecRatioInv->Divide(hEtaPosBottomInv);

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
    cEtaSpecRatioInv->cd();
    hEtaSpecRatioInv->SetLineColor(kRed);
    hEtaSpecRatioInvRef->SetLineColor(kBlack);
    hEtaSpecRatioInvRef->Draw("SAME HIST");
    hEtaSpecRatioInv->Draw("SAME HIST");
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

    // cEtaSpecRatio->cd();
    // lSpecRatio->Draw();
    // cPtSpecRatio->cd();
    // lSpecRatio->Draw();

    cEtaPos->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf(", fAnalysisResults, labelName));
    cEtaNeg->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResults, labelName));
    cEtaRatio->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResults, labelName));
    cEtaSpecRatio->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResults, labelName));
    cEtaSpecRatioInv->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResults, labelName));
    cPtPos->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResults, labelName));
    cPtNeg->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResults, labelName));
    cPtRatio->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResults, labelName));
    cPtSpecRatio->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResults, labelName));
    cPtSpecRatioInv->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf)", fAnalysisResults, labelName));

    fOutput->cd();
    cEtaPos->Write();
    cEtaNeg->Write();
    cEtaRatio->Write();
    cEtaSpecRatio->Write();
    cEtaSpecRatioInv->Write();
    cPtPos->Write();
    cPtNeg->Write();
    cPtRatio->Write();
    cPtSpecRatio->Write();
    cPtSpecRatioInv->Write();
    fOutput->Close();
}

int muonQA_pT_eta_single_muons()
{
    makePlots();
    return 0;
}