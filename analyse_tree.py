import os
import uproot
import ROOT
import pandas as pd
import numpy as np
from hipe4ml.tree_handler import TreeHandler
import argparse
import yaml

import sys
sys.path.append('utils')
import utils as utils

parser = argparse.ArgumentParser(description='Configure the parameters of the script.')

parser.add_argument('--config-file', dest='config_file',
                    help="path to the YAML file with configuration.", default='')
args = parser.parse_args()

if args.config_file == "":
    print("No config file provided, exiting...")
    exit(1)



config_file = open(args.config_file, 'r')
config = yaml.full_load(config_file)
mc = config['mc']
input_files_name = config['input_files']
output_dir_name = config['output_dir']
output_file_name = config['output_file']
selections = config['selection']
is_matter = config['is_matter']

# creating the dataframe
tree_name = 'O2he3hadtable' if not mc else 'O2he3hadtablemc'
tree_hdl = TreeHandler(input_files_name, tree_name, folder_name='DF*')
if (len(tree_hdl)== 0):
    print("No trees found in the file, trying with EM based tree")
    tree_name = 'DF/O2he3hadtable'
    tree_hdl = TreeHandler(input_files_name, tree_name)
    if (len(tree_hdl)== 0):
        print("No trees found in the file, exiting...")
        exit(1)

df = tree_hdl.get_data_frame()
# try to convert
utils.correct_and_convert_df(df, False)
print('df head: ', df.head())



### output histograms----------------------------------------------------------

hRecInvMass = ROOT.TH1F("hRecInvMass", ";m_{^{3}He + p} [GeV]; Counts ",  80, 3.743, 3.85)
hPtRec = ROOT.TH1F("hPtRec", "; #it{p}_{T}; Counts", 50, 0, 10)
hPtHe3 = ROOT.TH1F("hPtHe3", "; #it{p}_{T}; Counts", 50, 0, 10)
hPtPr = ROOT.TH1F("hPtPr", "; #it{p}_{T}; Counts", 50, 0, 10)
hNSigmaHe3 = ROOT.TH1F("hNSigmaHe3", "hNSigmaHe3", 100, -4, 4)
h2NSigmaProtonPt = ROOT.TH2F("h2NSigmaProtonPt", ";p_{T}^{p};n#sigma_{p}", 100, 0, 2, 100, -4, 4)
h2MassNsigmaHe3 = ROOT.TH2F("h2MassNsigmaHe3", ";m_{^{3}He + p} [GeV];n#sigma_{^{3}He}", 100, 3.743, 3.85, 100, -4, 4)
h2MassNClusTPCHe3 = ROOT.TH2F("h2MassNClusTPCHe3", ";m_{^{3}He + p} [GeV];N_{clus}^{TPC}", 100, 3.743, 3.85, 130, 30.5, 160.5)
h2He3PtNClusTPCHe3 = ROOT.TH2F("h2He3PtNClusTPCHe3", ";p_{T}^{^{3}He};N_{clus}^{TPC}", 100, 0, 6, 130, 30.5, 160.5)
h2MassPtHe3 = ROOT.TH2F("h2MassPtHe3", ";;p_{T}^{^{3}He}; m_{^{3}He + p} [GeV]", 100, 0, 6, 100, 3.743, 3.85)
h2MassPtReco = ROOT.TH2F("h2MassPtReco", ";;p_{T}^{rec}; m_{^{3}He + p} [GeV]", 100, 0, 6, 100, 3.743, 3.85)
h2PtRecoNsigmaHe3 = ROOT.TH2F("h2PtRecoNsigmaHe3", ";p_{T}^{rec};n#sigma_{^{3}He}", 100, 0, 6, 100, -4, 4)

h2MassPtProton = ROOT.TH2F("h2MassPtProton", ";m_{^{3}He + p} [GeV];p_{T}^{p}", 100, 3.743, 3.85, 100, 0, 2)
h2MassDCAxyHe3 = ROOT.TH2F("h2MassDCAxyHe3", ";m_{^{3}He + p} [GeV];DCA_{xy}^{^{3}He}", 100, 3.743, 3.85, 100, -0.03, 0.03)
h2MassDCAzHe3 = ROOT.TH2F("h2MassDCAzHe3", ";m_{^{3}He + p} [GeV];DCA_{z}^{^{3}He}", 100, 3.743, 3.85, 100, -0.01, 0.01)
h2PtHe3PtPr = ROOT.TH2F("h2PtHe3PtPr", ";p_{T}^{^{3}He};p_{T}^{p}", 100, 0, 6, 100, 0, 3)
h2TPCSignalHe3 = ROOT.TH2F("h2TPCSignalHe3", ";p^{TPC}{^{3}He}/z;TPC signal", 100, 0, 6, 100, 0, 2000)
h2TOFMassHe3 = ROOT.TH2F("h2TOFMassHe3", ";m_{^{3}He + p} [GeV];TOF mass", 100, 3.743, 3.85, 100, 1, 5)
h2TOFMassPr = ROOT.TH2F("h2TOFMassPr", ";p_{T}^{p};TOF mass", 100, 0, 3, 100, 0.7, 1.4)


