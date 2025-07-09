// DiMuonMassSpectrum.C
// Created: 07 NOV 2024
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

#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHist.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "RooChi2Var.h"
#include "RooGaussian.h"
#include "RooFormulaVar.h"
#include "RooCrystalBall.h"
#include "RooChebychev.h"
#include "RooFitResult.h"
#include "RooMsgService.h"
#include "RooWorkspace.h"

using namespace RooFit;

void Print(const char *name, Double_t value)
{
    std::cout << Form("%s = ", name) << value << std::endl;
}

Double_t calculateSigOverBkgRatio(RooRealVar *observable, TH1 *data, RooAbsPdf *SIG_model, RooAbsPdf *BKG_model, RooAbsPdf *full_model,
                                  RooRealVar *sigYield, RooRealVar *bkgYield, const std::string &meanName, const std::string &sigmaName)
{

    RooArgSet *params = SIG_model->getParameters((RooArgSet *)0);
    RooRealVar *mean = dynamic_cast<RooRealVar *>(params->find(meanName.c_str()));
    RooRealVar *sigma = dynamic_cast<RooRealVar *>(params->find(sigmaName.c_str()));

    // Integrate over mean +/- 3 sigma range
    double rangeMin = mean->getVal() - 3 * sigma->getVal();
    double rangeMax = mean->getVal() + 3 * sigma->getVal();
    observable->setRange("signalRange", rangeMin, rangeMax);

    /*
    // This method was implemented for bug fixing
    // The method agrees with the other (old) one as long as RooFit::NormSet is used
    // (i.e. it is proof that it is required)
    Double_t binCenter = data->GetBinCenter(data->FindBin(mean->getVal()));
    Double_t binLowEdge = data->GetBinLowEdge(data->FindBin(rangeMin));
    Double_t binHighEdge = data->GetBinLowEdge(data->FindBin(rangeMax)) + data->GetBinWidth(data->FindBin(mean->getVal()));
    observable->setVal(binCenter);          // invariant mass
    observable->setRange("binRange", binLowEdge, binHighEdge);
    std::cout<<"binLowEdge = "<<binLowEdge<<std::endl;
    std::cout<<"binHighEdge = "<<binHighEdge<<std::endl;
    Double_t sigIntegral = SIG_model->createIntegral(*observable, RooFit::NormSet(*observable),
                                                    RooFit::Range("binRange"))->getVal();
    Double_t bkgIntegral = BKG_model->createIntegral(*observable, RooFit::NormSet(*observable),
                                                    RooFit::Range("binRange"))->getVal();
    Double_t fullIntegral = full_model->createIntegral(*observable, RooFit::NormSet(*observable),
                                                    RooFit::Range("binRange"))->getVal();
    */

    Double_t sigIntegral = SIG_model->createIntegral(
                                        RooArgSet(*observable),
                                        NormSet(*observable),
                                        Range("signalRange"))
                               ->getVal();
    Double_t bkgIntegral = BKG_model->createIntegral(
                                        RooArgSet(*observable),
                                        NormSet(*observable),
                                        Range("signalRange"))
                               ->getVal();
    Double_t fullIntegral = full_model->createIntegral(
                                          RooArgSet(*observable),
                                          NormSet(*observable),
                                          Range("signalRange"))
                                ->getVal();

    Print("mean = ", mean->getVal());
    Print("sigIntegral = ", sigIntegral);
    Print("bkgIntegral = ", bkgIntegral);
    Print("fullIntegral = ", fullIntegral);
    Print("sigYield = ", sigYield->getVal());
    Print("bkgYield = ", bkgYield->getVal());

    // brackets are absolutely necessary!!
    std::cout << "s/b from function = " << (sigYield->getVal() * sigIntegral) / (bkgYield->getVal() * bkgIntegral) << std::endl;
    return (sigYield->getVal() * sigIntegral) / (bkgYield->getVal() * bkgIntegral);
}

Double_t calculateSignificance(RooRealVar *observable, RooAbsPdf *SIG_model, RooAbsPdf *BKG_model, RooAbsPdf *full_model,
                               RooRealVar *sigYield, RooRealVar *bkgYield, const std::string &meanName, const std::string &sigmaName)
{

    RooArgSet *params = SIG_model->getParameters((RooArgSet *)0);
    RooRealVar *mean = dynamic_cast<RooRealVar *>(params->find(meanName.c_str()));
    RooRealVar *sigma = dynamic_cast<RooRealVar *>(params->find(sigmaName.c_str()));

    // Integrate over mean +/- 3 sigma range
    double rangeMin = mean->getVal() - 3 * sigma->getVal();
    double rangeMax = mean->getVal() + 3 * sigma->getVal();
    // double rangeFullMin = observable->getMin();
    // double rangeFullMax = observable->getMax();
    std::cout << "integration range = [" << rangeMin << "," << rangeMax << "]" << std::endl;
    observable->setRange("signalRange", rangeMin, rangeMax);
    // observable->setRange("fullRange",rangeFullMin,rangeFullMax);

    // Should it be normalised to the whole 'model' in the range?
    Double_t sigIntegral = SIG_model->createIntegral(
                                        RooArgSet(*observable),
                                        NormSet(*observable),
                                        Range("signalRange"))
                               ->getVal();
    Double_t bkgIntegral = BKG_model->createIntegral(
                                        RooArgSet(*observable),
                                        NormSet(*observable),
                                        Range("signalRange"))
                               ->getVal();
    Double_t fullIntegral = full_model->createIntegral(
                                          RooArgSet(*observable),
                                          NormSet(*observable),
                                          Range("signalRange"))
                                ->getVal();
    Print("sigIntegralSignificance = ", sigIntegral);
    Print("bkgIntegralSignificance = ", bkgIntegral);
    Print("fullIntegral = ", fullIntegral);
    Print("sigYieldSignificance = ", sigYield->getVal());
    Print("bkgYieldSignificance = ", bkgYield->getVal());

    std::cout << "significance from function = " << (sigYield->getVal() * sigIntegral) / (sqrt((sigYield->getVal() * sigIntegral) + (bkgYield->getVal() * bkgIntegral))) << std::endl;

    return (sigYield->getVal() * sigIntegral) /
           (sqrt((sigYield->getVal() * sigIntegral) + (bkgYield->getVal() * bkgIntegral)));
}

Double_t calculateChi2(RooRealVar *observable, TH1 *data, RooAbsPdf *model,
                       RooRealVar *sigYield, RooRealVar *bkgYield)
{

    Double_t mMin = observable->getMin();
    Double_t mMax = observable->getMax();
    data->GetXaxis()->SetRangeUser(mMin, mMax);

    // Number of bins in the input histogram
    // Int_t nBins = data->GetNbinsX();
    Int_t nBinsOld = data->GetNbinsX();
    Double_t xMin = data->GetXaxis()->GetXmin();
    Double_t xMax = data->GetXaxis()->GetXmax();
    Double_t binWidth = (xMax - xMin) / nBinsOld;
    Int_t nBins = static_cast<Int_t>((mMax - mMin) / binWidth);

    // Calculate chi2 by hand
    Double_t chi2 = -1.;
    // Loop over the bins of the data
    for (Int_t i = data->FindBin(mMin); i <= data->FindBin(mMax); ++i)
    {
        Double_t dataValue = data->GetBinContent(i);
        Double_t dataError = data->GetBinError(i);
        Double_t binCenter = data->GetBinCenter(i);
        Double_t binLowEdge = data->GetBinLowEdge(i);
        Double_t binHighEdge = binLowEdge + data->GetBinWidth(i);

        // Integrate the model over the bin range
        observable->setVal(binCenter); // invariant mass
        observable->setRange("binRange", binLowEdge, binHighEdge);
        Double_t modelValue = model->createIntegral(*observable, RooFit::NormSet(*observable),
                                                    RooFit::Range("binRange"))
                                  ->getVal();
        // Multiply model value by combined yield to equalise to data values
        modelValue *= sigYield->getVal() + bkgYield->getVal();
        // Print("modelValueChi2",modelValue);
        // Print("dataValueChi2",dataValue);

        // Avoid division by zero and compute chi-squared
        if (dataError != 0)
        {
            chi2 += std::pow((dataValue - modelValue), 2) / dataValue;
        }
        else
        {
            std::cerr << "Warning: Zero error in bin " << i << ", skipping." << std::endl;
        }
    }

    // Return the calculated chi-squared value
    // for debugging print statement
    Int_t ndf = (data->FindBin(mMax) - data->FindBin(mMin)) - 8;
    std::cout << "chi2/ndf by hand = " << chi2 / ndf << std::endl;
    return chi2;
}

