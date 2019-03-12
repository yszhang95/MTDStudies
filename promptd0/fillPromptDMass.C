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
#include "TH3D.h"

inline float invBetaPion(const float& p){
      return std::sqrt(1 + std::pow(ana::massPion/p,2));
}

inline float invBetaKaon(const float& p){
      return std::sqrt(1 + std::pow(ana::massKaon/p,2));
}

void fillPromptDMass()
{
   TChain* chain = new TChain("PromptD");
   chain->Add("matchPromptD0Tree_fullSample_reRECO.root");
   PromptD* t = new PromptD(chain);
   std::cout << t->GetEntries() << std::endl;

   TH3D* hMassVsPtVsY = new TH3D("hMassVsPtVsY", "hMassVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);

   TH3D* hVtxProbVsPtVsY = new TH3D("hVtxProbVsPtVsY", "hVtxProbVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

   TH3D* hagl3DVsPtVsY = new TH3D("hagl3DVsPtVsY", "hagl3DVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);

   TH3D* hdlSig3DVsPtVsY = new TH3D("hdlSig3DVsPtVsY", "hdlSig3DVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);

   TH3D* hMassVsPtVsYMtd = new TH3D("hMassVsPtVsYMtd", "hMassVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);

   TH3D* hVtxProbVsPtVsYMtd = new TH3D("hVtxProbVsPtVsYMtd", "hVtxProbVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

   TH3D* hagl3DVsPtVsYMtd = new TH3D("hagl3DVsPtVsYMtd", "hagl3DVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);

   TH3D* hdlSig3DVsPtVsYMtd = new TH3D("hdlSig3DVsPtVsYMtd", "hdlSig3DVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);

   std::cout << hMassVsPtVsY->GetZaxis()->FindBin(1.7) << std::endl;

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      if(!ana::passKinematicCuts(t)) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);

      const float dInvBetaCut1 = std::fabs(t->EtaD1<1.5) ? ana::fExpBTL.Eval(pD1) : ana::fExpETL.Eval(pD1);
      const float dInvBetaCut2 = std::fabs(t->EtaD2<1.5) ? ana::fExpBTL.Eval(pD2) : ana::fExpETL.Eval(pD2);

      if(!ana::passTopoCuts(t)) continue;

      bool isFWHM = ana::isFWHM(t);

      hMassVsPtVsY->Fill(std::fabs(t->y), t->pT, t->mass);
      if( isFWHM ) {
         hVtxProbVsPtVsY->Fill(std::fabs(t->y), t->pT, t->VtxProb);
         hagl3DVsPtVsY->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
         hdlSig3DVsPtVsY->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
      }

      bool is1sigmaPionDau1;
      bool is1sigmaKaonDau1;
      bool is1sigmaPionDau2;
      bool is1sigmaKaonDau2;

      if(t->pT >1.5){
         is1sigmaPionDau1 = true;
         is1sigmaKaonDau1 = true;
         is1sigmaPionDau2 = true;
         is1sigmaKaonDau2 = true;
      } else{
         is1sigmaPionDau1 = false;
         is1sigmaKaonDau1 = false;
         is1sigmaPionDau2 = false;
         is1sigmaKaonDau2 = false;
      }

      if(t->isMtdDau1) is1sigmaPionDau1 = std::fabs(1./t->beta1_PV - invBetaPion(pD1) - ana::meanPion(t, 1) ) < 1.0 * dInvBetaCut1;
      if(t->isMtdDau1) is1sigmaKaonDau1 = std::fabs(1./t->beta1_PV - invBetaKaon(pD1) - ana::meanKaon(t, 1) ) < 1.0 * dInvBetaCut1;
      if(t->isMtdDau2) is1sigmaPionDau2 = std::fabs(1./t->beta2_PV - invBetaPion(pD2) - ana::meanPion(t, 2) ) < 1.0 * dInvBetaCut2;
      if(t->isMtdDau2) is1sigmaKaonDau2 = std::fabs(1./t->beta2_PV - invBetaKaon(pD2) - ana::meanKaon(t, 2) ) < 1.0 * dInvBetaCut2;

      if((t->flavor == 1 && is1sigmaPionDau1 && is1sigmaKaonDau2) || (t->flavor == -1 && is1sigmaKaonDau1 && is1sigmaPionDau2)){
         hMassVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->mass);
         if( isFWHM ) {
            hVtxProbVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->VtxProb);
            hagl3DVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
            hdlSig3DVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
         }
      }
   }

   TFile fout("PromptDMassHists_reRECO_all.root", "recreate");
   hMassVsPtVsY->Write();
   hMassVsPtVsYMtd->Write();
   hVtxProbVsPtVsY->Write();
   hVtxProbVsPtVsYMtd->Write();
   hagl3DVsPtVsY->Write();
   hagl3DVsPtVsYMtd->Write();
   hdlSig3DVsPtVsY->Write();
   hdlSig3DVsPtVsYMtd->Write();
}
