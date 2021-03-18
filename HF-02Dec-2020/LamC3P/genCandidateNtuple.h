//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb 28 22:44:09 2019 by ROOT version 6.12/07
// from TTree genCandidateNtuple/genCandidateNtuple
// found on file: /eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/PromptD0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root
//////////////////////////////////////////////////////////

#ifndef genCandidateNtuple_h
#define genCandidateNtuple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class genCandidateNtuple {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         pT_gen;
   Float_t         eta_gen;
   Float_t         y_gen;
   Int_t           status_gen;
   Int_t           MotherID_gen;
   Int_t           DauID1_gen;
   Int_t           DauID2_gen;
   Int_t           DauID3_gen;

   // List of branches
   TBranch        *b_pT_gen;   //!
   TBranch        *b_eta_gen;   //!
   TBranch        *b_y_gen;   //!
   TBranch        *b_status_gen;   //!
   TBranch        *b_MotherID_gen;   //!
   TBranch        *b_DauID1_gen;   //!
   TBranch        *b_DauID2_gen;   //!
   TBranch        *b_DauID3_gen;   //!

   genCandidateNtuple(TTree *tree=0);
   virtual ~genCandidateNtuple();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef genCandidateNtuple_cxx
genCandidateNtuple::genCandidateNtuple(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/PromptD0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/PromptD0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/PromptD0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root:/d0ana_mc");
      dir->GetObject("genCandidateNtuple",tree);

   }
   Init(tree);
}

genCandidateNtuple::~genCandidateNtuple()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t genCandidateNtuple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t genCandidateNtuple::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void genCandidateNtuple::Init(TTree *tree)
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
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("pT_gen", &pT_gen, &b_pT_gen);
   fChain->SetBranchAddress("eta_gen", &eta_gen, &b_eta_gen);
   fChain->SetBranchAddress("y_gen", &y_gen, &b_y_gen);
   fChain->SetBranchAddress("status_gen", &status_gen, &b_status_gen);
   fChain->SetBranchAddress("MotherID_gen", &MotherID_gen, &b_MotherID_gen);
   fChain->SetBranchAddress("DauID1_gen", &DauID1_gen, &b_DauID1_gen);
   fChain->SetBranchAddress("DauID2_gen", &DauID2_gen, &b_DauID2_gen);
   fChain->SetBranchAddress("DauID3_gen", &DauID3_gen, &b_DauID3_gen);
   Notify();
}

Bool_t genCandidateNtuple::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void genCandidateNtuple::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t genCandidateNtuple::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef genCandidateNtuple_cxx
