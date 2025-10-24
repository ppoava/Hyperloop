// run this to get a list of files to merge
// find ~/alice/Hyperloop/OOData -name "AO2D.root" > local_file_list.txt

#include <fstream>
#include <iostream>
#include <string>
#include <TChain.h>
#include <TFile.h>
#include <TKey.h>
#include <TROOT.h>

using namespace std;

// --- LoadData: dynamically finds directories that contain the tree
void LoadData(TChain *fChain, const std::string &TreeName, const std::string &FileName) {
    TFile *f = TFile::Open(FileName.c_str());
    if (!f || f->IsZombie()) {
        cerr << "⚠️ Skipping file (cannot open or corrupted): " << FileName << endl;
        return;
    }

    // Optional: check file size to skip obviously truncated files
    if (f->GetSize() < 1024*1024) { // less than 1 MB
        cerr << "⚠️ Skipping file (too small, probably truncated): " << FileName << endl;
        f->Close();
        return;
    }

    TIter keyList(f->GetListOfKeys());
    TKey *key;
    int counter = 0;
    while ((key = (TKey*)keyList()) && counter < 100) {
        std::cout << "counter = " << counter << std::endl;
        TObject *obj = key->ReadObj();
        TDirectory *dir = dynamic_cast<TDirectory*>(obj);
        if (!dir) continue;

        TTree *tree = (TTree*)dir->Get(TreeName.c_str());
        if (!tree) continue;

        cout << "Adding tree from directory: " << dir->GetName() << " in file: " << FileName << endl;
        fChain->Add(Form("%s?#%s/%s", FileName.c_str(), dir->GetName(), TreeName.c_str()));
        counter++;
    }

    std::cout << "Trying to close file" << std::endl;

    f->Close();
}

// --- mergeTreesFromList: reads a file list and merges all trees into a single file
void mergeTreesFromList(const std::string &fileList = "local_file_list.txt",
                        const std::string &TreeName = "O2rtdimuonall",
                        const std::string &outputFile = "AO2D_merged.root") {
    TChain *chain = new TChain(TreeName.c_str());
    ifstream in(fileList);
    if (!in.is_open()) {
        cerr << "Cannot open file list: " << fileList << endl;
        return;
    }

    string fname;
    while (getline(in, fname)) {
        if (fname.empty()) continue;
        LoadData(chain, TreeName, fname);
    }

    cout << "---- Total entries found: " << chain->GetEntries() << endl;
    if (chain->GetEntries() == 0) {
        cerr << "No entries found! Check file paths and TreeName: " << TreeName << endl;
        return;
    }

    cout << "--- Merging trees into: " << outputFile << endl;
    chain->Merge(outputFile.c_str());
    cout << "Merge complete." << endl;
}
