#ifndef HelpClass_H
#include "Ana/Common.h"
#endif
#ifdef HelpClass_H

bool ana::passTrackKinematicCuts(double eta, double pT) {
  if (std::abs(eta) > 3) return false;
  if (std::abs(eta) < 1.5 ? pT < 0.8 : pT * std::cosh(eta) < 0.7) return false;
  return true;
}

float ana::invBeta(const float pT, const int id)
{
  if (abs(id) == 211) return ana::invBetaPion(pT);
  if (abs(id) == 321) return ana::invBetaKaon(pT);
  if (abs(id) == 2212) return ana::invBetaProton(pT);
  return 0;
}

float ana::meanPion(const float eta, const float pT){
  const float p = std::cosh(eta) * pT;
  const float res = std::abs(eta) < 1.5 ? fPionResBTL.Eval(p) : fPionResETL(p);
  return p < 2 ? res : 0;
}
float ana::meanKaon(const float eta, const float pT){
  const float p = std::cosh(eta) * pT;
  const float res = std::abs(eta) < 1.5 ? fKaonResBTL.Eval(p) : fKaonResETL(p);
  return p < 2 ? res : 0;
}

// temporarily use Kaon
float ana::meanProton(const float eta, const float pT){
  const float p = std::cosh(eta) * pT;
  const float res = std::abs(eta) < 1.5 ? fKaonResBTL.Eval(p) : fKaonResETL(p);
  return p < 2 ? res : 0;
}

float ana::betaInvRes(const float eta, const float pT, const int id)
{
  if (abs(id) == 211) return ana::meanPion(eta, pT);
  if (abs(id) == 321) return ana::meanKaon(eta, pT);
  if (abs(id) == 2212) return ana::meanProton(eta, pT);
  return 0;
}

bool ana::passKinematic (const float pT, const float eta, const ana::KineCut& cut) {
  if (pT < cut.pTMin || pT > cut.pTMax
      || std::abs(eta) < cut.etaMin
      || std::abs(eta) > cut.etaMax) return false;
  return true;
}

bool ana::passTopoCuts (const float dl, const float agl, const float vtxPrb, const TopoCut& cut) { // angle, dl sig, vtx prob
  if (dl < cut.dlMin || dl > cut.dlMax
      || std::abs(agl) > cut.aglMax
      || vtxPrb < cut.vtxPrbMin) return false;
  return true;
}

bool ana::isFWHM(double mass, double mass_lw, double mass_up) {
  if( mass < mass_up && mass > mass_lw) return true;
  return false;
}

Particle::Particle(const Particle& p):
  _id(p._id), _selfConj(p._selfConj), _longLived(p._longLived),
  _flip(0), _treeIdx(USHRT_MAX)
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
    std::cerr << "No daughter found with pdgId " << _id << std::endl;
    return nullptr;
  }
}

Particle::ParticlePtr Particle::daughter(size_t i) const
{
  if (_daus.size()) {
    return _daus.at(i);
  }
  else {
    std::cerr << "No daughter found with pdgId " << _id << std::endl;
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
#endif
