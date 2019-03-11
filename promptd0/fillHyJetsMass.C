#include "myAna.h"
#include "HyJets.h"
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
#include "TH3F.h"

inline float invBetaPion(const float& p){
      return std::sqrt(1 + std::pow(ana::massPion/p,2));
}

inline float invBetaKaon(const float& p){
      return std::sqrt(1 + std::pow(ana::massKaon/p,2));
}


void fillHyJetsMass()
{
   TChain* chain = new TChain("d0ana_mc/VertexCompositeNtuple");
   TFileCollection* fc = new TFileCollection("dum", "", "newHyJets.list");
   chain->AddFileInfoList(fc->GetList()); 
   HyJets* t = new HyJets(chain);
   std::cout << t->GetEntries() << std::endl;

   TH3F* hMassVsPtVsY = new TH3F("hMassVsPtVsY", "hMassVsPtVsY", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, massMax);
   TH3F* hMassVsPtVsYCent = new TH3F("hMassVsPtVsYCent", "hMassVsPtVsYCent", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, massMax);

   TH3F* hMassVsPtVsYMtd = new TH3F("hMassVsPtVsYMtd", "hMassVsPtVsYMtd", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, massMax);
   TH3F* hMassVsPtVsYCentMtd = new TH3F("hMassVsPtVsYCentMtd", "hMassVsPtVsYCentMtd", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, massMax);

   std::cout << hMassVsPtVsY->GetZaxis()->FindBin(1.7);

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      if(!ana::passKinematicCuts(t)) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);

      bool isCentral = ana::isCentralEvt(*t);

      const float dInvBetaCut1 = std::fabs(t->EtaD1<1.5) ? ana::fExpBTL.Eval(pD1) : ana::fExpETL.Eval(pD1);
      const float dInvBetaCut2 = std::fabs(t->EtaD2<1.5) ? ana::fExpBTL.Eval(pD2) : ana::fExpETL.Eval(pD2);

      if(!ana::passTopoCuts(t)) continue;

      hMassVsPtVsY->Fill(t->y, t->pT, t->mass);
      if(isCentral) hMassVsPtVsYCent->Fill(t->y, t->pT, t->mass);

      bool is1sigmaPionDau1;
      bool is1sigmaKaonDau1;
      bool is1sigmaPionDau2;
      bool is1sigmaKaonDau2;

      if(ana::isKeepNoMtd){
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
         hMassVsPtVsYMtd->Fill(t->y, t->pT, t->mass);
         if(isCentral) hMassVsPtVsYCentMtd->Fill(t->y, t->pT, t->mass);
      }
   }

   TFile fout("hyjetsMassHists_reRECO_all.root", "recreate");
   hMassVsPtVsY->Write();
   hMassVsPtVsYCent->Write();
   hMassVsPtVsYMtd->Write();
   hMassVsPtVsYCentMtd->Write();
}