TGraphErrors *calculatePullHist(RooRealVar *observable, TH1 *data, RooAbsPdf *model,
                                RooRealVar *sigYield, RooRealVar *bkgYield)
{

    Double_t mMin = observable->getMin();
    Double_t mMax = observable->getMax();
    data->GetXaxis()->SetRangeUser(mMin, mMax);
    std::cout << "pullHist range = " << "[" << mMin << "," << mMax << "]" << std::endl;

    // Number of bins in the input histogram
    // Int_t nBins = data->GetNbinsX();
    Int_t nBinsOld = data->GetNbinsX();
    Double_t xMin = data->GetXaxis()->GetXmin();
    Double_t xMax = data->GetXaxis()->GetXmax();
    Double_t binWidth = (xMax - xMin) / nBinsOld;
    Int_t nBins = static_cast<Int_t>((mMax - mMin) / binWidth);

    // data->Scale(1.0 / data->Integral(data->FindBin(mMin),data->FindBin(mMax)));

    std::vector<Double_t> xVals;
    std::vector<Double_t> yVals;
    std::vector<Double_t> xErrors(data->GetNbinsX(), 0);
    std::vector<Double_t> yErrors(data->GetNbinsX(), 0);

    for (Int_t i = data->FindBin(mMin); i <= data->FindBin(mMax); ++i)
    {
        Double_t dataValue = data->GetBinContent(i);
        Double_t dataError = data->GetBinError(i);
        Double_t binCenter = data->GetBinCenter(i);
        Double_t binLowEdge = data->GetBinLowEdge(i);
        Double_t binHighEdge = binLowEdge + data->GetBinWidth(i);

        // Integrate the model over the bin range
        observable->setVal(binCenter); // invariant mass
        observable->setRange("binRange", binLowEdge, binHighEdge);
        Double_t modelValue = model->createIntegral(*observable, RooFit::NormSet(*observable),
                                                    RooFit::Range("binRange"))
                                  ->getVal();
        // Multiply model value by combined yield to equalise to data values
        modelValue *= sigYield->getVal() + bkgYield->getVal();
        // Print("modelValueChi2",modelValue);
        // Print("dataValueChi2",dataValue);
        // Print("modelValue",modelValue);
        // Print("dataValue",dataValue);

        if (dataError != 0)
        {
            // Compute the pull value and store
            Double_t pull = (dataValue - modelValue) / dataError;
            xVals.push_back(binCenter);
            yVals.push_back(pull);
            yErrors.push_back(0);
        }
        else
        {
            std::cerr << "Warning: Zero error in bin " << i << ", skipping." << std::endl;
        }
    }

    // Create a TGraphErrors object for the pull plot
    auto *pullPlot = new TGraphErrors(xVals.size(), xVals.data(), yVals.data(), xErrors.data(), yErrors.data());
    pullPlot->SetTitle("Pull Plot;Observable;Pull Value (data - model) / sigma");
    pullPlot->SetMarkerStyle(20); // Optional: Style of markers
    pullPlot->SetMarkerSize(0.8); // Optional: Size of markers

    Print("mMin", mMin);
    Print("mMax", mMax);
    Print("nBinsOld", nBinsOld);
    Print("nBins", nBins);
    return pullPlot;
}

TH1D *getTree(const char *fileName, Double_t ptMin, Double_t ptMax);
TH1 *GetTH1(const char* fileName, std::string histName);
TH1 *GetTH1FromTH2(const char* fileName, std::string histName, Double_t ptMin, Double_t ptMax);
void defSigModel(RooWorkspace &ws);
void defBkgModel(RooWorkspace &ws, std::string BKG_model, Double_t ptMin, Double_t ptMax);
void defCombinedModel(RooWorkspace &ws, Double_t ptMin, Double_t ptMax);
void fitModelToData(RooWorkspace &ws, TH1 *hist, std::string BKG_model, Double_t ptMin, Double_t ptMax);
void drawPlots(RooWorkspace &ws, TH1 *hist, const char *treeName, const char *realignmentLabel, Double_t ptMin, Double_t ptMax);

struct JpsiValues
{
    double JpsiWidth;
    double JpsiPeak;
    double JpsiWidthError;
    double JpsiPeakError;
};

// run macro with root -l 'DiMuonMassSpectrum.C(Double_t ptMin, Double_t ptMax)'
JpsiValues CalculateJpsiWidth(const char* fileName, std::string histName, const char *realignmentLabel, Double_t ptMin, Double_t ptMax)
{

    // From tutorial to make output less crowded: why doesn't it work?
    RooMsgService::instance().getStream(1).removeTopic(Minimization);
    RooMsgService::instance().getStream(1).removeTopic(Fitting);
    RooMsgService::instance().getStream(1).removeTopic(Generation);

    RooWorkspace wspace{"myWS"};

    // TH1D *hist = getTree(treeName, ptMin, ptMax);
    std::cout << "Reading fileName = " << fileName << " and histName = " << histName << " and realignmentLabel = " << realignmentLabel << std::endl;
    TH1 *hist = GetTH1FromTH2(fileName, histName, ptMin, ptMax);

    // For Upsilon: 8, 12
    Double_t mMin = 2.5;
    Double_t mMax = 4.5;
    RooRealVar *m = new RooRealVar("m", "M_{#mu^{+}#mu^{-}}", mMin, mMax, "GeV/c2");
    wspace.import(*m);

    defSigModel(wspace);
    defBkgModel(wspace, "Chebychev", ptMin, ptMax);
    defCombinedModel(wspace, ptMin, ptMax);
    fitModelToData(wspace, hist, "Chebychev", ptMin, ptMax);
    drawPlots(wspace, hist, fileName, realignmentLabel, ptMin, ptMax);

    JpsiValues JpsiValues;
    double JpsiWidth = wspace.var("sigma")->getVal();
    double JpsiPeak = wspace.var("m0")->getVal();
    double JpsiWidthError = wspace.var("sigma")->getError();
    double JpsiPeakError = wspace.var("m0")->getError();
    JpsiValues.JpsiWidth = JpsiWidth;
    JpsiValues.JpsiPeak = JpsiPeak;
    JpsiValues.JpsiWidthError = JpsiWidthError;
    JpsiValues.JpsiPeakError = JpsiPeakError;
    return JpsiValues;

} // int CalculateJpsiWidth()

