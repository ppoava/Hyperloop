#!/usr/bin/env python3

"""
Macro for preparation of input text file used for AOD merger  
---- How to run:
python3 getFileList.py -r 505713 -y 2021 -p NOV -u arakotoz
"""

"""
---- Can also run like:
python3 getFileList.py \
    -f AO2D.root \
    -i /alice/cern.ch/user/a/alihyperloop/jobs/0304/hy_30422* \
    -o input_data.txt
Be sure to do `alien-token-init` if necessary !
"""

"""
Use control shift down to select all lines below the cursor line
"""

import argparse
import alienpy.alien as alien
from glob import glob

def getRunFileList(FileName, Input, Output, Local):
    dirs = Input.split(",")  # split comma-separated list

    if Local:
        # Local paths (if any)
        for d in dirs:
            findCmd = f"{d}/*/{FileName}"
            print(findCmd)
            FileList = glob(findCmd, recursive=True)
            print(FileList)
            with open(Output, "a") as outFile:
                for f in FileList:
                    outFile.write(f"{f}\n")
    else:
        # Grid paths
        alien.setup_logging()
        jal = alien.AliEn()
        with open(Output, "a") as outFile:
            for d in dirs:
                findCmd = f"find {d} {FileName} -e AOD"
                print(findCmd)
                out = jal.run(findCmd)
                results = out.ansdict['results']
                for dic in results:
                    outFile.write(f"alien://{dic['lfn']}\n")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Get file lists")
    parser.add_argument("--FileName", "-f", help="Name of file to look for", required=True)
    parser.add_argument("--Input", "-i", help="Comma-separated directories to input files", required=True)
    parser.add_argument("--Output", "-o", help="Output file", default="./input_data.txt")
    parser.add_argument("--Local", "-l", action="store_true", help="Local mode")
    args = parser.parse_args()

    getRunFileList(args.FileName, args.Input, args.Output, args.Local)