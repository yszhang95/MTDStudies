#include "myAna.h"
#include "LamC3P.h"

#include "TMath.h"
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
#include "TStopwatch.h"
#include "TRandom3.h"

#include <iostream>
#include <sstream>

#define DEBUG 0

struct ST{
  ST(bool _isMtd, float _t, float _sigmat,
      float _t0, float _beta_PV)
    : isMtd(_isMtd), t(_t), sigmat(_sigmat),
      t0(_t0), beta_PV(_beta_PV)
  {};
  void print()
  {
    std::cout << "isMtd:" << isMtd << ", t:" << t
      << ", sigmat:" << sigmat << ", t0:" << t0
      << ", beta_PV:" << beta_PV << std::endl;
  };
  bool isMtd;
  float t;
  float sigmat;
  float t0;
  float beta_PV;
};

inline bool isETL (const float eta) {
  return std::abs(eta) < 3.0 && std::abs(eta) > 1.6;
}

ST smearBeta(TRandom& r, const ST& tinfo, const bool inEff, const float res)
{
  ST smear(tinfo.isMtd, tinfo.t, tinfo.sigmat, tinfo.t0, tinfo.beta_PV);
  // efficiency loss, 15%
  if (inEff && r.Uniform(0., 1.)<0.15) {
    smear.isMtd = false;
  }
  // if kept, smear by sqrt(2)
  // beta = path_length/t; beta_err = path_length/t^2 * t_err;
  // smeared beta_err = path_length/smeared_t^2 * t_err * sqrt(2)
  // smeared beta = path_length/smeared_t = beta*t/smeared_t;
  smear.sigmat = res;
  smear.t = r.Gaus(tinfo.t, smear.sigmat);
  smear.beta_PV = tinfo.beta_PV * (tinfo.t - tinfo.t0)/(smear.t - smear.t0);
  if(DEBUG) std::cout << "Calling smearBeta, ";
  if(DEBUG) smear.print();
  return smear;
}

