// drawDCATable.C
#include "TROOT.h"

TH1* GetTH1(const char* fileName, std::string histName)
{
    TFile *f = new TFile(fileName, "READ");
    std::cout << "Reading " << histName.c_str() << " from TFile" << std::endl;
    TH1* hist = (TH1*)f->Get(histName.c_str());
    if (hist == nullptr) { std::cout << ">> error retrieving histogram" << std::endl; }
    else { std::cout << ">> histogram sucessfully read from TFile" << std::endl; }
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

int drawDCATable() {

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
    const char *fAnalysisResults = "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root";
    const char *histName;

    // Using the following geometries and (fixed) MCH parameters, according to muon-qa root file structure:
    std::vector<const char*> vGeometries = {
            //"muon-qa_id30697",
            //"muon-qa_id30619",
            //"muon-qa_id31157",
            "muon-qa_id30992",
            "muon-qa_id30993",
            "muon-qa", // GeometryAlignedFix10Fix15Shift04Ch1BNew2
    };
    std::map<std::string, std::string> geometryNames = {
        //{"muon-qa_id30697", "reference"},
        //{"muon-qa_id30619", "GeometryAlignedFix10Fix15ShiftCh1BNew2"},
        //{"muon-qa_id31157", "GeometryAlignedFix10Fix15Shift04Ch1BNew2"},
        {"muon-qa_id30992", "GeometryAlignedFix110Fix15New2T5"},
        {"muon-qa_id30993", "GeometryAlignedFix110Fix15New2T5ShiftY"},
        {"muon-qa", "GeometryAlignedFix110Fix19NewIST7"},
    };

    std::array<const char*, 4> arrQuadrants = { "Q0", "Q1", "Q2", "Q3"};
    std::array<const char*, 2> arrXY = { "x", "y"};
    std::array<const char*, 2> arrPosNeg = { "pos", "neg"};

    ResultMap results;

    // Where the geometries are the following:
    // --- muon-qa_id30619: GeometryAlignedFix10Fix15ShiftCh1BNew2 (bottom only shift)
    // --- muon-qa_id30620: GeometryAlignedFix10Fix15ShiftCh1TBNew2 (top and bottom half shift)
    // --- muon-qa_id30697: GeometryAligned (reference)
    // --- muon-qa_id30991: GeometryAlignedShiftY (global shift Y)
    // --- muon-qa_id30992: GeometryAlignedFix110Fix15New2T5 (better javier new2)
    // --- muon-qa_id30993: GeometryAlignedFix110Fix15New2T5ShiftY (better javier new2 + shift)

    // Fill the result map
    for (const char* geometry : vGeometries) {
        for (const char* plane : arrXY) {
            for (const char* quadrant : arrQuadrants) {
                for (const char* posNeg : arrPosNeg) {
                    histName = Form("%s/Alignment/same-event/DCA/MCH/%s/DCA_%s_%s",
                                    geometry, quadrant, plane, posNeg);

                    TH1* hist = GetTH1(fAnalysisResults, histName);
                    std::string value = "N/A";
                    if (hist) {
                        double mean = hist->GetMean();
                        double stdDev = hist->GetStdDev();
                        char formatted[64];
                        snprintf(formatted, sizeof(formatted), "%.3f ± %.3f", mean, stdDev);
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
    for (const auto& geometry : vGeometries) {
        std::cout << std::setw(colWidth) << std::left << geometryNames[geometry] << "|";
    }
    std::cout << "\n" << std::string(colWidth * (vGeometries.size() + 1), '-') << "\n";

    // Print the data rows (example logic)
    for (const auto& plane : arrXY) {
        std::cout << std::setw(colWidth) << std::left << Form("DCA(%s)", plane) << "\n";
        for (const auto& quadrant : arrQuadrants) {
            std::cout << std::setw(colWidth) << std::left << ("  " + std::string(quadrant)) << "\n";
            for (const auto& posNeg : arrPosNeg) {
                std::cout << std::setw(colWidth) << std::left << ("    " + std::string(posNeg)) << "|";
                for (const auto& geometry : vGeometries) {
                    std::string val = results[plane][quadrant][posNeg][geometry];
                    std::cout << std::setw(colWidth) << std::right << val << "|";
                }
                std::cout << "\n";
            }
        }
    }

    return 0;
}