#!/bin/bash

# Define the ptMin and ptMax ranges
ptMinValues=(1 2 3 4 5)
ptMaxValues=(2 3 4 5 10 20 30)

# Loop over ptMin values
for ptMin in "${ptMinValues[@]}"; do
    # Loop over ptMax values
    for ptMax in "${ptMaxValues[@]}"; do
        # Only execute if ptMax is greater than ptMin
        if (( ptMax > ptMin )); then
            echo "Running ROOT for ptMin=$ptMin and ptMax=$ptMax"
            root -l -q -b "DiMuonMassSpectrum.C($ptMin, $ptMax)"
        fi
    done
done

