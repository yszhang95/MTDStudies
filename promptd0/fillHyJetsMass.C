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

bool passTopoCuts(HyJets* t)
{
   return true;
}

void fillHyJetsMass()
{
   //TChain* chain = new TChain("d0ana/VertexCompositeNtuple");
   //chain->Add("/eos/cms/store/group/phys_heavyions/MTD/anstahll/VertexCompositeAnalysis/HydJets_mc_mtd_NTUPLE_20190227/Hydjet_5p02TeV_TuneCP5_MTD/HydJets_mc_mtd_NTUPLE_20190227/190227_165027/0001/hyjets_mc_mtd_1165.root");
   //TFileCollection* fc = new TFileCollection("dum", "", "hyjets_sample.list");
   
   TChain* chain = new TChain("d0ana_mc/VertexCompositeNtuple");
   TFileCollection* fc = new TFileCollection("dum", "", "newhyjets.list");
   chain->AddFileInfoList(fc->GetList()); 
   HyJets* t = new HyJets(chain);
   std::cout << t->GetEntries() << std::endl;

//   TF1* fExpBTL = new TF1("fExpBTL_dInvBetaRMS","0.005 + 0.016*exp(-x/4.4)");
//   TF1* fExpETL = new TF1("fExpETL_dInvBetaRMS","0.003 + 0.006*exp(-x/7.6)");

   TH3F* hMassVsPtVsY = new TH3F("hMassVsPtVsY", "hMassVsPtVsY", 60, -3, 3, 100, 0, 10, 60, 1.7, 2.0);
   TH3F* hMassVsPtVsYCent = new TH3F("hMassVsPtVsYCent", "hMassVsPtVsYCent", 60, -3, 3, 100, 0, 10, 60, 1.7, 2.0);

   TH3F* hMassVsPtVsYMtd = new TH3F("hMassVsPtVsYMtd", "hMassVsPtVsYMtd", 60, -3, 3, 100, 0, 10, 60, 1.7, 2.0);
   TH3F* hMassVsPtVsYCentMtd = new TH3F("hMassVsPtVsYCentMtd", "hMassVsPtVsYCentMtd", 60, -3, 3, 100, 0, 10, 60, 1.7, 2.0);

   std::cout << hMassVsPtVsY->GetZaxis()->FindBin(1.7);

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      if(!ana::passKinematicCuts(t)) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);

      bool isCentral = ana::isCentralEvt(*t);

      const float dInvBetaCut1 = std::fabs(t->EtaD1<1.5) ? ana::fExpBTL.Eval(pD1) : ana::fExpETL.Eval(pD1);
      const float dInvBetaCut2 = std::fabs(t->EtaD2<1.5) ? ana::fExpBTL.Eval(pD2) : ana::fExpETL.Eval(pD2);

      if(!passTopoCuts(t)) continue;

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

      if(t->beta1_PV!=-99) is1sigmaPionDau1 = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 1.0 * dInvBetaCut1;
      if(t->beta1_PV!=-99) is1sigmaKaonDau1 = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 1.0 * dInvBetaCut1;
      if(t->beta2_PV!=-99) is1sigmaPionDau2 = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 1.0 * dInvBetaCut2;
      if(t->beta2_PV!=-99) is1sigmaKaonDau2 = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 1.0 * dInvBetaCut2;

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
