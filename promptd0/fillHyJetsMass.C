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
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);
   TH3F* hMassVsPtVsYCent = new TH3F("hMassVsPtVsYCent", "hMassVsPtVsYCent", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);

   TH3F* hVtxProbVsPtVsY = new TH3F("hVtxProbVsPtVsY", "hVtxProbVsPtVsY", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);
   TH3F* hVtxProbVsPtVsYCent = new TH3F("hVtxProbVsPtVsYCent", "hVtxProbVsPtVsYCent", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

   TH3F* hagl3DVsPtVsY = new TH3F("hagl3DVsPtVsY", "hagl3DVsPtVsY", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);
   TH3F* hagl3DVsPtVsYCent = new TH3F("hagl3DVsPtVsYCent", "hagl3DVsPtVsYCent", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);

   TH3F* hdlSig3DVsPtVsY = new TH3F("hdlSig3DVsPtVsY", "hdlSig3DVsPtVsY", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);
   TH3F* hdlSig3DVsPtVsYCent = new TH3F("hdlSig3DVsPtVsYCent", "hdlSig3DVsPtVsYCent", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);

   TH3F* hMassVsPtVsYMtd = new TH3F("hMassVsPtVsYMtd", "hMassVsPtVsYMtd", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);
   TH3F* hMassVsPtVsYCentMtd = new TH3F("hMassVsPtVsYCentMtd", "hMassVsPtVsYCentMtd", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);

   TH3F* hVtxProbVsPtVsYMtd = new TH3F("hVtxProbVsPtVsYMtd", "hVtxProbVsPtVsYMtd", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);
   TH3F* hVtxProbVsPtVsYCentMtd = new TH3F("hVtxProbVsPtVsYCentMtd", "hVtxProbVsPtVsYCentMtd", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

   TH3F* hagl3DVsPtVsYMtd = new TH3F("hagl3DVsPtVsYMtd", "hagl3DVsPtVsYMtd", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);
   TH3F* hagl3DVsPtVsYCentMtd = new TH3F("hagl3DVsPtVsYCentMtd", "hagl3DVsPtVsYCentMtd", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);

   TH3F* hdlSig3DVsPtVsYMtd = new TH3F("hdlSig3DVsPtVsYMtd", "hdlSig3DVsPtVsYMtd", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);
   TH3F* hdlSig3DVsPtVsYCentMtd = new TH3F("hdlSig3DVsPtVsYCentMtd", "hdlSig3DVsPtVsYCentMtd", 
         ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);

   std::cout << hMassVsPtVsY->GetZaxis()->FindBin(1.7) << std::endl;

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      if(!ana::passKinematicCuts(t)) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);

      bool isCentral = ana::isCentralEvt(*t);

      const float dInvBetaCut1 = std::fabs(t->EtaD1<1.5) ? ana::fExpBTL.Eval(pD1) : ana::fExpETL.Eval(pD1);
      const float dInvBetaCut2 = std::fabs(t->EtaD2<1.5) ? ana::fExpBTL.Eval(pD2) : ana::fExpETL.Eval(pD2);

      if(!ana::passTopoCuts(t)) continue;

      bool isFWHM = ana::isFWHM(t);

      hMassVsPtVsY->Fill(t->y, t->pT, t->mass);
      if(isCentral) hMassVsPtVsYCent->Fill(t->y, t->pT, t->mass);
      if( isFWHM ) {
         hVtxProbVsPtVsY->Fill(t->y, t->pT, t->VtxProb);
         hagl3DVsPtVsY->Fill(t->y, t->pT, t->m3DPointingAngle);
         hdlSig3DVsPtVsY->Fill(t->y, t->pT, t->m3DDecayLengthSignificance);
      }
      if( isFWHM && isCentral ) {
         hVtxProbVsPtVsYCent->Fill(t->y, t->pT, t->VtxProb);
         hagl3DVsPtVsYCent->Fill(t->y, t->pT, t->m3DPointingAngle);
         hdlSig3DVsPtVsYCent->Fill(t->y, t->pT, t->m3DDecayLengthSignificance);
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
         hMassVsPtVsYMtd->Fill(t->y, t->pT, t->mass);
         if( isFWHM ) {
            hVtxProbVsPtVsYMtd->Fill(t->y, t->pT, t->VtxProb);
            hagl3DVsPtVsYMtd->Fill(t->y, t->pT, t->m3DPointingAngle);
            hdlSig3DVsPtVsYMtd->Fill(t->y, t->pT, t->m3DDecayLengthSignificance);
         }
         if(isCentral) {
            hMassVsPtVsYCentMtd->Fill(t->y, t->pT, t->mass);
            if(isFWHM){
               hVtxProbVsPtVsYCentMtd->Fill(t->y, t->pT, t->VtxProb);
               hagl3DVsPtVsYCentMtd->Fill(t->y, t->pT, t->m3DPointingAngle);
               hdlSig3DVsPtVsYCentMtd->Fill(t->y, t->pT, t->m3DDecayLengthSignificance);
            }
         }
      }
   }

   TFile fout("hyjetsMassHists_reRECO_all.root", "recreate");
   hMassVsPtVsY->Write();
   hMassVsPtVsYCent->Write();
   hMassVsPtVsYMtd->Write();
   hMassVsPtVsYCentMtd->Write();
}
