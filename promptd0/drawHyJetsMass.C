#include "myAna.h"

void drawHyJetsMass()
{
   //TFile* f1 = new TFile("hyjetsMassHists_reRECO.root.old");
   //TFile* f1 = new TFile("hyjetsMassHists.root.old");
   //TFile* f1 = new TFile("hyjetsMassHists_reRECO.root");
   //TFile* f1 = new TFile("hyjetsMassHists.root");
   TFile* f1 = new TFile("hyjetsMassHists_reRECO_both_mtd.root");
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

   TLatex* ltx = new TLatex();
   ltx->SetTextSize(0.05);

   TCanvas* c[ana::nuOfY];
   for(int iy=0; iy<ana::nuOfY; iy++){
      c[iy] = new TCanvas(Form("c%d", iy), "", 450, 500);
      c[iy]->SetLeftMargin(0.15);
      gStyle->SetOptStat(0);
      float max = hMass[iy]->GetMaximum() * 1.3;
      hMass[iy]->SetTitle("");
      hMass[iy]->GetYaxis()->SetRangeUser(0, max);

      hMass[iy]->SetLineColor(kBlue+6);
      hMassCent[iy]->SetLineColor(kBlue-6);
      hMassMtd[iy]->SetLineColor(kGreen-6);
      hMassCentMtd[iy]->SetLineColor(kRed-6);

      hMass[iy]->Draw();
      hMassCent[iy]->Draw("same");
      hMassMtd[iy]->Draw("same");
      hMassCentMtd[iy]->Draw("same");
      TLegend *lgd = new TLegend(0.6, 0.75, 0.9, 0.95);
      lgd->AddEntry(hMass[iy], "MB w/o MTD", "lp");
      lgd->AddEntry(hMassCent[iy], "0-10% w/o MTD", "lp");
      lgd->AddEntry(hMassMtd[iy], "MB w/ MTD", "lp");
      lgd->AddEntry(hMassCentMtd[iy], "0-10% w/ MTD", "lp");
      lgd->Draw();
      ltx->DrawLatexNDC(0.6, 0.7, "p_{T}<0.5");
      ltx->DrawLatexNDC(0.6, 0.6, Form("%.1f < y < %.1f", ana::ybin[iy], ana::ybin[iy+1]));
   }

   TCanvas *c1 = new TCanvas("cfrac", "", 450, 500);
   c1->SetLeftMargin(0.16);
   hfrac->SetLineColor(kBlue);
   hfracCent->SetLineColor(kRed);
   hfrac->Draw();
   hfracCent->Draw("same");
   TLegend* l1 = new TLegend(0.6, 0.75, 0.9, 0.9);
   l1->AddEntry(hfrac, "MB", "lp");
   l1->AddEntry(hfracCent, "0~10%", "lp");
   l1->Draw();
   ltx->DrawLatexNDC(0.6, 0.7, "p_{T}<0.5");
}
