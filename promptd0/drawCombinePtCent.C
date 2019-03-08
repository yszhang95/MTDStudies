#include "myAna.h"
void drawCombinePtCent()
{
   TFile* f1 = new TFile("genPt_reRECO.root");
   TFile* f2 = new TFile("HEPData-ins1616207-v1.root");

   TH1F* hGenPt[ana::nuOfY];
   TH1F* hData;
   TH1F* hGenPtMidY;
   for(int iy=0; iy<ana::nuOfY; iy++){
      f1->GetObject(Form("hPt%d", iy), hGenPt[iy]);
   }
   f2->GetObject("Table 3/Hist1D_y2", hData);

   hGenPtMidY = (TH1F*) hGenPt[4]->Clone();
   hGenPtMidY->Add(hGenPt[5]);
   hGenPtMidY->Add(hGenPt[6]);
   hGenPtMidY->Add(hGenPt[7]);

   // try to scale gen spectrum
   double yield_mc_Pt2_3GeV = hGenPtMidY->Integral(20, 30);
   //double yield_data_Pt2_3GeV = hData->GetBinContent(1) * ana::TAA0_10 * ana::pbOvermb * 1 *ana::GeV * ana::evts_data_MB * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   double yield_data_Pt2_3GeV_perCentEvt = hData->GetBinContent(1) * ana::TAA0_10 * ana::pbOvermb * 1 *ana::GeV * ana::BR * 2; // 2 is because of the data is for (D+Dbar)/2
   double scale_factor_perCentEvt = yield_data_Pt2_3GeV_perCentEvt / yield_mc_Pt2_3GeV;

   hGenPtMidY->Scale(scale_factor_perCentEvt);

   TH1F* hdraw = new TH1F("hdraw", "", ana::nuOfPt, ana::ptbin);
   hdraw->SetBinContent(1, hGenPtMidY->Integral(1, 5)/(0.5*ana::TAA0_10*ana::BR * 2 ) * 1e9); // 1e9 for converting mb to pb
   hdraw->SetBinContent(2, hGenPtMidY->Integral(6, 10)/(0.5*ana::TAA0_10*ana::BR * 2 ) * 1e9); // 1e9 for converting mb to pb
   hdraw->SetBinContent(3, hGenPtMidY->Integral(10, 15)/(0.5*ana::TAA0_10*ana::BR * 2 ) * 1e9); // 1e9 for converting mb to pb
   hdraw->SetBinContent(4, hGenPtMidY->Integral(15, 20)/(0.5*ana::TAA0_10*ana::BR * 2 ) * 1e9); // 1e9 for converting mb to pb
   hdraw->SetBinContent(5, hData->GetBinContent(1));
   hdraw->SetBinContent(6, hData->GetBinContent(2));
   hdraw->SetBinContent(7, hData->GetBinContent(3));
   hdraw->SetBinContent(8, hData->GetBinContent(4));
   hdraw->SetBinContent(9, hData->GetBinContent(5));
   hdraw->SetBinContent(10, hData->GetBinContent(6));

   TCanvas* c1 = new TCanvas("c1", "", 575, 500);
   c1->SetLeftMargin(0.16);
   TLatex* ltx = new TLatex();
   gStyle->SetOptStat(0);
   hdraw->SetTitle(";p_{T} (GeV);#frac{dN}{N_{MB} T_{AA} dp_{T}} (#frac{pb}{GeV/c})");
   hdraw->Draw();
   ltx->SetTextSize(0.05);
   ltx->DrawLatexNDC(0.29, 0.93, "Phase II Simulation #sqrt{s} = 5.5 TeV");
   ltx->DrawLatexNDC(0.63, 0.75, "CMS");
   ltx->SetTextSize(0.035);
   ltx->DrawLatexNDC(0.63, 0.66, "Preliminary");
   //ltx->DrawLatexNDC(0.63, 0.6, "D -> K + #pi");
   ltx->DrawLatexNDC(0.59, 0.46, "centrality 0~10%");
   ltx->DrawLatexNDC(0.64, 0.36, "-1 < y < 1");
   //std::cout <<hGenPtMidY->Integral(20, 30)/(ana::TAA0_10*ana::BR * 2 ) * 1e9  << std::endl;
   //std::cout << hdraw->GetBinContent(5) << std::endl;
}
