//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb 28 22:43:52 2019 by ROOT version 6.12/07
// from TChain HyJets/HyJets
// found on file: /eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/HyJets0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root
//////////////////////////////////////////////////////////

#ifndef HyJets_h
#define HyJets_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "D0Cand.h"

// Header file for the classes stored in the TChain if any.

class HyJets : public D0Cand{
public :
   TChain          *fChain;   //!pointer to the analyzed TChain or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TChain if any.

   // Declaration of leaf types
   Float_t         pT;
   Float_t         y;
   Float_t         mass;
   Int_t           centrality;
   Int_t           Ntrkoffline;
   Int_t           Npixel;
   Float_t         HFsumET;
   Float_t         bestvtxX;
   Float_t         bestvtxY;
   Float_t         bestvtxZ;
   Float_t         flavor;
   Float_t         eta;
   Float_t         VtxProb;
   Float_t         m3DCosPointingAngle;
   Float_t         m3DPointingAngle;
   Float_t         m2DCosPointingAngle;
   Float_t         m2DPointingAngle;
   Float_t         m3DDecayLengthSignificance;
   Float_t         m3DDecayLength;
   Float_t         m2DDecayLengthSignificance;
   Float_t         m2DDecayLength;
   Float_t         zDCASignificanceDaugther1;
   Float_t         xyDCASignificanceDaugther1;
   Float_t         NHitD1;
   Bool_t          HighPurityDaugther1;
   Float_t         pTD1;
   Float_t         pTerrD1;
   Float_t         EtaD1;
   Float_t         dedxHarmonic2D1;
   Float_t         zDCASignificanceDaugther2;
   Float_t         xyDCASignificanceDaugther2;
   Float_t         NHitD2;
   Bool_t          HighPurityDaugther2;
   Float_t         pTD2;
   Float_t         pTerrD2;
   Float_t         EtaD2;
   Float_t         dedxHarmonic2D2;
   Float_t         t0_PV;
   Float_t         sigmat0_PV;
   Float_t         beta1_PV;
   Float_t         beta2_PV;
   Float_t         beta1_PVerr;
   Float_t         beta2_PVerr;
   Float_t         tmtd1;
   Float_t         tmtd2;
   Float_t         sigmatmtd1;
   Float_t         sigmatmtd2;
   Float_t         pathLength1;
   Float_t         pathLength2;
   Bool_t          isMtdDau1;
   Bool_t          isMtdDau2;
   Float_t         pT_gen;
   Float_t         eta_gen;
   Float_t         y_gen;
   Int_t           status_gen;
   Int_t           MotherID_gen;
   Int_t           DauID1_gen;
   Int_t           DauID2_gen;
   Int_t           DauID3_gen;

   // List of branches
   TBranch        *b_pT;   //!
   TBranch        *b_y;   //!
   TBranch        *b_mass;   //!
   TBranch        *b_centrality;   //!
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
   TBranch        *b_beta1_PV;   //!
   TBranch        *b_beta2_PV;   //!
   TBranch        *b_beta1_PVerr;   //!
   TBranch        *b_beta2_PVerr;   //!
   TBranch        *b_isMtdDau1;   //!
   TBranch        *b_isMtdDau2;   //!

   HyJets(TChain *tree=0);
   virtual ~HyJets();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t GetEntries(){ return fChain->GetEntries();}
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TChain *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual Float_t  Y(){return y;}
   virtual Float_t  PtD1(){return pTD1;}
   virtual Float_t  PtD2(){return pTD2;}
   virtual Float_t  etaD1(){return EtaD1;}
   virtual Float_t  etaD2(){return EtaD2;}
};

#endif

#ifndef HyJets_cxx
#define HyJets_cxx
HyJets::HyJets(TChain *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/HyJets0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/HyJets0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/eos/cms/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/HyJets0/D0_PiK_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/crab_prompt_d0_ntp_mc_mtd_full_v2/190228_033917/0000/promptd0_mc_mtd_99.root:/d0ana_mc");
      dir->GetObject("HyJets",tree);

   }
   Init(tree);
}

HyJets::~HyJets()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t HyJets::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t HyJets::LoadTree(Long64_t entry)
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

