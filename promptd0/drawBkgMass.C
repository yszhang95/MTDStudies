#include "myAna.h"

void drawBkgMass()
{
   TFile* f1 = new TFile("hyjetsMassHists_reRECO.root");
   TH1F* hMass[ana::nuOfY];
   TH1F* hMassMtd[ana::nuOfY];
   TH1F* hMassCent[ana::nuOfY];
   TH1F* hMassCentMtd[ana::nuOfY];
   TH1F* hfrac;
   TH1F* hfracCent;
   for(int iy=0; iy<ana::nuOfY; iy++){
      f1->GetObject(Form("hMass%d", iy), hMass[iy]);
      f1->GetObject(Form("hMassMtd%d", iy), hMassMtd[iy]);
      f1->GetObject(Form("hMassCent%d", iy), hMassCent[iy]);
      f1->GetObject(Form("hMassCentMtd%d", iy), hMassCentMtd[iy]);
   }
   f1->GetObject("hfrac", hfrac);
   f1->GetObject("hfracCent", hfracCent);


   TH1F* hMassMid = new TH1F(*hMass[5]);
   gStyle->SetOptStat(0);
   hMassMid->Add(hMass[4]);
   hMassMid->Add(hMass[6]);
   hMassMid->Add(hMass[7]);

   TH1F* hMassMtdMid = new TH1F(*hMassMtd[5]);
   gStyle->SetOptStat(0);
   hMassMtdMid->Add(hMassMtd[4]);
   hMassMtdMid->Add(hMassMtd[6]);
   hMassMtdMid->Add(hMassMtd[7]);

   TH1F* hMassCentMid = new TH1F(*hMassCent[5]);
   gStyle->SetOptStat(0);
   hMassCentMid->Add(hMassCent[4]);
   hMassCentMid->Add(hMassCent[6]);
   hMassCentMid->Add(hMassCent[7]);

   TH1F* hMassCentMtdMid = new TH1F(*hMassCentMtd[5]);
   gStyle->SetOptStat(0);
   hMassCentMtdMid->Add(hMassCentMtd[4]);
   hMassCentMtdMid->Add(hMassCentMtd[6]);
   hMassCentMtdMid->Add(hMassCentMtd[7]);


   TCanvas* c1 = new TCanvas("c1", "", 600, 450);

   TLatex* ltx = new TLatex();
   ltx->SetTextSize(0.05);

   hMassMid->SetTitle("");
   float max1 = hMassMid->GetMaximum();
   hMassMid->GetYaxis()->SetRangeUser(0, 1.3*max1);
   hMassMid->Sumw2();
   hMassMid->Draw("e");
   ltx->DrawLatexNDC(0.35, 0.93, "Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.67, 0.8, "CMS");
   ltx->DrawLatexNDC(0.65, 0.71, "Preliminary");
   ltx->DrawLatexNDC(0.67, 0.6, "Hydjet");
   ltx->DrawLatexNDC(0.67, 0.45, "MB events");
   ltx->DrawLatexNDC(0.65, 0.35, "p_{T} < 0.5 GeV/c");
   ltx->DrawLatexNDC(0.65, 0.26, "-1 < y < 1");

   TCanvas* c2 = new TCanvas("c2", "", 600, 450);

   hMassMtdMid->SetTitle("");
   float max2 = hMassMtdMid->GetMaximum();
   hMassMtdMid->GetYaxis()->SetRangeUser(0, 1.3*max2);
   hMassMtdMid->Sumw2();
   hMassMtdMid->Draw("e");
   ltx->DrawLatexNDC(0.35, 0.93, "Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.67, 0.8, "CMS");
   ltx->DrawLatexNDC(0.65, 0.71, "Preliminary");
   ltx->DrawLatexNDC(0.67, 0.6, "Hydjet");
   ltx->DrawLatexNDC(0.67, 0.45, "MB events");
   ltx->DrawLatexNDC(0.65, 0.35, "p_{T} < 0.5 GeV/c");
   ltx->DrawLatexNDC(0.65, 0.26, "-1 < y < 1");


   TCanvas* c3 = new TCanvas("c3", "", 600, 450);

   hMassCentMid->SetTitle("");
   float max3 = hMassCentMid->GetMaximum();
   hMassCentMid->GetYaxis()->SetRangeUser(0, 1.3*max3);
   hMassCentMid->Sumw2();
   hMassCentMid->Draw("e");
   ltx->DrawLatexNDC(0.35, 0.93, "Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.67, 0.8, "CMS");
   ltx->DrawLatexNDC(0.65, 0.71, "Preliminary");
   ltx->DrawLatexNDC(0.67, 0.6, "Hydjet");
   ltx->DrawLatexNDC(0.6, 0.45, "centrality 0 ~10%");
   ltx->DrawLatexNDC(0.65, 0.35, "p_{T} < 0.5 GeV/c");
   ltx->DrawLatexNDC(0.65, 0.26, "-1 < y < 1");

   TCanvas* c4 = new TCanvas("c4", "", 600, 450);

   hMassCentMtdMid->SetTitle("");
   float max4 = hMassCentMtdMid->GetMaximum();
   hMassCentMtdMid->GetYaxis()->SetRangeUser(0, 1.3*max4);
   hMassCentMtdMid->Sumw2();
   hMassCentMtdMid->Draw("e");
   ltx->DrawLatexNDC(0.35, 0.93, "Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.67, 0.8, "CMS");
   ltx->DrawLatexNDC(0.65, 0.71, "Preliminary");
   ltx->DrawLatexNDC(0.67, 0.6, "Hydjet");
   ltx->DrawLatexNDC(0.6, 0.45, "centrality 0 ~ 10%");
   ltx->DrawLatexNDC(0.65, 0.35, "p_{T} < 0.5 GeV/c");
   ltx->DrawLatexNDC(0.65, 0.26, "-1 < y < 1");

   TCanvas* c5 = new TCanvas("c5", "", 550, 450);
   hMassCentMid->Draw("e");
   hMassCentMtdMid->Draw("esame");
   ltx->SetTextSize(0.05);
   ltx->DrawLatexNDC(0.35, 0.93, "Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.63, 0.8, "CMS");
   ltx->SetTextSize(0.038);
   ltx->DrawLatexNDC(0.61, 0.71, "Preliminary");
   ltx->DrawLatexNDC(0.63, 0.6, "Hydjet");
   ltx->DrawLatexNDC(0.6, 0.4, "centrality 0 ~10%");
   ltx->DrawLatexNDC(0.6, 0.33, "p_{T} < 0.5 GeV/c");
   ltx->DrawLatexNDC(0.6, 0.26, "-1 < y < 1");
   TLegend* lgdCent = new TLegend(0.76, 0.75, 0.9, 0.9);
   lgdCent->AddEntry(hMassCentMid, "w/o MTD", "lp");
   lgdCent->AddEntry(hMassCentMtdMid, "w/ MTD", "lp");
   lgdCent->Draw();

   TCanvas* c6 = new TCanvas("c6", "", 550, 450);
   hMassMid->Draw("e");
   hMassMtdMid->Draw("esame");
   ltx->SetTextSize(0.05);
   ltx->DrawLatexNDC(0.35, 0.93, "Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.63, 0.8, "CMS");
   ltx->SetTextSize(0.038);
   ltx->DrawLatexNDC(0.61, 0.71, "Preliminary");
   ltx->DrawLatexNDC(0.63, 0.6, "Hydjet");
   ltx->DrawLatexNDC(0.6, 0.4, "MB events");
   ltx->DrawLatexNDC(0.6, 0.33, "p_{T} < 0.5 GeV/c");
   ltx->DrawLatexNDC(0.6, 0.26, "-1 < y < 1");
   TLegend* lgdMB = new TLegend(0.76, 0.75, 0.9, 0.9);
   lgdMB->AddEntry(hMassMid, "w/o MTD", "lp");
   lgdMB->AddEntry(hMassMtdMid, "w/ MTD", "lp");
   lgdMB->Draw();
}
