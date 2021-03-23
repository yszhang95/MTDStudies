#include <array>
#include <vector>

#include "TF1.h"

#ifndef _ANA_h_
#define _ANA_h_
namespace ana{
   // generic use
   const double c_cm_ns = 2.99792458e1; //[cm/ns]

   template<typename T>
   size_t whichBin(const T& vals, const double val) {
     size_t n = vals.size() - 1;
     for (size_t i=0; i<n; ++i) {
         if( val < vals.at(i+1) && val > vals.at(i)) return i;
     }
      return size_t(-1);
   }

   // for tracks
   const float massPion = 0.139570;
   const float massKaon = 0.493677;
   const float massProton = 0.938272013;

   bool passTrackKinematicCuts(double eta, double pT) {
      if (std::abs(eta) > 3) return false;
      if (std::abs(eta) < 1.5 ? pT < 0.8 : pT * std::cosh(eta) < 0.7) return false;
      return true;
   }

   inline float invBetaPion(const float& p){
      return std::sqrt(1 + std::pow(massPion/p,2));
   }
   inline float invBetaKaon(const float& p){
      return std::sqrt(1 + std::pow(massKaon/p,2));
   }
   inline float invBetaProton(const float& p){
     return std::sqrt(1 + std::pow(ana::massProton/p,2));
   }

   TF1 fExpBTL("fExpBTL_dInvBetaRMS","0.005 + 0.016*exp(-x/4.4)");
   TF1 fExpETL("fExpETL_dInvBetaRMS","0.003 + 0.006*exp(-x/7.6)");

   TF1 fPionResBTL("fPionResBTL", "0.0012*(x-2)*(x-2) + 0.0004", 0.7, 2);
   TF1 fPionResETL("fPionResETL", "0.00025*(x-2)*(x-2) + 0.0005", 0.7, 2);

   TF1 fKaonResBTL("fKaonResBTL", "0.003*(x-2)*(x-2) + 0.0001", 0.7, 2);
   TF1 fKaonResETL("fKaonResETL", "0.00025*(x-2)*(x-2) + 0.0003", 0.7, 2);

   float meanPion(const float eta, const float pT){
      const float p = std::cosh(eta) * pT;
      const float res = std::abs(eta) < 1.5 ? fPionResBTL.Eval(p) : fPionResETL(p);
      return p < 2 ? res : 0;
   }
   float meanKaon(const float eta, const float pT){
      const float p = std::cosh(eta) * pT;
      const float res = std::abs(eta) < 1.5 ? fKaonResBTL.Eval(p) : fKaonResETL(p);
      return p < 2 ? res : 0;
   }

   // temporarily use Kaon
   float meanProton(const float eta, const float pT){
      const float p = std::cosh(eta) * pT;
      const float res = std::abs(eta) < 1.5 ? fKaonResBTL.Eval(p) : fKaonResETL(p);
      return p < 2 ? res : 0;
   }

   struct KineCut {
     float pTMin;
     float pTMax;
     float etaMin;
     float etaMax;
     KineCut(float pTMin, float pTMax, float etaMin, float etaMax) :
       pTMin(pTMin), pTMax(pTMax), etaMin(etaMin), etaMax(etaMax) {}
   };

   struct TopoCut {
     float dlMin;
     float dlMax;
     float aglMax;
     float vtxPrbMin;
     TopoCut(float dlMin, float dlMax, float aglMax, float vtxPrbMin) :
       dlMin(dlMin), dlMax(dlMax), aglMax(aglMax), vtxPrbMin(vtxPrbMin) {}
   };

   bool passKinematic (const float pT, const float eta, const KineCut& cut) {
     if (pT < cut.pTMin || pT > cut.pTMax
         || std::abs(eta) < cut.etaMin
         || std::abs(eta) > cut.etaMax) return false;
     return true;
   }

