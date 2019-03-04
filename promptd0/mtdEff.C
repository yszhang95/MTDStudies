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
   //TFile* fFunc = new TFile("fFuncDInvBeta.root");
   TF1 *fExpPion, *fExpKaon, *fPoly2Pion, *fPoly2Kaon;
   //fFunc->GetObject("fExpPion_dInvBetaRMS", fExpPion);
   //fFunc->GetObject("fExpKaon_dInvBetaRMS", fExpKaon);
   //fFunc->GetObject("fPoly2Pion_dInvBetaRMS", fPoly2Pion);
   //fFunc->GetObject("fPoly2Kaon_dInvBetaRMS", fPoly2Kaon);

   fExpPion = new TF1("fExpPion_dInvBetaRMS","0.005 + 0.017*exp(-x/2.8)", 0.8, 10);
   fExpKaon = new TF1("fExpKaon_dInvBetaRMS","0.005 + 0.017*exp(-x/2.8)", 0.8, 10);

   TFile* f1 = new TFile("matchPromptD0_fullSample.root");
   TNtuple* tp = (TNtuple*) f1->Get("PromptD");
   matchD* t = new matchD(tp);
   std::cout << t->GetEntries() << std::endl;

   const bool daughterEff_Y_zeroPt = false;
   const bool daughterEff_P = false;
   const bool isD0MtdEff = true;

   TH1F* hPionEff;
   TH1F* hPion3sigmaEff;
   TH1F* hPion2sigmaEff;
   TH1F* hPion1sigmaEff;

   TH1F* hKaonEff;
   TH1F* hKaon3sigmaEff;
   TH1F* hKaon2sigmaEff;
   TH1F* hKaon1sigmaEff;

   TH1F* hD0Pt;
   TH1F* hD0Pt3sigma;
   TH1F* hD0Pt2sigma;
   TH1F* hD0Pt1sigma;

   if(daughterEff_Y_zeroPt){
      hPionEff = new TH1F("hPionEff", "hPionEff", ana::nuOfY, -3, 3);
      hPion3sigmaEff = new TH1F("hPion3sigmaEff", "hPion3sigmaEff", ana::nuOfY, -3, 3);
      hPion2sigmaEff = new TH1F("hPion2sigmaEff", "hPion2sigmaEff", ana::nuOfY, -3, 3);
      hPion1sigmaEff = new TH1F("hPion1sigmaEff", "hPion1sigmaEff", ana::nuOfY, -3, 3);

      hKaonEff = new TH1F("hKaonEff", "hKaonEff", ana::nuOfY, -3, 3);
      hKaon3sigmaEff = new TH1F("hKaon3sigmaEff", "hKaon3sigmaEff", ana::nuOfY, -3, 3);
      hKaon2sigmaEff = new TH1F("hKaon2sigmaEff", "hKaon2sigmaEff", ana::nuOfY, -3, 3);
      hKaon1sigmaEff = new TH1F("hKaon1sigmaEff", "hKaon1sigmaEff", ana::nuOfY, -3, 3);
   }

   if(daughterEff_P){
      hPionEff = new TH1F("hPionEff", "hPionEff", 100, 0, 10);
      hPion3sigmaEff = new TH1F("hPion3sigmaEff", "hPion3sigmaEff", 100, 0, 10);
      hPion2sigmaEff = new TH1F("hPion2sigmaEff", "hPion2sigmaEff", 100, 0, 10);
      hPion1sigmaEff = new TH1F("hPion1sigmaEff", "hPion1sigmaEff", 100, 0, 10);

      hKaonEff = new TH1F("hKaonEff", "hKaonEff", 100, 0, 10);
      hKaon3sigmaEff = new TH1F("hKaon3sigmaEff", "hKaon3sigmaEff", 100, 0, 10);
      hKaon2sigmaEff = new TH1F("hKaon2sigmaEff", "hKaon2sigmaEff", 100, 0, 10);
      hKaon1sigmaEff = new TH1F("hKaon1sigmaEff", "hKaon1sigmaEff", 100, 0, 10);
   }

   if(isD0MtdEff){
      hD0Pt = new TH1F("hD0Pt", "hD0Pt", 100, 0, 10);
      hD0Pt3sigma = new TH1F("hD0Pt3sigma", "hD0Pt3sigma", 100, 0, 10);
      hD0Pt2sigma = new TH1F("hD0Pt2sigma", "hD0Pt2sigma", 100, 0, 10);
      hD0Pt1sigma = new TH1F("hD0Pt1sigma", "hD0Pt1sigma", 100, 0, 10);
   }

   Long64_t nBothMTD = 0;
   Long64_t nOneMTD = 0;
   Long64_t nNoMTD = 0;
   
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

      if(daughterEff_Y_zeroPt){
         if(t->pT > 0.5) continue;
         if(t->flavor == 1){
            bool is3sigmaPion = false;
            bool is3sigmaKaon = false;

            bool is2sigmaPion = false;
            bool is2sigmaKaon = false;

            bool is1sigmaPion = false;
            bool is1sigmaKaon = false;

            if(t->isMtdDau1) hPionEff->Fill(t->y);
            if(t->isMtdDau1) is3sigmaPion = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 3.0 * fExpPion->Eval(pD1);
            if(t->isMtdDau1) is2sigmaPion = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 2.0 * fExpPion->Eval(pD1);
            if(t->isMtdDau1) is1sigmaPion = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 1.0 * fExpPion->Eval(pD1);
            if(is3sigmaPion && t->isMtdDau1) hPion3sigmaEff->Fill(t->y);
            if(is2sigmaPion && t->isMtdDau1) hPion2sigmaEff->Fill(t->y);
            if(is1sigmaPion && t->isMtdDau1) hPion1sigmaEff->Fill(t->y);

            if(t->isMtdDau2) hKaonEff->Fill(t->y);
            if(t->isMtdDau2) is3sigmaKaon = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 3.0 *fExpKaon->Eval(pD2);
            if(t->isMtdDau2) is2sigmaKaon = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 2.0 *fExpKaon->Eval(pD2);
            if(t->isMtdDau2) is1sigmaKaon = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 1.0 *fExpKaon->Eval(pD2);
            if(is3sigmaKaon && t->isMtdDau2) hKaon3sigmaEff->Fill(t->y);
            if(is2sigmaKaon && t->isMtdDau2) hKaon2sigmaEff->Fill(t->y);
            if(is1sigmaKaon && t->isMtdDau2) hKaon1sigmaEff->Fill(t->y);
         }

         if(t->flavor == -1){
            bool is3sigmaPion = false;
            bool is3sigmaKaon = false;

            bool is2sigmaPion = false;
            bool is2sigmaKaon = false;

            bool is1sigmaPion = false;
            bool is1sigmaKaon = false;

            if(t->isMtdDau1) hKaonEff->Fill(t->y);
            if(t->isMtdDau1) is3sigmaKaon = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 3.0 * fExpKaon->Eval(pD1);
            if(t->isMtdDau1) is2sigmaKaon = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 2.0 * fExpKaon->Eval(pD1);
            if(t->isMtdDau1) is1sigmaKaon = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 1.0 * fExpKaon->Eval(pD1);
            if(is3sigmaKaon && t->isMtdDau1) hKaon3sigmaEff->Fill(t->y);
            if(is2sigmaKaon && t->isMtdDau1) hKaon2sigmaEff->Fill(t->y);
            if(is1sigmaKaon && t->isMtdDau1) hKaon1sigmaEff->Fill(t->y);

            if(t->isMtdDau2) hPionEff->Fill(t->y);
            if(t->isMtdDau2) is3sigmaPion = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 3.0 * fExpPion->Eval(pD2);
            if(t->isMtdDau2) is2sigmaPion = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 2.0 * fExpPion->Eval(pD2);
            if(t->isMtdDau2) is1sigmaPion = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 1.0 * fExpPion->Eval(pD2);
            if(is3sigmaPion && t->isMtdDau2) hPion3sigmaEff->Fill(t->y);
            if(is2sigmaPion && t->isMtdDau2) hPion2sigmaEff->Fill(t->y);
            if(is1sigmaPion && t->isMtdDau2) hPion1sigmaEff->Fill(t->y);
         }
      }
      if(daughterEff_P){
         if(t->flavor == 1){
            bool is3sigmaPion = false;
            bool is3sigmaKaon = false;

            bool is2sigmaPion = false;
            bool is2sigmaKaon = false;

            bool is1sigmaPion = false;
            bool is1sigmaKaon = false;

            if(t->isMtdDau1) hPionEff->Fill(pD1);
            if(t->isMtdDau1) is3sigmaPion = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 3.0 * fExpPion->Eval(pD1);
            if(t->isMtdDau1) is2sigmaPion = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 2.0 * fExpPion->Eval(pD1);
            if(t->isMtdDau1) is1sigmaPion = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 1.0 * fExpPion->Eval(pD1);
            if(is3sigmaPion && t->isMtdDau1) hPion3sigmaEff->Fill(pD1);
            if(is2sigmaPion && t->isMtdDau1) hPion2sigmaEff->Fill(pD1);
            if(is1sigmaPion && t->isMtdDau1) hPion1sigmaEff->Fill(pD1);

            if(t->isMtdDau2) hKaonEff->Fill(pD2);
            if(t->isMtdDau2) is3sigmaKaon = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 3.0 *fExpKaon->Eval(pD2);
            if(t->isMtdDau2) is2sigmaKaon = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 2.0 *fExpKaon->Eval(pD2);
            if(t->isMtdDau2) is1sigmaKaon = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 1.0 *fExpKaon->Eval(pD2);
            if(is3sigmaKaon && t->isMtdDau2) hKaon3sigmaEff->Fill(pD2);
            if(is2sigmaKaon && t->isMtdDau2) hKaon2sigmaEff->Fill(pD2);
            if(is1sigmaKaon && t->isMtdDau2) hKaon1sigmaEff->Fill(pD2);
         }

         if(t->flavor == -1){
            bool is3sigmaPion = false;
            bool is3sigmaKaon = false;

            bool is2sigmaPion = false;
            bool is2sigmaKaon = false;

            bool is1sigmaPion = false;
            bool is1sigmaKaon = false;

            if(t->isMtdDau1) hKaonEff->Fill(pD1);
            if(t->isMtdDau1) is3sigmaKaon = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 3.0 * fExpKaon->Eval(pD1);
            if(t->isMtdDau1) is2sigmaKaon = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 2.0 * fExpKaon->Eval(pD1);
            if(t->isMtdDau1) is1sigmaKaon = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 1.0 * fExpKaon->Eval(pD1);
            if(is3sigmaKaon && t->isMtdDau1) hKaon3sigmaEff->Fill(pD1);
            if(is2sigmaKaon && t->isMtdDau1) hKaon2sigmaEff->Fill(pD1);
            if(is1sigmaKaon && t->isMtdDau1) hKaon1sigmaEff->Fill(pD1);

            if(t->isMtdDau2) hPionEff->Fill(pD2);
            if(t->isMtdDau2) is3sigmaPion = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 3.0 * fExpPion->Eval(pD2);
            if(t->isMtdDau2) is2sigmaPion = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 2.0 * fExpPion->Eval(pD2);
            if(t->isMtdDau2) is1sigmaPion = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 1.0 * fExpPion->Eval(pD2);
            if(is3sigmaPion && t->isMtdDau2) hPion3sigmaEff->Fill(pD2);
            if(is2sigmaPion && t->isMtdDau2) hPion2sigmaEff->Fill(pD2);
            if(is1sigmaPion && t->isMtdDau2) hPion1sigmaEff->Fill(pD2);
         }
      }

      if(isD0MtdEff && t->matchGEN && !t->isSwap){
         hD0Pt->Fill(t->pT);

         bool is3sigmaPionDau1 = true;
         bool is3sigmaKaonDau1 = true;
         bool is3sigmaPionDau2 = true;
         bool is3sigmaKaonDau2 = true;
         
         if(t->isMtdDau1) is3sigmaPionDau1 = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 3.0 * fExpPion->Eval(pD1);
         if(t->isMtdDau1) is3sigmaKaonDau1 = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 3.0 * fExpKaon->Eval(pD1);
         if(t->isMtdDau2) is3sigmaPionDau2 = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 3.0 * fExpPion->Eval(pD2);
         if(t->isMtdDau2) is3sigmaKaonDau2 = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 3.0 * fExpKaon->Eval(pD2);

         if((t->flavor == 1 && is3sigmaPionDau1 && is3sigmaKaonDau2) || (t->flavor == -1 && is3sigmaKaonDau1 && is3sigmaPionDau2)) hD0Pt3sigma->Fill(t->pT);

         bool is2sigmaPionDau1 = true;
         bool is2sigmaKaonDau1 = true;
         bool is2sigmaPionDau2 = true;
         bool is2sigmaKaonDau2 = true;

         if(t->isMtdDau1) is2sigmaPionDau1 = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 2.0 * fExpPion->Eval(pD1);
         if(t->isMtdDau1) is2sigmaKaonDau1 = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 2.0 * fExpKaon->Eval(pD1);
         if(t->isMtdDau2) is2sigmaPionDau2 = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 2.0 * fExpPion->Eval(pD2);
         if(t->isMtdDau2) is2sigmaKaonDau2 = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 2.0 * fExpKaon->Eval(pD2);

         if((t->flavor == 1 && is2sigmaPionDau1 && is2sigmaKaonDau2) || (t->flavor == -1 && is2sigmaKaonDau1 && is2sigmaPionDau2)) hD0Pt2sigma->Fill(t->pT);

         bool is1sigmaPionDau1 = true;
         bool is1sigmaKaonDau1 = true;
         bool is1sigmaPionDau2 = true;
         bool is1sigmaKaonDau2 = true;

         if(t->isMtdDau1) is1sigmaPionDau1 = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 1.0 * fExpPion->Eval(pD1);
         if(t->isMtdDau1) is1sigmaKaonDau1 = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 1.0 * fExpKaon->Eval(pD1);
         if(t->isMtdDau2) is1sigmaPionDau2 = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 1.0 * fExpPion->Eval(pD2);
         if(t->isMtdDau2) is1sigmaKaonDau2 = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 1.0 * fExpKaon->Eval(pD2);

         if((t->flavor == 1 && is1sigmaPionDau1 && is1sigmaKaonDau2) || (t->flavor == -1 && is1sigmaKaonDau1 && is1sigmaPionDau2)) hD0Pt1sigma->Fill(t->pT);

         if(t->isMtdDau1 && t->isMtdDau2) nBothMTD++;
         if(t->isMtdDau1 && !t->isMtdDau2) nOneMTD++;
         if(!t->isMtdDau1 && t->isMtdDau2) nOneMTD++;
         if(!t->isMtdDau1 && !t->isMtdDau2) nNoMTD++;
      }
   }
   if(daughterEff_Y_zeroPt){
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
   if(daughterEff_P){
      TCanvas* c1 = new TCanvas("c1", "PionEff", 500, 550);
      gStyle->SetOptStat(0);
      TH1F* hDrawPion = new TH1F("hDrawPion", "", 100, 0, 10);
      hDrawPion->GetYaxis()->SetTitle("Pion Yield Ratio");
      hDrawPion->GetYaxis()->SetRangeUser(0, 1.3);
      hDrawPion->GetXaxis()->SetTitle("p (GeV)");
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
      TH1F* hDrawKaon = new TH1F("hDrawKaon", "", 100, 0, 10);
      hDrawKaon->GetYaxis()->SetTitle("Kaon Yield Ratio");
      hDrawKaon->GetYaxis()->SetRangeUser(0, 1.3);
      hDrawKaon->GetXaxis()->SetTitle("p (GeV)");
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
   if(isD0MtdEff){
      TCanvas* c1 = new TCanvas("hD0pT", "", 450, 450);
      c1->SetLeftMargin(0.16);
      gStyle->SetOptStat(0);

      TH1F* hDraw = new TH1F("hDrawD0", "", 100, 0, 10);
      hDraw->GetYaxis()->SetRangeUser(0, 1.3);
      hDraw->GetYaxis()->SetTitle("D0 yield eff");
      hDraw->GetXaxis()->SetTitle("pT (GeV)");
      hDraw->Draw();

      hD0Pt->Sumw2();
      hD0Pt3sigma->Sumw2();
      hD0Pt2sigma->Sumw2();
      hD0Pt1sigma->Sumw2();

      hD0Pt3sigma->Divide(hD0Pt);
      hD0Pt2sigma->Divide(hD0Pt);
      hD0Pt1sigma->Divide(hD0Pt);

      hD0Pt3sigma->SetLineColor(kGreen-6);
      hD0Pt2sigma->SetLineColor(kBlue);
      hD0Pt1sigma->SetLineColor(kRed);

      hD0Pt3sigma->Draw("same");
      hD0Pt2sigma->Draw("same");
      hD0Pt1sigma->Draw("same");

      TLatex* latex = new TLatex();
      latex->SetTextSize(0.036);
      latex->DrawLatexNDC(0.6, 0.4, "-3 < D^{0} Rapidity < 3");

      TLegend *legend = new TLegend(0.7, 0.8, 0.9, 0.9);
      legend->AddEntry(hD0Pt3sigma, "3 RMS", "l");
      legend->AddEntry(hD0Pt2sigma, "2 RMS", "l");
      legend->AddEntry(hD0Pt1sigma, "1 RMS", "l");
      legend->Draw();
      
      std::cout << "both have mtd: " << nBothMTD << std::endl;
      std::cout << "only one have mtd: " << nOneMTD << std::endl;
      std::cout << "No one have mtd: " << nNoMTD << std::endl;
      std::cout << "Total: " << hD0Pt->GetEntries() << std::endl;

      std::string effFunc = Form("(x*x*%f + x*%f + %f)/%f", (float)nBothMTD, (float)nOneMTD, (float)nNoMTD, hD0Pt->GetEntries());
      TF1* effCal = new TF1("effCal", effFunc.c_str(), 0, 1);

      std::cout << "1 RMS, using eff_pi/k = 0.85, eff_D0 = " << effCal->Eval(0.85) << std::endl;
      std::cout << "2 RMS, using eff_pi/k = 0.95, eff_D0 = " << effCal->Eval(0.95) << std::endl;
      std::cout << "2 RMS, using eff_pi/k = 0.99, eff_D0 = " << effCal->Eval(0.99) << std::endl;
   }
}
