#include "matchD.h"
#include "myAna.h"
int whichY(const float& y)
{
   for(int i=0; i<ana::nuOfY; i++){
      if(y>ana::ybin[i] && y<ana::ybin[i+1]) return i;
   }
   return -1;
}

void mtdAcceptance()
{
   TFile* f1 = new TFile("matchPromptD0_fullSample.root");
   TNtuple* tp = (TNtuple*) f1->Get("PromptD");
   matchD* t = new matchD(tp);
   std::cout << t->GetEntries() << std::endl;

   // we have a look at D0 pT < 0.5 and full pT with |y| < 3
   // here is related to pT<0.5
   TH1F* hDraw = new TH1F("hDraw", "", ana::nuOfY, ana::ybin);
   TH1F* hNoMtd = new TH1F("hNotMtd", "hNoMtd", ana::nuOfY, ana::ybin);
   TH1F* hDau1Mtd = new TH1F("hDau1tMtd", "hDau1Mtd", ana::nuOfY, ana::ybin);
   TH1F* hDau2Mtd = new TH1F("hDau2tMtd", "hDau2Mtd", ana::nuOfY, ana::ybin);
   TH1F* hBothMtd = new TH1F("hBothtMtd", "hBothMtd", ana::nuOfY, ana::ybin);

   TH1F* hDrawY = new TH1F("hDrawY", "", ana::nuOfY, ana::ybin);
   TH1F* hAllY = new TH1F("hAllY", "hAllY", ana::nuOfY, ana::ybin);
   TH1F* hNoMtdY = new TH1F("hNoMtdY", "hNoMtdY", ana::nuOfY, ana::ybin);
   TH1F* hDau1MtdY = new TH1F("hDau1MtdY", "hDau1MtdY", ana::nuOfY, ana::ybin);
   TH1F* hDau2MtdY = new TH1F("hDau2MtdY", "hDau2MtdY", ana::nuOfY, ana::ybin);
   TH1F* hBothMtdY = new TH1F("hBothMtdY", "hBothMtdY", ana::nuOfY, ana::ybin);

   // here is related to full pT
   TH1F* hDrawPt = new TH1F("hDrawPt", "", 100, 0, 10);
   TH1F* hAllPt = new TH1F("hAllPt", "All", 100, 0, 10);
   TH1F* hNoMtdPt = new TH1F("hNoMtd", "hNoMtd", 100, 0, 10);
   TH1F* hDau1MtdPt = new TH1F("hDau1Mtd", "hDau1Mtd", 100, 0, 10);
   TH1F* hDau2MtdPt = new TH1F("hDau2Mtd", "hDau2Mtd", 100, 0, 10);
   TH1F* hBothMtdPt = new TH1F("hBothMtd", "hBothMtd", 100, 0, 10);

   std::vector<Long64_t> nNoMtd(ana::nuOfY, 0);
   std::vector<Long64_t> nDau1Mtd(ana::nuOfY, 0);
   std::vector<Long64_t> nDau2Mtd(ana::nuOfY, 0);
   std::vector<Long64_t> nBothMtd(ana::nuOfY, 0);

   std::vector<Long64_t> nTotal(ana::nuOfY, 0);

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      const int iy = whichY(t->y);
      if( iy == -1 ) continue;

      if(fabs(t->EtaD1) > 3) continue;
      if(fabs(t->EtaD2) > 3) continue;

      if(fabs(t->EtaD1) < 1.4 ? t->pTD1 <= 0.8 : t->pTD1 <= 0.5) continue;
      if(fabs(t->EtaD2) < 1.4 ? t->pTD2 <= 0.8 : t->pTD2 <= 0.5) continue;

      if(t->pT < 0.5){
         if(t->isMtdDau1) nDau1Mtd[iy]++;
         if(t->isMtdDau2) nDau2Mtd[iy]++;
         if(!t->isMtdDau1 && !t->isMtdDau2) nNoMtd[iy]++;
         if(t->isMtdDau1 && t->isMtdDau2) nBothMtd[iy]++;
         nTotal[iy]++;
      
         hAllY->Fill(t->y);
         if(t->isMtdDau1) hDau1MtdY->Fill(t->y);
         if(t->isMtdDau2) hDau2MtdY->Fill(t->y);
         if(!t->isMtdDau1 && !t->isMtdDau2) hNoMtdY->Fill(t->y);
         if(t->isMtdDau1 && t->isMtdDau2) hBothMtdY->Fill(t->y);
      }

      hAllPt->Fill(t->pT);
      if(t->isMtdDau1) hDau1MtdPt->Fill(t->pT);
      if(t->isMtdDau2) hDau2MtdPt->Fill(t->pT);
      if(!t->isMtdDau1 && !t->isMtdDau2) hNoMtdPt->Fill(t->pT);
      if(t->isMtdDau1 && t->isMtdDau2) hBothMtdPt->Fill(t->pT);
   }
   

   for(int iy=0; iy<ana::nuOfY; iy++){
      hNoMtd->SetBinContent(iy+1, (double)nNoMtd[iy]/nTotal[iy]);
      hDau1Mtd->SetBinContent(iy+1, (double)nDau1Mtd[iy]/nTotal[iy]);
      hDau2Mtd->SetBinContent(iy+1, (double)nDau2Mtd[iy]/nTotal[iy]);
      hBothMtd->SetBinContent(iy+1, (double)nBothMtd[iy]/nTotal[iy]);
   }