// Function is necessary to draw the pT dependence of the Jpsi widths
int DiMuonMassSpectrum_muonQA()
{
    std::vector<const char*> vTreeNames;
    // std::vector<const char*> vMuonIds; // trains are combined in Hyperloop with different configurations in the same output file
    std::vector<std::string> vHistNames;
    const char *labelName; // saves PDF with widhts and peaks using this label name
    const char *fAnalysisResults;
    std::string fMuonId; // trains are combined in Hyperloop with different configurations in the same output file

    // fAnalysisResults = "AnalysisResults-muonQA_LHC24an_pass1_skimmed_small_muonID-30697_no_realignment_muonID-30255_javier_new2_muon_ID-30698_globalShiftY_Hyperloop_17_06_2025.root";
    // fAnalysisResults = "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_6_geometries.root";
    fAnalysisResults = "AnalysisResults-muonQA_LHC24an_pass1_skimmed_Hyperloop_07_07_2025.root";
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
    // std::vector<const char*> vLegendEntries = {"integrated", "top-top","top-bottom/bottom-top", "bottom-bottom"};
    fMuonId = "muon-qa_id30697/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    /*
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TT");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_BB");
    */
    /*
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TPBN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TNBP");
    */

    // Left-Right
    /*
    labelName = "LHC24an_pass1_skimmed_no_realignment_left-right";
    // std::vector<const char*> vLegendEntries = {"integrated", "left-left","left-right/right-left", "right-right"};
    fMuonId = "muon-qa_id30697/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LL");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_RR");
    */
    /*
    // std::vector<const char*> vLegendEntries = {"reference LR or RL", "reference L+R-", "reference L-R+", "new geometry LR or RL", "new geometry L+R-", "new geometry L-R+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LPRN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LNRP");
    */

   // ======================[ GeometryAligned: Reference 2025 (no realignment) ]=====================
    // Top-Bottom
    
    /*
    labelName = "LHC25ac_vs_LHC24an_no_realignment_pT_integrated_top-bottom";
    std::vector<const char*> vLegendEntries = {"reference 2024 int", "reference 2024 TT","reference TB or BT 2024", "reference BB 2024", "reference 2025 int", "reference 2025 TT","reference 2025 TB or BT", "reference 2025 BB"};
    fAnalysisResults = "AnalysisResults_LHC25ac_muon_qa_no_realignment_Hyperloop_24_06_2025.root";
    fMuonId = "muon-qa/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TT");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_BB");
    */
    /*
    std::vector<const char*> vLegendEntries = {"reference TB or BT", "reference T+B-", "reference T-B+", "new geometry TB or BT", "new geometry T+B-", "new geometry T-B+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TPBN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TNBP");
    */

    // Left-Right
    /*
    labelName = "LHC25ac_vs_LHC24an_no_realignment_pT_integrated_left-right";
    std::vector<const char*> vLegendEntries = {"reference 2024 int", "reference 2024 LL","reference 2024 LR or RL", "reference 2024 RR", "reference 2025 int", "reference 2025 LL","reference LR or RL 2025", "reference RR 2025"};
    fAnalysisResults = "AnalysisResults_LHC25ac_muon_qa_no_realignment_Hyperloop_24_06_2025.root";
    fMuonId = "muon-qa/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LL");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_RR");
    */
    /*
    std::vector<const char*> vLegendEntries = {"reference LR or RL", "reference L+R-", "reference L-R+", "new geometry LR or RL", "new geometry L+R-", "new geometry L-R+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LPRN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LNRP");
    */

    // ======================[ (***) GeometryAlignedFix110Fix15New2T5: Javier new2 ]=====================
    // Top-Bottom
    
    /*
    labelName = "GeometryAlignedFix110Fix15New2T5_top-bottom";
    fMuonId = "muon-qa_id30992/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference TT","reference TB or BT", "reference BB", "new geometry int", "new geometry TT","new geometry TB or BT","new geometry BB"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TT");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_BB");
    */
    /*
    labelName = "GeometryAlignedFix110Fix15New2T5_top-bottom";
    fMuonId = "muon-qa_id30992/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference TB or BT", "reference T+B-", "reference T-B+", "new geometry TB or BT", "new geometry T+B-", "new geometry T-B+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TPBN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TNBP");
    */

    // Left-Right

    /*
    labelName = "GeometryAlignedFix110Fix15New2T5_left-right";
    fMuonId = "muon-qa_id30992/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference LL","reference LR or RR", "reference RR", "new geometry int", "new geometry LL","new geometry LR or RL","new geometry RR"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LL");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_RR");
    */
    /*
    std::vector<const char*> vLegendEntries = {"reference LR or RL", "reference L+R-", "reference L-R+", "new geometry LR or RL", "new geometry L+R-", "new geometry L-R+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LPRN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LNRP");
    */

    // ======================[ GeometryAlignedShiftY: globalShiftY ]=====================
    // Top-Bottom
    /*
    labelName = "GeometryAlignedShiftY_top-bottom";
    fMuonId = "muon-qa_id30991/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference TT","reference TB or BT", "reference BB", "new geometry int", "new geometry TT","new geometry TB or BT","new geometry BB"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TT");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_BB");
    */
    /*
    std::vector<const char*> vLegendEntries = {"reference TB or BT", "reference T+B-", "reference T-B+", "new geometry TB or BT", "new geometry T+B-", "new geometry T-B+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TPBN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TNBP");
    */

    // Left-Right

    /*
    labelName = "GeometryAlignedShiftY_left-right";
    fMuonId = "muon-qa_id30991/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference LL","reference LR or RR", "reference RR", "new geometry int", "new geometry LL","new geometry LR or RL","new geometry RR"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LL");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_RR");
    */
    /*
    std::vector<const char*> vLegendEntries = {"reference LR or RL", "reference L+R-", "reference L-R+", "new geometry LR or RL", "new geometry L+R-", "new geometry L-R+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LPRN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LNRP");
    */

   // ======================[ (***) GeometryAlignedFix110Fix15New2T5ShiftY: globalShiftY + javier new2 ]=====================
    // Top-Bottom
    /*
    labelName = "GeometryAlignedFix110Fix15New2T5ShiftY_top-bottom";
    fMuonId = "muon-qa_id30993/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference TT","reference TB or BT", "reference BB", "new geometry int", "new geometry TT","new geometry TB or BT","new geometry BB"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TT");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_BB");
    */
    /*
    labelName = "GeometryAlignedFix110Fix15New2T5ShiftY_top-bottom";
    fMuonId = "muon-qa_id30993/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference TB or BT", "reference T+B-", "reference T-B+", "new geometry TB or BT", "new geometry T+B-", "new geometry T-B+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TPBN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TNBP");
    */

    // Left-Right

    /*
    labelName = "GeometryAlignedFix110Fix15New2T5ShiftY_left-right";
    fMuonId = "muon-qa_id30993/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference LL","reference LR or RR", "reference RR", "new geometry int", "new geometry LL","new geometry LR or RL","new geometry RR"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LL");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_RR");
    */
    /*
    std::vector<const char*> vLegendEntries = {"reference LR or RL", "reference L+R-", "reference L-R+", "new geometry LR or RL", "new geometry L+R-", "new geometry L-R+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LPRN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LNRP");
    */

    // ======================[ (***) GeometryAlignedFix10Fix15ShiftCh1BNew2: bottom only CH1 shift ]=====================
    // Top-Bottom
    
    /*
    labelName = "GeometryAlignedFix10Fix15ShiftCh1BNew2_top-bottom";
    fMuonId = "muon-qa_id30619/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference TT","reference TB or BT", "reference BB", "new geometry int", "new geometry TT","new geometry TB or BT","new geometry BB"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TT");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_BB");
    */
    /*
    labelName = "GeometryAlignedFix10Fix15ShiftCh1BNew2_top-bottom";
    fMuonId = "muon-qa_id30619/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference TB or BT", "reference T+B-", "reference T-B+", "new geometry TB or BT", "new geometry T+B-", "new geometry T-B+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TPBN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TNBP");
    */

    // Left-Right

    /*
    labelName = "GeometryAlignedFix10Fix15ShiftCh1BNew2_left-right";
    fMuonId = "muon-qa_id30619/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference LL","reference LR or RR", "reference RR", "new geometry int", "new geometry LL","new geometry LR or RL","new geometry RR"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LL");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_RR");
    */
    /*
    std::vector<const char*> vLegendEntries = {"reference LR or RL", "reference L+R-", "reference L-R+", "new geometry LR or RL", "new geometry L+R-", "new geometry L-R+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LPRN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LNRP");
    */

    // ======================[ (***) GeometryAlignedFix10Fix15Shift04Ch1BNew2: bottom only CH1 shift with half the amount ]=====================
    // Top-Bottom

    
    // fAnalysisResults = "AnalysisResults_LHC24an_pass1_skimmed_Ch1BhalfShift_Hyperloop_24_06_2025.root";
    // labelName = "GeometryAlignedFix10Fix15Shift04Ch1BNew2_top-bottom";
    // fMuonId = "muon-qa/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    // fMuonId = "muon-qa_id31157/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    /*
    std::vector<const char*> vLegendEntries = {"reference int", "reference TT","reference TB or BT", "reference BB", "new geometry int", "new geometry TT","new geometry TB or BT", "new geometry BB"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TT");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_BB");
    */
    /*
    labelName = "GeometryAlignedFix10Fix15Shift04Ch1BNew2_top-bottom";
    fMuonId = "muon-qa_id31157/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference TB or BT", "reference T+B-", "reference T-B+", "new geometry TB or BT", "new geometry T+B-", "new geometry T-B+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TPBN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TNBP");
    */

    // Left-Right

    /*
    // fAnalysisResults = "AnalysisResults_LHC24an_pass1_skimmed_Ch1BhalfShift_Hyperloop_24_06_2025.root";
    labelName = "GeometryAlignedFix10Fix15Shift04Ch1BNew2_left-right";
    fMuonId = "muon-qa_id31157/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference LL","reference LR or RR", "reference RR", "new geometry int", "new geometry LL","new geometry LR or RL","new geometry RR"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LL");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_RR");
    */
    /*
    std::vector<const char*> vLegendEntries = {"reference LR or RL", "reference L+R-", "reference L-R+", "new geometry LR or RL", "new geometry L+R-", "new geometry L-R+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LPRN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LNRP");
    */


    // ======================[ GeometryAlignedFix10Fix15ShiftCh1TBNew2: bottom and top CH1 1/2 shift ]=====================
    // Top-Bottom
    
    /*
    labelName = "GeometryAlignedFix10Fix15ShiftCh1TBNew2_top-bottom";
    fMuonId = "muon-qa_id30620/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference TT","reference TB or BT", "reference BB", "new geometry int", "new geometry TT","new geometry TB or BT","new geometry BB"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TT");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_BB");
    */
    /*
    std::vector<const char*> vLegendEntries = {"reference TB or BT", "reference T+B-", "reference T-B+", "new geometry TB or BT", "new geometry T+B-", "new geometry T-B+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TPBN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TNBP");
    */

    // Left-Right

    /*
    labelName = "GeometryAlignedFix10Fix15ShiftCh1TBNew2_left-right";
    fMuonId = "muon-qa_id30620/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference LL","reference LR or RR", "reference RR", "new geometry int", "new geometry LL","new geometry LR or RL","new geometry RR"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LL");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_RR");
    */
    /*
    std::vector<const char*> vLegendEntries = {"reference LR or RL", "reference L+R-", "reference L-R+", "new geometry LR or RL", "new geometry L+R-", "new geometry L-R+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LPRN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LNRP");
    */

    // ======================[ (***) GeometryAlignedFix110Fix19NewIST7: changes? ]=====================
    // Top-Bottom
    
    /*
    fAnalysisResults = "AnalysisResults-muonQA_LHC24am_pass1_skimmed_GeometryAlignedFix110Fix19NewIST7_Hyperloop_08_07_2025.root";
    labelName = "GeometryAlignedFix110Fix19NewIST7_top-bottom";
    fMuonId = "muon-qa/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference TT","reference TB or BT", "reference BB", "new geometry int", "new geometry TT","new geometry TB or BT","new geometry BB"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TT");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_BB");
    */
    /*
    fAnalysisResults = "AnalysisResults-muonQA_LHC24am_pass1_skimmed_GeometryAlignedFix110Fix19NewIST7_Hyperloop_08_07_2025.root";
    labelName = "GeometryAlignedFix110Fix19NewIST7_top-bottom";
    fMuonId = "muon-qa/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference TB or BT", "reference T+B-", "reference T-B+", "new geometry TB or BT", "new geometry T+B-", "new geometry T-B+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TB");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TPBN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_TNBP");
    */

    // Left-Right

    /*
    fAnalysisResults = "AnalysisResults-muonQA_LHC24am_pass1_skimmed_GeometryAlignedFix110Fix19NewIST7_Hyperloop_08_07_2025.root";
    labelName = "GeometryAlignedFix110Fix19NewIST7_left-right";
    fMuonId = "muon-qa/dimuon/same-event/invariantMass_pT_MuonKine_MuonCuts";
    std::vector<const char*> vLegendEntries = {"reference int", "reference LL","reference LR or RR", "reference RR", "new geometry int", "new geometry LL","new geometry LR or RL","new geometry RR"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId);
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LL");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_RR");
    */
    /*
    std::vector<const char*> vLegendEntries = {"reference LR or RL", "reference L+R-", "reference L-R+", "new geometry LR or RL", "new geometry L+R-", "new geometry L-R+"};
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LR");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LPRN");
    vTreeNames.push_back(fAnalysisResults);
    vHistNames.push_back(fMuonId + "_LNRP");
    */

    






    // vTreeNames.push_back("AnalysisResults_merged_LHC24an_LHC24am_pass1_no_realignment_PbPb_values_01_05_2025_Hyperloop.root");
    // vTreeNames.push_back("AnalysisResults_merged_LHC24an_LHC24am_pass1_globalShiftY_PbPb_values_07_05_2025_Hyperloop.root");
    // vTreeNames.push_back("AnalysisResults_merged_LHC24an_LHC24am_pass1_CH1Bshift_new_PbPb_values_12_05_2025_Hyperloop.root");
    // vTreeNames.push_back("AnalysisResults_LHC24am_pass1_skimmed_CH1Bshift_top_bottom_by_half_PbPb_values_13_06_2025_Hyperloop.root");
    // vTreeNames.push_back("AnalysisResults_merged_LHC24an_LHC24am_pass1_javier_realignment_PbPb_values_01_05_2025_Hyperloop.root");
    // vTreeNames.push_back("AnalysisResults_merged_LHC24an_LHC24am_pass1_javier_new_realignment_PbPb_values_12_05_2025_Hyperloop.root");
    // vTreeNames.push_back("AnalysisResults_merged_LHC24an_LHC24am_pass1_javier_new2_realignment_PbPb_values_20_05_2025_Hyperloop.root");
    // vTreeNames.push_back("AnalysisResults_merged_LHC24an_LHC24am_pass1_javier_new2_chi_new_PR_realignment_PbPb_values_22_05_2025_Hyperloop.root");

    /*
    vTreeNames.push_back("AnalysisResults_merged_LHC24aq_LHC24ap_pass1_medium_no_realignment_pp_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_merged_LHC24aq_LHC24ap_pass1_medium_globalShiftY_pp_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_merged_LHC24aq_LHC24ap_pass1_medium_javier_realignment_pp_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_merged_LHC24aq_LHC24ap_pass1_medium_chi_realignment_pp_values_22_04_Hyperloop.root");
    */

    /*
    vTreeNames.push_back("AnalysisResults_merged_LHC24aq_LHC24ap_pass1_medium_no_realignment_PbPb_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_merged_LHC24aq_LHC24ap_pass1_medium_globalShiftY_PbPb_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_merged_LHC24aq_LHC24ap_pass1_medium_javier_realignment_PbPb_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_merged_LHC24aq_LHC24ap_pass1_medium_chi_realignment_PbPb_values_22_04_Hyperloop.root");
    */

    /*
    vTreeNames.push_back("AnalysisResults_LHC24aq_pass1_medium_no_realignment_pp_values_18_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24aq_pass1_medium_globalShiftY_pp_values_18_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24aq_pass1_medium_javier_realignment_pp_values_18_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24aq_pass1_medium_chi_realignment_pp_values_18_04_2025_Hyperloop.root");
    */

    /*
    vTreeNames.push_back("AnalysisResults_LHC24aq_pass1_medium_no_realignment_PbPb_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24aq_pass1_medium_globalShiftY_PbPb_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24aq_pass1_medium_javier_realignment_PbPb_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24aq_pass1_medium_chi_realignment_PbPb_values_22_04_2025_Hyperloop.root");
    */

    /*
    vTreeNames.push_back("AnalysisResults_LHC24ap_pass1_medium_no_realignment_pp_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24ap_pass1_medium_globalShiftY_pp_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24ap_pass1_medium_javier_realignment_pp_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24ap_pass1_medium_chi_realignment_pp_values_22_04_2025_Hyperloop.root");
    */

    /*
    vTreeNames.push_back("AnalysisResults_LHC24ap_pass1_medium_no_realignment_PbPb_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24ap_pass1_medium_globalShiftY_PbPb_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24ap_pass1_medium_javier_realignment_PbPb_values_22_04_2025_Hyperloop.root");
    vTreeNames.push_back("AnalysisResults_LHC24ap_pass1_medium_chi_realignment_PbPb_values_22_04_2025_Hyperloop.root");
    */

    TFile *fOutput = new TFile(Form("Plots_MCH_Quadrants/JpsiPeakAndWidth_%s.root", labelName), "RECREATE");

    // std::vector<Int_t> vLineStyles = {1, 2, 3, 4, 5, 6, 1, 1, 1, 2, 3, 1}; // in order to show reference everywhere..
    std::vector<Int_t> vLineStyles = {1, 2, 5, 1, 2, 5}; // for T+B-, B+T- and combined study
    // std::vector<Int_t> vLineColours = {1, 1, 1, 1, 1, 1, 2, 8, 4, 4, 4, 7};
    std::vector<Int_t> vLineColours = {1, 1, 1, 4, 4, 4}; // for T+B-, B+T- and combined study

    // std::vector<const char*> vLegendEntries = {"Integrated", "CH1 bottom shift TT","CH1 bottom shift TB or BT","CH1 bottom shift BB"};
    // std::vector<const char*> vLegendEntries = {"Integrated", "CH1 bottom shift LL","CH1 bottom shift LR or RL","CH1 bottom shift RR"};
    // std::vector<const char*> vLegendEntries = {"Integrated", "CH1 bottom and top 1/2 shift TT","CH1 bottom and top 1/2 shift TB or BT","CH1 bottom and top 1/2 shift BB"};
    // std::vector<const char*> vLegendEntries = {"Integrated", "CH1 bottom and top 1/2 shift LL","CH1 bottom and top 1/2 shift LR or RR","CH1 bottom and top 1/2 shift RR"};
    // const char* legendTitle = "comparison";
    // const char* legendTitle = "AnalysisResults_LHC24am_pass1_skimmed";
    const char *legendTitle = "PbPb-values";
    // const char* legendTitle = "LHC24aq + ap pp-values";
    // const char* legendTitle = "LHC24aq + ap PbPb-values";
    // const char* legendTitle = "LHC24aq pp-values";
    // const char* legendTitle = "LHC24aq PbPb-values";
    // const char* legendTitle = "LHC24ap pp-values";
    // const char* legendTitle = "LHC24ap PbPb-values";
    // std::vector<const char*> vLegendEntries = {"reference skimmed", "reference ap medium"};
    // std::vector<const char*> vLegendEntries = {"javier new2", "javier new2 Chi PR"};
    // std::vector<const char*> vLegendEntries = {"reference", "CH1B shift full shift", "CH1B half shifts"};
    // std::vector<const char *> vLegendEntries = {"reference", "javier", "javier new", "javier new2"};
    // std::vector<const char*> vLegendEntries = {"reference", "new geometry"};
    // std::vector<const char*> vLegendEntries = {"reference", "global shift Y", "Javier", "Chi"};

    // For muonQA: no pT stored, take just one bin
    std::vector<std::pair<double, double>> ptBins = {
        {0, 2}, {2, 4}, {4, 6}, {6, 8}, {8, 10}, {10, 12}, {12, 20}
    };
    // For muonQA: no pT stored, take just one bin
    // std::vector<std::pair<double, double>> ptBins = {
    //     {0, 20}
    // };
    // For Upsilon: {0,20}

    // Used for variable binning
    std::vector<double> binEdges;
    for (const auto &bin : ptBins)
    {
        binEdges.push_back(bin.first);
    }
    binEdges.push_back(ptBins.back().second); // Add last upper edge
    int nBins = binEdges.size() - 1;
    //

    // int nBins = ptBins.size();

    // TH1D *hTemplateWidths = new TH1D("hTemplateWidths", "J/#psi width vs. p_{T} range; p_{T} range (GeV/c); GeV/c^{2}",
    // nBins, 0, nBins);
    // TH1D *hTemplatePeaks = new TH1D("hTemplatePeaks", "J/#psi peak vs. p_{T} range; p_{T} range (GeV/c); GeV/c^{2}",
    // nBins, 0, nBins);
    TH1D *hTemplateWidths = new TH1D("hTemplateWidths", "J/#psi width; p_{T} (GeV/c); GeV/c^{2}",
                                     nBins, binEdges.data());
    TH1D *hTemplatePeaks = new TH1D("hTemplatePeaks", "J/#psi peak; p_{T} (GeV/c); GeV/c^{2}",
                                    nBins, binEdges.data());
    hTemplateWidths->SetTitle(Form("%s : J/#psi width; p_{T} (GeV/c)", labelName));
    hTemplatePeaks->SetTitle(Form("%s : J/#psi peak; p_{T} (GeV/c)", labelName));

    TLine *lineJpsiPDG = new TLine(binEdges.front()-2, 3.096, binEdges.back()+2, 3.096);
    // For Upsilon: TLine* lineUpsilonPDG = new TLine(binEdges.front(), 9.46, binEdges.back(), 9.46);
    // and change names below
    lineJpsiPDG->SetLineStyle(2);
    lineJpsiPDG->SetLineColor(kGray + 2);
    lineJpsiPDG->SetLineWidth(2);

    TH1D *hWidths;
    TH1D *hPeaks;

    // TCanvas *dummy = new TCanvas("dummy","dummy for drawing", 800, 600);
    // dummy->SaveAs(Form("Plots_MCH_Quadrants/%s.pdf(", labelName)); // assumes the same data set is used for reference and for re-alignment

    TCanvas *globalCanvasJpsiWidths = new TCanvas(Form("globalJpsiWidths_%s", labelName), Form("globalJpsiWidths_%s", labelName), 800, 600);
    globalCanvasJpsiWidths->cd();
    hTemplateWidths->GetYaxis()->SetRangeUser(0.04, 0.15);
    hTemplateWidths->SetStats(0);
    hTemplateWidths->Draw("PE");
    TCanvas *globalCanvasJpsiPeaks = new TCanvas(Form("globalJpsiPeaks_%s", labelName), Form("globalJpsiPeaks_%s", labelName), 800, 600);
    globalCanvasJpsiPeaks->cd();
    hTemplatePeaks->GetYaxis()->SetRangeUser(3.02, 3.17);
    hTemplatePeaks->SetStats(0);
    hTemplatePeaks->Draw("PE");
    // For Upsilon: change name here
    lineJpsiPDG->Draw("same");

    TLegend *legendWidths = new TLegend(0.4, 0.6, 0.6, 0.8);
    legendWidths->SetBorderSize(0);
    legendWidths->AddEntry((TObject *)0, legendTitle, "");
    legendWidths->SetTextSize(0.02);
    TLegend *legendPeaks = new TLegend(0.4, 0.6, 0.6, 0.8);
    legendPeaks->SetBorderSize(0);
    legendPeaks->AddEntry((TObject *)0, legendTitle, "");
    legendPeaks->SetTextSize(0.02);

    // std::vector<std::string> vTopBottom;
    // vTopBottom.push_back{"TT", "TB", "BT", "BB"};

    // Loop through different geometries
    for (int i = 0; i < vTreeNames.size(); i++)
    {
        const char *fileName = vTreeNames[i];
        std::string histName = vHistNames[i];
        // TODO: add top/bottom here, easiest to loop over vector with the names...
        // std::string histName = std::string(vMuonIds[i]) + "/dimuon/same-event/invariantMass_MuonKine_MuonCuts";
        // hWidths = new TH1D(Form("hWidths_%s", treeName), "J/#psi width vs. p_{T} range; p_{T} range (GeV/c); GeV/c^{2}",
        // nBins, 0, nBins);
        // hPeaks = new TH1D(Form("hPeaks_%s", treeName), "J/#psi peak vs. p_{T} range; p_{T} range (GeV/c); GeV/c^{2}",
        // nBins, 0, nBins);
        hWidths = new TH1D(Form("hWidths_%s", fileName), "J/ψ width; p_{T} (GeV/c); GeV/c^{2}",
                           nBins, binEdges.data());
        hPeaks = new TH1D(Form("hPeaks_%s", fileName), "J/ψ peak; p_{T} (GeV/c); GeV/c^{2}",
                          nBins, binEdges.data());

        // Loop through bins and calculate Jpsi width
        for (int j = 0; j < nBins; j++)
        {
            std::cout << "pT range = [" << ptBins[j].first << " , " << ptBins[j].second << "]" << std::endl;
            JpsiValues JpsiValues = CalculateJpsiWidth(fileName, histName, vLegendEntries[i], ptBins[j].first, ptBins[j].second);
            double width = JpsiValues.JpsiWidth;
            double peak = JpsiValues.JpsiPeak;
            double widthError = JpsiValues.JpsiWidthError;
            double peakError = JpsiValues.JpsiPeakError;
            hWidths->SetBinContent(j + 1, width);
            hWidths->SetBinError(j + 1, widthError);
            // hWidths->GetXaxis()->SetBinLabel(j + 1, Form("%.0f-%.0f", ptBins[j].first, ptBins[j].second));
            // hTemplateWidths->GetXaxis()->SetBinLabel(j + 1, Form("%.0f-%.0f", ptBins[j].first, ptBins[j].second));
            hPeaks->SetBinContent(j + 1, peak);
            hPeaks->SetBinError(j + 1, peakError);
            // hPeaks->GetXaxis()->SetBinLabel(j + 1, Form("%.0f-%.0f", ptBins[j].first, ptBins[j].second));
            // hTemplatePeaks->GetXaxis()->SetBinLabel(j + 1, Form("%.0f-%.0f", ptBins[j].first, ptBins[j].second));
        }

        // Correct bin labels
        // hWidths->LabelsOption("h", "X");
        // hPeaks->LabelsOption("h", "X");

        TCanvas *canvasJpsiWidths = new TCanvas(Form("cJpsiWidths_%s", fileName), Form("cJpsiWidths_%s", fileName), 800, 600);
        canvasJpsiWidths->cd();
        hWidths->SetStats(0);
        hWidths->Draw("PE");

        globalCanvasJpsiWidths->cd();
        hWidths->SetLineColor(vLineColours[i]);
        hWidths->SetLineStyle(vLineStyles[i]);
        hWidths->Draw("SAME PE");
        legendWidths->AddEntry(hWidths, vLegendEntries[i], "l");

        TCanvas *canvasJpsiPeaks = new TCanvas(Form("cJpsiPeaks_%s", fileName), Form("cJpsiPeaks_%s", fileName), 800, 600);
        canvasJpsiPeaks->cd();
        hPeaks->SetStats(0);
        hPeaks->Draw("PE");

        globalCanvasJpsiPeaks->cd();
        hPeaks->SetLineColor(vLineColours[i]);
        hPeaks->SetLineStyle(vLineStyles[i]);
        hPeaks->Draw("SAME PE");
        legendPeaks->AddEntry(hPeaks, vLegendEntries[i], "l");
        std::cout << "vLegendEntries[i] = " << vLegendEntries[i] << std::endl;

        CalculateJpsiWidth(fileName, histName, vLegendEntries[i], 0, 30);

        // canvasJpsiWidths->SaveAs(Form("Plots/%s_JpsiWidths.pdf", treeName));
        // canvasJpsiWidths->SaveAs(Form("Plots/%s_JpsiWidths.png", treeName));
        // canvasJpsiPeaks->SaveAs(Form("Plots/%s_JpsiPeaks.pdf", treeName));
        // canvasJpsiPeaks->SaveAs(Form("Plots/%s_JpsiPeaks.png", treeName));
    }

    globalCanvasJpsiWidths->cd();
    legendWidths->Draw();
    globalCanvasJpsiPeaks->cd();
    legendPeaks->Draw();

    globalCanvasJpsiWidths->SaveAs(Form("Plots_MCH_Quadrants/%s.pdf(", labelName));
    globalCanvasJpsiPeaks->SaveAs(Form("Plots_MCH_Quadrants/%s.pdf)", labelName));

    fOutput->cd();
    globalCanvasJpsiWidths->Write();
    globalCanvasJpsiPeaks->Write();
    fOutput->Close();

    // dummy->SaveAs(Form("Plots_MCH_Quadrants/%s.pdf)", labelName));
    
    // Save all outputs
    // For Upsilon: change output names
    /*
    globalCanvasJpsiWidths->SaveAs(Form("Plots/globalCanvasJpsiWidths_%s.pdf", legendTitle));
    globalCanvasJpsiWidths->SaveAs(Form("Plots/globalCanvasJpsiWidths_%s.png", legendTitle));
    globalCanvasJpsiPeaks->SaveAs(Form("Plots/globalCanvasJpsiPeaks_%s.pdf", legendTitle));
    globalCanvasJpsiPeaks->SaveAs(Form("Plots/globalCanvasJpsiPeaks_%s.png", legendTitle));
    */

    return 0;
}

