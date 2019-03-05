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

int whichY(const float& y)
{
      for(int i=0; i<ana::nuOfY; i++){
               if(y>ana::ybin[i] && y<ana::ybin[i+1]) return i;
                  }
         return -1;
}
inline float invBetaPion(const float& p){
      return std::sqrt(1 + std::pow(ana::massPion/p,2));
}

inline float invBetaKaon(const float& p){
      return std::sqrt(1 + std::pow(ana::massKaon/p,2));
}

void hyjetsMass()
{
   TF1 *fExpPion, *fExpKaon; 

   fExpPion = new TF1("fExpPion_dInvBetaRMS","0.005 + 0.017*exp(-x/2.8)", 0.8, 10);
   fExpKaon = new TF1("fExpKaon_dInvBetaRMS","0.005 + 0.017*exp(-x/2.8)", 0.8, 10);

   TChain* chain = new TChain("d0ana/VertexCompositeNtuple");
   //chain->Add("/eos/cms/store/group/phys_heavyions/MTD/anstahll/VertexCompositeAnalysis/HydJets_mc_mtd_NTUPLE_20190227/Hydjet_5p02TeV_TuneCP5_MTD/HydJets_mc_mtd_NTUPLE_20190227/190227_165027/0001/hyjets_mc_mtd_1165.root");
   TFileCollection* fc = new TFileCollection("dum", "", "hyjets_sample.list");
   chain->AddFileInfoList(fc->GetList()); 
   HyJets* t = new HyJets(chain);
   std::cout << t->GetEntries() << std::endl;

   TH2F* hDau1PvsY = new TH2F("hDau1PvsY", "hDau1PvsY", 100, -3, 3, 1000, 0, 10);
   TH2F* hDau2PvsY = new TH2F("hDau2PvsY", "hDau2PvsY", 100, -3, 3, 1000, 0, 10);

   TH1F* hMass[ana::nuOfY];
   TH1F* hMassMtd[ana::nuOfY];
   TH1F* hMassCent[ana::nuOfY];
   TH1F* hMassCentMtd[ana::nuOfY];

   for(int iy=0; iy<ana::nuOfY; iy++){
      hMass[iy] = new TH1F(Form("hMass%d", iy), Form("hMass%d", iy), 60, 1.7, 2.0);
      hMassMtd[iy] = new TH1F(Form("hMassMtd%d", iy), Form("hMassMtd%d", iy), 60, 1.7, 2.0);
      hMass[iy]->GetXaxis()->SetTitle("mass (GeV)");
      hMassMtd[iy]->GetXaxis()->SetTitle("mass (GeV)");
      hMass[iy]->SetLineColor(kBlue);
      hMassMtd[iy]->SetLineColor(kRed);

      hMassCent[iy] = new TH1F(Form("hMassCent%d", iy), Form("hMassCent%d", iy), 60, 1.7, 2.0);
      hMassCentMtd[iy] = new TH1F(Form("hMassCentMtd%d", iy), Form("hMassCentMtd%d", iy), 60, 1.7, 2.0);
      hMassCent[iy]->GetXaxis()->SetTitle("mass (GeV)");
      hMassCentMtd[iy]->GetXaxis()->SetTitle("mass (GeV)");
      hMassCent[iy]->SetLineColor(kBlue);
      hMassCentMtd[iy]->SetLineColor(kRed);
   }

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      const int iy = whichY(t->y);
      if( iy == -1 ) continue;

      // require eta<1.4 ? pT > 0.8 : pT > 0.5 and within MTD acceptance
      if(std::fabs(t->EtaD1) > 3) continue;
      if(std::fabs(t->EtaD2) > 3) continue;
      if(std::fabs(t->EtaD1) < 1.4 ? t->pTD1 <= 0.8 : t->pTD1 <= 0.5) continue;
      if(std::fabs(t->EtaD2) < 1.4 ? t->pTD2 <= 0.8 : t->pTD2 <= 0.5) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);
      const bool isCentral = t->centrality < 20;

      hMass[iy]->Fill(t->mass);
      if(isCentral) hMassCent[iy]->Fill(t->mass);

      bool is1sigmaPionDau1 = true;
      bool is1sigmaKaonDau1 = true;
      bool is1sigmaPionDau2 = true;
      bool is1sigmaKaonDau2 = true;

      if(t->beta1_PV!=99) hDau1PvsY->Fill(t->y, pD1);
      if(t->beta2_PV!=99) hDau2PvsY->Fill(t->y, pD2);

      if(t->beta1_PV!=-99) is1sigmaPionDau1 = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 1.0 * fExpPion->Eval(pD1);
      if(t->beta1_PV!=-99) is1sigmaKaonDau1 = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 1.0 * fExpKaon->Eval(pD1);
      if(t->beta2_PV!=-99) is1sigmaPionDau2 = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 1.0 * fExpPion->Eval(pD2);
      if(t->beta2_PV!=-99) is1sigmaKaonDau2 = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 1.0 * fExpKaon->Eval(pD2);
      if((t->flavor == 1 && is1sigmaPionDau1 && is1sigmaKaonDau2) || (t->flavor == -1 && is1sigmaKaonDau1 && is1sigmaPionDau2)){
         hMassMtd[iy]->Fill(t->mass);
         if(isCentral) hMassCentMtd[iy]->Fill(t->mass);
      }
   }

   TH1F* hFrac = new TH1F("hfrac", "hfrac", ana::nuOfY, ana::ybin);
   hFrac->GetXaxis()->SetTitle("y");
   hFrac->GetYaxis()->SetTitle("yield ratio");

   TH1F* hFracCent = new TH1F("hfracCent", "hfracCent", ana::nuOfY, ana::ybin);
   hFracCent->GetXaxis()->SetTitle("y");
   hFracCent->GetYaxis()->SetTitle("yield ratio");

   for(int iy = 0; iy<ana::nuOfY; iy++){
      float frac = hMassMtd[iy]->Integral() / hMass[iy]->Integral();
      hFrac->SetBinContent(iy+1, frac);
      float fracCent = hMassCentMtd[iy]->Integral() / hMassCent[iy]->Integral();
      hFracCent->SetBinContent(iy+1, fracCent);
   }

   TFile fout("hyjetsMassHists.root", "recreate");
   for(int iy=0; iy<ana::nuOfY; iy++){
      hMass[iy]->Write();
      hMassCent[iy]->Write();
      hMassMtd[iy]->Write();
      hMassCentMtd[iy]->Write();
   }
   hFrac->Write();
   hFracCent->Write();
   hDau1PvsY->Write();
   hDau2PvsY->Write();
}
