#include "HyJets.h"
#include "TMath.h"
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "THashList.h"
#include "TLatex.h"
#include "TFileCollection.h"
#include "TVector3.h"
#include "TNtuple.h"
#include "myAna.h"
#include "TLegend.h"

int whichY(const float& y)
{
   for(int i=0; i<ana::nuOfY; i++){
      if(y>ana::ybin[i] && y<ana::ybin[i+1]) return i;
   }
   return -1;
}

void mtdDauAccept_HyJets()
{
   //TChain* chain = new TChain("d0ana/VertexCompositeNtuple");
   //TFileCollection* fc = new TFileCollection("dum", "", "hyjets_sample.list");
   TChain* chain = new TChain("d0ana_mc/VertexCompositeNtuple");
   TFileCollection* fc = new TFileCollection("dum", "", "newhyjets.list");
   chain->AddFileInfoList(fc->GetList()); 
   HyJets* t = new HyJets(chain);
   std::cout << t->GetEntries() << std::endl;

   // here is related to full pT
   TH1F* hDrawDau1Pt = new TH1F("hDrawDau1Pt", "", 100, 0, 10);
   TH1F* hDrawDau2Pt = new TH1F("hDrawDau2Pt", "", 100, 0, 10);

   TH1F* hAllDau1Pt = new TH1F("hAllDau1Pt", "", 100, 0, 10);
   TH1F* hAllDau2Pt = new TH1F("hAllDau2Pt", "", 100, 0, 10);

   TH1F* hNoMtdDau1Pt = new TH1F("hNoMtdDau1Pt", "hNoMtdDau1Pt", 100, 0, 10);
   TH1F* hNoMtdDau2Pt = new TH1F("hNoMtdDau2Pt", "hNoMtdDau2Pt", 100, 0, 10);

   TH1F* hMtdDau1Pt = new TH1F("hMtdDau1Pt", "hMtdDau1Pt", 100, 0, 10);
   TH1F* hMtdDau2Pt = new TH1F("hMtdDau2Pt", "hMtdDau2Pt", 100, 0, 10);

   TH1F* hDrawDau1Eta = new TH1F("hDrawDau1Eta", "", 60, -3, 3);
   TH1F* hDrawDau2Eta = new TH1F("hDrawDau2Eta", "", 60, -3, 3);

   TH1F* hAllDau1Eta = new TH1F("hAllDau1Eta", "", 60, -3, 3);
   TH1F* hAllDau2Eta = new TH1F("hAllDau2Eta", "", 60, -3, 3);

   TH1F* hNoMtdDau1Eta = new TH1F("hNoMtdDau1Eta", "hNoMtdDau1Eta", 60, -3, 3);
   TH1F* hNoMtdDau2Eta = new TH1F("hNoMtdDau2Eta", "hNoMtdDau2Eta", 60, -3, 3);

   TH1F* hMtdDau1Eta = new TH1F("hMtdDau1Eta", "hMtdDau1Eta", 60, -3, 3);
   TH1F* hMtdDau2Eta = new TH1F("hMtdDau2Eta", "hMtdDau2Eta", 60, -3, 3);

   TH2F* hPtVsEtaDau1All = new TH2F("hPtVsEtaDau1All", "hPtVsEtaDau1All", 1000, -3, 3, 1000, 0, 5);
   TH2F* hPtVsEtaDau1Mtd = new TH2F("hPtVsEtaDau1Mtd", "hPtVsEtaDau1Mtd", 1000, -3, 3, 1000, 0, 5);

   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      const int iy = whichY(t->y);
      if( iy == -1 ) continue;

      if(fabs(t->EtaD1) < 3 && fabs(t->EtaD1) < 1.4 ? t->pTD1 > 0.8 : t->pTD1 > 0.5){
         hPtVsEtaDau1All->Fill(t->EtaD1, t->pTD1);
         if(t->beta1_PV!=-99) hPtVsEtaDau1Mtd->Fill(t->EtaD1, t->pTD1);
      }

      if(fabs(t->EtaD1) > 3) continue;
      if(fabs(t->EtaD2) > 3) continue;
//      if(fabs(t->EtaD1) < 1.6) continue;
//      if(fabs(t->EtaD2) < 1.6) continue;

      //if(fabs(t->EtaD1) < 1.4 ? t->pTD1 <= 0.8 : t->pTD1 <= 0.5) continue;
      //if(fabs(t->EtaD2) < 1.4 ? t->pTD2 <= 0.8 : t->pTD2 <= 0.5) continue;
      //
      const float pD1 = t->pTD1 * std::cosh( t->EtaD1 );
      const float pD2 = t->pTD2 * std::cosh( t->EtaD2 );
      if(fabs(t->EtaD1) < 1.4 ? t->pTD1 <= 0.8 : pD1 <= 0.7) continue;
      if(fabs(t->EtaD2) < 1.4 ? t->pTD2 <= 0.8 : pD1 <= 0.7) continue;

      hAllDau1Pt->Fill(t->pTD1);
      if(!(t->beta1_PV!=-99))hNoMtdDau1Pt->Fill(t->pTD1);
      if(t->beta1_PV!=-99)hMtdDau1Pt->Fill(t->pTD1);

      hAllDau2Pt->Fill(t->pTD2);
      if(!(t->beta2_PV!=-99))hNoMtdDau2Pt->Fill(t->pTD2);
      if(t->beta2_PV!=-99)hMtdDau2Pt->Fill(t->pTD2);

      hAllDau1Eta->Fill(t->EtaD1);
      if(!(t->beta1_PV!=-99))hNoMtdDau1Eta->Fill(t->EtaD1);
      if(t->beta1_PV!=-99)hMtdDau1Eta->Fill(t->EtaD1);

      hAllDau2Eta->Fill(t->EtaD2);
      if(!(t->beta2_PV!=-99))hNoMtdDau2Eta->Fill(t->EtaD2);
      if(t->beta2_PV!=-99)hMtdDau2Eta->Fill(t->EtaD2);
   }
   

   // full range of pT, with |y|<3 
   TLatex* latex = new TLatex();
   latex->SetTextSize(0.036);

   TCanvas* c1 = new TCanvas("cDau1pT", "", 450, 500);

   c1->SetLeftMargin(0.16);
   c1->SetRightMargin(0.06);
   gStyle->SetOptStat(0);

   hDrawDau1Pt->GetYaxis()->SetRangeUser(0, 1.3);
   hDrawDau1Pt->GetYaxis()->SetTitle("Dau1 yield fraction");
   hDrawDau1Pt->GetXaxis()->SetTitle("Dau1 pT (GeV)");
   hDrawDau1Pt->Draw();

   hMtdDau1Pt->Divide(hAllDau1Pt);
   hNoMtdDau1Pt->Divide(hAllDau1Pt);

   hMtdDau1Pt->SetLineColor(kBlue);
   hNoMtdDau1Pt->SetLineColor(kRed);

   hMtdDau1Pt->Draw("same");
   hNoMtdDau1Pt->Draw("same");

   latex->DrawLatexNDC(0.6, 0.68, "-3 < D^{0} Rapidity < 3");

   TLegend *l1 = new TLegend(0.5, 0.78, 0.97, 0.90);
   l1->AddEntry(hNoMtdDau1Pt, "dau1 no mtd hits", "pl");
   l1->AddEntry(hMtdDau1Pt, "dau1 has mtd", "pl");
   l1->Draw();
   c1->SaveAs("dau1Pt.gif");

   TCanvas* c2 = new TCanvas("cDau2pT", "", 450, 500);

   c2->SetLeftMargin(0.16);
   c2->SetRightMargin(0.06);
   gStyle->SetOptStat(0);

   hDrawDau2Pt->GetYaxis()->SetRangeUser(0, 1.3);
   hDrawDau2Pt->GetYaxis()->SetTitle("Dau2 yield fraction");
   hDrawDau2Pt->GetXaxis()->SetTitle("Dau2 pT (GeV)");
   hDrawDau2Pt->Draw();

   hMtdDau2Pt->Divide(hAllDau2Pt);
   hNoMtdDau2Pt->Divide(hAllDau2Pt);

   hMtdDau2Pt->SetLineColor(kBlue);
   hNoMtdDau2Pt->SetLineColor(kRed);

   hMtdDau2Pt->Draw("same");
   hNoMtdDau2Pt->Draw("same");

   latex->DrawLatexNDC(0.6, 0.68, "-3 < D^{0} Rapidity < 3");

   TLegend *l2 = new TLegend(0.5, 0.78, 0.97, 0.90);
   l2->AddEntry(hNoMtdDau2Pt, "dau2 no mtd hits", "pl");
   l2->AddEntry(hMtdDau2Pt, "dau2 has mtd", "pl");
   l2->Draw();
   c2->SaveAs("dau2Pt.gif");

   TCanvas* c3 = new TCanvas("cDau1Eta", "", 450, 500);

   c3->SetLeftMargin(0.16);
   c3->SetRightMargin(0.06);
   gStyle->SetOptStat(0);

   hDrawDau1Eta->GetYaxis()->SetRangeUser(0, 1.3);
   hDrawDau1Eta->GetYaxis()->SetTitle("Dau1 yield fraction");
   hDrawDau1Eta->GetXaxis()->SetTitle("Dau1 eta");
   hDrawDau1Eta->Draw();

   hMtdDau1Eta->Divide(hAllDau1Eta);
   hNoMtdDau1Eta->Divide(hAllDau1Eta);

   hMtdDau1Eta->SetLineColor(kBlue);
   hNoMtdDau1Eta->SetLineColor(kRed);

   hMtdDau1Eta->Draw("same");
   hNoMtdDau1Eta->Draw("same");

   latex->DrawLatexNDC(0.6, 0.68, "-3 < D^{0} Rapidity < 3");

   TLegend *l3 = new TLegend(0.5, 0.78, 0.97, 0.90);
   l3->AddEntry(hNoMtdDau1Eta, "dau1 no mtd hits", "pl");
   l3->AddEntry(hMtdDau1Eta, "dau1 has mtd", "pl");
   l3->Draw();
   c3->SaveAs("dau1Eta.gif");

   TCanvas* c4 = new TCanvas("cDau2Eta", "", 450, 500);

   c4->SetLeftMargin(0.16);
   c4->SetRightMargin(0.06);
   gStyle->SetOptStat(0);

   hDrawDau2Eta->GetYaxis()->SetRangeUser(0, 1.3);
   hDrawDau2Eta->GetYaxis()->SetTitle("Dau2 yield fraction");
   hDrawDau2Eta->GetXaxis()->SetTitle("Dau2 eta");
   hDrawDau2Eta->Draw();

   hMtdDau2Eta->Divide(hAllDau2Eta);
   hNoMtdDau2Eta->Divide(hAllDau2Eta);

   hMtdDau2Eta->SetLineColor(kBlue);
   hNoMtdDau2Eta->SetLineColor(kRed);

   hMtdDau2Eta->Draw("same");
   hNoMtdDau2Eta->Draw("same");

   latex->DrawLatexNDC(0.6, 0.68, "-3 < D^{0} Rapidity < 3");

   TLegend *l4 = new TLegend(0.5, 0.78, 0.97, 0.90);
   l4->AddEntry(hNoMtdDau2Eta, "dau2 no mtd hits", "pl");
   l4->AddEntry(hMtdDau2Eta, "dau2 has mtd", "pl");
   l4->Draw();
   c4->SaveAs("dau2Eta.gif");

   TFile fout("dauFrac_hyjets_reRECO.root", "recreate");
   hMtdDau1Pt->Write();
   hMtdDau2Pt->Write();
   hMtdDau1Eta->Write();
   hMtdDau2Eta->Write();
   hNoMtdDau1Pt->Write();
   hNoMtdDau2Pt->Write();
   hNoMtdDau1Eta->Write();
   hNoMtdDau2Eta->Write();
   hPtVsEtaDau1All->Write();
   hPtVsEtaDau1Mtd->Write();
   hAllDau1Pt->Write();
   hAllDau2Pt->Write();
   hAllDau1Eta->Write();
   hAllDau2Eta->Write();
}
