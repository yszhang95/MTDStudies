#ifndef ParticleTreeMC2_hxx
#include "TreeReader/ParticleTreeMC2.hxx"
#endif

#ifndef HelpClass_H
#include "Ana/Common.h"
#endif

#ifndef TreeHelpers_H
#define TreeHelpers_H


#include <vector>
#include <tuple>
#include <set>
#include <initializer_list>

#include <algorithm>
#include <climits>
#include <numeric>

#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/LorentzVectorfwd.h"
#include "Math/Vector4D.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
#include "Math/GenVector/VectorUtil.h"

// typedefs
using PtEtaPhiM_t = ROOT::Math::PtEtaPhiMVector;
using ParticleInfo = std::tuple<PtEtaPhiM_t, int, double, double, double, double>; // id, beta, tMtd, tMtdErr, pathLength

// functions
bool checkDecayChain(Particle& par, unsigned short genIdx, const ParticleTreeMC& p, bool isFirstGen=true);

PtEtaPhiM_t getRecoP4(size_t idx, const ParticleTreeMC& p);
PtEtaPhiM_t getRecoDauP4(size_t idx, size_t idau, const ParticleTreeMC& p);
PtEtaPhiM_t getGenP4(size_t idx, const ParticleTreeMC& p);

bool checkPassPID(const ParticleInfo& p, float cut);

struct MyNTuple
{
  TTree* t;
  //  Event info
  //  UChar_t         nPV;
  //  UShort_t        BXNb;
  //  UShort_t        Ntrkoffline;
  //  UInt_t          EventNb;
  //  UInt_t          LSNb;
  //  UInt_t          RunNb;
  //  Float_t         HFsumETMinus;
  //  Float_t         HFsumETPlus;
  //  Float_t         Npixel;
  //  Float_t         ZDCMinus;
  //  Float_t         ZDCPlus;
  //  Float_t         bestvtxX;
  //  Float_t         bestvtxY;
  //  Float_t         bestvtxZ;
  Float_t         bestvtxT;
  Float_t         bestvtxTerr;

  Bool_t          cand_matchGEN;

  Char_t          cand_charge;
  Float_t         cand_eta;
  Float_t         cand_mass;
  //  Float_t         cand_p;
  Float_t         cand_pT;
  Float_t         cand_y;
  Float_t         cand_phi;

  Int_t           cand_isSwap;

  Float_t         cand_angle2D;
  Float_t         cand_angle3D;
  Float_t         cand_dca;
  Float_t         cand_decayLength2D;
  Float_t         cand_decayLength3D;
  Float_t         cand_decayLengthError2D;
  Float_t         cand_decayLengthError3D;
  Float_t         cand_pseudoDecayLengthError2D;
  Float_t         cand_pseudoDecayLengthError3D;
  Float_t         cand_vtxChi2;
  Float_t         cand_vtxProb;

  unsigned short  nDau;
  Float_t         cand_etaDau[100];
  Float_t         cand_massDau[100];
  Float_t         cand_pTDau[100];
  Float_t         cand_phiDau[100];


  // daughter level
  unsigned short  nGDau;
  unsigned short  dauHasNGDau[100];

  Bool_t          cand_dau_matchGEN[100];
  Char_t          cand_dau_charge[100];
  Int_t           cand_dau_isSwap[100];

  // kinematics
  Float_t         cand_dau_eta[100];
  Float_t         cand_dau_mass[100];
  //  Float_t         cand_dau_p[100];
  Float_t         cand_dau_pT[100];
  Float_t         cand_dau_phi[100];
  //  Float_t         cand_dau_y[100];

  Float_t         cand_dau_etaDau[100][100];
  Float_t         cand_dau_pTDau[100][100];
  Float_t         cand_dau_massDau[100][100];
  Float_t         cand_dau_phiDau[100][100];

  Float_t         cand_dau_angle2D[100];
  Float_t         cand_dau_angle3D[100];
  Float_t         cand_dau_dca[100];
  Float_t         cand_dau_decayLength2D[100];
  Float_t         cand_dau_decayLength3D[100];
  Float_t         cand_dau_decayLengthError2D[100];
  Float_t         cand_dau_decayLengthError3D[100];
  Float_t         cand_dau_pseudoDecayLengthError2D[100];
  Float_t         cand_dau_pseudoDecayLengthError3D[100];
  Float_t         cand_dau_vtxChi2[100];
  Float_t         cand_dau_vtxProb[100];

