#include "myAna.h"

void calSig(TH3*, TH3*, const float&, 
      const float&, const float&, const float&, const float&, 
      const float&, const float&, 
      double&, double&, double&, double&, double&);

void calSigAll()
{
   TFile* f1 = new TFile("genPt_reRECO.root");
   TFile* f2 = new TFile("HEPData-ins1616207-v1.root");
   TFile* f3 = new TFile("promptd0MassHists_reRECO_all.root");
   TFile* f4 = new TFile("hyjetsMassHists_reRECO_all.root");

   TH1F* hGenPt[ana::nuOfY];
   TH1F* hGenPtMidY;
   TH1F* hData;
   TH3F* hSignal;
   TH3F* hBkg;
   TH3F* hSignalMtd;
   TH3F* hBkgMtd;

   for(int iy=0; iy<ana::nuOfY; iy++){
      f1->GetObject(Form("hPt%d", iy), hGenPt[iy]);
   }
   f2->GetObject("Table 3/Hist1D_y1", hData);
   f3->GetObject("hMassVsPtVsY", hSignal);
   f4->GetObject("hMassVsPtVsY", hBkg);
   f3->GetObject("hMassVsPtVsYMtd", hSignalMtd);
   f4->GetObject("hMassVsPtVsYMtd", hBkgMtd);

   hGenPtMidY = (TH1F*) hGenPt[4]->Clone();
   hGenPtMidY->Add(hGenPt[5]);
   hGenPtMidY->Add(hGenPt[6]);
   hGenPtMidY->Add(hGenPt[7]);

   // try to scale gen spectrum
   double scale_factor_perEvt[ana::nuOfPt];
   double yield_mc_Pt2_3GeV = hGenPtMidY->Integral(21, 30);
   double yield_data_Pt2_3GeV_perEvt = hData->GetBinContent(1) * ana::TAA0_100 * ana::pbOvermb * 1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt[0] = yield_data_Pt2_3GeV_perEvt / yield_mc_Pt2_3GeV; // 0 - 0.5 GeV
   scale_factor_perEvt[1] = scale_factor_perEvt[0]; // 0.5 - 1.0 GeV
   scale_factor_perEvt[2] = scale_factor_perEvt[0]; // 1.0 - 1.5 GeV
   scale_factor_perEvt[3] = scale_factor_perEvt[0]; // 1.5 - 2.0 GeV
   scale_factor_perEvt[4] = scale_factor_perEvt[0]; // 2.0 - 3.0 GeV

   double yield_data_Pt3_4GeV_perEvt = hData->GetBinContent(2) * ana::TAA0_100 * ana::pbOvermb * 1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt[5] = yield_data_Pt3_4GeV_perEvt / hGenPtMidY->Integral(31, 40); // 3.0 - 4.0 GeV

   double yield_data_Pt4_5GeV_perEvt = hData->GetBinContent(3) * ana::TAA0_100 * ana::pbOvermb * 1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt[6] = yield_data_Pt3_4GeV_perEvt / hGenPtMidY->Integral(41, 50); // 4.0 - 5.0 GeV

   double yield_data_Pt5_6GeV_perEvt = hData->GetBinContent(4) * ana::TAA0_100 * ana::pbOvermb * 1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt[7] = yield_data_Pt5_6GeV_perEvt / hGenPtMidY->Integral(51, 60); // 5.0 - 6.0 GeV

   double yield_data_Pt6_8GeV_perEvt = hData->GetBinContent(5) * ana::TAA0_100 * ana::pbOvermb * 1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt[8] = yield_data_Pt6_8GeV_perEvt / hGenPtMidY->Integral(61, 80); // 6.0 - 8.0 GeV

   double yield_data_Pt8_10GeV_perEvt = hData->GetBinContent(6) * ana::TAA0_100 * ana::pbOvermb * 1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt[9] = yield_data_Pt8_10GeV_perEvt / hGenPtMidY->Integral(81, 100); // 8.0 - 10.0 GeV

   TH1F* hSig[ana::nuOfPt];
   TH1F* hSigMtd[ana::nuOfPt];

   TH1F* hS[ana::nuOfPt];
   TH1F* hSMtd[ana::nuOfPt];

   TH1F* hB[ana::nuOfPt];
   TH1F* hBMtd[ana::nuOfPt];

   for(int ipt=0; ipt<ana::nuOfPt; ipt++){
   //for(int ipt=0; ipt<2; ipt++){

      TLatex* ltx = new TLatex();

      hSig[ipt] = new TH1F(Form("hSig%d", ipt), "", ana::nuOfY, ana::ybin);
      hSigMtd[ipt] = new TH1F(Form("hSigMtd%d", ipt), "", ana::nuOfY, ana::ybin);

      hS[ipt] = new TH1F(Form("hS%d", ipt), "", ana::nuOfY, ana::ybin);
      hSMtd[ipt] = new TH1F(Form("hSMtd%d", ipt), "", ana::nuOfY, ana::ybin);

      hB[ipt] = new TH1F(Form("hB%d", ipt), "", ana::nuOfY, ana::ybin);
      hBMtd[ipt] = new TH1F(Form("hBMtd%d", ipt), "", ana::nuOfY, ana::ybin);

      float pTCutMin = ana::ptbin[ipt];
      float pTCutMax = ana::ptbin[ipt+1];

      for(int iy=0; iy<ana::nuOfY; iy++){

         float yCutMin = ana::ybin[iy];
         float yCutMax = ana::ybin[iy+1];

         float massCutMin = ana::mass_lw[iy];
         float massCutMax = ana::mass_up[iy];

         double sig, s, b, sErr, bErr;
         double sigMtd, sMtd, bMtd, sMtdErr, bMtdErr;

         calSig(hSignal, hBkg, scale_factor_perEvt[ipt],
               yCutMin, yCutMax, pTCutMin, pTCutMax, massCutMin, massCutMax, 
               sig, s, b, sErr, bErr);
         calSig(hSignalMtd, hBkgMtd, scale_factor_perEvt[ipt],
               yCutMin, yCutMax, pTCutMin, pTCutMax, massCutMin, massCutMax, 
               sigMtd, sMtd, bMtd, sMtdErr, bMtdErr);

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
      }

      TCanvas* c1 = new TCanvas(Form("c1_%d", ipt), "", 600, 500);
      gStyle->SetOptStat(0);
      hSigMtd[ipt]->GetYaxis()->SetTitle("Significance");
      hSigMtd[ipt]->GetXaxis()->SetTitle("y");
      float max = hSigMtd[ipt]->GetMaximum();
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
      c1->SaveAs(Form("plot0309/sig_pT%.1f_%.1f.png", ana::ptbin[ipt], ana::ptbin[ipt+1]));

      TCanvas* c2 = new TCanvas(Form("c2_%d", ipt), "", 600, 500);
      c2->SetLeftMargin(0.15);
      gStyle->SetOptStat(0);
      hSMtd[ipt]->SetLineColor(kRed);
      hS[ipt]->GetYaxis()->SetTitle("S");
      hS[ipt]->GetXaxis()->SetTitle("y");
      float maxS = hS[ipt]->GetMaximum();
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
      c2->SaveAs(Form("plot0309/scaled_s_pT%.1f_%.1f.png", ana::ptbin[ipt], ana::ptbin[ipt+1]));

      TCanvas* c3 = new TCanvas(Form("c3_%d", ipt), "", 600, 500);
      c3->SetLeftMargin(0.15);
      TGaxis::SetMaxDigits(4);
      gStyle->SetOptStat(0);
      hB[ipt]->GetYaxis()->SetTitle("B");
      hB[ipt]->GetXaxis()->SetTitle("y");
      hBMtd[ipt]->SetLineColor(kRed);
      float maxB = hB[ipt]->GetMaximum();
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
      c3->SaveAs(Form("plot0309/scaled_b_pT%.1f_%.1f.png", ana::ptbin[ipt], ana::ptbin[ipt+1]));
   }


/*

   TCanvas* c3 = new TCanvas("c3", "", 600, 500);
   c3->SetLeftMargin(0.15);
   TGaxis::SetMaxDigits(4);
   gStyle->SetOptStat(0);
   hB->GetYaxis()->SetTitle("B");
   hB->GetXaxis()->SetTitle("y");
   hBMtd->SetLineColor(kRed);
   float maxB = hB->GetMaximum();
   hB->GetYaxis()->SetRangeUser(0, maxB*1.3);
   hB->Draw("e");
   hBMtd->Draw("esame");
   TLegend* lgdb = new TLegend(0.7, 0.8, 0.90, 0.90);
   lgdb->AddEntry(hBMtd, "w/ mtd", "lp");
   lgdb->AddEntry(hB, "w/o mtd", "lp");
   lgdb->Draw();
   ltx->DrawLatexNDC(0.35, 0.7, "MB 25B events");
   ltx->DrawLatexNDC(0.1, 0.95, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
   */
}

