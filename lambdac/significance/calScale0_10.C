#include "myAna.h"

int whichPt(const double& pt){
   double pT[11] = {0, 0.5, 1., 2, 3, 4, 5, 6, 7, 8, 10};
   for(int ipt=0; ipt<11; ipt++){
      if(pt < pT[ipt+1] && pt > pT[ipt]) return ipt;
   }
   return -1;
}
void calScale0_10(){
   TFile *f1 = new TFile("HEPData-ins1669819-v1-root");
   TH1F *hData;
   f1->GetObject("Table 1/Hist1D_y1", hData);

   TFile *f2 = new TFile("genPt_reRECO.root");
   TH1D* hGenPt;
   f2->GetObject("hGenPt", hGenPt);

   TH3D* hScale = new TH3D("hScale", "0-10", 60, 0, 3, 100, 0, 10, 60, 1.7, 2.0);

   std::vector<double> scale_factor_perEvt;

   double yield_mc_Pt1_2GeV = hGenPt->Integral(11, 20);
   double yield_data_Pt1_2GeV_perEvt = hData->GetBinContent(1) *  1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2

   scale_factor_perEvt.push_back(yield_data_Pt1_2GeV_perEvt / yield_mc_Pt1_2GeV); // 0.0 - 0.5 GeV
   scale_factor_perEvt.push_back(yield_data_Pt1_2GeV_perEvt / yield_mc_Pt1_2GeV); // 0.5 - 1.0 GeV
   scale_factor_perEvt.push_back(yield_data_Pt1_2GeV_perEvt / yield_mc_Pt1_2GeV); // 1.0 - 2.0 GeV

   double yield_data_Pt2_3GeV_perEvt = hData->GetBinContent(2) *  1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt.push_back(yield_data_Pt2_3GeV_perEvt / hGenPt->Integral(21, 30)); // 2.0 - 3.0 GeV

   double yield_data_Pt3_4GeV_perEvt = hData->GetBinContent(3) *  1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt.push_back(yield_data_Pt3_4GeV_perEvt / hGenPt->Integral(31, 40)); // 3.0 - 4.0 GeV

   double yield_data_Pt4_5GeV_perEvt = hData->GetBinContent(4) *  1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt.push_back(yield_data_Pt4_5GeV_perEvt / hGenPt->Integral(41, 50)); // 4.0 - 5.0 GeV

   double yield_data_Pt5_6GeV_perEvt = hData->GetBinContent(5) *  1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt.push_back(yield_data_Pt5_6GeV_perEvt / hGenPt->Integral(51, 60)); // 5.0 - 6.0 GeV

   double yield_data_Pt6_7GeV_perEvt = hData->GetBinContent(6) *  1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt.push_back(yield_data_Pt6_7GeV_perEvt / hGenPt->Integral(61, 70)); // 6.0 - 7.0 GeV

   double yield_data_Pt7_8GeV_perEvt = hData->GetBinContent(7) *  1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt.push_back(yield_data_Pt7_8GeV_perEvt / hGenPt->Integral(71, 80)); // 7.0 - 8.0 GeV

   double yield_data_Pt8_10GeV_perEvt = hData->GetBinContent(8) *  2 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   scale_factor_perEvt.push_back(yield_data_Pt8_10GeV_perEvt / hGenPt->Integral(81, 100)); // 8.0 - 10.0 GeV

   for(int ipt=0; ipt<ana::npt; ipt++){
      double pt = ((double)ipt + 0.5) * (ana::ptMax - ana::ptMin) / (double) ana::npt;
      int iscale = whichPt(pt);
      for(int iy=0; iy<ana::nyAbs; iy++){
         for(int iz=0; iz<60; iz++){
            if(iscale == -1) continue;
            hScale->SetBinContent(iy+1, ipt+1, iz+1, scale_factor_perEvt[iscale]);
            hScale->SetBinError(iy+1, ipt+1, iz+1, 0);
         }
         if( iscale!=-1 )hScale->SetBinContent(iy+1, ipt+1, 0, scale_factor_perEvt[iscale]);
         if( iscale!=-1 )hScale->SetBinContent(iy+1, ipt+1, 60+1, scale_factor_perEvt[iscale]);
      }
   }
   TFile fout("hscale0_10.root", "recreate");
   hScale->Write();
}
