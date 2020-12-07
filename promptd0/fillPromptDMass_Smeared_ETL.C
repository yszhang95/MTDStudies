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
// table
 // radius (cm) ineff_1layer    ineff_3halflayers eta
 // R> 108       22%              22%               eta<1.7457283
 // 108>R>88     15%              15%               1.7457283<eta<1.9404413
 // 88>R>39      15%              0%                1.9404413<eta<2.7375665
 // 39>R         25%              0%                2.7375665<eta<inf
 const int n_r_edge = 4;
 const int n_eta_edge = 4;
 const double rEdge[n_eta_edge] = {0., 39, 88, 108, /*infinity*/};//{infinity, 108, 88, 39, 0.};
 const double etaEdge[n_eta_edge] = {/*infinity,*/ 2.7375665, 1.9404413, 1.7457283, 0.}; //{0, 1.7457283, 1.9404413, 2.7375665};
 const double ineff_1layer[n_eta_edge] = {0.25, 0.15, 0.15, 0.22}; // {0.22, 0.15, 0.15, 0.25};
 const double ineff_3halflayers[n_eta_edge] = {0., 0., 0.15, 0.22};// {0.22, 0.15, 0., 0.}:
std::pair<double, double> ineff(const double& eta)
 {
    for(int ieta=0; ieta<n_eta_edge; ieta++){
       if(std::fabs(eta) >= etaEdge[ieta]) return std::pair<double, double>(ineff_1layer[ieta], ineff_3halflayers[ieta]);
    }
    return std::pair<double, double>(ineff_1layer[n_eta_edge-1], ineff_3halflayers[n_eta_edge-1]);
 }


inline float invBetaPion(const float& p){
      return std::sqrt(1 + std::pow(ana::massPion/p,2));
}

inline float invBetaKaon(const float& p){
      return std::sqrt(1 + std::pow(ana::massKaon/p,2));
}

