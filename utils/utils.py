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


def correct_and_convert_df(df, calibrate_he3_pt=False):

    kDefaultPID = 15
    kPionPID = 2
    kTritonPID = 6

    if not type(df) == pd.DataFrame:
        df = df._full_data_frame

    if 'fFlags' in df.columns:
        df['fHePIDHypo'] = np.right_shift(df['fFlags'], 4)
        df['fHadPIDHypo'] = np.bitwise_and(df['fFlags'], 0b1111)
    
    if not 'fTPCChi2He' in df.columns:
        ## set dummy column to one
        df['fTPCChi2He'] = 1

    # correct 3He momentum    

    if calibrate_he3_pt:
        # print(df.query('fIsReco==True')['fHePIDHypo'])
        no_pid_mask = np.logical_and(df['fHePIDHypo'] != kDefaultPID, df['fHePIDHypo'] != kPionPID)

        # if (no_pid_mask.sum() == 0):
        #     print("PID in tracking not detected, using old momentum re-calibration")
        #     df["fPtHe3"] += 2.98019e-02 + 7.66100e-01 * np.exp(-1.31641e+00 * df["fPtHe3"]) ### functional form given by mpuccio
        # else:
        #     print("PID in tracking detected, using new momentum re-calibration")
        #     df_Trit_PID = df.query('fHePIDHypo==6')
        #     df_else = df.query('fHePIDHypo!=6')
        #     ##pt_new = pt + kp0 + kp1 * pt + kp2 * pt^2 curveParams = {'kp0': -0.200281,'kp1': 0.103039,'kp2': -0.012325}, functional form given by G.A. Lucia
        #     df_Trit_PID["fPtHe3"] += -0.1286 - 0.1269 * df_Trit_PID["fPtHe3"] + 0.06 * df_Trit_PID["fPtHe3"]**2
        #     df_new = pd.concat([df_Trit_PID, df_else])
        #     ## assign the new dataframe to the original one
        #     df[:] = df_new.values


    # 3He momentum
    df['fSignedPtHe3'] = df['fPtHe3']
    df['fPtHe3'] = abs(df['fPtHe3'])
    df['fSignedPtHad'] = df['fPtHad']
    df['fPtHad'] = abs(df['fPtHad'])
    df.eval('fPxHe3 = fPtHe3 * cos(fPhiHe3)', inplace=True)
    df.eval('fPyHe3 = fPtHe3 * sin(fPhiHe3)', inplace=True)
    df.eval('fPzHe3 = fPtHe3 * sinh(fEtaHe3)', inplace=True)
    df.eval('fPHe3 = fPtHe3 * cosh(fEtaHe3)', inplace=True)
    df.eval('fEnHe3 = sqrt(fPHe3**2 + 2.8083916**2)', inplace=True)

    # pi momentum
    df.eval('fPxPr = fPtHad * cos(fPhiHad)', inplace=True)
    df.eval('fPyPr = fPtHad * sin(fPhiHad)', inplace=True)
    df.eval('fPzPr = fPtHad * sinh(fEtaHad)', inplace=True)
    df.eval('fPPr = fPtHad * cosh(fEtaHad)', inplace=True)
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

    if "fItsClusterSizeHe3" in df.columns:
    ## loop over the candidates and compute the average cluster size
        clSizesHe = df['fItsClusterSizeHe3'].to_numpy()
        clSizesPr = df['fItsClusterSizeHad'].to_numpy()
        clSizeHeAvg = np.zeros(len(clSizesHe))
        clSizePiAvg = np.zeros(len(clSizesPr))
        nHitsHe = np.zeros(len(clSizesHe))
        nHitsPi = np.zeros(len(clSizesPr))
        for iLayer in range(7):
            clSizeHeAvg += np.right_shift(clSizesHe, 4*iLayer) & 0b1111
            clSizePiAvg += np.right_shift(clSizesPr, 4*iLayer) & 0b1111
            nHitsHe += np.right_shift(clSizesHe, 4*iLayer) & 0b1111 > 0
            nHitsPi += np.right_shift(clSizesPr, 4*iLayer) & 0b1111 > 0

        clSizeHeAvg /= nHitsHe
        clSizePiAvg /= nHitsPi
        df['fAvgClusterSizeHe'] = clSizeHeAvg
        df['fAvgClusterSizeHad'] = clSizePiAvg
        df['nITSHitsHe'] = nHitsHe
        df['nITSHitsHad'] = nHitsPi


    # remove useless columns
    df.drop(columns=['fPxHe3', 'fPyHe3', 'fPzHe3', 'fPHe3', 'fEnHe3', 'fPxPr', 'fPyPr', 'fPzPr', 'fPPr', 'fEnPr', 'fPx', 'fPy', 'fPz', 'fP', 'fEn'])

    return df