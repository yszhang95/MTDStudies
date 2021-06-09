#include <array>
#include <vector>
#include <memory>
#include <climits>

#include "TF1.h"

#include "Math/GenVector/VectorUtil.h"
#include "TMath.h"

#ifndef HelpClass_H
#define HelpClass_H

namespace ana{
   // generic use
   const static double c_cm_ns = 2.99792458e1; //[cm/ns]

  template<typename T>
  inline size_t whichBin(const T& vals, const double val)
  {
    size_t n = vals.size() - 1;
    for (size_t i=0; i<n; ++i) {
      if( val < vals.at(i+1) && val >= vals.at(i)) return i;
    }
    return size_t(-1);
  }

  bool passTrackKinematicCuts(double eta, double pT);
   // for tracks
   const static float massPion = 0.139570;
   const static float massKaon = 0.493677;
   const static float massProton = 0.938272013;


   inline float invBetaPion(const float& p){
      return std::sqrt(1 + std::pow(massPion/p,2));
   }
   inline float invBetaKaon(const float& p){
      return std::sqrt(1 + std::pow(massKaon/p,2));
   }
   inline float invBetaProton(const float& p){
     return std::sqrt(1 + std::pow(ana::massProton/p,2));
   }

  float invBeta(const float pT, const int id);

   static TF1 fExpBTL("fExpBTL_dInvBetaRMS","0.005 + 0.016*exp(-x/4.4)");
   static TF1 fExpETL("fExpETL_dInvBetaRMS","0.003 + 0.006*exp(-x/7.6)");

   static TF1 fPionResBTL("fPionResBTL", "0.0012*(x-2)*(x-2) + 0.0004", 0.7, 2);
   static TF1 fPionResETL("fPionResETL", "0.00025*(x-2)*(x-2) + 0.0005", 0.7, 2);

   static TF1 fKaonResBTL("fKaonResBTL", "0.003*(x-2)*(x-2) + 0.0001", 0.7, 2);
   static TF1 fKaonResETL("fKaonResETL", "0.00025*(x-2)*(x-2) + 0.0003", 0.7, 2);

  float meanPion(const float eta, const float pT);
  float meanKaon(const float eta, const float pT);
   // temporarily use Kaon
  float meanProton(const float eta, const float pT);

  float betaInvRes(const float eta, const float pT, const int id);

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

  bool passKinematic (const float pT, const float eta, const KineCut& cut);
  bool passTopoCuts (const float dl, const float agl, const float vtxPrb, const TopoCut& cut);
  bool isFWHM(double mass, double mass_lw, double mass_up);

   const static double TAA0_100 = 5.61; // mb^-1
   const static double TAA0_10 = 23.2; // mb^-1
   const static double pbOvermb = 1e-12/1e-3;
   const static double GeV = 1;

   const static double evts_sim_MB = 25e9;

   // used in PyROOT
   template<typename T>
   T multiply(T& h1, T&h2) { return h1*h2; }
};

class Particle {
  public:
  using ParticlePtr = std::shared_ptr<Particle>;
  Particle(int id): _id(id), _selfConj(false), _flip(0), _longLived(false) ,_treeIdx(USHRT_MAX) {};
    Particle(const Particle& p);
    //~Particle() { cout << "Deleted" << endl; }

    bool isStable() const { return _daus.size(); }
    bool selfConj() const { return _selfConj;    }
    void selfConj(bool conj) { _selfConj = conj; }
  bool longLived() const    { return _longLived; }
  void longLived(bool isLL) { _longLived = isLL; }
    int  id()       const { return _id;          }
    bool isNull() const   { return _id == 0;     }
  std::vector<ParticlePtr> daughters() const { return _daus; }
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
  bool _longLived;
    int _flip;
    unsigned short _treeIdx;
  std::vector<ParticlePtr> _daus;
};

class MatchCriterion {
  public:
    template <typename T>
      bool match (const T& reco, const T& gen)
  {
    const auto dR = ROOT::Math::VectorUtil::DeltaR(reco, gen);
    const auto dRelPt = TMath::Abs(gen.Pt() - reco.Pt())/gen.Pt();
    return dR < _deltaR && dRelPt < _deltaRelPt;
  }

    MatchCriterion (const float dR, const float dRelPt) :
      _deltaR(dR), _deltaRelPt(dRelPt) { };
    ~MatchCriterion() {}
    void SetDeltaRelPt(const float dRelPt) { _deltaRelPt = dRelPt; }
    void SetDeltaR(const float dR) { _deltaR = dR; }

  private:
    float _deltaR;
    float _deltaRelPt;
};

#endif
