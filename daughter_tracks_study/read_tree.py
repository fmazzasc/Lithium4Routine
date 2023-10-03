import ROOT
import uproot
import pandas as pd
import numpy as np
import argparse
import os
import sys
sys.path.append("../")
import utils

ROOT.gROOT.SetBatch(True)

def fill_th1_hist(h, df, var):
    for var_val in df[var]:
        h.Fill(var_val)

def fill_th2_hist(h, df, var1, var2):
    for var1_val, var2_val in zip(df[var1], df[var2]):
        h.Fill(var1_val, var2_val)



path_dir = "../../lithium_results/"
tree = uproot.open(path_dir + "/" + f"DauTreeMC.root")["DauTreeMC"].arrays(library="pd")

tree.query("itsTPCPt > 0 and genPt<2 and abs(pdg)==1000020030", inplace=True)
tree["itsTPCPt"] *= 2
tree["tpcPt"] *= 2
print(tree.columns)

print(tree[["genPt", "itsTPCPt", "tpcPt","isITSTPCfake", "isITSfake", "isTPCfake"]])