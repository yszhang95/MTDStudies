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
TH2* hInvBetaVsPCent;
TH2* hInvBetaVsP30_50;

TH2* hInvBetaVsP1Sigma;
TH2* hInvBetaVsP1SigmaCent;
TH2* hInvBetaVsP1Sigma30_50;

void fillHyJetsDauInvBeta_Ineff(std::string name="", WhereLost whereLost=innerLost)
{
   TChain* chain = new TChain("d0ana_mc/VertexCompositeNtuple");
   TFileCollection* fc = new TFileCollection("dum", "", name.c_str());
   chain->AddFileInfoList(fc->GetList()); 
   HyJets* t = new HyJets(chain);
   std::cout << t->GetEntries() << std::endl;

   hInvBetaVsP = new TH2D("hInvBetaVsP", "hInvBetaVsP", 200, 0, 10, 200, 0.9, 1.4);
   hInvBetaVsPCent = new TH2D("hInvBetaVsPCent", "hInvBetaVsPCent", 200, 0, 10, 200, 0.9, 1.4);
   hInvBetaVsP30_50 = new TH2D("hInvBetaVsP30_50", "hInvBetaVsP30_50", 200, 0, 10, 200, 0.9, 1.4);

   hInvBetaVsP1Sigma = new TH2D("hInvBetaVsP1Sigma", "hInvBetaVsP1Sigma", 200, 0, 10, 200, 0.9, 1.4);
   hInvBetaVsP1SigmaCent = new TH2D("hInvBetaVsP1SigmaCent", "hInvBetaVsP1SigmaCent", 200, 0, 10, 200, 0.9, 1.4);
   hInvBetaVsP1Sigma30_50 = new TH2D("hInvBetaVsP1Sigma30_50", "hInvBetaVsP1Sigma30_50", 200, 0, 10, 200, 0.9, 1.4);

   TRandom3 r3;

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      if(!ana::passKinematicCuts(t)) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);

      bool isCentral = ana::isCentralEvt(*t);

      const float dInvBetaCut1 = std::fabs(t->EtaD1<1.5) ? ana::fExpBTL.Eval(pD1) : ana::fExpETL.Eval(pD1);
      const float dInvBetaCut2 = std::fabs(t->EtaD2<1.5) ? ana::fExpBTL.Eval(pD2) : ana::fExpETL.Eval(pD2);

      if(!ana::passTopoCuts(t)) continue;

      bool is1sigmaPionDau1 = t->pT > 1.5 ? true : false;
      bool is1sigmaPionDau2 = t->pT > 1.5 ? true : false;
      bool is1sigmaKaonDau1 = t->pT > 1.5 ? true : false;
      bool is1sigmaKaonDau2 = t->pT > 1.5 ? true : false;

      // dau 1
      if ( isETL( t->EtaD1 ) ) {
        if (whereLost == innerLost) {
          if ( t->EtaD1 > etaTurnPoint ) {
            // efficiency loss, 15%
            if (r3.Uniform(0., 1.)<0.15) {
              if(t->isMtdDau1) {
                hInvBetaVsP->Fill(pD1, 1./t->beta1_PV);
                if(t->centrality <100 && t->centrality > 60) hInvBetaVsP30_50->Fill(pD1, 1./t->beta1_PV);
                if(isCentral) hInvBetaVsPCent->Fill(pD1, 1./t->beta1_PV);

                is1sigmaPionDau1 = std::fabs(1./t->beta1_PV - invBetaPion(pD1) - ana::meanPion(t, 1) ) < 1.0 * dInvBetaCut1;
                if(is1sigmaPionDau1) hInvBetaVsP1Sigma->Fill(pD1, 1./t->beta1_PV);
                if(is1sigmaPionDau1 && t->centrality <100 && t->centrality > 60) hInvBetaVsP1Sigma30_50->Fill(pD1, 1./t->beta1_PV);
                if(is1sigmaPionDau1 && isCentral) hInvBetaVsP1SigmaCent->Fill(pD1, 1./t->beta1_PV);
              }
            } 
          }
        } else if (whereLost == outerLost){
          if ( t->EtaD1 < etaTurnPoint ) {
            // efficiency loss, 15%
            if (r3.Uniform(0., 1.)<0.15) {
              if(t->isMtdDau1) {
                if(t->centrality <100 && t->centrality > 60) hInvBetaVsP30_50->Fill(pD1, 1./t->beta1_PV);
                if(isCentral) hInvBetaVsPCent->Fill(pD1, 1./t->beta1_PV);
                hInvBetaVsP->Fill(pD1, 1./t->beta1_PV);
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
                if(t->centrality <100 && t->centrality > 60) hInvBetaVsP30_50->Fill(pD2, 1./t->beta2_PV);
                if(isCentral) hInvBetaVsPCent->Fill(pD2, 1./t->beta2_PV);
                hInvBetaVsP->Fill(pD2, 1./t->beta2_PV);
              }
            } 
          }
        } else if (whereLost == outerLost){
          if ( t->EtaD2 < etaTurnPoint ) {
            // efficiency loss, 15%
            if (r3.Uniform(0., 1.)<0.15) {
              if(t->isMtdDau2) {
                if(t->centrality <100 && t->centrality > 60) hInvBetaVsP30_50->Fill(pD2, 1./t->beta2_PV);
                if(isCentral) hInvBetaVsPCent->Fill(pD2, 1./t->beta2_PV);
                hInvBetaVsP->Fill(pD2, 1./t->beta2_PV);
              }
            }
          }
        }
      }
      */
   }

   TFile fout(Form("HyJetHists_reRECO_%s_OneAndHalf_%d_invbeta.root", name.c_str(), whereLost), "recreate");
   hInvBetaVsP->Write();
   hInvBetaVsPCent->Write();
   hInvBetaVsP30_50->Write();
   hInvBetaVsP1Sigma->Write();
   hInvBetaVsP1SigmaCent->Write();
   hInvBetaVsP1Sigma30_50->Write();
}