void fillLamC3PMass(const bool inEff = true, const float res=0.035, const float nRMS=1.0, const bool disableETL=false, const bool forceDisable=true,
    const bool acceptFwdTrk=true)
{
   TChain* chain = new TChain("LamC3P");
   chain->Add("matchLamC3PTree_fullSample_reRECO_all.root");
   LamC3P* t = new LamC3P(chain);

   TH3D* hMassVsPtVsY = new TH3D("hMassVsPtVsY", "hMassVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);
   TH3D* hMassVsPtVsYCent = new TH3D("hMassVsPtVsYCent", "hMassVsPtVsYCent", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);
   TH3D* hMassVsPtVsY30_50 = new TH3D("hMassVsPtVsY30_50", "hMassVsPtVsY30_50", 
            ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);

   TH3D* hVtxProbVsPtVsY = new TH3D("hVtxProbVsPtVsY", "hVtxProbVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);
   TH3D* hVtxProbVsPtVsYCent = new TH3D("hVtxProbVsPtVsYCent", "hVtxProbVsPtVsYCent", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

   TH3D* hagl3DVsPtVsY = new TH3D("hagl3DVsPtVsY", "hagl3DVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);
   TH3D* hagl3DVsPtVsYCent = new TH3D("hagl3DVsPtVsYCent", "hagl3DVsPtVsYCent", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);

   TH3D* hdlSig3DVsPtVsY = new TH3D("hdlSig3DVsPtVsY", "hdlSig3DVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);
   TH3D* hdlSig3DVsPtVsYCent = new TH3D("hdlSig3DVsPtVsYCent", "hdlSig3DVsPtVsYCent", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);

   TH3D* hMassVsPtVsYMtd = new TH3D("hMassVsPtVsYMtd", "hMassVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);
   TH3D* hMassVsPtVsYCentMtd = new TH3D("hMassVsPtVsYCentMtd", "hMassVsPtVsYCentMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);
   TH3D* hMassVsPtVsY30_50Mtd = new TH3D("hMassVsPtVsY30_50Mtd", "hMassVsPtVsY30_50Mtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);

   TH3D* hVtxProbVsPtVsYMtd = new TH3D("hVtxProbVsPtVsYMtd", "hVtxProbVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);
   TH3D* hVtxProbVsPtVsYCentMtd = new TH3D("hVtxProbVsPtVsYCentMtd", "hVtxProbVsPtVsYCentMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

   TH3D* hagl3DVsPtVsYMtd = new TH3D("hagl3DVsPtVsYMtd", "hagl3DVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);
   TH3D* hagl3DVsPtVsYCentMtd = new TH3D("hagl3DVsPtVsYCentMtd", "hagl3DVsPtVsYCentMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);

   TH3D* hdlSig3DVsPtVsYMtd = new TH3D("hdlSig3DVsPtVsYMtd", "hdlSig3DVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);
   TH3D* hdlSig3DVsPtVsYCentMtd = new TH3D("hdlSig3DVsPtVsYCentMtd", "hdlSig3DVsPtVsYCentMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);

   TStopwatch ts;
   ts.Start();

   TRandom3 r3;

   //Long64_t nentries = 100;
   Long64_t nentries = t->GetEntries();
   //std::cout << nentries << std::endl;
   for(Long64_t ientry=0; ientry<nentries; ientry++){
      t->GetEntry(ientry);

      if(!ana::passKinematicCuts(t)) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);
      const float pD3 = t->pTD3 * std::cosh(t->EtaD3);

      //bool isCentral = ana::isCentralEvt(*t);

      const float dInvBetaCut1 = std::fabs(t->EtaD1<1.5) ? ana::fExpBTL.Eval(pD1) : ana::fExpETL.Eval(pD1);
      const float dInvBetaCut2 = std::fabs(t->EtaD2<1.5) ? ana::fExpBTL.Eval(pD2) : ana::fExpETL.Eval(pD2);
      const float dInvBetaCut3 = std::fabs(t->EtaD3<1.5) ? ana::fExpBTL.Eval(pD3) : ana::fExpETL.Eval(pD3);

      if(!ana::passTopoCuts(t)) continue;
      if(t->pT < 1) continue;

      bool isFWHM = ana::isFWHM(t);
      //bool isFWHM = false;

      hMassVsPtVsY->Fill(std::fabs(t->y), t->pT, t->mass);
      //if(isCentral) hMassVsPtVsYCent->Fill(std::fabs(t->y), t->pT, t->mass);
      //if(t->centrality <100 && t->centrality > 60) hMassVsPtVsY30_50->Fill(std::fabs(t->y), t->pT, t->mass);
      if( isFWHM ) {
         hVtxProbVsPtVsY->Fill(std::fabs(t->y), t->pT, t->VtxProb);
         hagl3DVsPtVsY->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
         hdlSig3DVsPtVsY->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
      }
      //if( isFWHM && isCentral ) {
         hVtxProbVsPtVsYCent->Fill(std::fabs(t->y), t->pT, t->VtxProb);
         hagl3DVsPtVsYCent->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
         hdlSig3DVsPtVsYCent->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
      //}

      bool is1sigmaPionDau1 = pD1 > 5. ? true : false;
      bool is1sigmaKaonDau1 = pD1 > 5. ? true : false;
      bool is1sigmaProtonDau1 = pD1 > 5. ? true : false;

      bool is1sigmaPionDau2 = pD2 > 5. ? true : false;
      bool is1sigmaKaonDau2 = pD2 > 5. ? true : false;
      bool is1sigmaProtonDau2 = pD2 > 5. ? true : false;

      bool is1sigmaPionDau3 = pD2 > 5. ? true : false;
      bool is1sigmaKaonDau3 = pD2 > 5. ? true : false;
      bool is1sigmaProtonDau3 = pD2 > 5. ? true : false;

      if (isETL(t->EtaD1)) {
        is1sigmaPionDau1 = true;
        is1sigmaKaonDau1 = true;
        is1sigmaProtonDau1 = true;
      }

      if (isETL(t->EtaD2)) {
        is1sigmaPionDau2 = true;
        is1sigmaKaonDau2 =  true;
        is1sigmaProtonDau2 = true;
      }

      if (isETL(t->EtaD3)) {
        is1sigmaPionDau3 = true;
        is1sigmaKaonDau3 = true;
        is1sigmaProtonDau3 = true;
      }

      ST dau1(t->isMtdDau1, t->tmtd1, t->sigmatmtd1, t->t0_PV, t->beta1_PV);
      ST dau2(t->isMtdDau2, t->tmtd2, t->sigmatmtd2, t->t0_PV, t->beta2_PV);
      ST dau3(t->isMtdDau3, t->tmtd3, t->sigmatmtd3, t->t0_PV, t->beta3_PV);

      auto doSmear = [=, &r3](ST& tinfo, float eta)
      {
        if (isETL(eta)) {
          tinfo = smearBeta(r3, tinfo, inEff, res);
          if (disableETL) tinfo.isMtd = false;
        }
      };

      if(DEBUG) std::cout << "Before smearing, eta=" << t->EtaD1 <<", ";
      if(DEBUG) dau1.print();
      doSmear(dau1, t->EtaD1);
      if(DEBUG) std::cout << "After smearing, eta=" << t->EtaD1 <<", ";
      if(DEBUG) dau1.print();

      if(DEBUG) std::cout << "Before smearing, eta=" << t->EtaD2 <<", ";
      if(DEBUG) dau2.print();
      doSmear(dau2, t->EtaD2);
      if(DEBUG) std::cout << "After smearing, eta=" << t->EtaD2 <<", ";
      if(DEBUG) dau2.print();

      if(DEBUG) std::cout << "Before smearing, eta=" << t->EtaD3 <<", ";
      if(DEBUG) dau3.print();
      doSmear(dau3, t->EtaD3);
      if(DEBUG) std::cout << "After smearing, eta=" << t->EtaD3 <<", ";
      if(DEBUG) dau3.print();

      if(dau1.isMtd){
        is1sigmaPionDau1 = std::fabs(1./dau1.beta_PV - ana::invBetaPion(pD1) - ana::meanPion(t, 1)) < nRMS * dInvBetaCut1;
        is1sigmaKaonDau1 = std::fabs(1./dau1.beta_PV - ana::invBetaKaon(pD1) - ana::meanKaon(t, 1)) < nRMS * dInvBetaCut1;
        is1sigmaProtonDau1 = std::fabs(1./dau1.beta_PV - ana::invBetaProton(pD1) - ana::meanProton(t, 1)) < nRMS * dInvBetaCut1;
        if(!isETL(t->EtaD1)) {
          is1sigmaPionDau1 = std::fabs(1./dau1.beta_PV - ana::invBetaPion(pD1) - ana::meanPion(t, 1)) < 1 * dInvBetaCut1;
          is1sigmaKaonDau1 = std::fabs(1./dau1.beta_PV - ana::invBetaKaon(pD1) - ana::meanKaon(t, 1)) < 1 * dInvBetaCut1;
          is1sigmaProtonDau1 = std::fabs(1./dau1.beta_PV - ana::invBetaProton(pD1) - ana::meanProton(t, 1)) < 1 * dInvBetaCut1;
        } else if (acceptFwdTrk && pD1>5) {
          is1sigmaPionDau1 = true;
          is1sigmaKaonDau1 = true;
          is1sigmaProtonDau1 = true;
        }
      }
      if(dau2.isMtd){
        is1sigmaPionDau2 = std::fabs(1./dau2.beta_PV - ana::invBetaPion(pD2) - ana::meanPion(t, 2)) < nRMS * dInvBetaCut2;
        is1sigmaKaonDau2 = std::fabs(1./dau2.beta_PV - ana::invBetaKaon(pD2) - ana::meanKaon(t, 2)) < nRMS * dInvBetaCut2;
        is1sigmaProtonDau2 = std::fabs(1./dau2.beta_PV - ana::invBetaProton(pD2) - ana::meanProton(t, 2)) < nRMS * dInvBetaCut2;
        if(!isETL(t->EtaD2)) {
          is1sigmaPionDau2 = std::fabs(1./dau2.beta_PV - ana::invBetaPion(pD2) - ana::meanPion(t, 2)) < 1 * dInvBetaCut2;
          is1sigmaKaonDau2 = std::fabs(1./dau2.beta_PV - ana::invBetaKaon(pD2) - ana::meanKaon(t, 2)) < 1 * dInvBetaCut2;
          is1sigmaProtonDau2 = std::fabs(1./dau2.beta_PV - ana::invBetaProton(pD2) - ana::meanProton(t, 2)) < 1 * dInvBetaCut2;
        } else if (acceptFwdTrk && pD2>5) {
          is1sigmaPionDau2 = true;
          is1sigmaKaonDau2 = true;
          is1sigmaProtonDau2 = true;
        }
      }
      if(dau3.isMtd){
        is1sigmaPionDau3 = std::fabs(1./dau3.beta_PV - ana::invBetaPion(pD3) - ana::meanPion(t, 3)) < nRMS * dInvBetaCut3;
        is1sigmaKaonDau3 = std::fabs(1./dau3.beta_PV - ana::invBetaKaon(pD3) - ana::meanKaon(t, 3)) < nRMS * dInvBetaCut3;
        is1sigmaProtonDau3 = std::fabs(1./dau3.beta_PV - ana::invBetaProton(pD3) - ana::meanProton(t, 3)) < nRMS * dInvBetaCut3;
        if(!isETL(t->EtaD3)) {
          is1sigmaPionDau3 = std::fabs(1./dau3.beta_PV - ana::invBetaPion(pD3) - ana::meanPion(t, 3)) < 1 * dInvBetaCut3;
          is1sigmaKaonDau3 = std::fabs(1./dau3.beta_PV - ana::invBetaKaon(pD3) - ana::meanKaon(t, 3)) < 1 * dInvBetaCut3;
          is1sigmaProtonDau3 = std::fabs(1./dau3.beta_PV - ana::invBetaProton(pD3) - ana::meanProton(t, 3)) < 1 * dInvBetaCut3;
        } else if (acceptFwdTrk && pD3>5) {
          is1sigmaPionDau3 = true;
          is1sigmaKaonDau3 = true;
          is1sigmaProtonDau3 = true;
        }
      }

      if((t->flavor == 1 && is1sigmaKaonDau2 && ((is1sigmaPionDau1 && is1sigmaProtonDau3) || (is1sigmaPionDau3 && is1sigmaProtonDau1))) ||
         (t->flavor == -1 && is1sigmaKaonDau1 && ((is1sigmaPionDau2 && is1sigmaProtonDau3) || (is1sigmaPionDau3 && is1sigmaProtonDau2)))
        )
      {
         hMassVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->mass);
         //if(t->centrality <100 && t->centrality > 60) hMassVsPtVsY30_50Mtd->Fill(std::fabs(t->y), t->pT, t->mass);
         if( isFWHM ) {
            hVtxProbVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->VtxProb);
            hagl3DVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
            hdlSig3DVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
         }
         //if(isCentral) {
            hMassVsPtVsYCentMtd->Fill(std::fabs(t->y), t->pT, t->mass);
            if(isFWHM){
               hVtxProbVsPtVsYCentMtd->Fill(std::fabs(t->y), t->pT, t->VtxProb);
               hagl3DVsPtVsYCentMtd->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
               hdlSig3DVsPtVsYCentMtd->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
            }
         //}
      }
   }

   TString nrms = TString::Format("%.1f", nRMS);
   const char* postfix = disableETL ? Form("_NoETL.root") :
     Form("_%0.fps_%sLayer_%sRMS.root",  res * 1000, inEff ? "1" : "2",
         nrms.Replace(1, 1, "p").Data());
   TFile fout(Form("LamC3PMassHists_reRECO%s", postfix), "recreate");
   hMassVsPtVsY->Write();
   hMassVsPtVsYCent->Write();
   hMassVsPtVsYMtd->Write();
   hMassVsPtVsYCentMtd->Write();
   hMassVsPtVsY30_50->Write();
   hMassVsPtVsY30_50Mtd->Write();

   hVtxProbVsPtVsY->Write();
   hVtxProbVsPtVsYMtd->Write();
   hVtxProbVsPtVsYCent->Write();
   hVtxProbVsPtVsYCentMtd->Write();

   hagl3DVsPtVsY->Write();
   hagl3DVsPtVsYMtd->Write();
   hagl3DVsPtVsYCent->Write();
   hagl3DVsPtVsYCentMtd->Write();

   hdlSig3DVsPtVsY->Write();
   hdlSig3DVsPtVsYMtd->Write();
   hdlSig3DVsPtVsYCent->Write();
   hdlSig3DVsPtVsYCentMtd->Write();

   ts.Stop();
   ts.Print();
}