void calSig(TH3* hSignal, TH3* hBkg, const float& scale_factor_perEvt,
      const float& yCutMin, const float& yCutMax, 
      const float& pTCutMin, const float& pTCutMax, const float& zCutMin, const float& zCutMax, 
      double& sig, double& s, double& b, double& sErr, double& bErr)
{
   int ylw = 0;
   int yup = 0;
   int ptlw = 0;
   int ptup = 0;
   int zlw = 0;
   int zup = 0;

   int globalMin = hSignal->FindBin(yCutMin, pTCutMin, zCutMin);
   int globalMax = hSignal->FindBin(yCutMax, pTCutMax, zCutMax);

   hSignal->GetBinXYZ(globalMin, ylw, ptlw, zlw);
   hSignal->GetBinXYZ(globalMax, yup, ptup, zup);

   s = hSignal->IntegralAndError(ylw, yup-1, ptlw, ptup-1, zlw, zup, sErr); // ptup-1 and yup-1 due to we already know it would be at bin low edge,
   b = hBkg->IntegralAndError(ylw, yup-1, ptlw, ptup-1, zlw, zup, bErr);

   s *= scale_factor_perEvt * ana::evts_sim_MB;
   b *= ana::evts_sim_MB / ana::evts_bkg_MB;
   sErr *= scale_factor_perEvt * ana::evts_sim_MB;
   bErr *= ana::evts_sim_MB / ana::evts_bkg_MB;

   sig = s/sqrt(s+b);
   if(hBkg->Integral(ylw, yup-1, ptlw, ptup-1, zlw, zup) < 60) sig = 0;
}
