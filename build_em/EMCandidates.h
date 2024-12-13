
#include <TTree.h>


struct CollCandidate
{
    float fZVertex, fCentralityFT0C;
    int CollID = -1;
    void setCollBranchAddress(TTree * tree) {
        tree->SetBranchAddress("fZVertex", &fZVertex);
        tree->SetBranchAddress("fCentralityFT0C", &fCentralityFT0C);
    }
};


struct CollHadBracket
{
    int CollID, fHadStartIndex, fHadEndIndex;
    void SetMin(int min) {
        fHadStartIndex = min;
    }
    void SetMax(int max) {
        fHadEndIndex = max;
    }
    int GetMin() {
        return fHadStartIndex;
    }
    int GetMax() {
        return fHadEndIndex;
    }
};


struct He3Candidate
{
    float fPtHe3, fEtaHe3, fPhiHe3, fDCAxyHe3, fDCAzHe3, fSignalTPCHe3, fInnerParamTPCHe3, fMassTOFHe3;
    unsigned int fItsClusterSizeHe3;
    unsigned char fNClsTPCHe3, fSharedClustersHe3;
    float fNSigmaTPCHe3, fChi2TPCHe3;
    float fZVertex, fCentralityFT0C;
    int CollID = -1;
    
    void setHe3BranchAddress(TTree * tree) {
        tree->SetBranchAddress("fPtHe3", &fPtHe3);
        tree->SetBranchAddress("fEtaHe3", &fEtaHe3);
        tree->SetBranchAddress("fPhiHe3", &fPhiHe3);
        tree->SetBranchAddress("fDCAxyHe3", &fDCAxyHe3);
        tree->SetBranchAddress("fDCAzHe3", &fDCAzHe3);
        tree->SetBranchAddress("fSignalTPCHe3", &fSignalTPCHe3);
        tree->SetBranchAddress("fInnerParamTPCHe3", &fInnerParamTPCHe3);
        tree->SetBranchAddress("fMassTOFHe3", &fMassTOFHe3);
        tree->SetBranchAddress("fNClsTPCHe3", &fNClsTPCHe3);
        tree->SetBranchAddress("fItsClusterSizeHe3", &fItsClusterSizeHe3);
        tree->SetBranchAddress("fSharedClustersHe3", &fSharedClustersHe3);
        tree->SetBranchAddress("fNSigmaTPCHe3", &fNSigmaTPCHe3);
        tree->SetBranchAddress("fChi2TPCHe3", &fChi2TPCHe3);
    }

};

struct HadronCandidate
{
    float fPtHad, fEtaHad, fPhiHad, fDCAxyHad, fDCAzHad, fSignalTPCHad, fInnerParamTPCHad, fMassTOFHad;
    unsigned int fItsClusterSizeHad;
    unsigned char fSharedClustersHad;
    float fNSigmaTPCHad, fChi2TPCHad;
    int collIndex = -1;
    float fZVertex, fCentralityFT0C;

    void setHadronBranchAddress(TTree * tree) {
        tree->SetBranchAddress("fPtHad", &fPtHad);
        tree->SetBranchAddress("fEtaHad", &fEtaHad);
        tree->SetBranchAddress("fPhiHad", &fPhiHad);
        tree->SetBranchAddress("fDCAxyHad", &fDCAxyHad);
        tree->SetBranchAddress("fDCAzHad", &fDCAzHad);
        tree->SetBranchAddress("fSignalTPCHad", &fSignalTPCHad);
        tree->SetBranchAddress("fInnerParamTPCHad", &fInnerParamTPCHad);
        tree->SetBranchAddress("fMassTOFHad", &fMassTOFHad);
        tree->SetBranchAddress("fItsClusterSizeHad", &fItsClusterSizeHad);
        tree->SetBranchAddress("fSharedClustersHad", &fSharedClustersHad);
        tree->SetBranchAddress("fNSigmaTPCHad", &fNSigmaTPCHad);
        tree->SetBranchAddress("fChi2TPCHad", &fChi2TPCHad);
    }
};