//   TCanvas *c1 = new TCanvas();
//   gStyle->SetOptStat(0);
//   hDraw->GetYaxis()->SetRangeUser(0, 1.35);
//   hDraw->GetXaxis()->SetTitle("y");
//   hNoMtd->SetLineColor(kBlack);
//   hDau1Mtd->SetLineColor(kRed);
//   hDau2Mtd->SetLineColor(kBlue);
//   hBothMtd->SetLineColor(kGreen);
//   hDraw->Draw();
//   hNoMtd->Draw("same");
//   hDau1Mtd->Draw("same");
//   hDau2Mtd->Draw("same");
//   hBothMtd->Draw("same");
//   TLegend *l1 = new TLegend(0.5, 0.6, 0.97, 0.95);
//   l1->AddEntry(hNoMtd, "ratio of D0 yield w/o mtd and all D0", "pl");
//   l1->AddEntry(hDau1Mtd, "ratio of D0 yield whose dau1 w/ mtd to all D0", "pl");
//   l1->AddEntry(hDau2Mtd, "ratio of D0 yield whose dau2 w/ mtd to all D0", "pl");
//   l1->AddEntry(hBothMtd, "ratio of D0 yield whose dau1 & dau2 w/ mtd to all D0", "pl");
//   l1->Draw();

   // full range of pT, with |y|<3 
   TLatex* latex = new TLatex();
   latex->SetTextSize(0.036);

   TCanvas* c2 = new TCanvas("cD0pT", "", 450, 500);

   gStyle->SetOptStat(0);

   hDrawPt->GetYaxis()->SetRangeUser(0, 1.3);
   hDrawPt->GetYaxis()->SetTitle("D0 yield ratio");
   hDrawPt->GetXaxis()->SetTitle("D0 pT (GeV)");
   hDrawPt->Draw();

   hDau1MtdPt->Divide(hAllPt);
   hDau2MtdPt->Divide(hAllPt);
   hBothMtdPt->Divide(hAllPt);
   hNoMtdPt->Divide(hAllPt);

   hDau1MtdPt->SetLineColor(kBlue+6);
   hDau2MtdPt->SetLineColor(kBlue-6);
   hBothMtdPt->SetLineColor(kRed);
   hNoMtdPt->SetLineColor(kBlack);

   hDau1MtdPt->Draw("same");
   hDau2MtdPt->Draw("same");
   hBothMtdPt->Draw("same");
   hNoMtdPt->Draw("same");

   latex->DrawLatexNDC(0.6, 0.68, "-3 < D^{0} Rapidity < 3");

   TLegend *l2 = new TLegend(0.5, 0.75, 0.97, 0.95);
   l2->AddEntry(hNoMtdPt, "no daughter has mtd hits", "pl");
   l2->AddEntry(hDau1MtdPt, "dau1 w/ mtd", "pl");
   l2->AddEntry(hDau2MtdPt, "dau2 w/ mtd", "pl");
   l2->AddEntry(hBothMtdPt, "dau1 && dau2 w/ mtd", "pl");
   l2->Draw();

   TCanvas* c3 = new TCanvas("cD0Y", "", 450, 500);

   gStyle->SetOptStat(0);

   hDrawY->GetYaxis()->SetRangeUser(0, 1.3);
   hDrawY->GetYaxis()->SetTitle("D0 yield ratio");
   hDrawY->GetXaxis()->SetTitle("D0 y");
   hDrawY->Draw();

   hDau1MtdY->Divide(hAllY);
   hDau2MtdY->Divide(hAllY);
   hBothMtdY->Divide(hAllY);
   hNoMtdY->Divide(hAllY);

   hDau1MtdY->SetLineColor(kBlue+6);
   hDau2MtdY->SetLineColor(kBlue-6);
   hBothMtdY->SetLineColor(kRed);
   hNoMtdY->SetLineColor(kBlack);

   hDau1MtdY->Draw("same");
   hDau2MtdY->Draw("same");
   hBothMtdY->Draw("same");
   hNoMtdY->Draw("same");

   latex->DrawLatexNDC(0.6, 0.68, "D^{0} p_{T} < 0.5 GeV");

   TLegend *l3 = new TLegend(0.5, 0.75, 0.97, 0.95);
   l3->AddEntry(hNoMtdY, "no daughter has mtd hits", "pl");
   l3->AddEntry(hDau1MtdY, "dau1 w/ mtd", "pl");
   l3->AddEntry(hDau2MtdY, "dau2 w/ mtd", "pl");
   l3->AddEntry(hBothMtdY, "dau1 && dau2 w/ mtd", "pl");
   l3->Draw();

   //std::cout << hNoMtdY->GetBinContent(5+1) << std::endl;
   //std::cout << (double)nNoMtd[5]/nTotal[5] << std::endl;
   //std::cout << hDau1MtdY->GetBinContent(5+1) << std::endl;
   //std::cout << (double)nDau1Mtd[5]/nTotal[5] << std::endl;
   //std::cout << hDau2MtdY->GetBinContent(5+1) << std::endl;
   //std::cout << (double)nDau2Mtd[5]/nTotal[5] << std::endl;
   //std::cout << hBothMtdY->GetBinContent(5+1) << std::endl;
   //std::cout << (double)nBothMtd[5]/nTotal[5] << std::endl;
}
