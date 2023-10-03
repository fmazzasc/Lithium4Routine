import numpy as np
import pandas as pd
import ROOT

ROOT.gROOT.SetBatch(True)




def fill_th1_hist(h, df, var):
    for var_val in df[var]:
        h.Fill(var_val)


def fill_th1_hist_abs(h, df, var):
    for var_val in df[var]:
        h.Fill(abs(var_val))


def fill_th2_hist(h, df, var1, var2):
    for var1_val, var2_val in zip(df[var1], df[var2]):
        h.Fill(var1_val, var2_val)


def fill_th2_hist_abs(h, df, var1, var2):
    for var1_val, var2_val in zip(df[var1], df[var2]):
        h.Fill(abs(var1_val), var2_val)


def correct_and_convert_df(df, histo=None):

    # df['fPtHe3'] = df['fPtHe3'] + 2.98019e-02 + 7.66100e-01 * np.exp(-1.31641e+00 * df['fPtHe3'])

    # if not type(df) == pd.DataFrame:
    #     df = df._full_data_frame
    # # correct 3He momentum
    # if not histo == None:
    #     cloned_pt_arr = np.array(df['fPtHe3'])
    #     for i in range(len(cloned_pt_arr)):
    #         pt_shift = histo.GetBinContent(histo.FindBin(cloned_pt_arr[i]))
    #         cloned_pt_arr[i] -= pt_shift
    #     df['fPtHe3'] = cloned_pt_arr
    
    # df['fPtHe3'] = df['fPtHe3'] + 2.98019e-02 + 7.66100e-01 * np.exp(-1.31641e+00 * df['fPtHe3'])

    # 3He momentum
    df.eval('fPxHe3 = fPtHe3 * cos(fPhiHe3)', inplace=True)
    df.eval('fPyHe3 = fPtHe3 * sin(fPhiHe3)', inplace=True)
    df.eval('fPzHe3 = fPtHe3 * sinh(fEtaHe3)', inplace=True)
    df.eval('fPHe3 = fPtHe3 * cosh(fEtaHe3)', inplace=True)
    df.eval('fEnHe3 = sqrt(fPHe3**2 + 2.8083916**2)', inplace=True)

    # pi momentum
    df.eval('fPxPr = fPtPr * cos(fPhiPr)', inplace=True)
    df.eval('fPyPr = fPtPr * sin(fPhiPr)', inplace=True)
    df.eval('fPzPr = fPtPr * sinh(fEtaPr)', inplace=True)
    df.eval('fPPr = fPtPr * cosh(fEtaPr)', inplace=True)
    df.eval('fEnPr = sqrt(fPPr**2 + 0.93827**2)', inplace=True)
    # hypertriton momentum
    df.eval('fPx = fPxHe3 + fPxPr', inplace=True)
    df.eval('fPy = fPyHe3 + fPyPr', inplace=True)
    df.eval('fPz = fPzHe3 + fPzPr', inplace=True)
    df.eval('fP = sqrt(fPx**2 + fPy**2 + fPz**2)', inplace=True)
    df.eval('fEn = fEnHe3 + fEnPr', inplace=True)

    # Momentum variables to be stored
    df.eval('fPt = sqrt(fPx**2 + fPy**2)', inplace=True)
    df.eval('fEta = arccosh(fP/fPt)', inplace=True)
    df.eval('fPhi = arctan(fPy/fPx)', inplace=True)


    df.eval('fMass = sqrt(fEn**2 - fP**2)', inplace=True)
    # remove useless columns
    df.drop(columns=['fPxHe3', 'fPyHe3', 'fPzHe3', 'fPHe3', 'fEnHe3', 'fPxPr', 'fPyPr', 'fPzPr', 'fPPr', 'fEnPr', 'fPx', 'fPy', 'fPz', 'fP', 'fEn'])

    return df