   bool passTopoCuts (const float dl, const float agl, const float vtxPrb, const TopoCut& cut) { // angle, dl sig, vtx prob
     if (dl < cut.dlMin || dl > cut.dlMax
         || std::abs(agl) > cut.aglMax
         || vtxPrb < cut.vtxPrbMin) return false;
     return true;
   }

   bool isFWHM(double mass, double mass_lw, double mass_up) {
     if( mass < mass_up && mass > mass_lw) return true;
     return false;
   }

   const double TAA0_100 = 5.61; // mb^-1
   const double TAA0_10 = 23.2; // mb^-1
   const double pbOvermb = 1e-12/1e-3;
   const double GeV = 1;

   const double evts_sim_MB = 25e9;

   // used in PyROOT
   template<typename T>
   T multiply(T& h1, T&h2) { return h1*h2; }
};
#endif

#ifndef __HelpClass__
#define __HelpClass__
#include <memory>
#include <vector>

class Particle {
  public:
    using ParticlePtr = shared_ptr<Particle>;
    Particle(int id): _id(id), _selfConj(false), _flip(0) ,_treeIdx(USHRT_MAX) {};
    Particle(const Particle& p);
    //~Particle() { cout << "Deleted" << endl; }

    bool isStable() const { return _daus.size(); }
    bool selfConj() const { return _selfConj;    }
    void selfConj(bool conj) { _selfConj = conj; }
    int  id()       const { return _id;          }
    bool isNull() const   { return _id == 0;     }
    vector<ParticlePtr> daughters() const { return _daus; }
    void addDaughter(Particle& dau) { _daus.push_back(std::make_shared<Particle>(dau)); }
    ParticlePtr daughter(size_t i);
    ParticlePtr daughter(size_t i) const;
    void setTreeIdx(unsigned short treeIdx) { _treeIdx = treeIdx; }
    int flip() { return _flip; }
    unsigned short treeIdx() const { return _treeIdx; }
    void flipFlavor();
  protected:
    int _id;
    bool _selfConj;
    int _flip;
    unsigned short _treeIdx;
    vector<ParticlePtr> _daus;
};

Particle::Particle(const Particle& p):
  _id(p._id), _selfConj(p._selfConj), _flip(0), _treeIdx(USHRT_MAX)
{
  for (const auto& d : p._daus) {
    _daus.push_back(std::make_shared<Particle>(*d));
  }
}

Particle::ParticlePtr Particle::daughter(size_t i)
{ 
  if (_daus.size()) {
    return _daus[i];
  }
  else {
    cerr << "No daughter found with pdgId " << _id << endl;
    return nullptr;
  }
}

Particle::ParticlePtr Particle::daughter(size_t i) const
{ 
  if (_daus.size()) {
    return _daus.at(i);
  }
  else {
    cerr << "No daughter found with pdgId " << _id << endl;
    return nullptr;
  }
}

void Particle::flipFlavor()
{
  if(!_selfConj) {
    _id = -_id;
    for(auto& d : _daus) d->flipFlavor();
  }
  _flip++;
}

#include "Math/GenVector/VectorUtil.h"
#include "TMath.h"

class MatchCriterion {
  public:
    template <typename T>
      bool match (const T& reco, const T& gen);

    MatchCriterion (const float dR, const float dRelPt) :
      _deltaR(dR), _deltaRelPt(dRelPt) { };
    ~MatchCriterion() {}
    void SetDeltaRelPt(const float dRelPt) { _deltaRelPt = dRelPt; }
    void SetDeltaR(const float dR) { _deltaR = dR; }

  private:
    float _deltaR;
    float _deltaRelPt;
};

  template <typename T>
bool MatchCriterion::match (const T& reco, const T& gen)
{
  const auto dR = ROOT::Math::VectorUtil::DeltaR(reco, gen);
  const auto dRelPt = TMath::Abs(gen.Pt() - reco.Pt())/gen.Pt();
  return dR < _deltaR && dRelPt < _deltaRelPt;
}
#endif