TH1D *getTree(const char *fileName, Double_t ptMin, Double_t ptMax)
{

    // **********************************************
    // G  E  T    D  A  T  A
    // **********************************************

    THashList *listDiMuon;
    TList *subListDiMuon;
    TH2F *hMass_Pt;

    TFile *fDiMuon = new TFile(fileName, "READ");
    listDiMuon = (THashList *)fDiMuon->Get("analysis-same-event-pairing/output");
    // NAME DEPENDS ON CUT USED
    // PairsMuonSEPM_muonLowPt510SigmaPDCA
    // PairsMuonSEPM_muonQualityCuts
    // For Jpsi: PairsMuonSEPM_muonLowPt510SigmaPDCA
    // For Y(1S): PairsMuonSEPM_muonLowPt610SigmaPDCA
    subListDiMuon = (TList *)listDiMuon->FindObject("PairsMuonSEPM_muonLowPt510SigmaPDCA");
    hMass_Pt = (TH2F *)subListDiMuon->FindObject("Mass_Pt");

    // **********************************************
    // S  E  T    P  _  T    C  U  T
    // **********************************************

    TH1D *hDiMuonMass_PtCut;

    hMass_Pt = (TH2F *)hMass_Pt->Clone();
    hMass_Pt->GetYaxis()->SetRangeUser(ptMin, ptMax);
    hDiMuonMass_PtCut = hMass_Pt->ProjectionX("hDiMuonMass_PtCut");

    return hDiMuonMass_PtCut;

} // void getTree()

