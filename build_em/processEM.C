
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

    TH1D* hHe3BeforeEMAll = new TH1D("hHe3BeforeEMAll", "; p_{T} (GeV/c); Entries", 200, -10, 0);
    TH1D* hHe3BeforeEM = new TH1D("hHe3BeforeEM", "; p_{T} (GeV/c); Entries", 200, -10, 0);
    TH1D* hHe3AfterEM = new TH1D("hHe3AfterEM", "; p_{T} (GeV/c); Entries", 200, -10, 0);
    TH1D* hInvMassBeforeEM = new TH1D("hInvMassBeforeEM", "; Inv Mass (GeV/c^{2}); Entries", 200, 3.743, 4);
    TH1D* hInvMassAfterEM = new TH1D("hInvMassAfterEM", "; Inv Mass (GeV/c^{2}); Entries", 200, 3.743, 4);

    gRandom->SetSeed(1995);
    int mEMDepth = 100;

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
    std::vector<std::vector<CollHadBracket>> mCollBrackets; // first index is the collIndex, second and third are the indices of the hadron candidates
    mCollBrackets.resize(mUtils.mZetaBins * mUtils.mMultBins + 1);

    CollCandidate collCand;
    CollHadBracket collBracket;

    He3Candidate he3Cand;
    HadronCandidate hadCand;
    Li4Candidate li4Cand;
    

    collCand.setCollBranchAddress(inputCollsTree);
    he3Cand.setHe3BranchAddress(inputCandsTree);
    hadCand.setHadronBranchAddress(inputCandsTree);

    std::vector<He3Candidate> mHe3Cands;
    std::vector<HadronCandidate> mHadCands;
    std::vector<CollCandidate> mCollCands;

    collBracket.SetMin(-1);
    collBracket.SetMax(-1);
    CollCandidate oldCollCand;

    for (int iEntry = 0; iEntry < inputCollsTree->GetEntries(); iEntry++)
    {
        inputCollsTree->GetEntry(iEntry);
        inputCandsTree->GetEntry(iEntry);
        hadCand.fZVertex = collCand.fZVertex;
        hadCand.fCentralityFT0C = collCand.fCentralityFT0C;
        mHadCands.push_back(hadCand);

        li4Cand.setHadron(hadCand);
        li4Cand.setHe3(he3Cand);
        if (he3Cand.fPtHe3 < 0)
            hInvMassBeforeEM->Fill(li4Cand.calcInvMass());
        hHe3BeforeEMAll->Fill(he3Cand.fPtHe3);

        if (abs(oldCollCand.fZVertex - collCand.fZVertex) < 1e-5)
        {
            collBracket.SetMax(mHadCands.size() - 1);
            continue;
        }

        if (collBracket.GetMin() != -1)
        {
            int iBin = mUtils.getBinIndex(oldCollCand.fZVertex, oldCollCand.fCentralityFT0C);
            collBracket.CollID = iEntry - 1;
            mCollBrackets[iBin].push_back(collBracket);
        }

        // a new collision has been found, dumping collision and he3 candidates
        
        mHe3Cands.push_back(he3Cand); 
        mCollCands.push_back(collCand);
        hHe3BeforeEM->Fill(he3Cand.fPtHe3);

        collBracket.SetMin(mHadCands.size() - 1);
        collBracket.SetMax(mHadCands.size() - 1);
        oldCollCand = collCand;
    }

    std::cout << "--------------------------------" << std::endl;
    std::cout << "Size of Hadron Candidates to be mixed: " << mHadCands.size() << std::endl;
    std::cout << "Size of He3 Candidates to be mixed: " << mHe3Cands.size() << std::endl;
    std::cout << "Size of Coll Candidates: " << mCollCands.size() << std::endl;
    std::cout << "--------------------------------" << std::endl;

    Li4Candidate li4CandME;
    auto outputFile = TFile::Open("/data3/fmazzasc/lit_run3/EM/pbpb/outputEM.root", "RECREATE");
    auto outputTree = new TTree("DF", "DF");
    // flash the Mixed event structure in the output tree
    outputTree->Branch("O2he3hadtable", &li4CandME);

    for (int collIDHe3 = 0; collIDHe3 < mHe3Cands.size(); collIDHe3++)
    {

        // print 1% progress
        if (collIDHe3 % (mHe3Cands.size() / 100) == 0)
        {
            std::cout << "Processing entry: " << collIDHe3 << " / " << mHe3Cands.size() << ", " << 100 * collIDHe3 / mHe3Cands.size() << "%" << std::endl;
        }

        auto &he3Cand = mHe3Cands[collIDHe3];
        auto &collCand = mCollCands[collIDHe3];
        li4CandME.setHe3(he3Cand);
        int iBin = mUtils.getBinIndex(collCand.fZVertex, collCand.fCentralityFT0C);

        for (int iDepth = 0; iDepth < mEMDepth; iDepth++)
        {

            if (mCollBrackets[iBin].size() == 0 || iDepth >= mCollBrackets[iBin].size())
            {
                break;
            }

            int iCollEM;
            mCollBrackets[iBin].size() < mEMDepth ? iCollEM = iDepth : iCollEM = gRandom->Integer(mCollBrackets[iBin].size());
            int collIDHad = mCollBrackets[iBin][iCollEM].CollID;
            if (collIDHad == collIDHe3)
            {
                continue;
            }
            for (int iHad = mCollBrackets[iBin][iCollEM].GetMin(); iHad <= mCollBrackets[iBin][iCollEM].GetMax(); iHad++)
            {
                auto &hadCand = mHadCands[iHad];
                li4CandME.setHadron(hadCand);
                li4CandME.fZVertex = collCand.fZVertex;
                li4CandME.fCentralityFT0C = collCand.fCentralityFT0C;
                // std::cout << "inv mass: " << li4CandME.calcInvMass() << std::endl;
                if (li4CandME.calcInvMass() < 4.15314 && li4CandME.fPtHe3 * li4CandME.fPtHad > 0 && li4CandME.calcPt() > 2){
                    if (li4CandME.fPtHe3 < 0)
                    hInvMassAfterEM->Fill(li4CandME.calcInvMass());
                    hHe3AfterEM->Fill(he3Cand.fPtHe3);
                    outputTree->Fill();
                }
            }
        }
    }

    hHe3BeforeEMAll->Write();
    hHe3BeforeEM->Write();
    hHe3AfterEM->Write();

    hInvMassBeforeEM->Write();
    hInvMassAfterEM->Write();

    // plot in the same canvas
    hHe3BeforeEMAll->SetLineColor(kBlack);
    hHe3BeforeEM->SetLineColor(kRed);
    hHe3AfterEM->SetLineColor(kBlue);
    TCanvas *c = new TCanvas("c", "c", 800, 600);
    hHe3BeforeEMAll->DrawNormalized();
    hHe3BeforeEM->DrawNormalized("SAME");
    hHe3AfterEM->DrawNormalized("SAME");
    c->Write();

    hInvMassBeforeEM->SetLineColor(kRed);
    hInvMassAfterEM->SetLineColor(kBlue);
    TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
    hInvMassBeforeEM->DrawNormalized();
    hInvMassAfterEM->DrawNormalized("SAME");
    c2->Write();

    outputFile->Close();
}