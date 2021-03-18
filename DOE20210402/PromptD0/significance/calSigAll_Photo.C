#include "myAna.h"
#include <iomanip>

void calSigAll_Photo()
{
   TGaxis::SetMaxDigits(3);
   TFile* f1 = new TFile("genPt_reRECO.root");
   TFile* f2 = new TFile("HEPData-ins1616207-v1.root");
   TFile* f3 = new TFile("PromptDMassHists_reRECO_all.root");
   TFile* f4 = new TFile("hyjetsMassHists_reRECO_HYDJETS.root");

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

   std::ofstream fwMB("sigMBPhoto.txt");
   std::ofstream fwoMB("sigWoMBPhoto.txt");
   std::ofstream fSBwMB("SBMBPhoto.txt");
   std::ofstream fSBwoMB("SBMBWoMTDPhoto.txt");
   fSBwMB << "S0,SErr0,B0,BErr0" << endl;
   fSBwoMB << "S0,SErr0,B0,BErr0" << endl;

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

   for(int ipt=0; ipt<ana::nuOfPt; ipt++){
   //for(int ipt=0; ipt<2; ipt++){


      hSig[ipt] = new TH1D(Form("hSig%d", ipt), "", ana::nuOfY, ana::ybin);
      hSigMtd[ipt] = new TH1D(Form("hSigMtd%d", ipt), "", ana::nuOfY, ana::ybin);

      hS[ipt] = new TH1D(Form("hS%d", ipt), "", ana::nuOfY, ana::ybin);
      hSMtd[ipt] = new TH1D(Form("hSMtd%d", ipt), "", ana::nuOfY, ana::ybin);

      hB[ipt] = new TH1D(Form("hB%d", ipt), "", ana::nuOfY, ana::ybin);
      hBMtd[ipt] = new TH1D(Form("hBMtd%d", ipt), "", ana::nuOfY, ana::ybin);

      double pTCutMin = ana::ptbin[ipt];
      double pTCutMax = ana::ptbin[ipt+1];

      //for(int iy=0; iy<ana::nuOfY; iy++){
      for(int iy=0; iy<1; iy++){

         double yCutMin = 0.;
         double yCutMax = 1.2;

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

         fSBwoMB << s << "," << sErr << "," << b << "," << bErr;
         fSBwMB << sMtd << "," << sMtdErr << "," << bMtd << "," << bMtdErr;
      }

      fwMB  << Form("//%.1f < pT < %.1f ", ana::ptbin[ipt], ana::ptbin[ipt+1]) << std::endl;
      fwoMB  << Form("//%.1f < pT < %.1f ", ana::ptbin[ipt], ana::ptbin[ipt+1]) << std::endl;
      fSBwMB  << std::endl;
      fSBwoMB  << std::endl;
   }
}
