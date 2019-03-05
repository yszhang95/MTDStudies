#include "myAna.h"
void calSig()
{
   TFile* f1 = new TFile("genPt.root");
   TFile* f2 = new TFile("HEPData-ins1616207-v1.root");
   TFile* f3 = new TFile("promptd0MassHists.root");
   TFile* f4 = new TFile("hyjetsMassHists.root");

   TH1F* hGenPt[ana::nuOfY];
   TH1F* hRecoPt[ana::nuOfY];
   TH1F* hRecoPtMtd[ana::nuOfY];
   TH1F* hSignalMass[ana::nuOfY];
   TH1F* hSignalMassMtd[ana::nuOfY];
   TH1F* hBkgMass[ana::nuOfY];
   TH1F* hBkgMassMtd[ana::nuOfY];
   TH1F* hGenPtMidY;
   TH1F* hData;

   for(int iy=0; iy<ana::nuOfY; iy++){
      f1->GetObject(Form("hPt%d", iy), hGenPt[iy]);
      f3->GetObject(Form("hPt%d", iy), hRecoPt[iy]);
      f3->GetObject(Form("hPtMtd%d", iy), hRecoPtMtd[iy]);
      f3->GetObject(Form("hMass%d", iy), hSignalMass[iy]);
      f3->GetObject(Form("hMassMtd%d", iy), hSignalMassMtd[iy]);
      f4->GetObject(Form("hMass%d", iy), hBkgMass[iy]);
      f4->GetObject(Form("hMassMtd%d", iy), hBkgMassMtd[iy]);
   }
   f2->GetObject("Table 3/Hist1D_y1", hData);

   hGenPtMidY = (TH1F*) hGenPt[4]->Clone();
   hGenPtMidY->Add(hGenPt[5]);
   hGenPtMidY->Add(hGenPt[6]);
   hGenPtMidY->Add(hGenPt[7]);

   // try to scale gen spectrum
   double yield_mc_Pt2_3GeV = hGenPtMidY->Integral(20, 30);
   double yield_data_Pt2_3GeV = hData->GetBinContent(1) * ana::TAA0_100 * ana::pbOvermb * 1 *ana::GeV * ana::evts_data_MB * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   double scale_factor = yield_data_Pt2_3GeV / yield_mc_Pt2_3GeV;
   std::cout << scale_factor << std::endl;


   TH1F* hObsMtd[ana::nuOfY];
   TH1F* hSigMtd = new TH1F("hSigMtd", "", ana::nuOfY, ana::ybin);
   TH1F* hSig = new TH1F("hSig", "", ana::nuOfY, ana::ybin);

   TH1F* hSMtd = new TH1F("hSMtd", "", ana::nuOfY, ana::ybin);
   TH1F* hS = new TH1F("hS", "", ana::nuOfY, ana::ybin);

   TH1F* hBMtd = new TH1F("hBMtd", "", ana::nuOfY, ana::ybin);
   TH1F* hB = new TH1F("hB", "", ana::nuOfY, ana::ybin);

   for(int iy=0; iy<ana::nuOfY; iy++){
      hGenPt[iy]->Scale(scale_factor * ana::evts_sim_MB / ana::evts_data_MB);
      hSignalMassMtd[iy]->Scale(scale_factor * ana::evts_sim_MB / ana::evts_data_MB);
      hBkgMassMtd[iy]->Scale(ana::evts_sim_MB / ana::evts_bkg_MB);
      hObsMtd[iy] = (TH1F*) hSignalMassMtd[iy]->Clone();
      hObsMtd[iy]->Add(hBkgMassMtd[iy]);

      int masslw = hSignalMassMtd[iy]->FindBin(ana::mass_lw[iy]);
      int massup = hSignalMassMtd[iy]->FindBin(ana::mass_up[iy]);

      float sMtd = hSignalMassMtd[iy]->Integral(masslw, massup);
      float bMtd = hBkgMassMtd[iy]->Integral(masslw, massup);
      float sigMtd = sMtd/sqrt(sMtd+bMtd);

      hSignalMass[iy]->Scale(scale_factor * ana::evts_sim_MB / ana::evts_data_MB);
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

   TLatex* ltx = new TLatex();
   TCanvas* c[ana::nuOfY];
   for(int i=0; i<ana::nuOfY; i++){
      c[i] = new TCanvas(Form("c_%d", i), "", 450, 500);
      gStyle->SetOptStat(0);
      float max = hObsMtd[i]->GetMaximum();
      hObsMtd[i]->GetYaxis()->SetRangeUser(max*0.5, max*1.3);
      hObsMtd[i]->Draw();
      ltx->DrawLatexNDC(0.5, 0.6, Form("%.1f < y < %.1f", ana::ybin[i], ana::ybin[i+1]));
   }


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
}
