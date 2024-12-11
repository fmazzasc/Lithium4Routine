
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

void testEM(bool doMerge = true)
{
    gRandom->SetSeed(1995);
    int mEMDepth = 5;

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
    std::vector<std::vector<int>> mHe3CollsIndices;
    mHe3CollsIndices.resize(mUtils.mZetaBins * mUtils.mMultBins + 1);

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

        if (abs(zvtxHe3 - he3Cand.fZHe3) < 1e-6)
        { // fillHe3 only once per zvtx
            continue;
        }

        int iBin = mUtils.getBinIndex(he3Cand.fZHe3, he3Cand.fMultHe3);
        mHe3Cands.push_back(he3Cand);
        mHe3CollsIndices[iBin].push_back(mHe3Cands.size() - 1);
        zvtxHe3 = he3Cand.fZHe3;
    }

    std::cout << "--------------------------------" << std::endl;
    std::cout << "Size of Hadron Candidates to be mixed: " << mHadCands.size() << std::endl;
    std::cout << "Size of He3 Candidates to be mixed: " << mHe3Cands.size() << std::endl;
    std::cout << "--------------------------------" << std::endl;

    Li4Candidate li4CandME;
    auto outputFile = TFile::Open("outputEM.root", "RECREATE");
    auto outputTree = new TTree("outputTree", "outputTree");
    // flash the Mixed event structure in the output tree
    outputTree->Branch("MixedEvent", &li4CandME);

    for (int iHad = 0; iHad < mHadCands.size(); iHad++)
    {

        // print 1% progress
        if (iHad % (mHadCands.size() / 100) == 0)
        {
            std::cout << "Processing entry: " << iHad << " / " << mHadCands.size() << ", " << 100 * iHad / mHadCands.size() << "%" << std::endl;
        }

        auto &hadCand = mHadCands[iHad];
        li4CandME.setHadron(hadCand);
        int iBin = mUtils.getBinIndex(hadCand.fZHad, hadCand.fMultHad);
        int iDepth = 0, iSameColl = 0;

        while (iDepth < mEMDepth)
        {

            if (iSameColl > 10) // avoid hanging condition
            {
                break;
            }

            auto &collIndices = mHe3CollsIndices[iBin];
            int iCand = gRandom->Integer(collIndices.size());
            auto &he3Cand = mHe3Cands[collIndices[iCand]];

            if (abs(he3Cand.fZHe3 - hadCand.fZHad) < 1e-6 && abs(he3Cand.fMultHe3 - hadCand.fMultHad) < 1e-6)
            {
                iSameColl++;
                continue;
            }

            li4CandME.setHe3(mHe3Cands[iCand]);
            li4CandME.fZVertex = (he3Cand.fZHe3 + hadCand.fZHad) / 2;
            li4CandME.fCentralityFT0C = (he3Cand.fMultHe3 + hadCand.fMultHad) / 2;
            outputTree->Fill();
            iDepth++;
        }
    }
}