// Use this one for the dimuon spectra from Andrea/Chi's code
TH1* GetTH1(const char* fileName, std::string histName)
{
    TFile *f = new TFile(fileName, "READ");
    std::cout << "Reading " << histName.c_str() << " from TFile" << std::endl;
    TH1* hist = (TH1*)f->Get(histName.c_str());
    if (hist == nullptr) { std::cout << ">> error retrieving histogram" << std::endl; }
    else { std::cout << ">> histogram sucessfully read from TFile" << std::endl; }
    return hist;
}

// Use this one for the dimuon spectra from Andrea/Chi's code
// When getting TH2 (mass, pT) and making the projection
TH1* GetTH1FromTH2(const char* fileName, std::string histName, Double_t ptMin, Double_t ptMax)
{
    TFile *f = new TFile(fileName, "READ");
    std::cout << "Reading " << histName.c_str() << " from TFile" << std::endl;
    TH2 *hist = (TH2*)f->Get(histName.c_str());
    if (hist == nullptr) { std::cout << ">> error retrieving histogram" << std::endl; }
    else { std::cout << ">> histogram sucessfully read from TFile" << std::endl; }
    // TH2 *hMass_Pt = (TH2*)hist->Clone();
    hist->GetYaxis()->SetRangeUser(ptMin, ptMax);
    std::cout << "problem with projection?" << std::endl;
    TH1 *hDiMuonMass_PtCut = hist->ProjectionX("hDiMuonMass_PtCut");
    std::cout << "no problem with projection" << std::endl;
    return hDiMuonMass_PtCut;
}

