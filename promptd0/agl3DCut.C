#include "myAna.h"

void calSig(TH3*, TH3*, 
      const double&, const double&, const double&, const double&, 
      const double&, const double&, 
      double&, double&, double&, double&, double&);
int whichPt(const double& pt){
   for(int ipt=0; ipt<ana::nuOfPt; ipt++){
      if( pt < ana::ptbin[ipt+1] && pt > ana::ptbin[ipt]) return ipt;
   }
   return -1;
}

void agl3DCut()
{
   TFile* f1 = new TFile("genPt_reRECO.root");
   TFile* f2 = new TFile("HEPData-ins1616207-v1.root");
   TFile* f3 = new TFile("PromptDMassHists_reRECO_all.root");
   TFile* f4 = new TFile("hyjetsMassHists_reRECO_all.root");

   TH1F* hGenPt[ana::nuOfY];
   TH1D* hGenPtMidY;
   TH1F* hData;
   TH3F* hSignal;
   TH3F* hBkg;
   TH3F* hSignalMtd;
   TH3F* hBkgMtd;

   for(int iy=0; iy<ana::nuOfY; iy++){
      f1->GetObject(Form("hPt%d", iy), hGenPt[iy]);
   }

   f2->GetObject("Table 3/Hist1D_y1", hData);
   f3->GetObject("hagl3DVsPtVsY", hSignal);
   f4->GetObject("hagl3DVsPtVsY", hBkg);
   f3->GetObject("hagl3DVsPtVsYMtd", hSignalMtd);
   f4->GetObject("hagl3DVsPtVsYMtd", hBkgMtd);


   hGenPtMidY = (TH1D*) hGenPt[4]->Clone();
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

   TH3F* hScale = new TH3F("hScale", "", ana::ny, ana::yMin, ana::yMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);
   for(int ipt=0; ipt<ana::npt; ipt++){
      double pt = ((double)ipt + 0.5) * (ana::ptMax - ana::ptMin) / (double) ana::npt;
      int iscale = whichPt(pt);
      for(int iy=0; iy<ana::ny; iy++){
         for(int iz=0; iz<ana::nagl3D; iz++){
            if(iscale == -1) continue;
            hScale->SetBinContent(iy+1, ipt+1, iz+1, scale_factor_perEvt[iscale]);
            hScale->SetBinError(iy+1, ipt+1, iz+1, 0);
         }
      if( iscale!=-1 )hScale->SetBinContent(iy+1, ipt+1, 0, scale_factor_perEvt[iscale]);
      if( iscale!=-1 )hScale->SetBinContent(iy+1, ipt+1, ana::nVtxProb+1, scale_factor_perEvt[iscale]);
      }
   }

   TH3F *hSignalScale = new TH3F();
   *hSignalScale =  (*hSignal) * (*hScale);

   TH3F *hSignalMtdScale = new TH3F();
   *hSignalMtdScale = (*hSignalMtd) * (*hScale);

   TH1D* hSig[ana::nuOfPt][ana::nuOfY];
   TH1D* hSigMtd[ana::nuOfPt][ana::nuOfY];

   TH1D* hS[ana::nuOfPt][ana::nuOfY];
   TH1D* hSMtd[ana::nuOfPt][ana::nuOfY];

   TH1D* hB[ana::nuOfPt][ana::nuOfY];
   TH1D* hBMtd[ana::nuOfPt][ana::nuOfY];

   //for(int ipt=0; ipt<ana::nuOfPt; ipt++){
   for(int ipt=8; ipt<9; ipt++){

      TLatex* ltx = new TLatex();

      double pTCutMin = ana::ptbin[ipt];
      double pTCutMax = ana::ptbin[ipt+1];

      //for(int iy=0; iy<ana::nuOfY; iy++){
      for(int iy=5; iy<6; iy++){

         hSig[ipt][iy] = new TH1D(Form("hSigPt%dY%d", ipt, iy), "", ana::nagl3D, ana::agl3DMin, ana::agl3DMax);
         hSigMtd[ipt][iy] = new TH1D(Form("hSigMtdPt%dY%d", ipt, iy), "", ana::nagl3D, ana::agl3DMin, ana::agl3DMax);

         hS[ipt][iy] = new TH1D(Form("hSPt%dY%d", ipt, iy), "", ana::nagl3D, ana::agl3DMin, ana::agl3DMax);
         hSMtd[ipt][iy] = new TH1D(Form("hSMtdPt%dY%d", ipt, iy), "", ana::nagl3D, ana::agl3DMin, ana::agl3DMax);

         hB[ipt][iy] = new TH1D(Form("hBPt%dY%d", ipt, iy), "", ana::nagl3D, ana::agl3DMin, ana::agl3DMax);
         hBMtd[ipt][iy] = new TH1D(Form("hBMtdPt%dY%d", ipt, iy), "", ana::nagl3D, ana::agl3DMin, ana::agl3DMax);

         for(int iagl3D=0; iagl3D<ana::nagl3D; iagl3D++){
            double yCutMin = ana::ybin[iy];
            double yCutMax = ana::ybin[iy+1];

            double agl3DCutMin = ana::agl3DMin;
            double agl3DCutMax = (ana::agl3DMax - ana::agl3DMin) / (double) ana::nagl3D * iagl3D + ana::agl3DMin;

            double sig, s, b, sErr, bErr;
            double sigMtd, sMtd, bMtd, sMtdErr, bMtdErr;

            calSig(hSignalScale, hBkg, 
                  yCutMin, yCutMax, pTCutMin, pTCutMax, agl3DCutMin, agl3DCutMax, 
                  sig, s, b, sErr, bErr);
            calSig(hSignalMtdScale, hBkgMtd, 
                  yCutMin, yCutMax, pTCutMin, pTCutMax, agl3DCutMin, agl3DCutMax, 
                  sigMtd, sMtd, bMtd, sMtdErr, bMtdErr);

            hSig[ipt][iy]->SetBinContent(iagl3D+1, sig);
            hSigMtd[ipt][iy]->SetBinContent(iagl3D+1, sigMtd);

            hS[ipt][iy]->SetBinContent(iagl3D+1, s);
            hSMtd[ipt][iy]->SetBinContent(iagl3D+1, sMtd);
            hS[ipt][iy]->SetBinError(iagl3D+1, sErr);
            hSMtd[ipt][iy]->SetBinError(iagl3D+1, sMtdErr);

            hB[ipt][iy]->SetBinContent(iagl3D+1, b);
            hBMtd[ipt][iy]->SetBinContent(iagl3D+1, bMtd);
            hB[ipt][iy]->SetBinError(iagl3D+1, bErr);
            hBMtd[ipt][iy]->SetBinError(iagl3D+1, bMtdErr);
         }
         TCanvas* c1 = new TCanvas(Form("c1_%d%d", ipt, iy), "", 600, 500);
         gStyle->SetOptStat(0);
         hSigMtd[ipt][iy]->GetYaxis()->SetTitle("Significance");
         hSigMtd[ipt][iy]->GetXaxis()->SetTitle("3D Pointing Angle");
         double max = hSigMtd[ipt][iy]->GetMaximum();
         hSigMtd[ipt][iy]->GetYaxis()->SetRangeUser(0, max*1.3);
         hSigMtd[ipt][iy]->SetLineColor(kRed);
         hSigMtd[ipt][iy]->Draw();
         hSig[ipt][iy]->Draw("same");
         TLegend* lgd = new TLegend(0.7, 0.8, 0.90, 0.90);
         lgd->AddEntry(hSigMtd[ipt][iy], "w/ mtd", "lp");
         lgd->AddEntry(hSig[ipt][iy], "w/o mtd", "lp");
         lgd->Draw();
         ltx->SetTextSize(0.05);
         ltx->DrawLatexNDC(0.1, 0.93, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
         ltx->DrawLatexNDC(0.35, 0.75, "MB 25B events");
         ltx->SetTextSize(0.035);
         ltx->DrawLatexNDC(0.6, 0.3, Form("%.1f < p_{T} < %.1f GeV", ana::ptbin[ipt], ana::ptbin[ipt+1]));
         //c1->SaveAs(Form("plot0309/sig_pT%.1f_%.1f.png", ana::ptbin[ipt], ana::ptbin[ipt+1]));

         TCanvas* c2 = new TCanvas(Form("c2_%d%d", ipt, iy), "", 600, 500);
         c2->SetLeftMargin(0.15);
         gStyle->SetOptStat(0);
         hSMtd[ipt][iy]->SetLineColor(kRed);
         hS[ipt][iy]->GetYaxis()->SetTitle("S");
         hS[ipt][iy]->GetXaxis()->SetTitle("3D Poingting Angle");
         double maxS = hS[ipt][iy]->GetMaximum();
         hS[ipt][iy]->GetYaxis()->SetRangeUser(0, maxS*1.3);
         hS[ipt][iy]->Draw("e");
         hSMtd[ipt][iy]->Draw("esame");
         TLegend* lgds = new TLegend(0.7, 0.8, 0.90, 0.90);
         lgds->AddEntry(hSMtd[ipt][iy], "w/ mtd", "lp");
         lgds->AddEntry(hS[ipt][iy], "w/o mtd", "lp");
         lgds->Draw();
         ltx->SetTextSize(0.05);
         ltx->DrawLatexNDC(0.35, 0.7, "MB 25B events");
         ltx->DrawLatexNDC(0.1, 0.95, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
         ltx->SetTextSize(0.035);
         ltx->DrawLatexNDC(0.6, 0.3, Form("%.1f < p_{T} < %.1f GeV", ana::ptbin[ipt], ana::ptbin[ipt+1]));
         //c2->SaveAs(Form("plot0309/scaled_s_pT%.1f_%.1f.png", ana::ptbin[ipt], ana::ptbin[ipt+1]));

         TCanvas* c3 = new TCanvas(Form("c3_%d%d", ipt, iy), "", 600, 500);
         c3->SetLeftMargin(0.15);
         TGaxis::SetMaxDigits(4);
         gStyle->SetOptStat(0);
         hB[ipt][iy]->GetYaxis()->SetTitle("B");
         hB[ipt][iy]->GetXaxis()->SetTitle("3D Poingting Angle");
         hBMtd[ipt][iy]->SetLineColor(kRed);
         double maxB = hB[ipt][iy]->GetMaximum();
         hB[ipt][iy]->GetYaxis()->SetRangeUser(0, maxB*1.3);
         hB[ipt][iy]->Draw("e");
         hBMtd[ipt][iy]->Draw("esame");
         TLegend* lgdb = new TLegend(0.7, 0.8, 0.90, 0.90);
         lgdb->AddEntry(hBMtd[ipt][iy], "w/ mtd", "lp");
         lgdb->AddEntry(hB[ipt][iy], "w/o mtd", "lp");
         lgdb->Draw();
         ltx->SetTextSize(0.05);
         ltx->DrawLatexNDC(0.35, 0.7, "MB 25B events");
         ltx->DrawLatexNDC(0.1, 0.95, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
         ltx->SetTextSize(0.035);
         ltx->DrawLatexNDC(0.6, 0.3, Form("%.1f < p_{T} < %.1f GeV", ana::ptbin[ipt], ana::ptbin[ipt+1]));
         //c3->SaveAs(Form("plot0309/scaled_b_pT%.1f_%.1f.png", ana::ptbin[ipt], ana::ptbin[ipt+1]));
      }

   }
}

void calSig(TH3* hSignalScale, TH3* hBkg,
      const double& yCutMin, const double& yCutMax, 
      const double& pTCutMin, const double& pTCutMax, const double& zCutMin, const double& zCutMax, 
      double& sig, double& s, double& b, double& sErr, double& bErr)
{
   int ylw = 0;
   int yup = 0;
   int ptlw = 0;
   int ptup = 0;
   int zlw = 0;
   int zup = 0;

   int globalMin = hSignalScale->FindBin(yCutMin, pTCutMin, zCutMin);
   int globalMax = hSignalScale->FindBin(yCutMax, pTCutMax, zCutMax);

   hSignalScale->GetBinXYZ(globalMin, ylw, ptlw, zlw);
   hSignalScale->GetBinXYZ(globalMax, yup, ptup, zup);

   s = hSignalScale->IntegralAndError(ylw, yup-1, ptlw, ptup-1, zlw, zup, sErr); // ptup-1 and yup-1 due to we already know it would be at bin low edge,
   b = hBkg->IntegralAndError(ylw, yup-1, ptlw, ptup-1, zlw, zup, bErr);

   s *= ana::evts_sim_MB;
   b *= ana::evts_sim_MB / ana::evts_bkg_MB;
   sErr *= ana::evts_sim_MB;
   bErr *= ana::evts_sim_MB / ana::evts_bkg_MB;

   sig = s/sqrt(s+b);
   if(hBkg->Integral(ylw, yup-1, ptlw, ptup-1, zlw, zup) < 60) sig = 0;
}
