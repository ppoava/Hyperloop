// harryPlotter.C

#include <TFile.h>
#include <TH1.h>
#include <TDirectory.h>
#include <iostream>

TTree* getTH1(std::string filename) {
    // Open the ROOT file
    TFile* file = TFile::Open(filename.c_str(), "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
    }

    std::cout << "Opened file: " << filename << std::endl;

    // Change to the first directory (adjust if needed)
    /*
    TDirectory* dir = file->GetDirectory("DF_2423967966473344");
    if (!dir) {
        std::cerr << "Error: Cannot find directory DF_2423967966473344" << std::endl;
        file->Close();
    }
    dir->cd();
    */

    TTree* tree = dynamic_cast<TTree*>(file->Get("O2rtdimuonall"));
    if (!tree) {
        std::cerr << "Error: Cannot find TTree O2rtdimuonall" << std::endl;
        file->Close();
        return nullptr;
    }

    std::cout << "ROOT file contains the following histograms: " << std::endl;
    tree->Print();

    return tree;

    // Keep the file open if needed
    // file->Close(); // optionally close if not needed interactively
}

int harryPlotter() {
    TTree *tree = getTH1("AO2D_merged.root");

    TH1F* hMass = new TH1F("hMass", "fMass distribution", 100, 2, 4); // adjust bins/range
    tree->Draw("fMass>>hMass");  // fills histogram from branch

    return 0;
}