void defSigModel(RooWorkspace &ws)
{

    RooRealVar *m = ws.var("m");
    Double_t mMin = m->getMin("fitRange");
    Double_t mMax = m->getMax("fitRange");
    m->setRange("fitRange", mMin, mMax);

    // Crystal Ball for mass signal
    RooRealVar m0("m0", "#mu", 3.097, 3.05, 3.13);
    // For Upsilon: RooRealVar m0("m0","#mu",9.46,9.40,9.52);
    RooRealVar sigma("sigma", "#sigma", 0.08, 0.05, 0.12);

    // For psi(2s)
    // RooRealVar m0_psi2S("m0_psi2S","#mu_{ψ(2S)}",3.686,3.65,3.72);
    // RooRealVar sigma_psi2S("sigma_psi2S","#sigma_{ψ(2S)}",0.08,0.05,0.12); // TODO: multiply everything by 1.05
    RooFormulaVar m0_psi2S("m0_psi2S", "#mu_{ψ(2S)}", "m0 + 0.589", RooArgList(m0));
    RooFormulaVar sigma_psi2S("sigma_psi2S", "#sigma_{ψ(2S)}", "sigma * 1.05", RooArgList(sigma));

    RooRealVar alphaL("alphaL", "Alpha Left", 0.883, 0.5, 3.0);
    alphaL.setConstant();
    RooRealVar nL("nL", "Exponent Left", 9.4, 5.0, 20.0);
    nL.setConstant();
    RooRealVar alphaR("alphaR", "Alpha Right", 1.832, 1.0, 3.0);
    alphaR.setConstant();
    RooRealVar nR("nR", "Exponent Right", 15.323, 5.0, 25.0);
    nR.setConstant();

    RooCrystalBall *doubleSidedCB_jpsi = new RooCrystalBall("doubleSidedCB_jpsi", "Double Sided Crystal Ball for Jpsi",
                                                            *m, m0, sigma, alphaL, nL, alphaR, nR);
    RooCrystalBall *doubleSidedCB_psi2S = new RooCrystalBall("doubleSidedCB_psi2S", "Crystal Ball for psi(2S)",
                                                             *m, m0_psi2S, sigma_psi2S,
                                                             alphaL, nL, alphaR, nR);

    ws.import(*m);
    ws.import(*doubleSidedCB_jpsi);
    ws.import(*doubleSidedCB_psi2S);

} // void defSigModel()