  // track info
  Bool_t          trk_dau_isHP[100];
  UShort_t        trk_dau_nHit[100];
  Float_t         trk_dau_dEdx_dedxHarmonic2[100];
  Float_t         trk_dau_dEdx_dedxPixelHarmonic2[100];
  Float_t         trk_dau_dEdx_dedxPixelHarmonic2T40[100];
  Float_t         trk_dau_dEdx_dedxPixelMeanT40[100];
  Float_t         trk_dau_nChi2[100];
  Float_t         trk_dau_pTErr[100];
  Float_t         trk_dau_xyDCASignificance[100];
  Float_t         trk_dau_zDCASignificance[100];
  Float_t         trk_dau_beta[100];
  Float_t         trk_dau_gammaSq[100];
  Float_t         trk_dau_massSq[100];
  Float_t         trk_dau_pathLength[100];
  Float_t         trk_dau_tMTD[100];
  Float_t         trk_dau_tMTDErr[100];

  // granddaughter level
  Bool_t          cand_gdau_matchGEN[100];
  Char_t          cand_gdau_charge[100];
  Int_t           cand_gdau_isSwap[100];
  // kinematics
  Float_t         cand_gdau_eta[100];
  Float_t         cand_gdau_mass[100];
  // Float_t         cand_gdau_p[100];
  Float_t         cand_gdau_pT[100];
  Float_t         cand_gdau_phi[100];
  // Float_t         cand_gdau_y[100];

  // track info
  Bool_t          trk_gdau_isHP[100];
  UShort_t        trk_gdau_nHit[100];
  Float_t         trk_gdau_dEdx_dedxHarmonic2[100];
  Float_t         trk_gdau_dEdx_dedxPixelHarmonic2[100];
  Float_t         trk_gdau_dEdx_dedxPixelHarmonic2T40[100];
  Float_t         trk_gdau_dEdx_dedxPixelMeanT40[100];
  Float_t         trk_gdau_nChi2[100];
  Float_t         trk_gdau_pTErr[100];
  Float_t         trk_gdau_xyDCASignificance[100];
  Float_t         trk_gdau_zDCASignificance[100];
  Float_t         trk_gdau_beta[100];
  Float_t         trk_gdau_gammaSq[100];
  Float_t         trk_gdau_massSq[100];
  Float_t         trk_gdau_pathLength[100];
  Float_t         trk_gdau_tMTD[100];
  Float_t         trk_gdau_tMTDErr[100];

  MyNTuple(TTree* t) : t(t)
  {
    if (!t) {
      std::cerr << "[ERROR] Nullptr of TTree is given to NTuple!" << std::endl;
    }
    nDau = 0;
    nGDau = 0;
    for (auto& ngdau : dauHasNGDau) {
      ngdau = 0;
    }
  }
  ~MyNTuple()
  {
    std::cout << "Delete NTuple" << std::endl;
  }
  void  initNTuple();
  void  setNDau(const unsigned short, const unsigned short, unsigned short const*);
  void  setNDau(const unsigned short, const unsigned short, const std::initializer_list<unsigned short>&);
  Int_t fillNTuple();
  bool  retrieveTreeInfo(ParticleTreeMC2&, Long64_t);
  float value(const TString& s);
};

struct MyGenNTuple
{
  TTree* t;

  // Char_t          gen_charge;
  Float_t         gen_eta;
  Float_t         gen_mass;
  //  Float_t         gen_p;
  Float_t         gen_pT;
  Float_t         gen_y;
  Float_t         gen_phi;

  //Float_t         cand_angle2D;
  //Float_t         cand_angle3D;
  //Float_t         cand_dca;
  //Float_t         cand_decayLength2D;
  //Float_t         cand_decayLength3D;
  //Float_t         cand_decayLengthError2D;
  //Float_t         cand_decayLengthError3D;
  //Float_t         cand_pseudoDecayLengthError2D;
  //Float_t         cand_pseudoDecayLengthError3D;
  //Float_t         cand_vtxChi2;
  //Float_t         cand_vtxProb;

  unsigned short  nDau;
  // Char_t          gen_chargeDau[100];
  Float_t         gen_etaDau[100];
  Float_t         gen_massDau[100];
  Float_t         gen_pTDau[100];
  Float_t         gen_phiDau[100];

  // daughter level
  unsigned short  dauHasNGDau[100];
  // Char_t          gen_chargeDau[100];
  Float_t         gen_etaDau_GDau[100][100];
  Float_t         gen_massDau_GDau[100][100];
  Float_t         gen_pTDau_GDau[100][100];
  Float_t         gen_phiDau_GDau[100][100];

  MyGenNTuple(TTree* t) : t(t)
  {
    if (!t) {
      std::cerr << "[ERROR] Nullptr of TTree is given to NTuple!" << std::endl;
    }
    nDau = 0;
    for (auto& ngdau : dauHasNGDau) {
      ngdau = 0;
    }
  }
  ~MyGenNTuple()
  {
    std::cout << "Delete NTuple" << std::endl;
  }
  void  initNTuple();
  void  setNDau(const unsigned short, unsigned short const*);
  void  setNDau(const unsigned short, const std::initializer_list<unsigned short>&);
  Int_t fillNTuple();
  bool  retrieveTreeInfo(ParticleTreeMC2&, const Particle&);
  // float value(const TString& s);
};

#endif
