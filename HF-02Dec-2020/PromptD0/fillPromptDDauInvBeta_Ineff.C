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
#include "TRandom3.h"

inline float invBetaPion(const float& p){
  return std::sqrt(1 + std::pow(ana::massPion/p,2));
}

inline float invBetaKaon(const float& p){
  return std::sqrt(1 + std::pow(ana::massKaon/p,2));
}

inline bool isETL (const float eta) {
  return eta < 3.0 && eta > 1.6;
}

const float etaTurnPoint = 1.94;

enum WhereLost {innerLost, outerLost};

TH2* hInvBetaVsP;
TH2* hInvBetaVsP1Sigma;

void fillPromptDDauInvBeta_Ineff(WhereLost whereLost=innerLost)
{
   //TChain* chain = new TChain("d0ana_mc/VertexCompositeNtuple");
   //TFileCollection* fc = new TFileCollection("dum", "", name.c_str());
   //chain->AddFileInfoList(fc->GetList()); 
   //HyJets* t = new HyJets(chain);

   TChain* chain = new TChain("PromptD");
   chain->Add("matchPromptD0Tree_fullSample_reRECO.root");
   PromptD* t = new PromptD(chain);
   std::cout << t->GetEntries() << std::endl;

   hInvBetaVsP = new TH2D("hInvBetaVsP", "hInvBetaVsP", 200, 0, 10, 200, 0.9, 1.4);
   hInvBetaVsP1Sigma = new TH2D("hInvBetaVsP1Sigma", "hInvBetaVsP1Sigma", 200, 0, 10, 200, 0.9, 1.4);

   TRandom3 r3;

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      if(!ana::passKinematicCuts(t)) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);

      const float dInvBetaCut1 = std::fabs(t->EtaD1<1.5) ? ana::fExpBTL.Eval(pD1) : ana::fExpETL.Eval(pD1);
      const float dInvBetaCut2 = std::fabs(t->EtaD2<1.5) ? ana::fExpBTL.Eval(pD2) : ana::fExpETL.Eval(pD2);

      if(!ana::passTopoCuts(t)) continue;

      if(t->isSwap || !t->matchGEN) continue;

      bool is1sigmaPionDau1 = t->pT > 1.5 ? true : false;
      bool is1sigmaPionDau2 = t->pT > 1.5 ? true : false;
      bool is1sigmaKaonDau1 = t->pT > 1.5 ? true : false;
      bool is1sigmaKaonDau2 = t->pT > 1.5 ? true : false;

      // dau 1
      if ( isETL( t->EtaD1 ) && t->flavor == 1) {
        if (whereLost == innerLost) {
          if ( t->EtaD1 > etaTurnPoint ) {
            // efficiency loss, 15%
            if (r3.Uniform(0., 1.)<0.15) {
              if(t->isMtdDau1) {
                hInvBetaVsP->Fill(pD1, 1./t->beta1_PV);
                is1sigmaPionDau1 = std::fabs(1./t->beta1_PV - invBetaPion(pD1) - ana::meanPion(t, 1) ) < 1.0 * dInvBetaCut1;
                if(is1sigmaPionDau1) hInvBetaVsP1Sigma->Fill(pD1, 1./t->beta1_PV);
              }
            } 
          }
        } else if (whereLost == outerLost){
          if ( t->EtaD1 < etaTurnPoint ) {
            // efficiency loss, 15%
            if (r3.Uniform(0., 1.)<0.15) {
              if(t->isMtdDau1) {
                hInvBetaVsP->Fill(pD1, 1./t->beta1_PV);
                is1sigmaPionDau1 = std::fabs(1./t->beta1_PV - invBetaPion(pD1) - ana::meanPion(t, 1) ) < 1.0 * dInvBetaCut1;
                if(is1sigmaPionDau1) hInvBetaVsP1Sigma->Fill(pD1, 1./t->beta1_PV);
              }
            }
          }
        }
      }

      /*
      // dau 2
      if ( isETL( t->EtaD2 ) ) {
        if (whereLost == innerLost) {
          if ( t->EtaD2 > etaTurnPoint ) {
            // efficiency loss, 15%
            if (r3.Uniform(0., 1.)<0.15) {
              if(t->isMtdDau2) {
                hInvBetaVsP->Fill(pD2, 1./t->beta2_PV);
              }
            } 
          }
        } else if (whereLost == outerLost){
          if ( t->EtaD2 < etaTurnPoint ) {
            // efficiency loss, 15%
            if (r3.Uniform(0., 1.)<0.15) {
              if(t->isMtdDau2) {
                hInvBetaVsP->Fill(pD2, 1./t->beta2_PV);
              }
            }
          }
        }
      }
      */
   }

   TFile fout(Form("PromptDHists_reRECO_OneAndHalf_%d_invbeta.root", whereLost), "recreate");
   hInvBetaVsP->Write();
   hInvBetaVsP1Sigma->Write();
}