void defBkgModel(RooWorkspace &ws, std::string BKG_model, Double_t ptMin, Double_t ptMax)
{

    RooAbsPdf *BKG;
    RooRealVar *m = ws.var("m");

    // Exponential for background
    // RooRealVar a0("a0","a_{0}",-1,-5,0.000001);
    // RooExponential* BKG = new RooExponential("BKG","Power background", m, a0);

    // Initiallise Chebychev
    RooRealVar a0("a0", "a_{0}", -0.01, -2.0, 2.0);
    RooRealVar a1("a1", "a_{1}", -0.01, -1.0, 1.0);
    RooRealVar a2("a2", "a_{2}", -0.01, -0.5, 0.5);
    RooRealVar a3("a3", "a_{3}", 0.0, -0.5, 0.5);
    RooRealVar a4("a4", "a_{4}", 0.0, -0.3, 0.3);
    RooRealVar a5("a5", "a_{5}", 0.0, -0.3, 0.3);
    RooRealVar a6("a6", "a_{6}", 0.0, -0.1, 0.1);
    RooRealVar a7("a7", "a_{7}", 0.0, -0.05, 0.05);

    // Initialise a Gaussian for background
    RooRealVar BKG_mean("BKG_mean", "Mean of background Gaussian", 2.5, 0., 10);
    RooRealVar BKG_sigma("BKG_sigma", "Sigma of background Gaussian", 1.7, 0., 2.5);

    // Initialise an exponential for background
    RooRealVar exp0("exp0", "exp_{0}", -1, -5, 0.001);

    // Select background model (all are initialised, one is picked)
    RooRealVar *N_jpsi;
    RooRealVar *N_psi2S;
    RooRealVar *sigYield;
    RooRealVar *bkgYield;

    if (BKG_model == "Chebychev")
    {
        BKG = new RooChebychev("BKG", "Chebyshev background", *m, {a0, a1, a2, a3, a4, a5, a6, a7});
        N_jpsi = new RooRealVar("N_jpsi", "N_{jpsi}", 20000, 0., 10000000);
        N_psi2S = new RooRealVar("N_psi2S", "N_{psi2S}", 200, 0., 100000);
        bkgYield = new RooRealVar("bkgYield", "N_{bkg}", 150000, 0., 100000000);
    }

    // TODO: add sigYields for jpsi and psi2S below
    if (BKG_model == "Gaussian")
    {
        BKG = new RooGaussian("BKG", "Background gaussian", *m, BKG_mean, BKG_sigma);
        sigYield = new RooRealVar("sigYield", "N_{sig}", 50000, 0., 100000000);
        bkgYield = new RooRealVar("bkgYield", "N_{bkg}", 1000000, 0., 1000000000);
    }

    if (BKG_model == "Exponential")
    {
        BKG = new RooExponential("BKG", "Power background", *m, exp0);
        sigYield = new RooRealVar("sigYield", "N_{sig}", 5000, 0., 100000000);
        bkgYield = new RooRealVar("bkgYield", "N_{bkg}", 10000000, 0., 1000000000);
    }

    // Add parameters to the workspace
    // This step is necessary for the iterative fitting later
    ws.import(a0);
    ws.import(a1);
    ws.import(a2);
    ws.import(a3);
    ws.import(a4);
    ws.import(a5);
    ws.import(a6);
    ws.import(a7);
    // Add models
    ws.import(*BKG);
    ws.import(*N_jpsi);
    ws.import(*N_psi2S);
    ws.import(*bkgYield);

} // void defBkgModel()

void defCombinedModel(RooWorkspace &ws, Double_t ptMin, Double_t ptMax)
{

    // Add signal and background models
    RooAbsPdf *model = new RooAddPdf("model", "combined mass model",
                                     RooArgList(*ws.pdf("doubleSidedCB_jpsi"), *ws.pdf("doubleSidedCB_psi2S"), *ws.pdf("BKG")),
                                     RooArgList(*ws.var("N_jpsi"), *ws.var("N_psi2S"), *ws.var("bkgYield")));

    ws.import(*model);

} // defCombinedModel

void fitModelToData(RooWorkspace &ws, TH1 *hist, std::string BKG_model, Double_t ptMin, Double_t ptMax)
{

    RooRealVar *m = ws.var("m");
    Double_t mMin = m->getMin("fitRange");
    Double_t mMax = m->getMax("fitRange");
    m->setRange("fitRange", mMin, mMax);
    RooDataHist *data = new RooDataHist(Form("data_Pt_%.0f_%.0f", ptMin, ptMax), "Di-muon spectrum", *m, Import(*hist));
    RooAbsPdf *model = ws.pdf("model");
    RooRealVar *N_jpsi = ws.var("N_jpsi");
    RooRealVar *N_psi2S = ws.var("N_psi2S");
    RooRealVar *sigYield = new RooRealVar("sigYield_real", "Total signal yield", 0);
    sigYield->setVal(N_jpsi->getVal() + N_psi2S->getVal());
    RooRealVar *bkgYield = ws.var("bkgYield");

    // Retreive Chebyshev parameters
    std::vector<RooRealVar *> parameters = {
        ws.var("a0"),
        ws.var("a1"),
        ws.var("a2"),
        ws.var("a3"),
        ws.var("a4"),
        ws.var("a5"),
        ws.var("a6"),
        ws.var("a7")};
    // Initially set all parameters to constant (kTRUE)
    // Iterative fitting releases parameters one-by-one
    for (auto &param : parameters)
    {
        param->setConstant(kTRUE);
    }

    /*
    // Start iterative fitting (only for Chebychev)
    // Release orders one-by-one as long as reduced χ2 is above desired value
    Double_t chi2M = -1.; // arbitrary starting value
    if (BKG_model == "Chebychev") {
        for (Int_t i = 0; i < parameters.size(); i++) {
            if (chi2M < 0. || chi2M > 2. || std::isnan(chi2M)) {
                frame = nullptr;
                parameters[i]->setConstant(kFALSE);
                // IntegrateBins(1e-3): integrate per bin?
                model->fitTo(*data,Range("fitRange"),PrintLevel(-1),Extended(kTRUE),Verbose(kFALSE));
                frame = m->frame();
                data->plotOn(frame,MarkerSize(0.4),Range("fitRange"));
                model->plotOn(frame,LineColor(kBlue),Name("full_Model"),Range("fitRange"));
                chi2M = frame->chiSquare();
                std::cout<<"chi2M = "<<chi2M<<std::endl;
            }
            else { break; }
            if (i == 7 && (chi2M < 0. || chi2M > 2.)) { std::cout<<"WARNING. Fitting failed. No convergence"<<std::endl; }
        }
    }
    */

    // Alternative approach that prevents drawing frames every iteration
    Double_t chi2M = -1.; // arbitrary starting value
    Int_t ndf = (hist->FindBin(mMax) - hist->FindBin(mMin)) - 8;
    if (BKG_model == "Chebychev")
    {
        for (Int_t i = 0; i < parameters.size(); i++)
        {
            if (chi2M < 0. || chi2M > 2. || std::isnan(chi2M))
            {
                parameters[i]->setConstant(kFALSE);
                // IntegrateBins(1e-3): integrate per bin?
                model->fitTo(*data, Range("fitRange"), PrintLevel(-1), Extended(kTRUE), Verbose(kFALSE));
                sigYield->setVal(N_jpsi->getVal() + N_psi2S->getVal());
                chi2M = calculateChi2(m, hist, model, sigYield, bkgYield) / ndf;
                std::cout << "chi2New = " << chi2M << std::endl;
            }
            else
            {
                break;
            }
            if (i == 7 && (chi2M < 0. || chi2M > 2.))
            {
                std::cout << "WARNING. Fitting failed. No convergence" << std::endl;
            }
        }
    }

    // Pull needs to be calculated before more fitting is done
    // (so that it is calculated with respect to the correct model)
    // RooHist *hpull = frame->pullHist();
    RooRealVar chi2Var("chi2M", "Chi-squared value", chi2M, -1., 1e6);

    // ws.import(*hpull);
    ws.import(chi2Var);
    // ws.import(*frame);

} // void fitModelToData()

