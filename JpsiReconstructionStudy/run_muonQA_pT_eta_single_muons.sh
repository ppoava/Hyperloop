#!/bin/bash

# Description of muon IDs:
# (***) muon-qa_id30619: GeometryAlignedFix10Fix15ShiftCh1BNew2 (bottom only shift)
# (***) muon-qa_id31157: GeometryAlignedFix10Fix15Shift04Ch1BNew2: bottom only CH1 shift with half the amount
# muon-qa_id30620: GeometryAlignedFix10Fix15ShiftCh1TBNew2 (top and bottom half shift)
# muon-qa_id30697: GeometryAligned (reference)
# muon-qa_id30991: GeometryAlignedShiftY (global shift Y)
# (***) muon-qa_id30992: GeometryAlignedFix110Fix15New2T5 (better javier new2)
# (***) muon-qa_id30993: GeometryAlignedFix110Fix15New2T5ShiftY (better javier new2 + shift)
# (***) muon-qa: GeometryAlignedFix110Fix19NewIST7 (no ID yet..)

##### LHC24an_pass1_skimmed #####
# top-bottom
# --- generic
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24an_top-bottom_GeometryAlignedFix10Fix15ShiftCh1BNew2", "top-bottom", "draw_generic", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30619/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24an_top-bottom_GeometryAlignedFix10Fix15Shift04Ch1BNew2", "top-bottom", "draw_generic", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id31157/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24an_top-bottom_GeometryAlignedFix110Fix15New2T5", "top-bottom", "draw_generic", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30992/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24an_top-bottom_GeometryAlignedFix110Fix15New2T5ShiftY", "top-bottom", "draw_generic", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30993/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24an_top-bottom_GeometryAlignedFix110Fix19NewIST7", "top-bottom", "draw_generic", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa/muons/")'

# left-right
# --- generic
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24an_left-right_GeometryAlignedFix10Fix15ShiftCh1BNew2", "left-right", "draw_generic", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30619/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24an_left-right_GeometryAlignedFix10Fix15Shift04Ch1BNew2", "left-right", "draw_generic", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id31157/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24an_left-right_GeometryAlignedFix110Fix15New2T5", "left-right", "draw_generic", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/dimuon/muons/", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30992/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24an_left-right_GeometryAlignedFix110Fix15New2T5ShiftY", "left-right", "draw_generic", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30993/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24an_left-right_GeometryAlignedFix110Fix19NewIST7", "left-right", "draw_generic", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24an_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa/muons/")'

##### LHC24am_pass1_skimmed #####
# top-bottom
# --- generic
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24am_top-bottom_GeometryAlignedFix10Fix15ShiftCh1BNew2", "top-bottom", "draw_generic", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30619/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24am_top-bottom_GeometryAlignedFix10Fix15Shift04Ch1BNew2", "top-bottom", "draw_generic", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id31157/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24am_top-bottom_GeometryAlignedFix110Fix15New2T5", "top-bottom", "draw_generic", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30992/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24am_top-bottom_GeometryAlignedFix110Fix15New2T5ShiftY", "top-bottom", "draw_generic", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30993/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24am_top-bottom_GeometryAlignedFix110Fix19NewIST7", "top-bottom", "draw_generic", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa/muons/")'

# left-right
# --- generic
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24am_left-right_GeometryAlignedFix10Fix15ShiftCh1BNew2", "left-right", "draw_generic", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30619/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24am_left-right_GeometryAlignedFix10Fix15Shift04Ch1BNew2", "left-right", "draw_generic", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id31157/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24am_left-right_GeometryAlignedFix110Fix15New2T5", "left-right", "draw_generic", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/dimuon/muons/", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30992/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24am_left-right_GeometryAlignedFix110Fix15New2T5ShiftY", "left-right", "draw_generic", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30993/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("LHC24am_left-right_GeometryAlignedFix110Fix19NewIST7", "left-right", "draw_generic", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_LHC24am_pass1_skimmed_all_geometries_Hyperloop_11_07_2025.root", "muon-qa/muons/")'

#### merged LHC24an_LHC24am_pass1_skimmed #####
# top-bottom
# --- generic
root -l -q -b 'muonQA_pT_eta_single_muons.C("merged_LHC24an_LHC24am_top-bottom_GeometryAlignedFix10Fix15ShiftCh1BNew2", "top-bottom", "draw_generic", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30619/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("merged_LHC24an_LHC24am_top-bottom_GeometryAlignedFix10Fix15Shift04Ch1BNew2", "top-bottom", "draw_generic", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id31157/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("merged_LHC24an_LHC24am_top-bottom_GeometryAlignedFix110Fix15New2T5", "top-bottom", "draw_generic", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30992/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("merged_LHC24an_LHC24am_top-bottom_GeometryAlignedFix110Fix15New2T5ShiftY", "top-bottom", "draw_generic", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30993/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("merged_LHC24an_LHC24am_top-bottom_GeometryAlignedFix110Fix19NewIST7", "top-bottom", "draw_generic", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa/muons/")'

# left-right
# --- generic
root -l -q -b 'muonQA_pT_eta_single_muons.C("merged_LHC24an_LHC24am_left-right_GeometryAlignedFix10Fix15ShiftCh1BNew2", "left-right", "draw_generic", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30619/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("merged_LHC24an_LHC24am_left-right_GeometryAlignedFix10Fix15Shift04Ch1BNew2", "left-right", "draw_generic", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id31157/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("merged_LHC24an_LHC24am_left-right_GeometryAlignedFix110Fix15New2T5", "left-right", "draw_generic", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/dimuon/muons/", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30992/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("merged_LHC24an_LHC24am_left-right_GeometryAlignedFix110Fix15New2T5ShiftY", "left-right", "draw_generic", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30993/muons/")'
root -l -q -b 'muonQA_pT_eta_single_muons.C("merged_LHC24an_LHC24am_left-right_GeometryAlignedFix110Fix19NewIST7", "left-right", "draw_generic", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa_id30697/muons/", "AnalysisResults-muonQA_merged_LHC24an_LHC24am_all_geometries_Hyperloop_11_07_2025.root", "muon-qa/muons/")'
