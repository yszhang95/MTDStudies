//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Mar  5 23:46:39 2019 by ROOT version 6.12/07
// from TChain VertexCompositeNtuple/VertexCompositeNtuple
// found on file: /eos/cms/store/group/phys_heavyions/MTD/davidlw/Hydjet_5p02TeV_TuneCP5_MTD/crab_Cand_mc_lamc3p_mtd_pt2_y-10to05_v8/190304_232024/0000/hyjets_mc_mtd_cent_pt2to3_y0to05_pca5mm_9.root
//////////////////////////////////////////////////////////

#ifndef Cand_h
#define Cand_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TChain if any.

class Cand {
public :
   TChain          *fChain;   //!pointer to the analyzed TChain or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   Cand(TChain *tree=0);
   virtual ~Cand();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t GetEntries(){ return fChain->GetEntries();}
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TChain *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual Float_t  Y(){return 0;}
   virtual Float_t  pT(){ return 0;}
   virtual Float_t  Mass(){return 0;}
   virtual Float_t  PtD1(){return 0;}
   virtual Float_t  PtD2(){return 0;}
   virtual Float_t  etaD1(){return 0;}
   virtual Float_t  etaD2(){return 0;}
};

#endif

#ifndef Cand_cxx
#define Cand_cxx
Cand::Cand(TChain *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   Init(tree);
}

Cand::~Cand()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Cand::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Cand::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   return 0;
}

void Cand::Init(TChain *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
}

Bool_t Cand::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TChain in a TChain or when when a new TChain
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Cand::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Cand::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifndef Cand_cxx
