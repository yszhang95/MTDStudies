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
#include "TNtuple.h"
#include "myAna.h"
#include "TLegend.h"
#include "TRandom3.h"

int whichY(const float& y)
{
   for(int i=0; i<ana::nuOfY; i++){
      if(y>ana::ybin[i] && y<ana::ybin[i+1]) return i;
   }
   return -1;
}

void fillHyJetsDauInvBetaVsP()
{
   const double c_cm_ns = 2.99792458e1; //[cm/ns] // mtd

   TFile fout("smeared.root", "recreate");

   TChain* chain = new TChain("d0ana_mc/VertexCompositeNtuple");
   TFileCollection* fc = new TFileCollection("dum", "", "newHyJets.list");
   chain->AddFileInfoList(fc->GetList()); 
   HyJets* t = new HyJets(chain);
   std::cout << t->GetEntries() << std::endl;

   TH2D hDau1InvBetaVsP("hDau1InvBetaVsP", "", 1000, 0, 5, 1000, 0.9, 1.7);
   TH2D hDau1InvBetaVsP_Smeared("hDau1InvBetaVsP_Smeared", "", 1000, 0, 5, 1000, 0.9, 1.7);

   // here is related to full pT
   TRandom3 r3;
   r3.SetSeed(0);
//   double width = 1./(std::sqrt(std::pow(1./0.035, 2) - std::pow(1./0.05, 2)));
   double width = 0.035;
   TF1 fRatioETL = TF1("fRatioETL", "300.*(0.0025 + 0.0045*exp(-x/7.6)+0.0025*exp(-fabs(x-4)/3.))");

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
   //for(Long64_t ientry=0; ientry<20; ientry++){
      t->GetEntry(ientry);

      const int iy = whichY(t->y);
      if( iy == -1 ) continue;

      if(std::fabs(t->EtaD1)<1.4 ? t->pTD1 <= 0.8 : t->pTD1 * std::cosh(t->EtaD1) <= 0.7) continue;
      if(std::fabs(t->EtaD2)<1.4 ? t->pTD2 <= 0.8 : t->pTD2 * std::cosh(t->EtaD2) <= 0.7) continue;

      const float pD1 = t->pTD1 * std::cosh( t->EtaD1 );
      const float pD2 = t->pTD2 * std::cosh( t->EtaD2 );

      if(std::fabs(t->EtaD1)<1.6) continue;

      if(!t->isMtdDau1) continue;
      double tmtd1 = r3.Gaus(t->tmtd1, width*fRatioETL.Eval(pD1));
      const float beta1_PV = (t->pathLength1/(tmtd1-t->t0_PV))*(1./c_cm_ns);

      hDau1InvBetaVsP.Fill(pD1, 1./t->beta1_PV);
      hDau1InvBetaVsP_Smeared.Fill(pD1, 1./beta1_PV);
   }

   // full range of pT, with |y|<3 
   fout.cd();

   hDau1InvBetaVsP.Write();
   hDau1InvBetaVsP_Smeared.Write();
}
