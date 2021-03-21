#ifndef ParticleTreeMC2_cxx
#define ParticleTreeMC2_cxx
#include <iostream>
#include "ParticleTreeMC2.hxx"

using std::cout;
using std::endl;

ParticleTreeMC2::ParticleTreeMC2(TTree *tree) : fChain(tree)
{
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) {
    fChain == nullptr;
    return;
  }
  ParticleTree::Init(tree);
  ParticleTreeMC::Init(tree);
  Init(tree);
}

ParticleTreeMC2::~ParticleTreeMC2()
{
}

void ParticleTreeMC2::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set object pointer
  _trk_beta = 0;
  _trk_gammaSq = 0;
  _trk_massSq = 0;
  _trk_pathLength = 0;
  _trk_tMTD = 0;
  _trk_tMTDErr = 0;

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("bestvtxT", &_bestvtxT, &b_bestvtxT);
  fChain->SetBranchAddress("bestvtxTerr", &_bestvtxTerr, &b_bestvtxTerr);

  fChain->SetBranchAddress("trk_beta", &_trk_beta, &b_trk_beta);
  fChain->SetBranchAddress("trk_gammaSq", &_trk_gammaSq, &b_trk_gammaSq);
  fChain->SetBranchAddress("trk_massSq", &_trk_massSq, &b_trk_massSq);
  fChain->SetBranchAddress("trk_pathLength", &_trk_pathLength, &b_trk_pathLength);
  fChain->SetBranchAddress("trk_tMTD", &_trk_tMTD, &b_trk_tMTD);
  fChain->SetBranchAddress("trk_tMTDErr", &_trk_tMTDErr, &b_trk_tMTDErr);
}
#endif
