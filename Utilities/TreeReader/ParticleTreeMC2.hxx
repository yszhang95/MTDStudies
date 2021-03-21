//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb 18 17:08:18 2021 by ROOT version 6.06/01
// from TTree ParticleTree/ParticleTree
// found on file: /eos/cms/store/group/phys_heavyions/yousen/RiceHIN/OpenHF2020_LamCKsP/MC/LambdaC-KsPr_LCpT-0p9_pPb-EmbEPOS_8p16_Pythia8/PA8TeV_pPb_LamCKsP0p9_pT0p9to6p1_y1p1_MC_Training_20210217/Merged_PA8TeV_pPb_LamCKsP0p9_pT0p9to6p1_y1p1_MC_Training_20210217.root
//////////////////////////////////////////////////////////

#ifndef ParticleTreeMC2_hxx
#define ParticleTreeMC2_hxx

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

#include "ParticleTreeMC.hxx"

class ParticleTreeMC2 : public ParticleTreeMC {
  public :
    ParticleTreeMC2(TTree *tree=0);
    virtual ~ParticleTreeMC2();

    Float_t   bestvtxT()    const { return _bestvtxT;    }
    Float_t   bestvtxTerr() const { return _bestvtxTerr; }

    std::vector<float>&   trk_beta()       const { return *_trk_beta;       }
    std::vector<float>&   trk_gammaSq()    const { return *_trk_gammaSq;    }
    std::vector<float>&   trk_massSq()     const { return *_trk_massSq;     }
    std::vector<float>&   trk_pathLength() const { return *_trk_pathLength; }
    std::vector<float>&   trk_tMTD()       const { return *_trk_tMTD;       }
    std::vector<float>&   trk_tMTDErr()    const { return *_trk_tMTDErr;    }

  private:
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain

    // Fixed size dimensions of array or collections stored in the TTree if any.

    // Declaration of leaf types
    Float_t         _bestvtxT;
    Float_t         _bestvtxTerr;

    std::vector<float>   *_trk_beta;
    std::vector<float>   *_trk_gammaSq;
    std::vector<float>   *_trk_massSq;
    std::vector<float>   *_trk_pathLength;
    std::vector<float>   *_trk_tMTD;
    std::vector<float>   *_trk_tMTDErr;

    // List of branches
    TBranch        *b_bestvtxT;   //!
    TBranch        *b_bestvtxTerr;   //!

    TBranch        *b_trk_beta;   //!
    TBranch        *b_trk_gammaSq;   //!
    TBranch        *b_trk_massSq;   //!
    TBranch        *b_trk_pathLength;   //!
    TBranch        *b_trk_tMTD;   //!
    TBranch        *b_trk_tMTDErr;   //!

    virtual void     Init(TTree *tree);
};

#endif
