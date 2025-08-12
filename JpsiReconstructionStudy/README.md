## Useful links
- CERNBox with AnalysisResults files (from Hyperloop trains): [link](https://cernbox.cern.ch/s/QKzp9k7555DEG4a)
- CERNBox with previous results (from analysing AnalysisResults with macros): [link](https://cernbox.cern.ch/s/grt0xWM5EtEKgdD)
- Hyperloop analysis page: [link](https://alimonitor.cern.ch/hyperloop/view-analysis/51136)  
  - Note that there are a lot of AnalysisResults files uploaded to the CERNBox. The easiest way to find more recent productions is to look at the `Hyperloop_dd_mm_yyyy` tag. Alternatively, the most recent productions can still be found on the Hyperloop page.

## Overview of files
The three most useful scripts are the following:  
- `drawDCATable.C`  
- `run_DiMuonMassSpectrum-muonQA.sh`  
- `run_muonQA_pT_eta_single_muons.sh`  

  The first macro (`drawDCATable.C`) produces DCA tables and figures.  
  - One needs to change the `const char *AnalysisResults` to the correct file to be used (for the moment can only be 1 file input at the same time) and then add the `muon_id` and name to be displayed (in the `vGeometries` vector).

  The two bash scripts should work as-is, but theory and practice do not always coincide.  
  - They both call other macros to do the work. One for dimuons (producing the TT, TB, BB plots, as well as the Jpsi fitting and retrieving the width and peak), the other for single muons (producing T+B-, B-T+ and single muon distributions for several variables, within Jpsi range and for different Jpsi pT).  
    - For dimuons: `run_DiMuonMassSpectrum-muonQA.sh` → `DiMuonMassSpectrum-muonQA.C`  
    - For single muons: `run_muonQA_pT_eta_single_muons.sh` → `muonQA_pT_eta_single_muons.C`  
  - The structure for both bash scripts is very similar, so I will describe them in one go.  
    - For example, for dimuons, the command is:  
      ```bash
      root -l -q -b 'DiMuonMassSpectrum-muonQA.C("output_name", "top-bottom/left-right", "generic/special/dimuons(last one only for the single muon plots)", "input_file_reference_geometry", "muon_id for reference geometry", "input_file_new_geometry", "muon_id for new geometry")'
      ```
      - The easiest is to copy the relevant previous line (e.g. for a certain data taking period) and then modify appropriately the things that change — usually the name, the input file for the new geometry, and the muon_id.  
      - In particular, the last one (`muon_id`) can have a slightly unusual structure, so it's easiest to copy it from an existing example.

## Note on Hyperloop trains
To run a Hyperloop train on a new geometry, it’s easiest to copy an already existing wagon in the **MCH-MFT relative alignment** page ([link](https://alimonitor.cern.ch/hyperloop/view-analysis/51136)). Take one with the name `muon-qa-realign-etc-etc`. You can change the name to include the new geometry for clarity.  
    - Note: when running just one geometry, it is useful to run also the `DUMMY` train so that both configurations get their own `muon_id`. This `muon_id` is consistent across different trains and data sets for the same geometry. If you only run one geometry, the `muon_id` will simply be `muon_id`, which could give issues if you merge with another train run that also only had one geometry (it will have the same `muon_id`, but being a different geometry..)

The only parameters that need to be changed are:  
- `geoPathRealign` (obviously)  
- `ccdb-no-later-than-new`  
- `ccdb-no-later-than-ref`  

Appropriate values can be found in the **CCDB** pages for the geometry ([link](https://alice-ccdb.cern.ch/browse)).