void HyJets::Init(TChain *tree)
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
   fChain->SetBranchAddress("centrality", &centrality, &b_centrality);
   fChain->SetBranchAddress("Ntrkoffline", &Ntrkoffline, &b_Ntrkoffline);
   fChain->SetBranchAddress("Npixel", &Npixel, &b_Npixel);
   fChain->SetBranchAddress("HFsumET", &HFsumET, &b_HFsumET);
   fChain->SetBranchAddress("bestvtxX", &bestvtxX, &b_bestvtxX);
   fChain->SetBranchAddress("bestvtxY", &bestvtxY, &b_bestvtxY);
   fChain->SetBranchAddress("bestvtxZ", &bestvtxZ, &b_bestvtxZ);
   fChain->SetBranchAddress("flavor", &flavor, &b_flavor);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("VtxProb", &VtxProb, &b_VtxProb);
   fChain->SetBranchAddress("3DCosPointingAngle", &m3DCosPointingAngle, &b_3DCosPointingAngleF);
   fChain->SetBranchAddress("3DPointingAngle", &m3DPointingAngle, &b_3DPointingAngle);
   fChain->SetBranchAddress("2DCosPointingAngle", &m2DCosPointingAngle, &b_2DCosPointingAngle);
   fChain->SetBranchAddress("2DPointingAngle", &m2DPointingAngle, &b_2DPointingAngle);
   fChain->SetBranchAddress("3DDecayLengthSignificance", &m3DDecayLengthSignificance, &b_3DDecayLengthSignificance);
   fChain->SetBranchAddress("3DDecayLength", &m3DDecayLength, &b_3DDecayLength);
   fChain->SetBranchAddress("2DDecayLengthSignificance", &m2DDecayLengthSignificance, &b_2DDecayLengthSignificance);
   fChain->SetBranchAddress("2DDecayLength", &m2DDecayLength, &b_2DDecayLength);
   fChain->SetBranchAddress("zDCASignificanceDaugther1", &zDCASignificanceDaugther1, &b_zDCASignificanceDaugther1);
   fChain->SetBranchAddress("xyDCASignificanceDaugther1", &xyDCASignificanceDaugther1, &b_xyDCASignificanceDaugther1);
   fChain->SetBranchAddress("NHitD1", &NHitD1, &b_NHitD1);
   fChain->SetBranchAddress("HighPurityDaugther1", &HighPurityDaugther1, &b_HighPurityDaugther1);
   fChain->SetBranchAddress("pTD1", &pTD1, &b_pTD1);
   fChain->SetBranchAddress("pTerrD1", &pTerrD1, &b_pTerrD1);
   fChain->SetBranchAddress("EtaD1", &EtaD1, &b_EtaD1);
   fChain->SetBranchAddress("dedxHarmonic2D1", &dedxHarmonic2D1, &b_dedxHarmonic2D1);
   fChain->SetBranchAddress("zDCASignificanceDaugther2", &zDCASignificanceDaugther2, &b_zDCASignificanceDaugther2);
   fChain->SetBranchAddress("xyDCASignificanceDaugther2", &xyDCASignificanceDaugther2, &b_xyDCASignificanceDaugther2);
   fChain->SetBranchAddress("NHitD2", &NHitD2, &b_NHitD2);
   fChain->SetBranchAddress("HighPurityDaugther2", &HighPurityDaugther2, &b_HighPurityDaugther2);
   fChain->SetBranchAddress("pTD2", &pTD2, &b_pTD2);
   fChain->SetBranchAddress("pTerrD2", &pTerrD2, &b_pTerrD2);
   fChain->SetBranchAddress("EtaD2", &EtaD2, &b_EtaD2);
   fChain->SetBranchAddress("dedxHarmonic2D2", &dedxHarmonic2D2, &b_dedxHarmonic2D2);
   fChain->SetBranchAddress("beta1_PV", &beta1_PV, &b_beta1_PV);
   fChain->SetBranchAddress("beta2_PV", &beta2_PV, &b_beta2_PV);
   fChain->SetBranchAddress("beta1_PVerr", &beta1_PVerr, &b_beta1_PVerr);
   fChain->SetBranchAddress("beta2_PVerr", &beta2_PVerr, &b_beta2_PVerr);
   fChain->SetBranchAddress("isMtdDau1", &isMtdDau1, &b_isMtdDau1);
   fChain->SetBranchAddress("isMtdDau2", &isMtdDau2, &b_isMtdDau2);
   fChain->SetBranchStatus("2D*", 0);
   fChain->SetBranchStatus("3D*", 0);
   fChain->SetBranchStatus("VtxProb", 0);
   fChain->SetBranchStatus("best*", 0);
   fChain->SetBranchStatus("*DCAS*", 0);
   fChain->SetBranchStatus("High*", 0);
   //fChain->SetBranchStatus("N*", 0);
   fChain->SetBranchStatus("dedx*", 0);
   Notify();
}

Bool_t HyJets::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TChain in a TChain or when when a new TChain
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void HyJets::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t HyJets::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifndef HyJets_cxx
