#include "myAna.h"
#include "LamC3P.h"
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

bool passTopo(LamC3P* t)
{
   return true;
}
bool passPreCut(LamC3P* t)
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

void fillLamC3PAccept()
{
   bool isUsePreCuts = false;
   bool isUseAllCuts = false;

   TChain* chain = new TChain("LamC3P");
   chain->Add("matchLamC3PTree_fullSample_reRECO_all.root");
   LamC3P* t = new LamC3P(chain);
   std::cout << t->GetEntries() << std::endl;

   TH2D* hPtVsEtaPos[3];
   TH2D* hPtVsEtaPosMtd[3];
   TH2D* hPtVsEtaNeg[3];
   TH2D* hPtVsEtaNegMtd[3];

   for(int i=0; i<3; i++){
      hPtVsEtaPos[i] = new TH2D(Form("hPtVsEtaPos_y%d", i), "hPtVsEtaPos", 
         800, -3.2, 3.2, 600, 0, 3);

      hPtVsEtaNeg[i] = new TH2D(Form("hPtVsEtaNeg_y%d", i), "hPtVsEtaNeg", 
         800, -3.2, 3.2, 600, 0, 3);

      hPtVsEtaPosMtd[i] = new TH2D(Form("hPtVsEtaPosMtd_y%d", i), "hPtVsEtaPosMtd", 
         800, -3.2, 3.2, 600, 0, 3);

      hPtVsEtaNegMtd[i] = new TH2D(Form("hPtVsEtaNegMtd_y%d", i), "hPtVsEtaNegMtd", 
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

      if(t->flavor == 1){
         hPtVsEtaPos[iy]->Fill(t->EtaD1, t->pTD1);
         hPtVsEtaPos[iy]->Fill(t->EtaD3, t->pTD3);

         hPtVsEtaNeg[iy]->Fill(t->EtaD2, t->pTD2);
      }
      if(t->flavor == -1){
         hPtVsEtaPos[iy]->Fill(t->EtaD1, t->pTD1);

         hPtVsEtaNeg[iy]->Fill(t->EtaD2, t->pTD2);
         hPtVsEtaNeg[iy]->Fill(t->EtaD3, t->pTD3);
      }

      if(t->flavor == 1){
         if(t->isMtdDau1)  
            hPtVsEtaPosMtd[iy]->Fill(t->EtaD1, t->pTD1);
         if(t->isMtdDau3)  
            hPtVsEtaPosMtd[iy]->Fill(t->EtaD3, t->pTD3);

         if(t->isMtdDau2)  
            hPtVsEtaNegMtd[iy]->Fill(t->EtaD2, t->pTD2);
      }
      if(t->flavor == -1){
         if(t->isMtdDau1)  
            hPtVsEtaPosMtd[iy]->Fill(t->EtaD1, t->pTD1);

         if(t->isMtdDau2)  
            hPtVsEtaNegMtd[iy]->Fill(t->EtaD2, t->pTD2);
         if(t->isMtdDau3)  
            hPtVsEtaNegMtd[iy]->Fill(t->EtaD3, t->pTD3);
      }
   }

   TFile fout("LamC3PAcceptHists_reRECO_all_pt0_3.root", "recreate");
   for(int i=0; i<3; i++){
      hPtVsEtaPos[i]->Write();
      hPtVsEtaNeg[i]->Write();
      hPtVsEtaPosMtd[i]->Write();
      hPtVsEtaNegMtd[i]->Write();
   }
}
