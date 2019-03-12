#include "TMath.h"
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "THashList.h"
#include "TLatex.h"
#include "TFileCollection.h"
#include "TVector3.h"
#include "TNtuple.h"
#include "TFile.h"

#include "myAna.h"

int whichY(const float& y)
{
   for(int i=0; i<ana::nuOfY; i++){
      if(y>ana::ybin[i] && y<ana::ybin[i+1]) return i;
   }
   return -1;
}

void fillGen()
{
   TH1F::SetDefaultSumw2(true);

   TChain *t = new TChain("lamc3pana_mc/genCandidateNtuple");
   TFileCollection* fc = new TFileCollection("dum", "", "lamc3psignal_new.list");
   t->AddFileInfoList(fc->GetList()); 
   
   std::cout << "total entries: " << t->GetEntries() << std::endl;

   TH1F* hPt[ana::nuOfY];
   for(int iy=0; iy<ana::nuOfY; iy++){
      hPt[iy] = new TH1F(Form("hPt%d", iy), "", 100, 0, 10);
   }

   float pT_gen;
   float y_gen;
   int dau1ID_gen;
   int dau2ID_gen;
   int dau3ID_gen;

   t->SetBranchAddress("pT_gen", &pT_gen);
   t->SetBranchAddress("y_gen", &y_gen);
   t->SetBranchAddress("DauID1_gen", &dau1ID_gen);
   t->SetBranchAddress("DauID2_gen", &dau2ID_gen);
   t->SetBranchAddress("DauID3_gen", &dau3ID_gen);

   for(Long64_t ientry=0; ientry<t->GetEntries(); ++ientry){
      t->GetEntry(ientry);

      const int iy = whichY(y_gen);
      if( iy == -1 ) continue;

      if((std::fabs(dau1ID_gen) == 211 && std::fabs(dau2ID_gen) == 321 && std::fabs(dau3ID_gen) == 2212) 
      || (std::fabs(dau1ID_gen) == 321 && std::fabs(dau2ID_gen) == 211 && std::fabs(dau3ID_gen) == 2212)
      || (std::fabs(dau1ID_gen) == 211 && std::fabs(dau3ID_gen) == 321 && std::fabs(dau2ID_gen) == 2212) 
      || (std::fabs(dau1ID_gen) == 321 && std::fabs(dau3ID_gen) == 211 && std::fabs(dau2ID_gen) == 2212)
      || (std::fabs(dau3ID_gen) == 211 && std::fabs(dau2ID_gen) == 321 && std::fabs(dau1ID_gen) == 2212) 
      || (std::fabs(dau3ID_gen) == 321 && std::fabs(dau2ID_gen) == 211 && std::fabs(dau1ID_gen) == 2212)
      )
            hPt[iy]->Fill(pT_gen);
   }
   TFile* fout;
   fout = new TFile("genPt_reRECO.root", "recreate");
   for(int iy=0; iy<ana::nuOfY; iy++)
      hPt[iy]->Write();
}
