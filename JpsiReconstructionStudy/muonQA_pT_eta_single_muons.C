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
    std::cout << "Reading " << histName.c_str() << " from TFile" << std::endl;
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
    std::cout << "Reading " << histName.c_str() << " from TFile" << std::endl;
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
    fAnalysisResults = "AnalysisResults_test.root";
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

    labelName = "LHC24an_pass1_skimmed_no_realignment_top-bottom";
    std::vector<const char *> vLegendEntries = {"integrated", "top", "bottom"};
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
    labelName = "LHC24an_pass1_skimmed_no_realignment_left-right";
    std::vector<const char*> vLegendEntries = {"integrated", "left", "right"};
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

    /*
    labelName = "GeometryAlignedFix110Fix15New2T5_top-bottom";
    fMuonId = "muon-qa_id30992/muons/";
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
    fMuonId = "muon-qa_id30619/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
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
    fAnalysisResults = "AnalysisResults_LHC24an_pass1_skimmed_Ch1BhalfShift_Hyperloop_24_06_2025.root";
    labelName = "GeometryAlignedFix10Fix15Shift04Ch1BNew2_top-bottom";
    // fMuonId = "muon-qa/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    fMuonId = "muon-qa/muons/";
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
    fAnalysisResults = "AnalysisResults_LHC24an_pass1_skimmed_Ch1BhalfShift_Hyperloop_24_06_2025.root";
    labelName = "GeometryAlignedFix10Fix15Shift04Ch1BNew2_left-right";
    fMuonId = "muon-qa/muons/";
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

   TFile *fOutput = new TFile(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.root", fAnalysisResults, labelName), "RECREATE");

    std::vector<Int_t> vLineStyles = {1, 2, 6, 1, 1, 1}; // in order to show reference everywhere..
    std::vector<Int_t> vLineColours = {1, 1, 1, 2, 8, 7};

    // make canvases
    TCanvas *cEtaPos = new TCanvas("cEtaPos", "cEtaPos", 800, 600);
    TCanvas *cEtaNeg = new TCanvas("cEtaNeg", "cEtaNeg", 800, 600);
    TCanvas *cPtPos = new TCanvas("cPtPos", "cPtPos", 800, 600);
    TCanvas *cPtNeg = new TCanvas("cPtNeg", "cPtNeg", 800, 600);

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
        maxPtPos  = std::max(maxPtPos,  hPtPos->GetMaximum());
        maxPtNeg  = std::max(maxPtNeg,  hPtNeg->GetMaximum());
    }

    // make templates
    TH1F *hTemplateEtaPos = new TH1F("hTemplateEtaPos", Form("#eta for #mu^{+}: %s", labelName), 80, -5, -1);
    TH1F *hTemplateEtaNeg = new TH1F("hTemplateEtaNeg", Form("#eta for #mu^{-}: %s", labelName), 80, -5, -1);
    TH1F *hTemplatePtPos  = new TH1F("hTemplatePtPos",  Form("p_{T} for #mu^{+}: %s", labelName), 100, 0, 10);
    TH1F *hTemplatePtNeg  = new TH1F("hTemplatePtNeg",  Form("p_{T} for #mu^{-}: %s", labelName), 100, 0, 10);
    // set max y-axis range
    double margin = 1.1;
    hTemplateEtaPos->SetMaximum(margin * maxEtaPos);
    hTemplateEtaNeg->SetMaximum(margin * maxEtaNeg);
    hTemplatePtPos->SetMaximum(margin * maxPtPos);
    hTemplatePtNeg->SetMaximum(margin * maxPtNeg);

    cEtaPos->cd();
    hTemplateEtaPos->SetStats(0);
    hTemplateEtaPos->Draw();
    cEtaNeg->cd();
    hTemplateEtaNeg->SetStats(0);
    hTemplateEtaNeg->Draw();
    cPtPos->cd();
    hTemplatePtPos->SetStats(0);
    hTemplatePtPos->Draw();
    cPtNeg->cd();
    hTemplatePtNeg->SetStats(0);
    hTemplatePtNeg->Draw();

    // make legends
    TLegend *l = new TLegend(0.15, 0.65, 0.35, 0.85);
    l->SetBorderSize(0);
    l->SetTextSize(0.02);

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
        hPtNeg = (TH1*)hEtaPos->Clone(Form("hEtaPos_clone_%d", i));
        TH1 *hEtaNeg = GetTH1(fNameEtaNeg, hNameEtaNeg);
        hEtaNeg = (TH1*)hEtaNeg->Clone(Form("hEtaNeg_clone_%d", i));
        TH1 *hPtPos = GetTH1FromTH2(fNamePtPos, hNamePtPos, -4, 4);
        hPtPos = (TH1*)hPtPos->Clone(Form("hPtPos_clone_%d", i));
        TH1 *hPtNeg = GetTH1FromTH2(fNamePtNeg, hNamePtNeg, -4, 4);
        hPtNeg = (TH1*)hPtNeg->Clone(Form("hPtNeg_clone_%d", i));
        hEtaPos->SetLineStyle(vLineStyles[i]);
        hEtaNeg->SetLineStyle(vLineStyles[i]);
        hPtPos->SetLineStyle(vLineStyles[i]);
        hPtNeg->SetLineStyle(vLineStyles[i]);
        hEtaPos->SetLineColor(vLineColours[i]);
        hEtaNeg->SetLineColor(vLineColours[i]);
        hPtPos->SetLineColor(vLineColours[i]);
        hPtNeg->SetLineColor(vLineColours[i]);
        cEtaPos->cd();
        hEtaPos->Draw("SAME");
        cEtaNeg->cd();
        hEtaNeg->Draw("SAME");
        cPtPos->cd();
        hPtPos->Draw("SAME");
        cPtNeg->cd();
        hPtNeg->Draw("SAME");
        l->AddEntry(hEtaPos, vLegendEntries[i], "l");
    }

    cEtaPos->cd();
    l->Draw();
    cEtaNeg->cd();
    l->Draw();
    cPtPos->cd();
    l->Draw();
    cPtNeg->cd();
    l->Draw();

    cEtaPos->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf(", fAnalysisResults, labelName));
    cEtaNeg->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResults, labelName));
    cPtPos->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf", fAnalysisResults, labelName));
    cPtNeg->SaveAs(Form("Plots_MCH_Quadrants/singleMuonKinematics_%s_%s.pdf)", fAnalysisResults, labelName));

    fOutput->cd();
    cEtaPos->Write();
    cEtaNeg->Write();
    cPtPos->Write();
    cPtNeg->Write();
    fOutput->Close();
}

int muonQA_pT_eta_single_muons()
{
    makePlots();
    return 0;
}