#include "myAna.h"
#include "PromptD.h"
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
#include "TF1.h"
#include "TH2D.h"

const int nuofY = 3;
const double ybin[nuofY+1] = {0, 1, 2, 3};

bool passTopo(PromptD* t)
{
   return true;
}
bool passPreCut(PromptD* t)
{
   return true;
}

int whichY(const float& y)
{
   for(int i=0; i<nuofY; i++){
      if(std::fabs(y)<ybin[i+1] && std::fabs(y)>ybin[i]) return i;
   }
   return -1;
}

void fillPromptDAccept()
{
   bool isUsePreCuts = false;
   bool isUseAllCuts = false;

   TChain* chain = new TChain("PromptD");
   chain->Add("matchPromptD0Tree_fullSample_reRECO.root");
   PromptD* t = new PromptD(chain);
   std::cout << t->GetEntries() << std::endl;

   TH2D* hPtVsEtaDau1[3];
   TH2D* hPtVsEtaDau1Mtd[3];
   TH2D* hPtVsEtaDau2[3];
   TH2D* hPtVsEtaDau2Mtd[3];

   for(int i=0; i<3; i++){
      hPtVsEtaDau1[i] = new TH2D(Form("hPtVsEtaDau1_y%d", i), "hPtVsEtaDau1", 
         800, -3.2, 3.2, 600, 0, 3);

      hPtVsEtaDau2[i] = new TH2D(Form("hPtVsEtaDau2_y%d", i), "hPtVsEtaDau2", 
         800, -3.2, 3.2, 600, 0, 3);

      hPtVsEtaDau1Mtd[i] = new TH2D(Form("hPtVsEtaDau1Mtd_y%d", i), "hPtVsEtaDau1Mtd", 
         800, -3.2, 3.2, 600, 0, 3);

      hPtVsEtaDau2Mtd[i] = new TH2D(Form("hPtVsEtaDau2Mtd_y%d", i), "hPtVsEtaDau2Mtd", 
         800, -3.2, 3.2, 600, 0, 3);
   }

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      if(t->isSwap || !t->matchGEN) continue;

      if(isUsePreCuts){ 
         if(!passPreCut(t)) continue;
         if(isUseAllCuts && !passTopo(t)) continue;
      }

      const int iy = whichY(t->y);
      if(iy == -1) continue;
      hPtVsEtaDau1[iy]->Fill(t->EtaD1, t->pTD1);
      hPtVsEtaDau2[iy]->Fill(t->EtaD2, t->pTD2);

      if(t->isMtdDau1)  
         hPtVsEtaDau1Mtd[iy]->Fill(t->EtaD1, t->pTD1);
      if(t->isMtdDau2) 
         hPtVsEtaDau2Mtd[iy]->Fill(t->EtaD2, t->pTD2);
   }

   TFile fout("PromptDAcceptHists_reRECO_all_pt0_3.root", "recreate");
   for(int i=0; i<3; i++){
      hPtVsEtaDau1[i]->Write();
      hPtVsEtaDau2[i]->Write();
      hPtVsEtaDau1Mtd[i]->Write();
      hPtVsEtaDau2Mtd[i]->Write();
   }
}
