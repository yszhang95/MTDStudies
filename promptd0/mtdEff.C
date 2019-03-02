#include "myAna.h"
#include "matchD.h"

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

void mtdEff()
{
   TFile* fFunc = new TFile("fFuncDInvBeta.root");
   TF1 *fExpPion, *fExpKaon, *fPoly2Pion, *fPoly2Kaon;
   fFunc->GetObject("fExpPion_dInvBetaRMS", fExpPion);
   fFunc->GetObject("fExpKaon_dInvBetaRMS", fExpKaon);
   fFunc->GetObject("fPoly2Pion_dInvBetaRMS", fPoly2Pion);
   fFunc->GetObject("fPoly2Kaon_dInvBetaRMS", fPoly2Kaon);

   TFile* f1 = new TFile("matchPromptD0_fullSample.root");
   TNtuple* tp = (TNtuple*) f1->Get("PromptD");
   matchD* t = new matchD(tp);
   std::cout << t->GetEntries() << std::endl;

   bool daughterEff = true;

   TH1F* hPionEff = new TH1F("hPionEff", "hPionEff", 12, -3, 3);
   TH1F* hPion3sigmaEff = new TH1F("hPion3sigmaEff", "hPion3sigmaEff", ana::nuOfY, -3, 3);
   TH1F* hPion2sigmaEff = new TH1F("hPion2sigmaEff", "hPion2sigmaEff", ana::nuOfY, -3, 3);
   TH1F* hPion1sigmaEff = new TH1F("hPion1sigmaEff", "hPion1sigmaEff", ana::nuOfY, -3, 3);

   TH1F* hKaonEff = new TH1F("hKaonEff", "hKaonEff", ana::nuOfY, -3, 3);
   TH1F* hKaon3sigmaEff = new TH1F("hKaon3sigmaEff", "hKaon3sigmaEff", ana::nuOfY, -3, 3);
   TH1F* hKaon2sigmaEff = new TH1F("hKaon2sigmaEff", "hKaon2sigmaEff", ana::nuOfY, -3, 3);
   TH1F* hKaon1sigmaEff = new TH1F("hKaon1sigmaEff", "hKaon1sigmaEff", ana::nuOfY, -3, 3);

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      const int iy = whichY(t->y);
      if( iy == -1 ) continue;

      if(t->pT > 0.5) continue;

      // require eta<1.4 ? pT > 0.8 : pT > 0.5
      if(t->pTD1<0.8 && t->EtaD1<1.4) continue;
      if(t->pTD2<0.8 && t->EtaD2<1.4) continue;
      if(t->pTD1<0.5 && t->EtaD1>1.4) continue;
      if(t->pTD2<0.5 && t->EtaD2>1.4) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);

      if(daughterEff){
         bool is3sigmaPion = false;
         bool is3sigmaKaon = false;

         bool is2sigmaPion = false;
         bool is2sigmaKaon = false;

         bool is1sigmaPion = false;
         bool is1sigmaKaon = false;

         if(t->flavor == 1){
            if(t->isMtdDau1) hPionEff->Fill(t->y);
            if(t->isMtdDau1) is3sigmaPion = (1./t->beta1_PV - invBetaPion(pD1)) < 3.0 * fExpPion->Eval(pD1);
            if(t->isMtdDau1) is2sigmaPion = (1./t->beta1_PV - invBetaPion(pD1)) < 2.0 * fExpPion->Eval(pD1);
            if(t->isMtdDau1) is1sigmaPion = (1./t->beta1_PV - invBetaPion(pD1)) < 1.0 * fExpPion->Eval(pD1);
            if(is3sigmaPion) hPion3sigmaEff->Fill(t->y);
            if(is2sigmaPion) hPion2sigmaEff->Fill(t->y);
            if(is1sigmaPion) hPion1sigmaEff->Fill(t->y);

            if(t->isMtdDau2) hKaonEff->Fill(t->y);
            if(t->isMtdDau2) is3sigmaKaon = (1./t->beta2_PV - invBetaKaon(pD2)) < 3.0 *fExpKaon->Eval(pD2);
            if(t->isMtdDau2) is2sigmaKaon = (1./t->beta2_PV - invBetaKaon(pD2)) < 2.0 *fExpKaon->Eval(pD2);
            if(t->isMtdDau2) is1sigmaKaon = (1./t->beta2_PV - invBetaKaon(pD2)) < 1.0 *fExpKaon->Eval(pD2);
            if(is3sigmaKaon) hKaon3sigmaEff->Fill(t->y);
            if(is2sigmaKaon) hKaon2sigmaEff->Fill(t->y);
            if(is1sigmaKaon) hKaon1sigmaEff->Fill(t->y);
         }

         if(t->flavor == -1){
            if(t->isMtdDau1) hKaonEff->Fill(t->y);
            if(t->isMtdDau1) is3sigmaKaon = (1./t->beta1_PV - invBetaKaon(pD1)) < 3.0 * fExpKaon->Eval(pD1);
            if(t->isMtdDau1) is2sigmaKaon = (1./t->beta1_PV - invBetaKaon(pD1)) < 2.0 * fExpKaon->Eval(pD1);
            if(t->isMtdDau1) is1sigmaKaon = (1./t->beta1_PV - invBetaKaon(pD1)) < 1.0 * fExpKaon->Eval(pD1);
            if(is3sigmaKaon) hKaon3sigmaEff->Fill(t->y);
            if(is2sigmaKaon) hKaon2sigmaEff->Fill(t->y);
            if(is1sigmaKaon) hKaon1sigmaEff->Fill(t->y);

            if(t->isMtdDau2) hPionEff->Fill(t->y);
            if(t->isMtdDau2) is3sigmaPion = (1./t->beta2_PV - invBetaPion(pD2)) < 3.0 * fExpPion->Eval(pD2);
            if(t->isMtdDau2) is2sigmaPion = (1./t->beta2_PV - invBetaPion(pD2)) < 2.0 * fExpPion->Eval(pD2);
            if(t->isMtdDau2) is1sigmaPion = (1./t->beta2_PV - invBetaPion(pD2)) < 1.0 * fExpPion->Eval(pD2);
            if(is3sigmaPion) hPion3sigmaEff->Fill(t->y);
            if(is2sigmaPion) hPion2sigmaEff->Fill(t->y);
            if(is1sigmaPion) hPion1sigmaEff->Fill(t->y);
         }
      }
   }
   if(daughterEff){
      TCanvas* c1 = new TCanvas("c1", "PionEff", 500, 550);
      gStyle->SetOptStat(0);
      TH1F* hDrawPion = new TH1F("hDrawPion", "", ana::nuOfY, -3, 3);
      hDrawPion->GetYaxis()->SetTitle("Pion Yield Ratio");
      hDrawPion->GetYaxis()->SetRangeUser(0, 1.3);
      hDrawPion->GetXaxis()->SetTitle("D0 rapidity");
      hDrawPion->Draw();
      hPion3sigmaEff->Divide(hPionEff);
      hPion2sigmaEff->Divide(hPionEff);
      hPion1sigmaEff->Divide(hPionEff);
      hPion3sigmaEff->SetLineColor(kGreen-6);
      hPion2sigmaEff->SetLineColor(kBlue);
      hPion1sigmaEff->SetLineColor(kRed);
      hPion3sigmaEff->Draw("same");
      hPion2sigmaEff->Draw("same");
      hPion1sigmaEff->Draw("same");
      TLegend *lPion = new TLegend(0.7, 0.8, 0.9, 0.9);
      lPion->AddEntry(hPion3sigmaEff, "3 RMS", "l");
      lPion->AddEntry(hPion2sigmaEff, "2 RMS", "l");
      lPion->AddEntry(hPion1sigmaEff, "1 RMS", "l");
      lPion->Draw();

      TCanvas* c2 = new TCanvas("c2", "KaonEff", 500, 550);
      gStyle->SetOptStat(0);
      TH1F* hDrawKaon = new TH1F("hDrawKaon", "", ana::nuOfY, -3, 3);
      hDrawKaon->GetYaxis()->SetTitle("Kaon Yield Ratio");
      hDrawKaon->GetYaxis()->SetRangeUser(0, 1.3);
      hDrawKaon->GetXaxis()->SetTitle("D0 rapidity");
      hDrawKaon->Draw();
      hKaon3sigmaEff->Divide(hKaonEff);
      hKaon2sigmaEff->Divide(hKaonEff);
      hKaon1sigmaEff->Divide(hKaonEff);
      hKaon3sigmaEff->SetLineColor(kGreen-6);
      hKaon2sigmaEff->SetLineColor(kBlue);
      hKaon1sigmaEff->SetLineColor(kRed);
      hKaon3sigmaEff->Draw("same");
      hKaon2sigmaEff->Draw("same");
      hKaon1sigmaEff->Draw("same");
      TLegend *lKaon = new TLegend(0.7, 0.8, 0.9, 0.9);
      lKaon->AddEntry(hKaon3sigmaEff, "3 RMS", "l");
      lKaon->AddEntry(hKaon2sigmaEff, "2 RMS", "l");
      lKaon->AddEntry(hKaon1sigmaEff, "1 RMS", "l");
      lKaon->Draw();
   }
}
