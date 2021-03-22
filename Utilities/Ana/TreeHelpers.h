#ifndef __TreeHelpers__
#define __TreeHelpers__

#include <vector>
#include <tuple>
#include <set>

#include <algorithm>
#include <climits>
#include <numeric>

#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
#include "Math/GenVector/VectorUtil.h"

#include "TreeReader/ParticleTreeMC2.hxx"
#include "Ana/Common.h"

// typedefs
using PtEtaPhiM_t = ROOT::Math::PtEtaPhiM4D<double>;
using ParticleInfo = tuple<PtEtaPhiM_t, int, double, double, double, double>; // id, beta, tMtd, tMtdErr, pathLength

// functions
bool checkDecayChain(Particle& par, unsigned short genIdx, const ParticleTreeMC& p, bool isFirstGen=true)
{
  // check pdg Id
  if (par.id() * p.gen_pdgId().at(genIdx) < 0 && isFirstGen) par.flipFlavor();
  if (par.id() != p.gen_pdgId().at(genIdx)) return false;

  // get daughters out
  const auto dauIdxs = p.gen_dauIdx().at(genIdx);

  // check number of daughters
  if (par.daughters().size() != dauIdxs.size()) return false;

  // if no daughters, both of these two have the same decay chain
  if (dauIdxs.size() == 0) {
      par.setTreeIdx(genIdx);
      return true;
  }

  // fill pdgIds and check them
  std::multiset<int> genDauPdgId; // check dau ID
  std::multiset<int> desiredPdgId; // check dau ID
  std::vector<int> vec_desiredPdgId; // for later permutation
  for (const auto idx : dauIdxs) {
    genDauPdgId.insert(p.gen_pdgId().at(idx));
  }
  for (const auto dau : par.daughters()) {
    desiredPdgId.insert(dau->id());
    vec_desiredPdgId.push_back(dau->id());
  }
  bool sameIds = desiredPdgId == genDauPdgId;
  if (!sameIds) return false; // they do not have the same set of decay products
  
  // if they have same daughter Ids
  // check their id order
  std::vector<unsigned int> idxs(vec_desiredPdgId.size());
  std::iota(std::begin(idxs), std::end(idxs), 0);
  do {
    bool sameOrder = true;
    for (size_t i=0;  i<idxs.size(); i++) {
      auto dIdx = dauIdxs[idxs[i]];
      sameOrder = sameOrder && (p.gen_pdgId().at(dIdx) == vec_desiredPdgId[i]);
    }
    if(sameOrder) {
      break;
    }
  } while(std::next_permutation(idxs.begin(), idxs.end()));

  // associated them if they are compatible
  bool sameChain = true;
  for (size_t i=0;  i<idxs.size(); i++) {
    sameChain = sameChain && checkDecayChain(*par.daughter(i), dauIdxs[idxs[i]], p, false);
    if (sameChain) par.daughter(i)->setTreeIdx( dauIdxs[idxs[i]] );
  }

  return sameChain;
}


PtEtaPhiM_t getRecoP4(size_t idx, const ParticleTreeMC& p)
{
  return PtEtaPhiM_t (
            p.cand_pT()[idx],
            p.cand_eta()[idx],
            p.cand_phi()[idx],
            p.cand_mass()[idx]
            );
}
PtEtaPhiM_t getRecoDauP4(size_t idx, size_t idau, const ParticleTreeMC& p)
{
  return PtEtaPhiM_t (
            p.cand_pTDau()[idx].at(idau),
            p.cand_etaDau()[idx].at(idau),
            p.cand_phiDau()[idx].at(idau),
            p.cand_massDau()[idx].at(idau)
            );
}

PtEtaPhiM_t getGenP4(size_t idx, const ParticleTreeMC& p)
{
  return PtEtaPhiM_t (
            p.gen_pT()[idx],
            p.gen_eta()[idx],
            p.gen_phi()[idx],
            p.gen_mass()[idx]
            );
}

bool checkPassPID(const ParticleInfo& p, float cut)
{
  double eta = std::get<0>(p).Eta();
  double mom = std::get<0>(p).P();
  double pT  = std::get<0>(p).Pt();
  if (mom > 5.) return true;
  const float dInvBetaCut = std::fabs(eta<1.5) ? ana::fExpBTL.Eval(mom) : ana::fExpETL.Eval(mom);
  double beta = std::get<2>(p);
  bool isValid = std::get<4>(p) > 0;
  if (!isValid) return true;
  bool pass = true;
  int pid = std::abs(std::get<1>(p));
  if (pid == 211) pass = std::fabs(1./beta - ana::invBetaPion(mom) - ana::meanPion(eta, pT)) < cut * dInvBetaCut;
  if (pid == 321) pass = std::fabs(1./beta - ana::invBetaKaon(mom) - ana::meanKaon(eta, pT)) < cut * dInvBetaCut;
  if (pid == 2212) pass = std::fabs(1./beta - ana::invBetaProton(mom) - ana::meanProton(eta, pT)) < cut * dInvBetaCut;
  return pass;
}

#endif
