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
   TFile* f1 = new TFile("matchPromptD0.root");
   TNtuple* tp = (TNtuple*) f1->Get("PromptD");
   matchD* t = new matchD(tp);
   std::cout << t->GetEntries() << std::endl;

   // we only have a look at D0 pT < 0.5
   TH1F* hDraw = new TH1F("hDraw", "", ana::nuOfY, ana::ybin);
   TH1F* hNoMtd = new TH1F("hNotMtd", "hNoMtd", ana::nuOfY, ana::ybin);
   TH1F* hDau1Mtd = new TH1F("hDau1tMtd", "hDau1Mtd", ana::nuOfY, ana::ybin);
   TH1F* hDau2Mtd = new TH1F("hDau2tMtd", "hDau2Mtd", ana::nuOfY, ana::ybin);
   TH1F* hBothMtd = new TH1F("hBothtMtd", "hBothMtd", ana::nuOfY, ana::ybin);

   std::vector<Long64_t> nNoMtd(ana::nuOfY, 0);
   std::vector<Long64_t> nDau1Mtd(ana::nuOfY, 0);
   std::vector<Long64_t> nDau2Mtd(ana::nuOfY, 0);
   std::vector<Long64_t> nBothMtd(ana::nuOfY, 0);

   std::vector<Long64_t> nTotal(ana::nuOfY, 0);

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      const int iy = whichY(t->y);
      if( iy == -1 ) continue;

      if(t->isMtdDau1) nDau1Mtd[iy]++;
      if(t->isMtdDau2) nDau2Mtd[iy]++;
      if(!t->isMtdDau1 && !t->isMtdDau2) nNoMtd[iy]++;
      if(t->isMtdDau2 && t->isMtdDau2) nBothMtd[iy]++;

      nTotal[iy]++;
   }
   
   for(int iy=0; iy<ana::nuOfY; iy++){
      hNoMtd->SetBinContent(iy+1, (double)nNoMtd[iy]/nTotal[iy]);
      hDau1Mtd->SetBinContent(iy+1, (double)nDau1Mtd[iy]/nTotal[iy]);
      hDau2Mtd->SetBinContent(iy+1, (double)nDau2Mtd[iy]/nTotal[iy]);
      hBothMtd->SetBinContent(iy+1, (double)nBothMtd[iy]/nTotal[iy]);
   }

   TCanvas *c1 = new TCanvas();
   gStyle->SetOptStat(0);
   hDraw->GetYaxis()->SetRangeUser(0, 1.35);
   hDraw->GetXaxis()->SetTitle("y");
   hNoMtd->SetLineColor(kBlack);
   hDau1Mtd->SetLineColor(kRed);
   hDau2Mtd->SetLineColor(kBlue);
   hBothMtd->SetLineColor(kGreen);
   hDraw->Draw();
   hNoMtd->Draw("same");
   hDau1Mtd->Draw("same");
   hDau2Mtd->Draw("same");
   hBothMtd->Draw("same");
   TLegend *l1 = new TLegend(0.5, 0.6, 0.97, 0.95);
   l1->AddEntry(hNoMtd, "ratio of D0 yield w/o mtd and all D0", "pl");
   l1->AddEntry(hDau1Mtd, "ratio of D0 yield whose dau1 w/ mtd to all D0", "pl");
   l1->AddEntry(hDau2Mtd, "ratio of D0 yield whose dau2 w/ mtd to all D0", "pl");
   l1->AddEntry(hBothMtd, "ratio of D0 yield whose dau1 & dau2 w/ mtd to all D0", "pl");
   l1->Draw();
}
