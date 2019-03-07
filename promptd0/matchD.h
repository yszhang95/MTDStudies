//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb 28 22:43:52 2019 by ROOT version 6.12/07
// from TTree matchD/matchD
// found on file: /eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/matchD0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root
//////////////////////////////////////////////////////////

#ifndef matchD_h
#define matchD_h

#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class matchD {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TTree
   Int_t           fCurrent; //!current Tree number in a TTree

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         pT;
   Float_t         y;
   Float_t         mass;
   Float_t         flavor;
   Float_t         eta;
   Float_t         VtxProb;
   Float_t         isSwap;
   Float_t         matchGEN;
   Float_t         pTD1;
   Float_t         EtaD1;
   Float_t         pTD2;
   Float_t         EtaD2;
   Float_t         beta1_PV;
   Float_t         beta2_PV;
   Float_t         beta1_PVerr;
   Float_t         beta2_PVerr;
   Float_t         sigmatmtd1;
   Float_t         sigmatmtd2;
   Float_t         isMtdDau1;
   Float_t         isMtdDau2;
   Float_t         isGoodMtdDau1;
   Float_t         isGoodMtdDau2;

   // List of branches
   TBranch        *b_pT;   //!
   TBranch        *b_y;   //!
   TBranch        *b_mass;   //!
   TBranch        *b_Ntrkoffline;   //!
   TBranch        *b_Npixel;   //!
   TBranch        *b_HFsumET;   //!
   TBranch        *b_bestvtxX;   //!
   TBranch        *b_bestvtxY;   //!
   TBranch        *b_bestvtxZ;   //!
   TBranch        *b_flavor;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_VtxProb;   //!
   TBranch        *b_3DCosPointingAngleF;   //!
   TBranch        *b_3DPointingAngle;   //!
   TBranch        *b_2DCosPointingAngle;   //!
   TBranch        *b_2DPointingAngle;   //!
   TBranch        *b_3DDecayLengthSignificance;   //!
   TBranch        *b_3DDecayLength;   //!
   TBranch        *b_2DDecayLengthSignificance;   //!
   TBranch        *b_2DDecayLength;   //!
   TBranch        *b_isSwap;   //!
   TBranch        *b_idmom_reco;   //!
   TBranch        *b_matchGEN;   //!
   TBranch        *b_zDCASignificanceDaugther1;   //!
   TBranch        *b_xyDCASignificanceDaugther1;   //!
   TBranch        *b_NHitD1;   //!
   TBranch        *b_HighPurityDaugther1;   //!
   TBranch        *b_pTD1;   //!
   TBranch        *b_pTerrD1;   //!
   TBranch        *b_EtaD1;   //!
   TBranch        *b_dedxHarmonic2D1;   //!
   TBranch        *b_zDCASignificanceDaugther2;   //!
   TBranch        *b_xyDCASignificanceDaugther2;   //!
   TBranch        *b_NHitD2;   //!
   TBranch        *b_HighPurityDaugther2;   //!
   TBranch        *b_pTD2;   //!
   TBranch        *b_pTerrD2;   //!
   TBranch        *b_EtaD2;   //!
   TBranch        *b_dedxHarmonic2D2;   //!
   TBranch        *b_t0_PV;   //!
   TBranch        *b_sigmat0_PV;   //!
   TBranch        *b_beta1_PV;   //!
   TBranch        *b_beta2_PV;   //!
   TBranch        *b_beta1_PVerr;   //!
   TBranch        *b_beta2_PVerr;   //!
   TBranch        *b_tmtd1;   //!
   TBranch        *b_tmtd2;   //!
   TBranch        *b_sigmatmtd1;   //!
   TBranch        *b_sigmatmtd2;   //!
   TBranch        *b_pathLength1;   //!
   TBranch        *b_pathLength2;   //!
   TBranch        *b_isMtdDau1;   //!
   TBranch        *b_isMtdDau2;   //!
   TBranch        *b_isGoodMtdDau1;   //!
   TBranch        *b_isGoodMtdDau2;   //!
   TBranch        *b_pT_gen;   //!
   TBranch        *b_eta_gen;   //!
   TBranch        *b_y_gen;   //!
   TBranch        *b_status_gen;   //!
   TBranch        *b_MotherID_gen;   //!
   TBranch        *b_DauID1_gen;   //!
   TBranch        *b_DauID2_gen;   //!
   TBranch        *b_DauID3_gen;   //!

   matchD(TTree *tree=0);
   virtual ~matchD();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t GetEntries(){ return fChain->GetEntries();}
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifndef matchD_cxx
#define matchD_cxx
matchD::matchD(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/matchD0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/matchD0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/matchD0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root:/d0ana_mc");
      dir->GetObject("matchD",tree);

   }
   Init(tree);
}

matchD::~matchD()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t matchD::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t matchD::LoadTree(Long64_t entry)
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

void matchD::Init(TTree *tree)
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

   fChain->SetBranchAddress("pT", &pT, &b_pT);
   fChain->SetBranchAddress("y", &y, &b_y);
   fChain->SetBranchAddress("mass", &mass, &b_mass);
   fChain->SetBranchAddress("flavor", &flavor, &b_flavor);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("isSwap", &isSwap, &b_isSwap);
   fChain->SetBranchAddress("matchGEN", &matchGEN, &b_matchGEN);
   fChain->SetBranchAddress("pTD1", &pTD1, &b_pTD1);
   fChain->SetBranchAddress("EtaD1", &EtaD1, &b_EtaD1);
   fChain->SetBranchAddress("pTD2", &pTD2, &b_pTD2);
   fChain->SetBranchAddress("EtaD2", &EtaD2, &b_EtaD2);
   fChain->SetBranchAddress("beta1_PV", &beta1_PV, &b_beta1_PV);
   fChain->SetBranchAddress("beta2_PV", &beta2_PV, &b_beta2_PV);
   fChain->SetBranchAddress("beta1_PVerr", &beta1_PVerr, &b_beta1_PVerr);
   fChain->SetBranchAddress("beta2_PVerr", &beta2_PVerr, &b_beta2_PVerr);
   fChain->SetBranchAddress("sigmatmtd1", &sigmatmtd1, &b_sigmatmtd1);
   fChain->SetBranchAddress("sigmatmtd2", &sigmatmtd2, &b_sigmatmtd2);
   fChain->SetBranchAddress("isMtdDau1", &isMtdDau1, &b_isMtdDau1);
   fChain->SetBranchAddress("isMtdDau2", &isMtdDau2, &b_isMtdDau2);
   fChain->SetBranchAddress("isGoodMtdDau1", &isGoodMtdDau1, &b_isGoodMtdDau1);
   fChain->SetBranchAddress("isGoodMtdDau2", &isGoodMtdDau2, &b_isGoodMtdDau2);
   Notify();
}

Bool_t matchD::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TTree or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void matchD::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t matchD::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifndef matchD_cxx
