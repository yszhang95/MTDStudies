#include "myAna.h"
void calSigCent()
{
   TLatex* ltx = new TLatex();
   //TFile* f1 = new TFile("genPt.root");
   //TFile* f2 = new TFile("HEPData-ins1616207-v1.root");
   //TFile* f3 = new TFile("promptd0MassHists.root");
   //TFile* f4 = new TFile("hyjetsMassHists.root");
   TFile* f1 = new TFile("genPt_reRECO.root");
   TFile* f2 = new TFile("HEPData-ins1616207-v1.root");
   TFile* f3 = new TFile("promptd0MassHists_reRECO.root");
   TFile* f4 = new TFile("hyjetsMassHists_reRECO.root");

   TH1F* hGenPt[ana::nuOfY];
   TH1F* hRecoPt[ana::nuOfY];
   TH1F* hRecoPtMtd[ana::nuOfY];
   TH1F* hSignalMass[ana::nuOfY];
   TH1F* hSignalMassMtd[ana::nuOfY];
   TH1F* hBkgMass[ana::nuOfY];
   TH1F* hBkgMassMtd[ana::nuOfY];
   TH1F* hSignalMassCent[ana::nuOfY];
   TH1F* hSignalMassCentMtd[ana::nuOfY];
   TH1F* hBkgMassCent[ana::nuOfY];
   TH1F* hBkgMassCentMtd[ana::nuOfY];
   TH1F* hGenPtMidY;
   TH1F* hData;
   TH1F* hDataCent;

   for(int iy=0; iy<ana::nuOfY; iy++){
      f1->GetObject(Form("hPt%d", iy), hGenPt[iy]);

      f3->GetObject(Form("hPt%d", iy), hRecoPt[iy]);
      f3->GetObject(Form("hPtMtd%d", iy), hRecoPtMtd[iy]);

      f3->GetObject(Form("hMass%d", iy), hSignalMass[iy]);
      f3->GetObject(Form("hMassMtd%d", iy), hSignalMassMtd[iy]);

      //hSignalMassCent[iy] = new TH1F(*hSignalMass[iy]);
      //hSignalMassCentMtd[iy] = new TH1F(*hSignalMassMtd[iy]);
      hSignalMassCent[iy] = (TH1F*)hSignalMass[iy]->Clone();
      hSignalMassCentMtd[iy] = (TH1F*)hSignalMassMtd[iy]->Clone();

      f4->GetObject(Form("hMass%d", iy), hBkgMass[iy]);
      f4->GetObject(Form("hMassMtd%d", iy), hBkgMassMtd[iy]);

      f4->GetObject(Form("hMassCent%d", iy), hBkgMassCent[iy]);
      f4->GetObject(Form("hMassCentMtd%d", iy), hBkgMassCentMtd[iy]);
   }

   f2->GetObject("Table 3/Hist1D_y1", hData);
   f2->GetObject("Table 3/Hist1D_y2", hDataCent);

   hGenPtMidY = (TH1F*) hGenPt[4]->Clone();
   hGenPtMidY->Add(hGenPt[5]);
   hGenPtMidY->Add(hGenPt[6]);
   hGenPtMidY->Add(hGenPt[7]);

   // try to scale gen spectrum
   double yield_mc_Pt2_3GeV = hGenPtMidY->Integral(20, 30);

   //double yield_data_Pt2_3GeV = hData->GetBinContent(1) * ana::TAA0_100 * ana::pbOvermb * 1 *ana::GeV * ana::evts_data_MB * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   const double yield_data_Pt2_3GeV_perEvt = hData->GetBinContent(1) * ana::TAA0_100 * ana::pbOvermb * 1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2

   const double yield_data_Pt2_3GeV_perEvt_cent = hDataCent->GetBinContent(1) * ana::TAA0_10 * ana::pbOvermb * 1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2

   const double scale_factor_perEvt = yield_data_Pt2_3GeV_perEvt / yield_mc_Pt2_3GeV;
   const double scale_factor_perEvt_cent = yield_data_Pt2_3GeV_perEvt_cent / yield_mc_Pt2_3GeV;

   //std::cout << scale_factor << std::endl;
   std::cout << scale_factor_perEvt << std::endl;
   std::cout << scale_factor_perEvt_cent << std::endl;


   TH1F* hObsMtd[ana::nuOfY];
   TH1F* hSigMtd = new TH1F("hSigMtd", "", ana::nuOfY, ana::ybin);
   TH1F* hSig = new TH1F("hSig", "", ana::nuOfY, ana::ybin);

   TH1F* hSMtd = new TH1F("hSMtd", "", ana::nuOfY, ana::ybin);
   TH1F* hS = new TH1F("hS", "", ana::nuOfY, ana::ybin);

   TH1F* hBMtd = new TH1F("hBMtd", "", ana::nuOfY, ana::ybin);
   TH1F* hB = new TH1F("hB", "", ana::nuOfY, ana::ybin);

   TH1F* hSigCentMtd = new TH1F("hSigCentMtd", "", ana::nuOfY, ana::ybin);
   TH1F* hSigCent = new TH1F("hSigCent", "", ana::nuOfY, ana::ybin);

   TH1F* hSCentMtd = new TH1F("hSCentMtd", "", ana::nuOfY, ana::ybin);
   TH1F* hSCent = new TH1F("hSCent", "", ana::nuOfY, ana::ybin);

   TH1F* hBCentMtd = new TH1F("hBCentMtd", "", ana::nuOfY, ana::ybin);
   TH1F* hBCent = new TH1F("hBCent", "", ana::nuOfY, ana::ybin);
   /*

   for(int iy=0; iy<ana::nuOfY; iy++){
      //hGenPt[iy]->Scale(scale_factor * ana::evts_sim_MB / ana::evts_data_MB);
      hGenPt[iy]->Scale(scale_factor_perEvt * ana::evts_sim_MB);
      //hSignalMassMtd[iy]->Scale(scale_factor * ana::evts_sim_MB / ana::evts_data_MB);
      hSignalMassMtd[iy]->Scale(scale_factor_perEvt * ana::evts_sim_MB);
      hBkgMassMtd[iy]->Scale(ana::evts_sim_MB / ana::evts_bkg_MB);
      hObsMtd[iy] = (TH1F*) hSignalMassMtd[iy]->Clone();
      hObsMtd[iy]->Add(hBkgMassMtd[iy]);

      int masslw = hSignalMassMtd[iy]->FindBin(ana::mass_lw[iy]);
      int massup = hSignalMassMtd[iy]->FindBin(ana::mass_up[iy]);

      float sMtd = hSignalMassMtd[iy]->Integral(masslw, massup);
      float bMtd = hBkgMassMtd[iy]->Integral(masslw, massup);
      float sigMtd = sMtd/sqrt(sMtd+bMtd);

      //hSignalMass[iy]->Scale(scale_factor * ana::evts_sim_MB / ana::evts_data_MB);
      hSignalMass[iy]->Scale(scale_factor_perEvt * ana::evts_sim_MB);
      hBkgMass[iy]->Scale(ana::evts_sim_MB / ana::evts_bkg_MB);
      
      float s = hSignalMass[iy]->Integral(masslw, massup);
      float b = hBkgMass[iy]->Integral(masslw, massup);
      float sig = s/sqrt(s+b);

      hSigMtd->SetBinContent(iy+1, sigMtd);
      hSig->SetBinContent(iy+1, sig);

      hSMtd->SetBinContent(iy+1, sMtd);
      hS->SetBinContent(iy+1, s);

      hBMtd->SetBinContent(iy+1, bMtd);
      hB->SetBinContent(iy+1, b);
   }

   TCanvas* c[ana::nuOfY];
//   for(int i=0; i<ana::nuOfY; i++){
//      c[i] = new TCanvas(Form("c_%d", i), "", 450, 500);
//      gStyle->SetOptStat(0);
//      float max = hObsMtd[i]->GetMaximum();
//      hObsMtd[i]->GetYaxis()->SetRangeUser(max*0.5, max*1.3);
//      hObsMtd[i]->Draw();
//      ltx->DrawLatexNDC(0.5, 0.6, Form("%.1f < y < %.1f", ana::ybin[i], ana::ybin[i+1]));
//   }


   TCanvas* c1 = new TCanvas("c1", "", 450, 500);
   gStyle->SetOptStat(0);
   hSigMtd->GetYaxis()->SetTitle("Significance");
   hSigMtd->GetXaxis()->SetTitle("y");
   float max = hSigMtd->GetMaximum();
   hSigMtd->GetYaxis()->SetRangeUser(0, max*1.3);
   hSigMtd->SetLineColor(kRed);
   hSigMtd->Draw();
   hSig->Draw("same");
   TLegend* lgd = new TLegend(0.7, 0.8, 0.95, 0.95);
   lgd->AddEntry(hSigMtd, "w/ mtd", "lp");
   lgd->AddEntry(hSig, "w/o mtd", "lp");
   lgd->Draw();
   ltx->DrawLatexNDC(0.5, 0.7, "MB 25B events");

   TCanvas* c2 = new TCanvas("c2", "", 450, 500);
   gStyle->SetOptStat(0);
   hSMtd->SetLineColor(kRed);
   hS->GetYaxis()->SetTitle("S");
   hS->GetXaxis()->SetTitle("y");
   float maxS = hS->GetMaximum();
   hS->GetYaxis()->SetRangeUser(0, maxS*1.3);
   hS->Draw();
   hSMtd->Draw("same");
   TLegend* lgds = new TLegend(0.7, 0.8, 0.95, 0.95);
   lgds->AddEntry(hSMtd, "w/ mtd", "lp");
   lgds->AddEntry(hS, "w/o mtd", "lp");
   lgds->Draw();
   ltx->DrawLatexNDC(0.5, 0.7, "MB 25B events");

   TCanvas* c3 = new TCanvas("c3", "", 450, 500);
   gStyle->SetOptStat(0);
   hB->GetYaxis()->SetTitle("B");
   hB->GetXaxis()->SetTitle("y");
   hBMtd->SetLineColor(kRed);
   float maxB = hB->GetMaximum();
   hB->GetYaxis()->SetRangeUser(0, maxB*1.3);
   hB->Draw();
   hBMtd->Draw("same");
   TLegend* lgdb = new TLegend(0.7, 0.8, 0.95, 0.95);
   lgdb->AddEntry(hBMtd, "w/ mtd", "lp");
   lgdb->AddEntry(hB, "w/o mtd", "lp");
   lgdb->Draw();
   ltx->DrawLatexNDC(0.5, 0.7, "MB 25B events");
   */

   for(int iy=0; iy<ana::nuOfY; iy++){
      hSignalMassCentMtd[iy]->Scale(scale_factor_perEvt_cent * ana::evts_sim_central);
      hBkgMassCentMtd[iy]->Scale(ana::evts_sim_central / ana::evts_bkg_central);
      //std::cout << ana::evts_sim_central *scale_factor_perEvt_cent << std::endl;
      //std::cout << ana::evts_sim_MB * scale_factor_perEvt<< std::endl;

      int masslw = hSignalMassCentMtd[iy]->FindBin(ana::mass_lw[iy]);
      int massup = hSignalMassCentMtd[iy]->FindBin(ana::mass_up[iy]);
      
      double sMtdErr;
      double bMtdErr;
      float sMtd = hSignalMassCentMtd[iy]->IntegralAndError(masslw, massup, sMtdErr);
      float bMtd = hBkgMassCentMtd[iy]->IntegralAndError(masslw, massup, bMtdErr);
      float sigMtd = sMtd/sqrt(sMtd+bMtd);

      hSignalMassCent[iy]->Scale(scale_factor_perEvt_cent * ana::evts_sim_central);
      hBkgMassCent[iy]->Scale(ana::evts_sim_central / ana::evts_bkg_central);
      
      double sErr;
      double bErr;
      float s = hSignalMassCent[iy]->IntegralAndError(masslw, massup, sErr);
      float b = hBkgMassCent[iy]->IntegralAndError(masslw, massup, bErr);
      float sig = s/sqrt(s+b);

      hSigCentMtd->SetBinContent(iy+1, sigMtd);
      hSigCent->SetBinContent(iy+1, sig);

      hSCentMtd->SetBinContent(iy+1, sMtd);
      hSCentMtd->SetBinError(iy+1, sMtdErr);
      hSCent->SetBinContent(iy+1, s);
      hSCent->SetBinError(iy+1, sErr);

      hBCentMtd->SetBinContent(iy+1, bMtd);
      hBCentMtd->SetBinError(iy+1, bMtdErr);
      hBCent->SetBinContent(iy+1, b);
      hBCent->SetBinError(iy+1, bErr);
   }

   TCanvas* c4 = new TCanvas("c4", "", 600, 500);
   gStyle->SetOptStat(0);
   hSigCentMtd->GetYaxis()->SetTitle("Significance");
   hSigCentMtd->GetXaxis()->SetTitle("y");
   float maxCent = hSigCentMtd->GetMaximum();
   hSigCentMtd->GetYaxis()->SetRangeUser(0, maxCent*1.3);
   hSigCentMtd->SetLineColor(kRed);
   hSigCentMtd->Draw();
   hSigCent->Draw("same");
   TLegend* lgdCent = new TLegend(0.7, 0.8, 0.9, 0.90);
   lgdCent->AddEntry(hSigCentMtd, "w/ mtd", "lp");
   lgdCent->AddEntry(hSigCent, "w/o mtd", "lp");
   lgdCent->Draw();
   ltx->DrawLatexNDC(0.1, 0.93, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.35, 0.75, Form("0~10%% %1.0f B events", ana::evts_sim_central / 1e9));

   TCanvas* c5 = new TCanvas("c5", "", 600, 500);
   gStyle->SetOptStat(0);
   hSCentMtd->SetLineColor(kRed);
   hSCent->GetYaxis()->SetTitle("S");
   hSCent->GetXaxis()->SetTitle("y");
   float maxSCent = hSCent->GetMaximum();
   hSCent->GetYaxis()->SetRangeUser(0, maxSCent*1.3);
   hSCent->Draw();
   hSCentMtd->Draw("same");
   TLegend* lgdsCent = new TLegend(0.7, 0.8, 0.9, 0.9);
   lgdsCent->AddEntry(hSCentMtd, "w/ mtd", "lp");
   lgdsCent->AddEntry(hSCent, "w/o mtd", "lp");
   lgdsCent->Draw();
   ltx->DrawLatexNDC(0.1, 0.95, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.34, 0.7, Form("0~10%% %1.0f B events", ana::evts_sim_central / 1e9));

   TCanvas* c6 = new TCanvas("c6", "", 600, 500);
   TGaxis::SetMaxDigits(4);
   gStyle->SetOptStat(0);
   hBCent->GetYaxis()->SetTitle("B");
   hBCent->GetXaxis()->SetTitle("y");
   hBCentMtd->SetLineColor(kRed);
   float maxBCent = hBCent->GetMaximum();
   hBCent->GetYaxis()->SetRangeUser(0, maxBCent*1.3);
   hBCent->Draw();
   hBCentMtd->Draw("same");
   TLegend* lgdbCent = new TLegend(0.7, 0.8, 0.90, 0.90);
   lgdbCent->AddEntry(hBCentMtd, "w/ mtd", "lp");
   lgdbCent->AddEntry(hBCent, "w/o mtd", "lp");
   lgdbCent->Draw();
   ltx->DrawLatexNDC(0.1, 0.95, "Lumi = 3 nb^{-1}  Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.34, 0.7, Form("0~10%% %1.0f B events", ana::evts_sim_central / 1e9));
}
