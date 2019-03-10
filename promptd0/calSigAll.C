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
   double yield_mc_Pt2_3GeV = hGenPtMidY->Integral(20, 30);
   double yield_data_Pt2_3GeV_perEvt = hData->GetBinContent(1) * ana::TAA0_100 * ana::pbOvermb * 1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   double scale_factor_perEvt = yield_data_Pt2_3GeV_perEvt / yield_mc_Pt2_3GeV;

   TH1F* hSig[ana::nuOfPt];
   TH1F* hSigMtd[ana::nuOfPt];

   //for(int ipt=0; ipt<ana::nuOfPt; ipt++){
   for(int ipt=0; ipt<2; ipt++){

      TLatex* ltx = new TLatex();

      hSig[ipt] = new TH1F(Form("hSig%d", ipt), "", ana::nuOfY, ana::ybin);
      hSigMtd[ipt] = new TH1F(Form("hSigMtd%d", ipt), "", ana::nuOfY, ana::ybin);

      float pTCutMin = ana::ptbin[ipt];
      float pTCutMax = ana::ptbin[ipt+1];

      for(int iy=0; iy<ana::nuOfY; iy++){

         float yCutMin = ana::ybin[iy];
         float yCutMax = ana::ybin[iy+1];

         float massCutMin = ana::mass_lw[iy];
         float massCutMax = ana::mass_up[iy];

         double sig, s, b, sErr, bErr;
         double sigMtd, sMtd, bMtd, sMtdErr, bMtdErr;

         calSig(hSignal, hBkg, scale_factor_perEvt,
               yCutMin, yCutMax, pTCutMin, pTCutMax, massCutMin, massCutMax, 
               sig, s, b, sErr, bErr);
         calSig(hSignalMtd, hBkgMtd, scale_factor_perEvt,
               yCutMin, yCutMax, pTCutMin, pTCutMax, massCutMin, massCutMax, 
               sigMtd, sMtd, bMtd, sMtdErr, bMtdErr);

         hSig[ipt]->SetBinContent(iy+1, sig);
         hSigMtd[ipt]->SetBinContent(iy+1, sigMtd);
         std::cout << hSig[ipt]->GetBinContent(iy+1) << std::endl;
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
   }


/*
   TCanvas* c1 = new TCanvas("c1", "", 600, 500);
   gStyle->SetOptStat(0);
   hSigMtd->GetYaxis()->SetTitle("Significance");
   hSigMtd->GetXaxis()->SetTitle("y");
   float max = hSigMtd->GetMaximum();
   hSigMtd->GetYaxis()->SetRangeUser(0, max*1.3);
   hSigMtd->SetLineColor(kRed);
   hSigMtd->Draw();
   hSig->Draw("same");
   TLegend* lgd = new TLegend(0.7, 0.8, 0.90, 0.90);
   lgd->AddEntry(hSigMtd, "w/ mtd", "lp");
   lgd->AddEntry(hSig, "w/o mtd", "lp");
   lgd->Draw();
   ltx->SetTextSize(0.05);
   ltx->DrawLatexNDC(0.1, 0.93, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.35, 0.75, "MB 25B events");

   TCanvas* c2 = new TCanvas("c2", "", 600, 500);
   c2->SetLeftMargin(0.15);
   gStyle->SetOptStat(0);
   hSMtd->SetLineColor(kRed);
   hS->GetYaxis()->SetTitle("S");
   hS->GetXaxis()->SetTitle("y");
   float maxS = hS->GetMaximum();
   hS->GetYaxis()->SetRangeUser(0, maxS*1.3);
   hS->Draw("e");
   hSMtd->Draw("esame");
   TLegend* lgds = new TLegend(0.7, 0.8, 0.90, 0.90);
   lgds->AddEntry(hSMtd, "w/ mtd", "lp");
   lgds->AddEntry(hS, "w/o mtd", "lp");
   lgds->Draw();
   ltx->DrawLatexNDC(0.35, 0.7, "MB 25B events");
   ltx->DrawLatexNDC(0.1, 0.95, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");

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
}
