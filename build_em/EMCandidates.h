
#include <TTree.h>


struct CollCandidate
{
    float fZVertex, fCentralityFT0C;
    void setCollBranchAddress(TTree * tree) {
        tree->SetBranchAddress("fZVertex", &fZVertex);
        tree->SetBranchAddress("fCentralityFT0C", &fCentralityFT0C);
    }
};


struct He3Candidate
{
    float fPtHe3, fEtaHe3, fPhiHe3, fDCAxyHe3, fDCAzHe3, fSignalTPCHe3, fInnerParamTPCHe3, fMassTOFHe3;
    unsigned int fItsClusterSizeHe3;
    unsigned char fNClsTPCHe3, fSharedClustersHe3;
    float fNSigmaTPCHe3, fChi2TPCHe3;
    float fZHe3, fMultHe3;
    
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
    float fZHad, fMultHad;

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

};