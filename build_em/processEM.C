
#include <iostream>
#include <vector>
#include <string>
#include <TTree.h>
#include <TFile.h>
#include <TKey.h>
#include <TDirectory.h>
#include <TList.h>
#include <TString.h>
#include "TRandom.h"
#include "IndexTableUtils.h"
#include "EMCandidates.h"

void TreeMerging(const char *inputFileName, const char *treeName, TFile *outputFile)
{

    TFile *inputFile = TFile::Open(inputFileName, "READ");
    TList *treeList = new TList();
    TIter nextDir(inputFile->GetListOfKeys());
    TKey *key;
    while ((key = (TKey *)nextDir()))
    {
        std::cout << "Reading directory: " << key->GetName() << std::endl;
        TObject *obj = key->ReadObj();

        if (obj->InheritsFrom(TDirectory::Class()))
        {
            TDirectory *dir = (TDirectory *)obj;
            TTree *tmpTree = (TTree *)dir->Get(treeName);
            treeList->Add(tmpTree);
        }
        else
        {
            std::cerr << "Missing trees in directory: " << key->GetName() << std::endl;
        }
    }

    outputFile->cd();
    TTree *tree = TTree::MergeTrees(treeList);
    tree->Write();
    inputFile->Close();
}

void processEM(bool doMerge = false)
{
    gRandom->SetSeed(1995);
    int mEMDepth = 200;

    if (doMerge)
    {
        std::string inputFileName = "/data3/fmazzasc/lit_run3/DATA/pbpb/AO2D.root";
        std::string treeNameCands = "O2he3hadtable";
        std::string treeNameColls = "O2he3hadmult";
        TreeMerging(inputFileName.c_str(), treeNameCands.c_str(), TFile::Open("inputCands.root", "RECREATE"));
        TreeMerging(inputFileName.c_str(), treeNameColls.c_str(), TFile::Open("inputColls.root", "RECREATE"));
    }

    TFile *inputCandsFile = TFile::Open("inputCands.root");
    TTree *inputCandsTree = (TTree *)inputCandsFile->Get("O2he3hadtable");
    TFile *inputCollsFile = TFile::Open("inputColls.root");
    TTree *inputCollsTree = (TTree *)inputCollsFile->Get("O2he3hadmult");

    IndexTableUtils mUtils;
    std::vector<std::vector<int>> mHadIndices;
    mHadIndices.resize(mUtils.mZetaBins * mUtils.mMultBins + 1);

    CollCandidate collCand;
    He3Candidate he3Cand;
    HadronCandidate hadCand;
    collCand.setCollBranchAddress(inputCollsTree);
    he3Cand.setHe3BranchAddress(inputCandsTree);
    hadCand.setHadronBranchAddress(inputCandsTree);

    std::vector<He3Candidate> mHe3Cands;
    std::vector<HadronCandidate> mHadCands;

    float zvtxHe3; // zvtxHe3 is the zvtx of the last He3 candidate
    for (int iEntry = 0; iEntry < inputCollsTree->GetEntries(); iEntry++)
    {
        inputCollsTree->GetEntry(iEntry);
        inputCandsTree->GetEntry(iEntry);
        hadCand.fZHad = collCand.fZVertex;
        hadCand.fMultHad = collCand.fCentralityFT0C;
        he3Cand.fZHe3 = collCand.fZVertex;
        he3Cand.fMultHe3 = collCand.fCentralityFT0C;
        mHadCands.push_back(hadCand);
        int iBin = mUtils.getBinIndex(hadCand.fZHad, hadCand.fMultHad);
        mHadIndices[iBin].push_back(mHadCands.size() - 1);

        // fillHe3 only once per zvtx
        if (abs(zvtxHe3 - he3Cand.fZHe3) < 1e-6)
        {
            continue;
        }
        mHe3Cands.push_back(he3Cand);
        zvtxHe3 = he3Cand.fZHe3;
    }

    std::cout << "--------------------------------" << std::endl;
    std::cout << "Size of Hadron Candidates to be mixed: " << mHadCands.size() << std::endl;
    std::cout << "Size of He3 Candidates to be mixed: " << mHe3Cands.size() << std::endl;
    std::cout << "--------------------------------" << std::endl;

    // Li4Candidate li4CandME;
    // auto outputFile = TFile::Open("/data3/fmazzasc/lit_run3/EM/pbpb/outputEM.root", "RECREATE");
    // auto outputTree = new TTree("DF", "DF");
    // // flash the Mixed event structure in the output tree
    // outputTree->Branch("O2he3hadtable", &li4CandME);

    // for (int iHe3 = 0; iHe3 < mHe3Cands.size(); iHe3++)
    // {

    //     // print 1% progress
    //     if (iHe3 % (mHe3Cands.size() / 100) == 0)
    //     {
    //         std::cout << "Processing entry: " << iHe3 << " / " << mHe3Cands.size() << ", " << 100 * iHe3 / mHe3Cands.size() << "%" << std::endl;
    //     }

    //     auto &he3Cand = mHe3Cands[iHe3];
    //     li4CandME.setHe3(he3Cand);
    //     int iBin = mUtils.getBinIndex(he3Cand.fZHe3, he3Cand.fMultHe3);
    //     int iDepth = 0;

    //     for (int iHad = 0; iHad < mHadIndices[iBin].size(); iHad++)
    //     {
    //         if (iDepth >= mEMDepth)
    //         {
    //             // std::cout << "Reached maximum depth of " << mEMDepth << " for this event" << std::endl;
    //             break;
    //         }
    //         auto &hadCand = mHadCands[mHadIndices[iBin][iHad]];
    //         li4CandME.setHadron(hadCand);

    //         if ((abs(he3Cand.fZHe3 - hadCand.fZHad) < 1e-6 && abs(he3Cand.fMultHe3 - hadCand.fMultHad) < 1e-6) || li4CandME.calcInvMass() > 4.15314)
    //         {
    //             continue;
    //         }

    //         li4CandME.fZVertex = (he3Cand.fZHe3 + hadCand.fZHad) / 2;
    //         li4CandME.fCentralityFT0C = (he3Cand.fMultHe3 + hadCand.fMultHad) / 2;
    //         outputTree->Fill();
    //         iDepth++;
    //     }
    // }
}