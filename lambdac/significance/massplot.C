#include "myAna.h"
//#include "myAnaTight.h"

void massplot()
{
   gStyle->SetOptStat(0);
   TLatex *ltx = new TLatex();

   TFile* f1 = new TFile("genPt_reRECO.root");
   TFile* f2 = new TFile("HEPData-ins1616207-v1.root");
   TFile* f3 = new TFile("lamC3PMassHists_reRECO_all.root");
   //TFile* f3 = new TFile("lamC3PMassHists_reRECO_all_tight.root");
   //TFile* f4 = new TFile("hyjetsMassHists_reRECO_all.root");
  // TFile* f4 = new TFile("hyjetsMassHists_reRECO_hyjets_test.list.root");
   TFile* f4 = new TFile("hyjetsMassHists_reRECO_wPID_4950.root");
   //TFile* f4 = new TFile("hyjetsMassHists_reRECO_wPID_4950_tight.root");

   TF1* fbkgMtd[ana::nuOfPt] ;
   TF1* fbkg[ana::nuOfPt] ;

   TH1D* hGenPtMidY;
   TH1F* hData;
   TH3D* hSignal;
   TH3D* hBkg;
   TH3D* hSignalMtd;
   TH3D* hBkgMtd;

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

   TH1D* hSAndBMtd[ana::nuOfY];
   TH1D* hSAndB[ana::nuOfY];

   //for(int ipt=0; ipt<ana::nuOfPt; ipt++){
   //for(int ipt=4; ipt<6; ipt++){
   for(int ipt=7; ipt<8; ipt++){
      //fbkgMtd[ipt]= new TF1(Form("fbkgMtd%d", ipt), "[0] + [1] *x  + [2] * x *x", 2.15, 2.45);
      //fbkg[ipt]= new TF1(Form("fbkg%d", ipt), "[0] + [1] *x  + [2] * x *x", 2.15, 2.45);
      fbkgMtd[ipt]= new TF1(Form("fbkgMtd%d", ipt), "[0] + [1] *x  ", 2.15, 2.45);
      fbkg[ipt]= new TF1(Form("fbkg%d", ipt), "[0] + [1] *x  ", 2.15, 2.45);
      double pTCutMin = ana::ptbin[ipt];
      double pTCutMax = ana::ptbin[ipt+1];

      //for(int iy=0; iy<ana::nuOfY; iy++){
      //for(int iy=1; iy<2; iy++){
      for(int iy=0; iy<1; iy++){
         double yCutMin = ana::ybin[iy];
         double yCutMax = ana::ybin[iy+1];

         int ptbinlw = hSignalMtdScale->GetYaxis()->FindBin(pTCutMin);
         int ptbinup = hSignalMtdScale->GetYaxis()->FindBin(pTCutMax);
         int ybinlw = hSignalMtdScale->GetXaxis()->FindBin(yCutMin);
         int ybinup = hSignalMtdScale->GetXaxis()->FindBin(yCutMax);

         TH1D* hSMtd = hSignalMtdScale->ProjectionZ(Form("mtd_s_mass_pT%.1f_%.1f_y%.1f_%.1f", pTCutMin, pTCutMax, yCutMin, yCutMax), ybinlw, ybinup, ptbinlw, ptbinup);
         TCanvas* c1 = new TCanvas();
         c1->cd();
         //hSMtd->Smooth(3);
         hSMtd->SetLineColor(kRed);

         TH1D* hBMtd = hBkgMtd->ProjectionZ(Form("mtd_b_mass_pT%.1f_%.1f_y%.1f_%.1f", pTCutMin, pTCutMax, yCutMin, yCutMax), ybinlw, ybinup, ptbinlw, ptbinup);
         hBMtd->GetYaxis()->SetRangeUser(0, hBMtd->GetMaximum() * 1.3);
         //hBMtd->Smooth(10);
         fbkgMtd[ipt]->SetNpx(60);
         //hBMtd->Fit(fbkgMtd[ipt], "Q R", "", 2.266, 2.306);
         //hBMtd->Fit(fbkgMtd[ipt], "Q R", "", 2.266, 2.306);
         //hBMtd->Fit(fbkgMtd[ipt], "Q R", "", 2.266, 2.306);
         hBMtd->Fit(fbkgMtd[ipt]);
         hBMtd->Fit(fbkgMtd[ipt]);
         hBMtd->Fit(fbkgMtd[ipt]);
         auto hBkgMtdFit = fbkgMtd[ipt]->CreateHistogram();
         hBkgMtdFit->SetLineColor(kBlue);
         hBkgMtdFit->Scale(25e9 / 4900);
         for(int ibin=0; ibin<60; ibin++){
            hBkgMtdFit->SetBinError(ibin+1, sqrt(hBkgMtdFit->GetBinContent(ibin + 1)));
         }
         hSMtd->Scale(25e9);
         hBkgMtdFit->GetXaxis()->SetRangeUser(2.25, 2.3);

         hBMtd->Scale(25e9/4900);
         hSMtd->SetLineColor(kGreen-5);
         hSAndBMtd[ipt] = new TH1D(*hSMtd);
         hSAndBMtd[ipt]->SetName("SAndBMtd");
         hSAndBMtd[ipt]->Add(hBkgMtdFit);
         for(int ibin=0; ibin<60; ibin++){
            hSAndBMtd[ipt]->SetBinError(ipt+1, sqrt(hSAndBMtd[ipt]->GetBinContent(ipt+1)));
         }
         hSAndBMtd[ipt]->SetLineColor(kRed);
         hSAndBMtd[ipt]->GetXaxis()->SetRangeUser(2.2, 2.35);
         hSAndBMtd[ipt]->GetYaxis()->SetRangeUser(hSAndBMtd[ipt]->GetMinimum()*0.99, hSAndBMtd[ipt]->GetMaximum() * 1.01);
         hSAndBMtd[ipt]->GetYaxis()->SetRangeUser(hSAndBMtd[ipt]->GetMinimum() * 0.99, hSAndBMtd[ipt]->GetMaximum() * 1.01);
         hSAndBMtd[ipt]->SetLineColor(kRed);
         hSAndBMtd[ipt]->Draw();
         hSMtd->Draw("same");
         //hBkgMtdFit->Draw("same");

         
         //hSAndBMtd[ipt]->Draw("same");
         hSAndBMtd[ipt]->SetLineColor(kBlue);
         ltx->DrawLatexNDC(0.7, 0.7, Form("%.1f < p_T < %.1f", ana::ptbin[ipt], ana::ptbin[ipt+1]));
         ltx->DrawLatexNDC(0.7, 0.6, Form("%.1f < |y| < %.1f", ana::ybin[iy], ana::ybin[iy+1]));
         ltx->DrawLatexNDC(0.7, 0.5, "MTD");
         c1->SaveAs(Form("lamC%d%d.png", ipt, iy));
         TCanvas* c2 = new TCanvas();
         c2->cd();
         hBMtd->Scale(4900/25e9);
         hBMtd->Draw();
      }
   }

}
