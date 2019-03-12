#include "myAna.h"

void vtxProbCut()
{
   std::vector<std::vector<double>> vtxCut;

   TFile* f1 = new TFile("genPt_reRECO.root");
   TFile* f2 = new TFile("HEPData-ins1616207-v1.root");
   TFile* f3 = new TFile("lamC3PMassHists_reRECO_all.root");
   TFile* f4 = new TFile("hyjetsMassHists_reRECO_all.root");

   TH1D* hGenPtMidY;
   TH1F* hData;
   TH3D* hSignal;
   TH3D* hBkg;
   TH3D* hSignalMtd;
   TH3D* hBkgMtd;

   f1->GetObject("hGenPtMidY", hGenPtMidY);

   f2->GetObject("Table 3/Hist1D_y1", hData);
   f3->GetObject("hVtxProbVsPtVsY", hSignal);
   f4->GetObject("hVtxProbVsPtVsY", hBkg);
   f3->GetObject("hVtxProbVsPtVsYMtd", hSignalMtd);
   f4->GetObject("hVtxProbVsPtVsYMtd", hBkgMtd);

   // try to scale gen spectrum
   std::vector<double> scale_factor_perEvt(ana::nuOfPt);

   TH3D* hScale = new TH3D("hScale", "", ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

   calScalePerEvt(hGenPtMidY, hData, hScale, scale_factor_perEvt, ana::nVtxProb);

   TH3D *hSignalScale = new TH3D();
   *hSignalScale =  (*hSignal) * (*hScale);

   TH3D *hSignalMtdScale = new TH3D();
   *hSignalMtdScale = (*hSignalMtd) * (*hScale);

   TH1D* hSig[ana::nuOfPt][ana::nuOfY];
   TH1D* hSigMtd[ana::nuOfPt][ana::nuOfY];

   TH1D* hS[ana::nuOfPt][ana::nuOfY];
   TH1D* hSMtd[ana::nuOfPt][ana::nuOfY];

   TH1D* hB[ana::nuOfPt][ana::nuOfY];
   TH1D* hBMtd[ana::nuOfPt][ana::nuOfY];

   for(int ipt=0; ipt<ana::nuOfPt; ipt++){

      TLatex* ltx = new TLatex();

      double pTCutMin = ana::ptbin[ipt];
      double pTCutMax = ana::ptbin[ipt+1];

      std::vector<double> vtxCut_temp;

      for(int iy=0; iy<ana::nuOfY; iy++){

         hSig[ipt][iy] = new TH1D(Form("hSigPt%dY%d", ipt, iy), "", ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);
         hSigMtd[ipt][iy] = new TH1D(Form("hSigMtdPt%dY%d", ipt, iy), "", ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

         hS[ipt][iy] = new TH1D(Form("hSPt%dY%d", ipt, iy), "", ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);
         hSMtd[ipt][iy] = new TH1D(Form("hSMtdPt%dY%d", ipt, iy), "", ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

         hB[ipt][iy] = new TH1D(Form("hBPt%dY%d", ipt, iy), "", ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);
         hBMtd[ipt][iy] = new TH1D(Form("hBMtdPt%dY%d", ipt, iy), "", ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

         for(int iVtxProb=0; iVtxProb<ana::nVtxProb; iVtxProb++){
            double yCutMin = ana::ybin[iy];
            double yCutMax = ana::ybin[iy+1];

            double VtxProbCutMin = (ana::VtxProbMax - ana::VtxProbMin) / (double) ana::nVtxProb * iVtxProb + ana::VtxProbMin;
            double VtxProbCutMax = ana::VtxProbMax + 0.1;

            double sig, s, b, sErr, bErr;
            double sigMtd, sMtd, bMtd, sMtdErr, bMtdErr;

            calSig(hSignalScale, hBkg, 
                  yCutMin, yCutMax, pTCutMin, pTCutMax, VtxProbCutMin, VtxProbCutMax, 
                  sig, s, b, sErr, bErr);
            calSig(hSignalMtdScale, hBkgMtd, 
                  yCutMin, yCutMax, pTCutMin, pTCutMax, VtxProbCutMin, VtxProbCutMax, 
                  sigMtd, sMtd, bMtd, sMtdErr, bMtdErr);

            hSig[ipt][iy]->SetBinContent(iVtxProb+1, sig);
            hSigMtd[ipt][iy]->SetBinContent(iVtxProb+1, sigMtd);

            hS[ipt][iy]->SetBinContent(iVtxProb+1, s);
            hSMtd[ipt][iy]->SetBinContent(iVtxProb+1, sMtd);
            hS[ipt][iy]->SetBinError(iVtxProb+1, sErr);
            hSMtd[ipt][iy]->SetBinError(iVtxProb+1, sMtdErr);

            hB[ipt][iy]->SetBinContent(iVtxProb+1, b);
            hBMtd[ipt][iy]->SetBinContent(iVtxProb+1, bMtd);
            hB[ipt][iy]->SetBinError(iVtxProb+1, bErr);
            hBMtd[ipt][iy]->SetBinError(iVtxProb+1, bMtdErr);
         }
         double tmp = hSig[ipt][iy]->GetBinCenter(hSig[ipt][iy]->GetMaximumBin());
         vtxCut_temp.push_back(tmp);

         TCanvas* c1 = new TCanvas(Form("c1_%d%d", ipt, iy), "", 600, 500);
         gStyle->SetOptStat(0);
         hSigMtd[ipt][iy]->GetYaxis()->SetTitle("Significance");
         hSigMtd[ipt][iy]->GetXaxis()->SetTitle("vtxProb");
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
         ltx->DrawLatexNDC(0.6, 0.2, Form("%.1f < |y| < %.1f GeV", ana::ybin[iy], ana::ybin[iy+1]));
         c1->SaveAs(Form("optimizationLamC/vtxProb/y%d/sig_pT%.1f_%.1f.png", iy, ana::ptbin[ipt], ana::ptbin[ipt+1]));

         TCanvas* c2 = new TCanvas(Form("c2_%d%d", ipt, iy), "", 600, 500);
         c2->SetLeftMargin(0.15);
         gStyle->SetOptStat(0);
         hSMtd[ipt][iy]->SetLineColor(kRed);
         hS[ipt][iy]->GetYaxis()->SetTitle("S");
         hS[ipt][iy]->GetXaxis()->SetTitle("vtxProb");
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
         ltx->DrawLatexNDC(0.6, 0.2, Form("%.1f < |y| < %.1f GeV", ana::ybin[iy], ana::ybin[iy+1]));
         c2->SaveAs(Form("optimizationLamC/vtxProb/y%d/scaled_s_pT%.1f_%.1f.png", iy, ana::ptbin[ipt], ana::ptbin[ipt+1]));

         TCanvas* c3 = new TCanvas(Form("c3_%d%d", ipt, iy), "", 600, 500);
         c3->SetLeftMargin(0.15);
         TGaxis::SetMaxDigits(4);
         gStyle->SetOptStat(0);
         hB[ipt][iy]->GetYaxis()->SetTitle("B");
         hB[ipt][iy]->GetXaxis()->SetTitle("vtxProb");
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
         ltx->DrawLatexNDC(0.6, 0.2, Form("%.1f < |y| < %.1f GeV", ana::ybin[iy], ana::ybin[iy+1]));
         c3->SaveAs(Form("optimizationLamC/vtxProb/y%d/scaled_b_pT%.1f_%.1f.png", iy, ana::ptbin[ipt], ana::ptbin[ipt+1]));
      }
      vtxCut.push_back(vtxCut_temp);
      vtxCut_temp.clear();
   }
   std::ofstream fout("vtxProbCut.txt");
   fout << "double vtxProbCut[nuOfPt][nuOfY] = {" << std::endl;
   for(int ipt=0; ipt<ana::nuOfPt; ipt++){
      for(int iy=0; iy<ana::nuOfY; iy++){
         fout << vtxCut[ipt][iy] << ", ";
      }
      fout << std::endl;
   }
   fout << "};" << std::endl;
}