void drawPlots(RooWorkspace &ws, TH1 *hist, const char *treeName, const char *realignmentLabel, Double_t ptMin, Double_t ptMax)
{

    // Collect everything from workspace
    RooRealVar *m = ws.var("m");
    Double_t mMin = m->getMin("fitRange");
    Double_t mMax = m->getMax("fitRange");
    m->setRange("fitRange", mMin, mMax);

    RooAbsPdf *doubleSidedCB_jpsi = ws.pdf("doubleSidedCB_jpsi");
    RooAbsPdf *doubleSidedCB_psi2S = ws.pdf("doubleSidedCB_psi2S");
    RooAbsPdf *BKG = ws.pdf("BKG");
    RooAbsPdf *model = ws.pdf("model");
    RooRealVar *N_jpsi = ws.var("N_jpsi");
    RooRealVar *N_psi2S = ws.var("N_psi2S");
    RooRealVar *bkgYield = ws.var("bkgYield");
    RooDataHist *data = new RooDataHist(Form("data_Pt_%.0f_%.0f", ptMin, ptMax), "Di-muon spectrum", *m, Import(*hist));

    // RooPlot *frame = dynamic_cast<RooPlot*>(ws.obj("frame"));
    RooPlot *frame = m->frame();
    // RooHist *hpull = dynamic_cast<RooHist*>(ws.obj("hpull"));
    RooRealVar *sigYield = new RooRealVar("sigYield_real", "Total signal yield", 0);
    sigYield->setVal(N_jpsi->getVal() + N_psi2S->getVal());
    TGraphErrors *hpull = calculatePullHist(m, hist, model, sigYield, bkgYield);

    TCanvas *canvas = new TCanvas(Form("%s_canvas_Pt_%.0f_%.0f", realignmentLabel, ptMin, ptMax),
                                  Form("%s_Double Sided Crystal Ball Fit %.0f < p_{T} < %.0f", realignmentLabel, ptMin, ptMax),
                                  800, 600);
    canvas->SetBottomMargin(0);
    canvas->cd();

    TPad *megaPad = new TPad("megaPad", "megaPad", 0.0, 0.4, 1.0, 1.0); // 0.3
    megaPad->SetRightMargin(0.05);
    megaPad->SetBottomMargin(0.10); // 1e-3
    megaPad->SetLeftMargin(0.10);
    megaPad->SetTicks(1, 1);
    megaPad->Draw();
    TPad *miniPad = new TPad("miniPad", "miniPad", 0.0, 0.0, 1.0, 0.3);
    miniPad->SetTopMargin(1e-3);
    miniPad->SetRightMargin(0.05);
    miniPad->SetBottomMargin(0.40);
    miniPad->SetLeftMargin(0.10);
    miniPad->SetTicks(1, 1);
    miniPad->Draw();

    megaPad->cd();
    gPad->SetLogy();
    data->plotOn(frame, MarkerSize(0.4), Range("fitRange"));
    model->plotOn(frame, LineColor(kBlue), Name("full_Model"), Range("fitRange"));
    model->plotOn(frame, Components(*doubleSidedCB_jpsi), LineStyle(kDashed), LineColor(kRed), Name("jpsi"), Range("fitRange"));
    model->plotOn(frame, Components(*doubleSidedCB_psi2S), LineStyle(kDashed), LineColor(kGreen), Name("psi2S"), Range("fitRange"));
    model->plotOn(frame, Components(*BKG), LineStyle(kDashed), LineColor(kBlue), Name("bkg_Model"), Range("fitRange"));
    model->paramOn(
        frame,
        Parameters(RooArgSet(*N_jpsi, *N_psi2S)),
        ShowConstants(false),
        Format("NEU", AutoPrecision(3)),
        Layout(0.55, 0.9, 0.85));
    std::cout << "model parameters:" << std::endl;
    model->Print("t");

    // Beautify the box displaying the parameters
    TPaveText *paramsBox = (TPaveText *)frame->findObject("model_paramBox");
    paramsBox->SetTextSize(0.04);
    paramsBox->SetTextFont(42);
    // This part below doesn't work?
    // ** //
    paramsBox->SetX1NDC(0.70);
    paramsBox->SetY1NDC(0.45);
    paramsBox->SetX2NDC(0.95);
    paramsBox->SetY2NDC(0.90);
    // ** //
    std::cout << "TPaveText found with coordinates: "
              << paramsBox->GetX1NDC() << ", "
              << paramsBox->GetY1NDC() << ", "
              << paramsBox->GetX2NDC() << ", "
              << paramsBox->GetY2NDC() << std::endl;
    paramsBox->Draw();

    // Beautify the canvas and frame
    frame->SetTitle("J/#psi invariant mass plot");
    frame->SetMinimum(1e-1);
    frame->SetMaximum(1.5 * frame->GetMaximum());
    frame->GetXaxis()->SetTitle("");
    frame->GetXaxis()->SetLabelSize(0.035);
    frame->GetYaxis()->SetRangeUser(1e-1, 1e8);
    // frame->GetYaxis()->SetRangeUser(0, 1e5);

    frame->Draw();

    // alternative chi2 method
    /*
    RooChi2Var* chi2Var = new RooChi2Var("chi2","chi2", *doubleSidedCB, *data, true, RooAbsData::ErrorType::Poisson);
    double chi2M = chi2Var->getVal();
    */

    TLegend *legend = new TLegend(0.28, 0.60, 0.52, 0.88);
    // For Upsilon: TLegend *legend = new TLegend(0.13,0.16,0.37,0.44);
    legend->SetBorderSize(0);
    legend->SetTextSize(0.04);
    legend->AddEntry("", Form("%s realignment", realignmentLabel), "");
    legend->AddEntry("", Form("%.0f < p_{T} < %.0f [GeV]", ptMin, ptMax), "");
    legend->AddEntry(frame->getObject(0), "Data", "point");
    legend->AddEntry(frame->getObject(1), "Full model", "l");
    legend->AddEntry("", Form("#chi^{2}/ndf = %.2f", ws.var("chi2M")->getVal()), "");
    Int_t ndf = (hist->FindBin(mMax) - hist->FindBin(mMin)) - 8;
    // legend->AddEntry("",Form("hand-made #chi^{2}/ndf = %.2f",calculateChi2(m,hist,model,sigYield,bkgYield)/ndf),"");
    // legend->AddEntry("",Form("S/B_{jpsi} (3#sigma) = %.3f",calculateSigOverBkgRatio(m,hist,doubleSidedCB_jpsi,BKG,model,N_jpsi,bkgYield,"m0","sigma")),"");
    // legend->AddEntry("",Form("S/#sqrt{S+B}_{jpsi} (3#sigma) = %.2f",calculateSignificance(m,doubleSidedCB_jpsi,BKG,model,N_jpsi,bkgYield,"m0","sigma")),"");
    // TODO: doesn't work for RooFormulaVar with this set-up; change how arguments are given or define template function..
    // legend->AddEntry("",Form("S/B_psi2S (3#sigma) = %.3f",calculateSigOverBkgRatio(m,hist,doubleSidedCB_psi2S,BKG,model,N_psi2S,bkgYield,"m0_psi2S","sigma_psi2S")),"");
    // legend->AddEntry("",Form("S/#sqrt{S+B}_psi2S (3#sigma) = %.2f",calculateSignificance(m,doubleSidedCB_psi2S,BKG,model,N_psi2S,bkgYield,"m0_psi2S","sigma_psi2S")),"");
    legend->Draw();
    // check output
    std::cout << "chi2/ndf by machine = " << ws.var("chi2M")->getVal() << std::endl;

    /*
    m0.Print();
    sigma.Print();
    alphaL.Print();
    nL.Print();
    alphaR.Print();
    nR.Print();
    */

    miniPad->cd();
    Double_t factor = 3.33;
    hpull->SetTitle("");
    hpull->GetYaxis()->SetTitle("pull");
    hpull->GetYaxis()->SetTitleSize(factor * frame->GetYaxis()->GetTitleSize());
    hpull->GetYaxis()->SetTitleOffset(0.35);
    hpull->GetYaxis()->SetLabelSize(0.8 * factor * frame->GetYaxis()->GetLabelSize());
    hpull->GetYaxis()->SetTickLength(frame->GetYaxis()->GetTickLength());
    //
    hpull->GetXaxis()->SetTitle("mass [GeV/c^2]");
    hpull->GetXaxis()->SetTitleSize(factor * frame->GetXaxis()->GetTitleSize());
    hpull->GetXaxis()->SetTitleOffset(1.5);
    hpull->GetXaxis()->SetLabelOffset(0.08);
    // hpull->GetXaxis()->SetTickLength(factor*frame->GetXaxis()->GetTickLength());
    hpull->GetXaxis()->SetLabelSize(factor * 0.035);
    hpull->GetXaxis()->SetRangeUser(frame->GetXaxis()->GetXmin(), frame->GetXaxis()->GetXmax());
    hpull->GetXaxis()->SetTickLength(frame->GetXaxis()->GetTickLength());
    hpull->Draw();

    // FIX THIS
    TLine *line1 = new TLine(frame->GetXaxis()->GetXmin(), -1, frame->GetXaxis()->GetXmax(), -1);
    TLine *line2 = new TLine(frame->GetXaxis()->GetXmin(), 1, frame->GetXaxis()->GetXmax(), 1);
    line1->SetLineWidth(3);
    line1->SetLineColor(kRed);
    //
    line2->SetLineWidth(3);
    line2->SetLineColor(kRed);
    line1->Draw();
    line2->Draw();
    // miniPad->Update();

    // Save outputs
    // For Upsilon: change name
    canvas->SaveAs(Form("Plots/JPsiFit_%s_[%.0f_%.0f].pdf", treeName, ptMin, ptMax));
    canvas->SaveAs(Form("Plots/JPsiFit_%s_[%.0f_%.0f].png", treeName, ptMin, ptMax));

    // Obsolete
    // canvas->SaveAs(Form("Plots/SimpleJpsiFitting/pTRange_[%.0f,%.0f]_JpsiSingleMuonCut1GeV.pdf",ptMin,ptMax));
    // canvas->SaveAs(Form("Plots/SimpleJpsiFitting/pTRange_[%.0f,%.0f]_JpsiSingleMuonCut1GeV.png",ptMin,ptMax));

    // Calculate in mass range the entries for histogram
    int bin_min = hist->FindBin(mMin);
    int bin_max = hist->FindBin(mMax);
    double sumEntries = 0;
    for (int bin = bin_min; bin <= bin_max; ++bin)
    {
        sumEntries += hist->GetBinContent(bin);
    }
    std::cout << "Entries in range [" << mMin << ", " << mMax << "]: " << sumEntries << std::endl;
    sigYield->setVal(N_jpsi->getVal() + N_psi2S->getVal());
    std::cout << "sigYield + bkgYield = " << sigYield->getVal() + bkgYield->getVal() << std::endl;

} // void drawPlots()