void fillPromptDMass_Smeared_ETL()
{
   const double c_cm_ns = 2.99792458e1; //[cm/ns] // mtd
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

   TH3D* hMassVsPtVsYMtd_Smeared = new TH3D("hMassVsPtVsYMtd_Smeared", "hMassVsPtVsYMtd_Smeared", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);

   TH3D* hMassVsPtVsYMtd_1layer = new TH3D("hMassVsPtVsYMtd_1layer", "hMassVsPtVsYMtd_1layer",
          ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);
 
   TH3D* hMassVsPtVsYMtd_3halflayers = new TH3D("hMassVsPtVsYMtd_3halflayers", "hMassVsPtVsYMtd_3halflayers",
          ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);

   //std::cout << hMassVsPtVsY->GetZaxis()->FindBin(1.7) << std::endl;
   TRandom3 r3;
   r3.SetSeed(0);
   double width = 0.035;
   TF1 fRatioETL = TF1("fRatioETL", "300.*(0.0025 + 0.0045*exp(-x/7.6)+0.0025*exp(-fabs(x-4)/3.))");

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      if(fabs(t->EtaD1)<1.5 || fabs(t->EtaD2)<1.5) continue;

      if(!ana::passKinematicCuts(t)) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);

      const float dInvBetaCut1 = std::fabs(t->EtaD1<1.5) ? ana::fExpBTL.Eval(pD1) : ana::fExpETL.Eval(pD1);
      const float dInvBetaCut2 = std::fabs(t->EtaD2<1.5) ? ana::fExpBTL.Eval(pD2) : ana::fExpETL.Eval(pD2);
      const float dInvBetaCut1_Smeared = std::fabs(t->EtaD1<1.5) ? ana::fExpBTL.Eval(pD1) : 1.4* ana::fExpETL.Eval(pD1);
      const float dInvBetaCut2_Smeared = std::fabs(t->EtaD2<1.5) ? ana::fExpBTL.Eval(pD2) : 1.4* ana::fExpETL.Eval(pD2);

      if(!ana::passTopoCuts(t)) continue;

      if(t->isSwap || !t->matchGEN) continue;

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

      // without correction
       bool isMtdDau1;
       bool isMtdDau2;

      double tmtd1 = r3.Gaus(t->tmtd1, width*fRatioETL.Eval(pD1));
      const float beta1_PV = std::fabs(t->EtaD1<1.5) ? t->beta1_PV : (t->pathLength1/(tmtd1-t->t0_PV))*(1./c_cm_ns);
      double tmtd2 = r3.Gaus(t->tmtd2, width*fRatioETL.Eval(pD2));
      const float beta2_PV = std::fabs(t->EtaD2<1.5) ? t->beta2_PV : (t->pathLength2/(tmtd2-t->t0_PV))*(1./c_cm_ns);

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


      if(t->isMtdDau1) is1sigmaPionDau1 = std::fabs(1./beta1_PV - invBetaPion(pD1) - ana::meanPion(t, 1) ) < 1.0 * dInvBetaCut1_Smeared;
      if(t->isMtdDau1) is1sigmaKaonDau1 = std::fabs(1./beta1_PV - invBetaKaon(pD1) - ana::meanKaon(t, 1) ) < 1.0 * dInvBetaCut1_Smeared;
      if(t->isMtdDau2) is1sigmaPionDau2 = std::fabs(1./beta2_PV - invBetaPion(pD2) - ana::meanPion(t, 2) ) < 1.0 * dInvBetaCut2_Smeared;
      if(t->isMtdDau2) is1sigmaKaonDau2 = std::fabs(1./beta2_PV - invBetaKaon(pD2) - ana::meanKaon(t, 2) ) < 1.0 * dInvBetaCut2_Smeared;

      if((t->flavor == 1 && is1sigmaPionDau1 && is1sigmaKaonDau2) || (t->flavor == -1 && is1sigmaKaonDau1 && is1sigmaPionDau2)){
         hMassVsPtVsYMtd_Smeared->Fill(std::fabs(t->y), t->pT, t->mass);
      }
      // eff correction
       auto ineffs_D1 = ineff(t->EtaD1);
       auto ineffs_D2 = ineff(t->EtaD2);
          
       // 1 layer
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
       
       if(r3.Uniform(0., 1.) < ineffs_D1.first) {
          isMtdDau1 = std::fabs(t->EtaD1<1.5) ? t->isMtdDau1 : false;
       } else{
          isMtdDau1 = t->isMtdDau1;
       }  
       
       if(r3.Uniform(0., 1.) < ineffs_D2.first) {
          isMtdDau2 = std::fabs(t->EtaD2<1.5) ? t->isMtdDau2 : false;
       } else{
          isMtdDau2 = t->isMtdDau2;
       }
       
       if(isMtdDau1) is1sigmaPionDau1 = std::fabs(1./beta1_PV - invBetaPion(pD1) - ana::meanPion(t, 1) ) < 1.0 * dInvBetaCut1_Smeared;
       if(isMtdDau1) is1sigmaKaonDau1 = std::fabs(1./beta1_PV - invBetaKaon(pD1) - ana::meanKaon(t, 1) ) < 1.0 * dInvBetaCut1_Smeared;
       if(isMtdDau2) is1sigmaPionDau2 = std::fabs(1./beta2_PV - invBetaPion(pD2) - ana::meanPion(t, 2) ) < 1.0 * dInvBetaCut2_Smeared;
       if(isMtdDau2) is1sigmaKaonDau2 = std::fabs(1./beta2_PV - invBetaKaon(pD2) - ana::meanKaon(t, 2) ) < 1.0 * dInvBetaCut2_Smeared;

      if((t->flavor == 1 && is1sigmaPionDau1 && is1sigmaKaonDau2) || (t->flavor == -1 && is1sigmaKaonDau1 && is1sigmaPionDau2)){
          hMassVsPtVsYMtd_1layer->Fill(std::fabs(t->y), t->pT, t->mass);
       }
       
       // 1.5 layers
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

       if(r3.Uniform(0., 1.) < ineffs_D1.second) {
          isMtdDau1 = std::fabs(t->EtaD1<1.5) ? t->isMtdDau1 : false;
       } else{
          isMtdDau1 = t->isMtdDau1;
       }  
       
       if(r3.Uniform(0., 1.) < ineffs_D2.second) {
          isMtdDau2 = std::fabs(t->EtaD2<1.5) ? t->isMtdDau2 : false;
       } else{
          isMtdDau2 = t->isMtdDau2;
       }
       
       if(isMtdDau1) is1sigmaPionDau1 = std::fabs(1./beta1_PV - invBetaPion(pD1) - ana::meanPion(t, 1) ) < 1.0 * dInvBetaCut1_Smeared;
       if(isMtdDau1) is1sigmaKaonDau1 = std::fabs(1./beta1_PV - invBetaKaon(pD1) - ana::meanKaon(t, 1) ) < 1.0 * dInvBetaCut1_Smeared;
       if(isMtdDau2) is1sigmaPionDau2 = std::fabs(1./beta2_PV - invBetaPion(pD2) - ana::meanPion(t, 2) ) < 1.0 * dInvBetaCut2_Smeared;
       if(isMtdDau2) is1sigmaKaonDau2 = std::fabs(1./beta2_PV - invBetaKaon(pD2) - ana::meanKaon(t, 2) ) < 1.0 * dInvBetaCut2_Smeared;
       
       if((t->flavor == 1 && is1sigmaPionDau1 && is1sigmaKaonDau2) || (t->flavor == -1 && is1sigmaKaonDau1 && is1sigmaPionDau2)){
          hMassVsPtVsYMtd_3halflayers->Fill(std::fabs(t->y), t->pT, t->mass);
       }
   }

   //TFile fout("PromptDMassHists_reRECO_all.root", "recreate");
   //TFile fout("PromptDMassHists_reRECO_all_notopo.root", "recreate");
   //TFile fout("PromptDMassHists_reRECO_all_smeared.root", "recreate");
   TFile fout("PromptDMassHists_reRECO_all_smeared_ineff.root", "recreate");
   hMassVsPtVsY->Write();
   hMassVsPtVsYMtd->Write();
   hVtxProbVsPtVsY->Write();
   hVtxProbVsPtVsYMtd->Write();
   hagl3DVsPtVsY->Write();
   hagl3DVsPtVsYMtd->Write();
   hdlSig3DVsPtVsY->Write();
   hdlSig3DVsPtVsYMtd->Write();

   hMassVsPtVsYMtd_Smeared->Write();
   hMassVsPtVsYMtd_1layer->Write();
   hMassVsPtVsYMtd_3halflayers->Write();
}