hGenMass = ROOT.TH1F("hLitGenMass", ";m_{^{3}He + p} [GeV]; Counts", 100, 3.743, 4.)
hPtGen = ROOT.TH1F("hPtGen", "; #it{p}_{T}; Counts", 50, 1, 10)
hResolutionPtvsPt = ROOT.TH2F("hResolutionPtvsPt", ";#it{p}_{T}^{gen} (GeV/#it{c});(#it{p}_{T}^{rec} - #it{p}_{T}^{gen}) / #it{p}_{T}^{gen}", 50, 1, 10, 50, -0.2, 0.2)
hResolutionPtvsMass = ROOT.TH2F("hResolutionPtvsMass", ";m_{^{3}He + p} (GeV);(#it{p}_{T}^{rec} - #it{p}_{T}^{gen}) / #it{p}_{T}^{gen}", 100, 3.743, 4., 50, -0.2, 0.2)
hResolutionPtvsNTPCClus = ROOT.TH2F("hResolutionPtvsNTPCClus", ";N_{clus}^{TPC};(#it{p}_{T}^{rec} - #it{p}_{T}^{gen}) / #it{p}_{T}^{gen}", 130, 30.5, 160.5, 50, -0.2, 0.2)
#-----------------------------------------------------------------------------   
if selections == '':
    if is_matter == 1:
        selections = 'fSignedPtHe3 > 0 and fSignedPtHad > 0'
    elif is_matter == 0:
        selections = 'fSignedPtHe3 < 0 and fSignedPtHad < 0'
else:
    if is_matter == 1:
        selections = selections + ' and fSignedPtHe3 > 0 and fSignedPtHad > 0'
    elif is_matter == 0:
        selections = selections + ' and fSignedPtHe3 < 0 and fSignedPtHad < 0'

print("selections: ", selections)
# apply selections
if selections != '':
    df.query(selections, inplace=True)
    df.reset_index(drop=True, inplace=True)
if mc:
    if is_matter == 1: ## matter
        utils.fill_th1_hist_abs(hPtGen, df.query('fSignedPtMC>0', inplace=False), 'fSignedPtMC')
    elif is_matter == 0: ## anti-matter
        utils.fill_th1_hist_abs(hPtGen, df.query('fSignedPtMC<0', inplace=False), 'fSignedPtMC')
    else: ## both
        utils.fill_th1_hist_abs(hPtGen, df, 'fSignedPtMC')    
    utils.fill_th1_hist(hGenMass, df, "fMassMC")

    df.eval('resPt = (fPt - abs(fSignedPtMC))/abs(fSignedPtMC)', inplace=True)
    utils.fill_th2_hist_abs(hResolutionPtvsPt, df, 'fSignedPtMC', 'resPt')
    utils.fill_th2_hist(hResolutionPtvsMass, df, 'fMass', 'resPt')
    utils.fill_th2_hist(hResolutionPtvsNTPCClus, df, 'fNClsTPCHe3', 'resPt')
    df.query('fPt > 0', inplace=True)



print('df columns: ', df.columns)


utils.fill_th1_hist(hPtRec, df, "fPt")
utils.fill_th1_hist(hPtHe3, df, "fPtHe3")
utils.fill_th1_hist(hPtPr, df, "fPtHad")
utils.fill_th1_hist(hRecInvMass, df, "fMass")
utils.fill_th1_hist(hNSigmaHe3, df, "fNSigmaTPCHe3")
utils.fill_th2_hist(h2MassNsigmaHe3, df, "fMass", "fNSigmaTPCHe3")
utils.fill_th2_hist(h2MassNClusTPCHe3, df, "fMass", "fNClsTPCHe3")
utils.fill_th2_hist(h2He3PtNClusTPCHe3, df, "fPtHe3", "fNClsTPCHe3")
utils.fill_th2_hist(h2NSigmaProtonPt, df, "fPtHad", "fNSigmaTPCHad")
utils.fill_th2_hist(h2MassPtHe3, df, "fPtHe3", "fMass")
utils.fill_th2_hist(h2MassPtReco, df, "fPt", "fMass")
utils.fill_th2_hist(h2MassPtProton, df, "fMass", "fPtHad")
utils.fill_th2_hist(h2PtRecoNsigmaHe3, df, "fPt", "fNSigmaTPCHe3")
utils.fill_th2_hist(h2MassDCAxyHe3, df, "fMass", "fDCAxyHe3")
utils.fill_th2_hist(h2MassDCAzHe3, df, "fMass", "fDCAzHe3")
utils.fill_th2_hist(h2PtHe3PtPr, df, "fPtHe3", "fPtHad")
utils.fill_th2_hist(h2TPCSignalHe3, df, "fInnerParamTPCHe3", "fSignalTPCHe3")
utils.fill_th2_hist(h2TOFMassHe3, df, "fMass", "fMassTOFHe3")
utils.fill_th2_hist(h2TOFMassPr, df, "fPtHad", "fMassTOFHad")


###compute eff vs pT
hEff = hPtRec.Clone("hEffPt")
hEff.Sumw2()
hEff.Divide(hPtGen)




if not os.path.isdir(output_dir_name):
    os.makedirs(output_dir_name)

ffile = ROOT.TFile(output_dir_name + output_file_name, "RECREATE")
hRecInvMass.Write()
hPtRec.Write()
hPtHe3.Write()
hPtPr.Write()
hNSigmaHe3.Write()
h2NSigmaProtonPt.Write()

h2MassNsigmaHe3.Write()
h2MassPtHe3.Write()
h2MassNClusTPCHe3.Write()
h2He3PtNClusTPCHe3.Write()
h2MassPtProton.Write()
h2MassPtReco.Write()
h2MassDCAxyHe3.Write()
h2MassDCAzHe3.Write()
h2PtRecoNsigmaHe3.Write()
h2PtHe3PtPr.Write()
h2TPCSignalHe3.Write()
h2TOFMassHe3.Write()
h2TOFMassPr.Write()

hPtGen.Write()
hEff.Write()
hResolutionPtvsPt.Write()
hResolutionPtvsMass.Write()
hResolutionPtvsNTPCClus.Write()
hGenMass.Write()


ffile.Close()