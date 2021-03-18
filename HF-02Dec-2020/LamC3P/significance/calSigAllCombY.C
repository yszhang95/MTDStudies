#include "myAna.h"

void calSigAllCombY()
{
   TFile* f1 = new TFile("genPt_reRECO.root");
   TFile* f2 = new TFile("HEPData-ins1616207-v1.root");
   TFile* f3 = new TFile("lamC3PMassHists_reRECO_all.root");
   TFile* f4 = new TFile("hyjetsMassHists_reRECO_all.root");

   std::ofstream fwMB("sigMB.txt");
   std::ofstream fwoMB("sigWoMB.txt");

   TH1D* hGenPtMidY;
   TH1F* hData;
   TH3D* hSignal;
   TH3D* hBkg;
   TH3D* hSignalMtd;
   TH3D* hBkgMtd;

   TH1D* hSigVsPt = new TH1D("hSigVsPt", "", ana::nuOfPt, ana::ptbin);
   TH1D* hSigMtdVsPt = new TH1D("hSigMtdVsPt", "", ana::nuOfPt, ana::ptbin);

   TH1D* hSVsPt = new TH1D("hSVsPt", "", ana::nuOfPt, ana::ptbin);
   TH1D* hBVsPt = new TH1D("hBVsPt", "", ana::nuOfPt, ana::ptbin);

   TH1D* hSMtdVsPt = new TH1D("hSMtdVsPt", "", ana::nuOfPt, ana::ptbin);
   TH1D* hBMtdVsPt = new TH1D("hBMtdVsPt", "", ana::nuOfPt, ana::ptbin);

   f1->GetObject("hGenPtMidY", hGenPtMidY);

   f2->GetObject("Table 3/Hist1D_y1", hData);
   f3->GetObject("hMassVsPtVsY", hSignal);
   f4->GetObject("hMassVsPtVsY", hBkg);
   f3->GetObject("hMassVsPtVsYMtd", hSignalMtd);
   f4->GetObject("hMassVsPtVsYMtd", hBkgMtd);

   // try to scale gen spectrum
   std::vector<double> scale_factor_perEvt(ana::nuOfPt);

   TH3D* hScale = new TH3D("hScale", "", ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);

   ana::calScalePerEvt(hGenPtMidY, hData, hScale, scale_factor_perEvt, ana::nmass);
   TH3D *hSignalScale = new TH3D();
   *hSignalScale =  (*hSignal) * (*hScale);

   TH3D *hSignalMtdScale = new TH3D();
   *hSignalMtdScale = (*hSignalMtd) * (*hScale);

   TH1D* hSig[ana::nuOfPt];
   TH1D* hSigMtd[ana::nuOfPt];

   TH1D* hS[ana::nuOfPt];
   TH1D* hSMtd[ana::nuOfPt];

   TH1D* hB[ana::nuOfPt];
   TH1D* hBMtd[ana::nuOfPt];

   TLatex* ltx = new TLatex();
   for(int ipt=0; ipt<ana::nuOfPt; ipt++){
   //for(int ipt=0; ipt<2; ipt++){
      double sint = 0;
      double sMtdint = 0;
      double bint = 0;
      double bMtdint = 0;


      hSig[ipt] = new TH1D(Form("hSig%d", ipt), "", ana::nuOfY, ana::ybin);
      hSigMtd[ipt] = new TH1D(Form("hSigMtd%d", ipt), "", ana::nuOfY, ana::ybin);

      hS[ipt] = new TH1D(Form("hS%d", ipt), "", ana::nuOfY, ana::ybin);
      hSMtd[ipt] = new TH1D(Form("hSMtd%d", ipt), "", ana::nuOfY, ana::ybin);

      hB[ipt] = new TH1D(Form("hB%d", ipt), "", ana::nuOfY, ana::ybin);
      hBMtd[ipt] = new TH1D(Form("hBMtd%d", ipt), "", ana::nuOfY, ana::ybin);

      double pTCutMin = ana::ptbin[ipt];
      double pTCutMax = ana::ptbin[ipt+1];

      for(int iy=0; iy<ana::nuOfY; iy++){

         double yCutMin = ana::ybin[iy];
         double yCutMax = ana::ybin[iy+1];

         double massCutMin = ana::mass_lw[iy];
         double massCutMax = ana::mass_up[iy];

         double sig, s, b, sErr, bErr;
         double sigMtd, sMtd, bMtd, sMtdErr, bMtdErr;

         calSig(hSignalScale, hBkg, 
               yCutMin, yCutMax, pTCutMin, pTCutMax, massCutMin, massCutMax, 
               sig, s, b, sErr, bErr);
         calSig(hSignalMtdScale, hBkgMtd, 
               yCutMin, yCutMax, pTCutMin, pTCutMax, massCutMin, massCutMax, 
               sigMtd, sMtd, bMtd, sMtdErr, bMtdErr);

         sint += s;
         sMtdint += sMtd;

         bint += b;
         bMtdint += bMtd;

         hSig[ipt]->SetBinContent(iy+1, sig);
         hSigMtd[ipt]->SetBinContent(iy+1, sigMtd);

         hS[ipt]->SetBinContent(iy+1, s);
         hSMtd[ipt]->SetBinContent(iy+1, sMtd);
         hS[ipt]->SetBinError(iy+1, sErr);
         hSMtd[ipt]->SetBinError(iy+1, sMtdErr);

         hB[ipt]->SetBinContent(iy+1, b);
         hBMtd[ipt]->SetBinContent(iy+1, bMtd);
         hB[ipt]->SetBinError(iy+1, bErr);
         hBMtd[ipt]->SetBinError(iy+1, bMtdErr);

         fwMB << sigMtd << ", ";
         fwoMB << sig << ", " ;
      }
      fwMB << std::endl;
      fwoMB << std::endl;

      double sigint = sint / sqrt(sint + bint);
      double sigMtdint = sMtdint / sqrt(sMtdint + bMtdint);

      hSigVsPt->SetBinContent(ipt+1, sigint);
      hSigMtdVsPt->SetBinContent(ipt+1, sigMtdint);
      hSVsPt->SetBinContent(ipt+1, sint);
      hSMtdVsPt->SetBinContent(ipt+1, sMtdint);
      hBVsPt->SetBinContent(ipt+1, bint);
      hBMtdVsPt->SetBinContent(ipt+1, bMtdint);
/*
      TCanvas* c1 = new TCanvas(Form("c1_%d", ipt), "", 600, 500);
      gStyle->SetOptStat(0);
      hSigMtd[ipt]->GetYaxis()->SetTitle("Significance");
      hSigMtd[ipt]->GetXaxis()->SetTitle("y");
      double max = hSigMtd[ipt]->GetMaximum();
      hSigMtd[ipt]->GetYaxis()->SetRangeUser(0, max*1.3);
      hSigMtd[ipt]->SetLineColor(kRed);
      hSigMtd[ipt]->Draw();
      hSig[ipt]->Draw("same");
      TLegend* lgd = new TLegend(0.7, 0.8, 0.90, 0.90);
      lgd->AddEntry(hSigMtd[ipt], "w/ mtd", "lp");
      lgd->AddEntry(hSig[ipt], "w/o mtd", "lp");
      lgd->Draw();
      ltx->SetTextSize(0.05);
      ltx->DrawLatexNDC(0.1, 0.93, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
      ltx->DrawLatexNDC(0.35, 0.75, "MB 25B events");
      ltx->SetTextSize(0.035);
      ltx->DrawLatexNDC(0.6, 0.3, Form("%.1f < p_{T} < %.1f GeV", ana::ptbin[ipt], ana::ptbin[ipt+1]));
      c1->SaveAs(Form("LamCSigMB/sig_pT%.1f_%.1f.png", ana::ptbin[ipt], ana::ptbin[ipt+1]));

      TCanvas* c2 = new TCanvas(Form("c2_%d", ipt), "", 600, 500);
      c2->SetLeftMargin(0.15);
      gStyle->SetOptStat(0);
      hSMtd[ipt]->SetLineColor(kRed);
      hS[ipt]->GetYaxis()->SetTitle("S");
      hS[ipt]->GetXaxis()->SetTitle("y");
      double maxS = hS[ipt]->GetMaximum();
      hS[ipt]->GetYaxis()->SetRangeUser(0, maxS*1.3);
      hS[ipt]->Draw("e");
      hSMtd[ipt]->Draw("esame");
      TLegend* lgds = new TLegend(0.7, 0.8, 0.90, 0.90);
      lgds->AddEntry(hSMtd[ipt], "w/ mtd", "lp");
      lgds->AddEntry(hS[ipt], "w/o mtd", "lp");
      lgds->Draw();
      ltx->SetTextSize(0.05);
      ltx->DrawLatexNDC(0.35, 0.7, "MB 25B events");
      ltx->DrawLatexNDC(0.1, 0.95, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
      ltx->SetTextSize(0.035);
      ltx->DrawLatexNDC(0.6, 0.3, Form("%.1f < p_{T} < %.1f GeV", ana::ptbin[ipt], ana::ptbin[ipt+1]));
      c2->SaveAs(Form("LamCSigMB/scaled_s_pT%.1f_%.1f.png", ana::ptbin[ipt], ana::ptbin[ipt+1]));

      TCanvas* c3 = new TCanvas(Form("c3_%d", ipt), "", 600, 500);
      c3->SetLeftMargin(0.15);
      TGaxis::SetMaxDigits(4);
      gStyle->SetOptStat(0);
      hB[ipt]->GetYaxis()->SetTitle("B");
      hB[ipt]->GetXaxis()->SetTitle("y");
      hBMtd[ipt]->SetLineColor(kRed);
      double maxB = hB[ipt]->GetMaximum();
      hB[ipt]->GetYaxis()->SetRangeUser(0, maxB*1.3);
      hB[ipt]->Draw("e");
      hBMtd[ipt]->Draw("esame");
      TLegend* lgdb = new TLegend(0.7, 0.8, 0.90, 0.90);
      lgdb->AddEntry(hBMtd[ipt], "w/ mtd", "lp");
      lgdb->AddEntry(hB[ipt], "w/o mtd", "lp");
      lgdb->Draw();
      ltx->SetTextSize(0.05);
      ltx->DrawLatexNDC(0.35, 0.7, "MB 25B events");
      ltx->DrawLatexNDC(0.1, 0.95, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
      ltx->SetTextSize(0.035);
      ltx->DrawLatexNDC(0.6, 0.3, Form("%.1f < p_{T} < %.1f GeV", ana::ptbin[ipt], ana::ptbin[ipt+1]));
      c3->SaveAs(Form("LamCSigMB/scaled_b_pT%.1f_%.1f.png", ana::ptbin[ipt], ana::ptbin[ipt+1]));
      */
   }

   hSigMtdVsPt->SetBinContent(1, 0);
   hSigMtdVsPt->SetBinContent(2, 0);
   hSigMtdVsPt->SetBinContent(3, 0);

   hSigVsPt->SetBinContent(1, 0);
   hSigVsPt->SetBinContent(2, 0);
   hSigVsPt->SetBinContent(3, 0);

   TCanvas* c1 = new TCanvas("c1", "", 600, 500);
   c1->SetLeftMargin(0.16);
   TGaxis::SetMaxDigits(3);
   gStyle->SetOptStat(0);
   hSigMtdVsPt->GetYaxis()->SetTitle("Significance");
   hSigMtdVsPt->GetXaxis()->SetTitle("pT (GeV)");
   double max = hSigMtdVsPt->GetMaximum();
   hSigMtdVsPt->GetYaxis()->SetRangeUser(0, 50);
   hSigMtdVsPt->SetLineColor(kRed);
   hSigMtdVsPt->Draw();
   hSigVsPt->Draw("same");
   TLegend* lgd = new TLegend(0.7, 0.8, 0.90, 0.90);
   lgd->AddEntry(hSigMtdVsPt, "w/ mtd", "lp");
   lgd->AddEntry(hSigVsPt, "w/o mtd", "lp");
   lgd->Draw();
   ltx->SetTextSize(0.05);
   ltx->DrawLatexNDC(0.1, 0.93, "L^{PbPb}_{int} = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.35, 0.75, "MB 25B events");
   ltx->SetTextSize(0.035);
   ltx->DrawLatexNDC(0.6, 0.3, "|y|<3");
   c1->SaveAs("LamCSigMB.png");

   TCanvas* c2 = new TCanvas("c2", "", 600, 500);
   c2->SetLeftMargin(0.16);
   gStyle->SetOptStat(0);
   hSVsPt->GetYaxis()->SetTitle("S");
   hSVsPt->GetXaxis()->SetTitle("pT (GeV)");
   double maxs = hSVsPt->GetMaximum();
   hSVsPt->GetYaxis()->SetRangeUser(0, maxs*1.3);
   hSMtdVsPt->SetLineColor(kRed);
   hSVsPt->Draw();
   hSMtdVsPt->Draw("same");
   TLegend* lgds = new TLegend(0.7, 0.8, 0.90, 0.90);
   lgds->AddEntry(hSMtdVsPt, "w/ mtd", "lp");
   lgds->AddEntry(hSVsPt, "w/o mtd", "lp");
   lgds->Draw();
   ltx->SetTextSize(0.05);
   ltx->DrawLatexNDC(0.1, 0.93, "L^{PbPb}_{int} = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.35, 0.75, "MB 25B events");
   ltx->SetTextSize(0.035);
   ltx->DrawLatexNDC(0.6, 0.3, "|y|<3");
   c2->SaveAs("LamCSMB.png");

   TCanvas* c3 = new TCanvas("c3", "", 600, 500);
   c3->SetLeftMargin(0.16);
   gStyle->SetOptStat(0);
   hBVsPt->GetYaxis()->SetTitle("B");
   hBVsPt->GetXaxis()->SetTitle("pT (GeV)");
   double maxb = hBVsPt->GetMaximum();
   hBVsPt->GetYaxis()->SetRangeUser(0, maxb*1.3);
   hBMtdVsPt->SetLineColor(kRed);
   hBVsPt->Draw();
   hBMtdVsPt->Draw("same");
   TLegend* lgdb = new TLegend(0.7, 0.8, 0.90, 0.90);
   lgdb->AddEntry(hBMtdVsPt, "w/ mtd", "lp");
   lgdb->AddEntry(hBVsPt, "w/o mtd", "lp");
   lgdb->Draw();
   ltx->SetTextSize(0.05);
   ltx->DrawLatexNDC(0.1, 0.93, "L^{PbPb}_{int} = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.35, 0.75, "MB 25B events");
   ltx->SetTextSize(0.035);
   ltx->DrawLatexNDC(0.6, 0.3, "|y|<3");
   c3->SaveAs("LamCBMB.png");

   std::cout << "w/o mtd" << std::endl;
   for(int ipt=0; ipt<ana::nuOfPt; ipt++){
      std::cout << hSigVsPt->GetBinContent(ipt+1) << std::endl;
   }
   std::cout << "w/ mtd" << std::endl;
   for(int ipt=0; ipt<ana::nuOfPt; ipt++){
      std::cout << hSigMtdVsPt->GetBinContent(ipt+1) << std::endl;;
   }
}
