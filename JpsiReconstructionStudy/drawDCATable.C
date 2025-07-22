// drawDCATable.C
#include "TROOT.h"

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

// Create table in this shape:
//              geometry1 geometry2 geometry3 ...
// DCA(x)
// -> Q0
// -----> pos
// -----> neg
// -> Q1
// -----> pos
// etc.
//
// DCA(y)
//
//

// This is done by filling a vector with the following shape:
using ResultMap = std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, std::string>>>>;
// Accessed like: results[plane][quadrant][posNeg][geometry] = "mean ± std"

// Don't forget errors

int drawDCATable()
{

    /*
    const char *fAnalysisResults = "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_6_geometries.root";
    const char *histName;

    // Using the following geometries and (fixed) MCH parameters, according to muon-qa root file structure:
    std::vector<const char*> vGeometries = {
            "muon-qa_id30619", // GeometryAlignedFix10Fix15ShiftCh1BNew2 (bottom only shift)
            "muon-qa_id30620", // GeometryAlignedFix10Fix15ShiftCh1TBNew2 (top and bottom half shift)
            "muon-qa_id30697", // GeometryAligned (reference)
            "muon-qa_id30991", // GeometryAlignedShiftY (global shift Y)
            "muon-qa_id30992", // GeometryAlignedFix110Fix15New2T5 (better javier new2)
            "muon-qa_id30993"  // GeometryAlignedFix110Fix15New2T5ShiftY (better javier new2 + shift)
    };
    std::map<std::string, std::string> geometryNames = {
        {"muon-qa_id30619", "GeometryAlignedFix10Fix15ShiftCh1BNew2"},
        {"muon-qa_id30620", "GeometryAlignedFix10Fix15ShiftCh1TBNew2"},
        {"muon-qa_id30697", "GeometryAligned"},
        {"muon-qa_id30991", "GeometryAlignedShiftY"},
        {"muon-qa_id30992", "GeometryAlignedFix110Fix15New2T5"},
        {"muon-qa_id30993", "GeometryAlignedFix110Fix15New2T5ShiftY"}
    };
    */

    // const char *fAnalysisResults = "AnalysisResults_LHC24an_pass1_skimmed_Ch1BhalfShift_Hyperloop_24_06_2025.root";
    // const char *fAnalysisResults = "AnalysisResults-muonQA_LHC24am_pass1_skimmed_GeometryAlignedFix110Fix19NewIST7_Hyperloop_08_07_2025.root";

    // const char *fAnalysisResults = "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root";
    // const char *fAnalysisResults = "AnalysisResults-muonQA_LHC24al_pass1_skimmed_GeometryAlignedFix110Fix19alnNewadSU1_Hyperloop_18_07_2025.root";
    // const char *fAnalysisResults = "AnalysisResults-muonQA_LHC24an_pass1_skimmed_GeometryAlignedFix110Fix19alnNewadSU1_Hyperloop_22_07_2025.root";
    const char *fAnalysisResults = "AnalysisResults-muonQA_merged_LHC24an_LHC24al_pass1_merged_all_geometries_Hyperloop_22_07_2025.root";
    const char *histName;

    // Using the following geometries and (fixed) MCH parameters, according to muon-qa root file structure:
    /*
    std::vector<const char *> vGeometries = {
        "muon-qa_id30697",
        "muon-qa_id30619",
        "muon-qa_id31157",
        "muon-qa_id30992",
        "muon-qa_id30993",
        "muon-qa", // GeometryAlignedFix10Fix15Shift04Ch1BNew2
    };
    std::map<std::string, std::string> geometryNames = {
        {"muon-qa_id30697", "reference"},
        {"muon-qa_id30619", "GeometryAlignedFix10Fix15ShiftCh1BNew2"},
        {"muon-qa_id31157", "GeometryAlignedFix10Fix15Shift04Ch1BNew2"},
        {"muon-qa_id30992", "GeometryAlignedFix110Fix15New2T5"},
        {"muon-qa_id30993", "GeometryAlignedFix110Fix15New2T5ShiftY"},
        {"muon-qa", "GeometryAlignedFix110Fix19NewIST7"},
    };
    */

   std::vector<const char *> vGeometries = {
        "muon-qa_id32943",
    };
    std::map<std::string, std::string> geometryNames = {
        {"muon-qa_id32943", "GeometryAlignedFix110Fix19alnNewadSU1"},
    };

    std::array<const char *, 4> arrQuadrants = {"Q0", "Q1", "Q2", "Q3"};
    std::array<const char *, 2> arrXY = {"x", "y"};
    std::array<const char *, 2> arrPosNeg = {"pos", "neg"};

    ResultMap results;

    // Where the geometries are the following:
    // --- muon-qa_id30619: GeometryAlignedFix10Fix15ShiftCh1BNew2 (bottom only shift)
    // --- muon-qa_id30620: GeometryAlignedFix10Fix15ShiftCh1TBNew2 (top and bottom half shift)
    // --- muon-qa_id30697: GeometryAligned (reference)
    // --- muon-qa_id30991: GeometryAlignedShiftY (global shift Y)
    // --- muon-qa_id30992: GeometryAlignedFix110Fix15New2T5 (better javier new2)
    // --- muon-qa_id30993: GeometryAlignedFix110Fix15New2T5ShiftY (better javier new2 + shift)

    // Fill the result map
    for (const char *geometry : vGeometries)
    {
        for (const char *plane : arrXY)
        {
            for (const char *quadrant : arrQuadrants)
            {
                for (const char *posNeg : arrPosNeg)
                {
                    histName = Form("%s/Alignment/same-event/DCA/MCH/%s/DCA_%s_%s",
                                    geometry, quadrant, plane, posNeg);

                    TH1 *hist = GetTH1(fAnalysisResults, histName);
                    std::string value = "N/A";
                    if (hist)
                    {
                        double mean = hist->GetMean();
                        double stdDev = hist->GetStdDev();
                        int nEntries = hist->GetEntries();
                        double stdError = (nEntries > 0) ? stdDev / std::sqrt(nEntries) : 0.0;
                        char formatted[64];
                        snprintf(formatted, sizeof(formatted), "%.3f ± %.4f", mean, stdError);
                        value = formatted;
                    }
                    results[std::string(plane)][std::string(quadrant)][std::string(posNeg)][std::string(geometry)] = value;
                }
            }
        }
    }

    const int colWidth = 36;

    // Print header row
    std::cout << std::setw(colWidth) << std::left << "" << "|";
    for (const auto &geometry : vGeometries)
    {
        std::cout << std::setw(colWidth) << std::left << geometryNames[geometry] << "|";
    }
    std::cout << "\n"
              << std::string(colWidth * (vGeometries.size() + 1), '-') << "\n";

    // Print the rows
    for (const auto &plane : arrXY)
    {
        std::cout << std::setw(colWidth) << std::left << Form("DCA(%s)", plane) << "\n";
        for (const auto &quadrant : arrQuadrants)
        {
            std::cout << std::setw(colWidth) << std::left << ("  " + std::string(quadrant)) << "\n";
            for (const auto &posNeg : arrPosNeg)
            {
                std::cout << std::setw(colWidth) << std::left << ("    " + std::string(posNeg)) << "|";
                for (const auto &geometry : vGeometries)
                {
                    std::string val = results[plane][quadrant][posNeg][geometry];
                    std::cout << std::setw(colWidth) << std::right << val << "|";
                }
                std::cout << "\n";
            }
        }
    }

    // Now try putting the DCAs in a canvas
    TString outPdf = Form("Plots_DCAs/geometry_dca_positions_%s.pdf", fAnalysisResults);
    TCanvas dummy;
    dummy.Print(outPdf + "["); // Open PDF

    std::map<std::string, Color_t> quadrantColors = {
        {"Q0", kRed + 1},
        {"Q1", kBlue + 1},
        {"Q2", kGreen + 2},
        {"Q3", kOrange + 7}};

    std::vector<TCanvas *> canvases;
    std::vector<TGraph *> graphs;

    for (const auto &geometry : vGeometries)
    {
        const std::string g = geometry;
        std::string canvasName = Form("c_%s", g.c_str());
        TCanvas *c = new TCanvas(canvasName.c_str(), geometryNames[g].c_str(), 800, 800);
        canvases.push_back(c);

        TMultiGraph *mg = new TMultiGraph();

        for (const auto &quadrant : arrQuadrants)
        {
            for (const auto &posNeg : arrPosNeg)
            {
                std::string xValStr = results["x"][quadrant][posNeg][g];
                std::string yValStr = results["y"][quadrant][posNeg][g];

                if (xValStr != "N/A" && yValStr != "N/A")
                {
                    double x, y;
                    sscanf(xValStr.c_str(), "%lf", &x);
                    sscanf(yValStr.c_str(), "%lf", &y);

                    TGraph *gr = new TGraph(1);
                    // gr->GetXaxis()->SetLimits(-1.5, 1.5);
                    // gr->GetYaxis()->SetLimits(-1.5, 1.5);
                    gr->SetPoint(0, x, y);
                    gr->SetMarkerStyle((std::string(posNeg) == "pos") ? 2 : 5); // '+' or 'x'
                    gr->SetMarkerColor(quadrantColors[quadrant]);
                    gr->SetMarkerSize(2.5);
                    gr->SetTitle(Form("%s %s", quadrant, posNeg));

                    mg->Add(gr);
                    graphs.push_back(gr); // keep reference
                }
            }
        }

        mg->SetTitle(Form("DCA Mean Positions - %s;x [cm];y [cm]", geometryNames[g].c_str()));
        mg->Draw("AP");
        mg->GetXaxis()->SetLimits(-1.5, 1.5);
        mg->SetMinimum(-1.5);
        mg->SetMaximum(1.5);

        // Dashed vertical line at x = 0
        TLine *lineX = new TLine(0, -1.5, 0, 1.5);
        lineX->SetLineStyle(2); // dashed
        lineX->SetLineColor(kGray + 2);
        lineX->SetBit(kCanDelete, false);
        lineX->Draw("same");

        // Dashed horizontal line at y = 0
        TLine *lineY = new TLine(-1.5, 0, 1.5, 0);
        lineY->SetLineStyle(2); // dashed
        lineY->SetLineColor(kGray + 2);
        lineY->SetBit(kCanDelete, false);
        lineY->Draw("same");

        c->BuildLegend();
        c->Update();
        c->Print(outPdf);
    }
    dummy.Print(outPdf + "]");

    return 0;
}