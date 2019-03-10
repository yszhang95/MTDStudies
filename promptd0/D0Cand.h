//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb 28 22:43:52 2019 by ROOT version 6.12/07
// from TChain D0Cand/D0Cand
// found on file: /eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/D0Cand0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root
//////////////////////////////////////////////////////////

#ifndef D0Cand_h
#define D0Cand_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TChain if any.

class D0Cand {
public :
   TChain          *fChain;   //!pointer to the analyzed TChain or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TChain if any.

   // Declaration of leaf types

   D0Cand(TChain *tree=0);
   virtual ~D0Cand();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t GetEntries(){ return fChain->GetEntries();}
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TChain *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual Float_t  Y(){return 0;}
   virtual Float_t  PtD1(){return 0;}
   virtual Float_t  PtD2(){return 0;}
   virtual Float_t  etaD1(){return 0;}
   virtual Float_t  etaD2(){return 0;}
};

#endif

#ifndef D0Cand_cxx
#define D0Cand_cxx
D0Cand::D0Cand(TChain *tree) : fChain(0) 
{
   if (tree == 0) {
      }
   Init(tree);
}

D0Cand::~D0Cand()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t D0Cand::GetEntry(Long64_t entry)
{
   if (!fChain) return 0;
   return -1;
}
Long64_t D0Cand::LoadTree(Long64_t entry)
{
   return 0;
}

void D0Cand::Init(TChain *tree)
{
   if (!tree) return;
   fChain = 0;
   return ;
}

Bool_t D0Cand::Notify()
{
   return kTRUE;
}

void D0Cand::Show(Long64_t entry)
{
}
Int_t D0Cand::Cut(Long64_t entry)
{
   return 1;
}
#endif // #ifndef D0Cand_cxx