struct Li4Candidate
{
    float fPtHe3, fEtaHe3, fPhiHe3, fPtHad, fEtaHad, fPhiHad, fDCAxyHe3, fDCAzHe3, fDCAxyHad, fDCAzHad, fSignalTPCHe3, fInnerParamTPCHe3, fSignalTPCHad, fInnerParamTPCHad, fMassTOFHe3, fMassTOFHad;
    unsigned int fItsClusterSizeHe3, fItsClusterSizeHad;
    unsigned char fNClsTPCHe3, fSharedClustersHe3, fSharedClustersHad;
    float fNSigmaTPCHe3, fNSigmaTPCHad, fChi2TPCHe3, fChi2TPCHad;
    float fZVertex, fCentralityFT0C;

    void setHe3(He3Candidate & he3) {
        fPtHe3 = he3.fPtHe3;
        fEtaHe3 = he3.fEtaHe3;
        fPhiHe3 = he3.fPhiHe3;
        fDCAxyHe3 = he3.fDCAxyHe3;
        fDCAzHe3 = he3.fDCAzHe3;
        fSignalTPCHe3 = he3.fSignalTPCHe3;
        fInnerParamTPCHe3 = he3.fInnerParamTPCHe3;
        fMassTOFHe3 = he3.fMassTOFHe3;
        fNClsTPCHe3 = he3.fNClsTPCHe3;
        fItsClusterSizeHe3 = he3.fItsClusterSizeHe3;
        fSharedClustersHe3 = he3.fSharedClustersHe3;
        fNSigmaTPCHe3 = he3.fNSigmaTPCHe3;
        fChi2TPCHe3 = he3.fChi2TPCHe3;
    }

    void setHadron(HadronCandidate & had) {
        fPtHad = had.fPtHad;
        fEtaHad = had.fEtaHad;
        fPhiHad = had.fPhiHad;
        fDCAxyHad = had.fDCAxyHad;
        fDCAzHad = had.fDCAzHad;
        fSignalTPCHad = had.fSignalTPCHad;
        fInnerParamTPCHad = had.fInnerParamTPCHad;
        fMassTOFHad = had.fMassTOFHad;
        fItsClusterSizeHad = had.fItsClusterSizeHad;
        fSharedClustersHad = had.fSharedClustersHad;
        fNSigmaTPCHad = had.fNSigmaTPCHad;
        fChi2TPCHad = had.fChi2TPCHad;
    }


    float calcInvMass() {
        float pxHe3 = std::abs(fPtHe3) * TMath::Cos(fPhiHe3);
        float pyHe3 = std::abs(fPtHe3) * TMath::Sin(fPhiHe3);
        float pzHe3 = std::abs(fPtHe3) * TMath::SinH(fEtaHe3);
        float pHe3 = TMath::Sqrt(pxHe3 * pxHe3 + pyHe3 * pyHe3 + pzHe3 * pzHe3);
        float pxHad = std::abs(fPtHad) * TMath::Cos(fPhiHad);
        float pyHad = std::abs(fPtHad) * TMath::Sin(fPhiHad);
        float pzHad = std::abs(fPtHad) * TMath::SinH(fEtaHad);
        float pHad = TMath::Sqrt(pxHad * pxHad + pyHad * pyHad + pzHad * pzHad);
        float eHe3 = TMath::Sqrt(pHe3 * pHe3 + 2.8083916 * 2.8083916);
        float eHad = TMath::Sqrt(pHad * pHad + 0.938272 * 0.938272);
        float pTot = TMath::Sqrt((pxHe3 + pxHad) * (pxHe3 + pxHad) + (pyHe3 + pyHad) * (pyHe3 + pyHad) + (pzHe3 + pzHad) * (pzHe3 + pzHad));
        float eTot = eHe3 + eHad;
        return TMath::Sqrt(eTot * eTot - pTot * pTot);
    }

    float calcPt() {
        float pxTot = std::abs(fPtHe3) * TMath::Cos(fPhiHe3) + std::abs(fPtHad) * TMath::Cos(fPhiHad);
        float pyTot = std::abs(fPtHe3) * TMath::Sin(fPhiHe3) + std::abs(fPtHad) * TMath::Sin(fPhiHad);
        return TMath::Sqrt(pxTot * pxTot + pyTot * pyTot);
    }

};