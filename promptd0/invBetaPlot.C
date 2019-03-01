#include "matchD.h"
#include "myAna.h"
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


void setPalette()
{
   gStyle->SetPalette(55);
   const Int_t NRGBs = 5;
   const Int_t NCont = 255;
   Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
   Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
   Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
   Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
   TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
   gStyle->SetNumberContours(NCont);
}

void invBetaPlot()
{
   bool draw = true;
   bool draw2D = false;
   bool projection = false;
   bool profile= true;
   TCanvas *c[4];
   TFile* f1 = new TFile("matchPromptD0.root");
   TNtuple* tp = (TNtuple*) f1->Get("PromptD");
   matchD* t = new matchD(tp);
   std::cout << t->GetEntries() << std::endl;


   TH2F* hdInvBetaPionVsPDau1D = new TH2F("hdInvBetaPionVsPDau1D", "hdInvBetaPionVsPDau1D" , 100, 0, 10, 1000, -0.1, 0.1);
   TH2F* hdInvBetaKaonVsPDau2D = new TH2F("hdInvBetaKaonVsPDau2D", "hdInvBetaKaonVsPDau2D", 100, 0, 10, 1000, -0.1, 0.1);
   TH2F* hdInvBetaKaonVsPDau1Dbar = new TH2F("hdInvBetaKaonVsPDau1Dbar", "hdInvBetaKaonVsPDau1Dbar", 100, 0, 10, 1000, -0.1, 0.1);
   TH2F* hdInvBetaPionVsPDau2Dbar = new TH2F("hdInvBetaPionVsPDau2Dbar", "hdInvBetaPionVsPDau2Dbar", 100, 0, 10, 1000, -0.1, 0.1);

   TH2F* hdInvBetaPionVsP = new TH2F("hdInvBetaPionVsP", "hdInvBetaPionVsP", 50, 0, 10, 1000, -0.1, 0.1);
   TH2F* hdInvBetaKaonVsP = new TH2F("hdInvBetaKaonVsP", "hdInvBetaKaonVsP", 50, 0, 10, 1000, -0.1, 0.1);

   // only talk about D0 pt<0.5
   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);
      if(t->y<-3 || t->y>3) continue;
      //if(t->pT>0.5) continue;
      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);
      if(t->flavor == 1 && t->isMtdDau1) hdInvBetaPionVsPDau1D->Fill(pD1, 1./t->beta1_PV - invBetaPion(pD1));
      if(t->flavor == 1 && t->isMtdDau2) hdInvBetaKaonVsPDau2D->Fill(pD2, 1./t->beta2_PV - invBetaKaon(pD2));
      if(t->flavor == -1 && t->isMtdDau1) hdInvBetaKaonVsPDau1Dbar->Fill(pD1, 1./t->beta1_PV - invBetaKaon(pD1));
      if(t->flavor == -1 && t->isMtdDau2) hdInvBetaPionVsPDau2Dbar->Fill(pD2, 1./t->beta2_PV - invBetaPion(pD2));

      if(t->flavor == 1 && t->isMtdDau1) hdInvBetaPionVsP->Fill(pD1, 1./t->beta1_PV - invBetaPion(pD1));
      if(t->flavor == -1 && t->isMtdDau2) hdInvBetaPionVsP->Fill(pD2, 1./t->beta2_PV - invBetaPion(pD2));

      if(t->flavor == -1 && t->isMtdDau1) hdInvBetaKaonVsP->Fill(pD1, 1./t->beta1_PV - invBetaKaon(pD1));
      if(t->flavor == 1 && t->isMtdDau2) hdInvBetaKaonVsP->Fill(pD2, 1./t->beta2_PV - invBetaKaon(pD2));
   }
   if(draw){
      setPalette();
      for(int i=0; i<4; i++){
         c[i] = new TCanvas(Form("c%d", i),Form("c%d", i), 450, 500);
         c[i]->SetLogz();
         c[i]->SetLeftMargin(0.16);
         gStyle->SetOptStat(0);
      }
   }
   if(draw && draw2D){
	   c[0]->cd();
	   hdInvBetaPionVsPDau1D->GetXaxis()->SetTitle("p (GeV)");
	   hdInvBetaPionVsPDau1D->GetYaxis()->SetTitle("1/#beta - 1/#beta_{#pi}");
	   hdInvBetaPionVsPDau1D->SetMinimum(1.0);
	   hdInvBetaPionVsPDau1D->Draw("COLZ");
	   c[1]->cd();
	   hdInvBetaKaonVsPDau2D->GetXaxis()->SetTitle("p (GeV)");
	   hdInvBetaKaonVsPDau2D->GetYaxis()->SetTitle("1/#beta - 1/#beta_{K}");
	   hdInvBetaKaonVsPDau2D->SetMinimum(1.0);
	   hdInvBetaKaonVsPDau2D->Draw("COLZ");
	   c[2]->cd();
	   hdInvBetaKaonVsPDau1Dbar->GetXaxis()->SetTitle("p (GeV)");
	   hdInvBetaKaonVsPDau1Dbar->GetYaxis()->SetTitle("1/#beta - 1/#beta_{K}");
	   hdInvBetaKaonVsPDau1Dbar->SetMinimum(1.0);
	   hdInvBetaKaonVsPDau1Dbar->Draw("COLZ");
	   c[3]->cd();
	   hdInvBetaPionVsPDau2Dbar->GetXaxis()->SetTitle("p (GeV)");
	   hdInvBetaPionVsPDau2Dbar->GetYaxis()->SetTitle("1/#beta - 1/#beta_{#pi}");
	   hdInvBetaPionVsPDau2Dbar->SetMinimum(1.0);
	   hdInvBetaPionVsPDau2Dbar->Draw("COLZ");
	   //std::cout << hdInvBetaPionVsPDau1D->ProjectionY()->Integral()/hdInvBetaPionVsPDau1D->ProjectionY()->Integral(0, 100000) << std::endl;
	   //std::cout << hdInvBetaKaonVsPDau2D->ProjectionY()->Integral()/hdInvBetaKaonVsPDau2D->ProjectionY()->Integral(0, 100000) << std::endl;
	   //std::cout << hdInvBetaPionVsPDau2Dbar->ProjectionY()->Integral()/hdInvBetaPionVsPDau2Dbar->ProjectionY()->Integral(0, 100000) << std::endl;
	   //std::cout << hdInvBetaKaonVsPDau1Dbar->ProjectionY()->Integral()/hdInvBetaKaonVsPDau1Dbar->ProjectionY()->Integral(0, 100000) << std::endl;
   }

   if(draw && projection){
      c[0]->cd();
      TH1D* hdInvBetaDau1D =  hdInvBetaPionVsPDau1D->ProjectionY("dau1D0", 1, 30);
      hdInvBetaDau1D->Fit("gaus");
      c[1]->cd();
      TH1D* hdInvBetaDau2D =  hdInvBetaKaonVsPDau2D->ProjectionY("dau2D0", 1, 30);
      hdInvBetaDau2D->Fit("gaus");
      c[3]->cd();
      TH1D* hdInvBetaDau1Dbar =  hdInvBetaKaonVsPDau1Dbar->ProjectionY("dau1D0bar", 1, 30);
      hdInvBetaDau1Dbar->Fit("gaus");
      c[1]->cd();
      TH1D* hdInvBetaDau2Dbar =  hdInvBetaPionVsPDau2Dbar->ProjectionY("dau2D0bar", 1, 30);
      hdInvBetaDau2Dbar->Fit("gaus");
   }
   if(draw && profile){
      c[0]->cd();
      TProfile* hdInvBetaPion = hdInvBetaPionVsP->ProfileX("Pion", 1, -1, "i");
      hdInvBetaPion->GetYaxis()->SetRangeUser(-0.001, 0.001);
      hdInvBetaPion->GetYaxis()->SetTitle("#sigma/#sqrt{N}");
      hdInvBetaPion->GetXaxis()->SetTitle("p (GeV)");
      hdInvBetaPion->Draw();

      c[1]->cd();
      TProfile* hdInvBetaPionstd = hdInvBetaPionVsP->ProfileX("Pionstd", 1, -1, "s");
      hdInvBetaPionstd->GetYaxis()->SetRangeUser(-0.03, 0.03);
      hdInvBetaPionstd->GetYaxis()->SetTitle("#sigma");
      hdInvBetaPionstd->GetXaxis()->SetTitle("p (GeV)");
      hdInvBetaPionstd->Draw();

      c[2]->cd();
      TProfile* hdInvBetaKaon = hdInvBetaKaonVsP->ProfileX("Kion", 1, -1, "i");
      hdInvBetaKaon->GetYaxis()->SetRangeUser(-0.001, 0.001);
      hdInvBetaKaon->GetYaxis()->SetTitle("#sigma/#sqrt{N}");
      hdInvBetaKaon->GetXaxis()->SetTitle("p (GeV)");
      hdInvBetaKaon->Draw();

      c[3]->cd();
      TProfile* hdInvBetaKaonstd = hdInvBetaKaonVsP->ProfileX("Kionstd", 1, -1, "s");
      hdInvBetaKaonstd->GetYaxis()->SetRangeUser(-0.03, 0.03);
      hdInvBetaKaonstd->GetYaxis()->SetTitle("#sigma");
      hdInvBetaKaonstd->GetXaxis()->SetTitle("p (GeV)");
      hdInvBetaKaonstd->Draw();
//      diffBetaRms[7] = {0}:
//      for(int i=0; i<7; i++){
         //std::cout << hdInvBetaPion->GetBinContent(i+1) << std::endl;;
         //std::cout << hdInvBetaPionstd->GetBinContent(i+1) << std::endl;;
         //std::cout << hdInvBetaKaon->GetBinContent(i+1) << std::endl;;
         //std::cout << hdInvBetaKaonstd->GetBinContent(i+1) << std::endl;;
         //std::cout << hdInvBetaPion->GetBinError(i+1) << std::endl;;
         //std::cout << hdInvBetaPionstd->GetBinError(i+1) << std::endl;;
         //std::cout << hdInvBetaKaon->GetBinError(i+1) << std::endl;;
         //std::cout << hdInvBetaKaonstd->GetBinError(i+1) << std::endl;;
 //        diffBetaRms[i] = {hdInvBetaPionstd->GetBinError(i+1) + hdInvBetaKaonstd->GetBinError(i+1) }/ 2.;
  //    }
      //TH1F* hPionFrac3Sigma = new TH1F("hPionFrac3Sigma", "hPionFrac3Sigma", 1000, 0, 10)
      //TH2F* hPion = hdInvBetaPionVsPDau1D->Clone();
      //hPion->Add(hdInvBetaPionVsPDau2Dbar);
      //for(int ip=0; ip<1000; ip++){
      //   float frac = 
         
      //}

   }
}
