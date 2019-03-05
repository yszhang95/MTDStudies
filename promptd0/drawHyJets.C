void setPalette()
{
   gStyle->SetPalette(55);
   const Int_t NRGBs = 5;
   const Int_t NCont = 255;
   Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
   Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
   Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
   Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
   TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
   gStyle->SetNumberContours(NCont);
}

void drawHyJets()
{
   TFile *f1 = new TFile("hists.root");
   TH1F* hD0Pt, * hD0Pt3sigma, *hD0Pt2sigma, *hD0Pt1sigma;
   TH1F* hPionEff, * hPion3sigmaEff, *hPion2sigmaEff, *hPion1sigmaEff;
   TH1F* hKaonEff, * hKaon3sigmaEff, *hKaon2sigmaEff, *hKaon1sigmaEff;
	TH2F* hD0PtVsDau1PMtd;
	TH2F* hD0PtVsDau2PMtd;

   f1->GetObject("hD0Pt", hD0Pt);
   f1->GetObject("hD0Pt3sigma", hD0Pt3sigma);
   f1->GetObject("hD0Pt2sigma", hD0Pt2sigma);
   f1->GetObject("hD0Pt1sigma", hD0Pt1sigma);

	f1->GetObject("hPionEff",hPionEff);
	f1->GetObject("hPion3sigmaEff",hPion3sigmaEff);
	f1->GetObject("hPion2sigmaEff",hPion2sigmaEff);
	f1->GetObject("hPion1sigmaEff",hPion1sigmaEff);

	f1->GetObject("hKaonEff",hKaonEff);
	f1->GetObject("hKaon3sigmaEff",hKaon3sigmaEff);
	f1->GetObject("hKaon2sigmaEff",hKaon2sigmaEff);
	f1->GetObject("hKaon1sigmaEff",hKaon1sigmaEff);

	f1->GetObject("hD0PtVsDau1PMtd", hD0PtVsDau1PMtd);
	f1->GetObject("hD0PtVsDau2PMtd", hD0PtVsDau2PMtd);

   TH1F* hD0PtCent, * hD0PtCent3sigma, *hD0PtCent2sigma, *hD0PtCent1sigma;
   TH1F* hPionEffCent, * hPion3sigmaEffCent, *hPion2sigmaEffCent, *hPion1sigmaEffCent;
   TH1F* hKaonEffCent, * hKaon3sigmaEffCent, *hKaon2sigmaEffCent, *hKaon1sigmaEffCent;

   f1->GetObject("hD0PtCent", hD0PtCent);
   f1->GetObject("hD0PtCent3sigma", hD0PtCent3sigma);
   f1->GetObject("hD0PtCent2sigma", hD0PtCent2sigma);
   f1->GetObject("hD0PtCent1sigma", hD0PtCent1sigma);

	f1->GetObject("hPionEffCent",hPionEffCent);
	f1->GetObject("hPion3sigmaEffCent",hPion3sigmaEffCent);
	f1->GetObject("hPion2sigmaEffCent",hPion2sigmaEffCent);
	f1->GetObject("hPion1sigmaEffCent",hPion1sigmaEffCent);

	f1->GetObject("hKaonEffCent",hKaonEffCent);
	f1->GetObject("hKaon3sigmaEffCent",hKaon3sigmaEffCent);
	f1->GetObject("hKaon2sigmaEffCent",hKaon2sigmaEffCent);
	f1->GetObject("hKaon1sigmaEffCent",hKaon1sigmaEffCent);

   bool drawDau = true;
   bool drawD0 = true;
   bool drawDauCent = true;
   bool drawD0Cent = true;

   setPalette();

   if(drawDau){
      TLatex* latex = new TLatex();
      latex->SetTextSize(0.036);
      TCanvas* c1 = new TCanvas("cPionEff", "PionEff", 500, 550);
      gStyle->SetOptStat(0);
      TH1F* hDrawPion = new TH1F("hDrawPion", "", 100, 0, 10);
      hDrawPion->GetYaxis()->SetTitle("Pion Yield Ratio");
      hDrawPion->GetYaxis()->SetRangeUser(0, 1.3);
      hDrawPion->GetXaxis()->SetTitle("p (GeV)");
      hDrawPion->Draw();
      hPion3sigmaEff->Divide(hPionEff);
      hPion2sigmaEff->Divide(hPionEff);
      hPion1sigmaEff->Divide(hPionEff);
      hPion3sigmaEff->SetLineColor(kGreen-6);
      hPion2sigmaEff->SetLineColor(kBlue);
      hPion1sigmaEff->SetLineColor(kRed);
      hPion3sigmaEff->Draw("same");
      hPion2sigmaEff->Draw("same");
      hPion1sigmaEff->Draw("same");
      TLegend *lPion = new TLegend(0.7, 0.8, 0.9, 0.9);
      lPion->AddEntry(hPion3sigmaEff, "3 RMS", "l");
      lPion->AddEntry(hPion2sigmaEff, "2 RMS", "l");
      lPion->AddEntry(hPion1sigmaEff, "1 RMS", "l");
      lPion->Draw();
      latex->DrawLatexNDC(0.6, 0.4, "-3 < D^{0} Rapidity < 3");
      latex->DrawLatexNDC(0.6, 0.5, "MB events");

      TCanvas* c2 = new TCanvas("KaonEff", "KaonEff", 500, 550);
      gStyle->SetOptStat(0);
      TH1F* hDrawKaon = new TH1F("hDrawKaon", "", 100, 0, 10);
      hDrawKaon->GetYaxis()->SetTitle("Kaon Yield Ratio");
      hDrawKaon->GetYaxis()->SetRangeUser(0, 1.3);
      hDrawKaon->GetXaxis()->SetTitle("p (GeV)");
      hDrawKaon->Draw();
      hKaon3sigmaEff->Divide(hKaonEff);
      hKaon2sigmaEff->Divide(hKaonEff);
      hKaon1sigmaEff->Divide(hKaonEff);
      hKaon3sigmaEff->SetLineColor(kGreen-6);
      hKaon2sigmaEff->SetLineColor(kBlue);
      hKaon1sigmaEff->SetLineColor(kRed);
      hKaon3sigmaEff->Draw("same");
      hKaon2sigmaEff->Draw("same");
      hKaon1sigmaEff->Draw("same");
      TLegend *lKaon = new TLegend(0.7, 0.8, 0.9, 0.9);
      lKaon->AddEntry(hKaon3sigmaEff, "3 RMS", "l");
      lKaon->AddEntry(hKaon2sigmaEff, "2 RMS", "l");
      lKaon->AddEntry(hKaon1sigmaEff, "1 RMS", "l");
		lKaon->Draw();
      latex->DrawLatexNDC(0.6, 0.4, "-3 < D^{0} Rapidity < 3");
      latex->DrawLatexNDC(0.6, 0.5, "MB events");
   }
   if(drawD0){
   	TCanvas* c1 = new TCanvas("hD0pT", "", 450, 450);
      c1->SetLeftMargin(0.16);
      gStyle->SetOptStat(0);

      TH1F* hDraw = new TH1F("hDrawD0", "", 100, 0, 10);
      hDraw->GetYaxis()->SetRangeUser(0, 1.3);
      hDraw->GetYaxis()->SetTitle("D0 yield eff");
      hDraw->GetXaxis()->SetTitle("pT (GeV)");
      hDraw->Draw();

      hD0Pt->Sumw2();
      hD0Pt3sigma->Sumw2();
      hD0Pt2sigma->Sumw2();
      hD0Pt1sigma->Sumw2();

      hD0Pt3sigma->Divide(hD0Pt);
      hD0Pt2sigma->Divide(hD0Pt);
      hD0Pt1sigma->Divide(hD0Pt);

      hD0Pt3sigma->SetLineColor(kGreen-6);
      hD0Pt2sigma->SetLineColor(kBlue);
      hD0Pt1sigma->SetLineColor(kRed);

      hD0Pt3sigma->Draw("same");
      hD0Pt2sigma->Draw("same");
      hD0Pt1sigma->Draw("same");

      TLatex* latex = new TLatex();
      latex->SetTextSize(0.036);
      latex->DrawLatexNDC(0.6, 0.4, "-3 < D^{0} Rapidity < 3");
      latex->DrawLatexNDC(0.6, 0.5, "MB events");

      TLegend *legend = new TLegend(0.7, 0.8, 0.9, 0.9);
      legend->AddEntry(hD0Pt3sigma, "3 RMS", "l");
      legend->AddEntry(hD0Pt2sigma, "2 RMS", "l");
      legend->AddEntry(hD0Pt1sigma, "1 RMS", "l");
      legend->Draw();

		TCanvas *c2D1 = new TCanvas("c2Ddau1", "c2Ddau1", 450, 500);
		c2D1->SetLogz();
		c2D1->SetLeftMargin(0.16);
      gStyle->SetOptStat(0);
      hD0PtVsDau1PMtd->GetYaxis()->SetTitle("dau1 p (GeV)");
      hD0PtVsDau1PMtd->GetXaxis()->SetTitle("D^{0} p_{T} (GeV)");
      hD0PtVsDau1PMtd->Draw("COLZ");

		TCanvas *c2D2 = new TCanvas("c2Ddau2", "c2Ddau2", 450, 500);
		c2D2->SetLogz();
		c2D2->SetLeftMargin(0.16);
      hD0PtVsDau2PMtd->GetYaxis()->SetTitle("dau2 p (GeV)");
      hD0PtVsDau2PMtd->GetXaxis()->SetTitle("D^{0} p_{T} (GeV)");
      gStyle->SetOptStat(0);
      hD0PtVsDau2PMtd->Draw("COLZ");
   }

   if(drawDauCent){
      TLatex* latex = new TLatex();
      latex->SetTextSize(0.036);
      TCanvas* c1 = new TCanvas("PionEffCent", "PionEffCent", 500, 550);
      gStyle->SetOptStat(0);
      TH1F* hDrawPion = new TH1F("hDrawPionCent", "", 100, 0, 10);
      hDrawPion->GetYaxis()->SetTitle("Pion Yield Ratio");
      hDrawPion->GetYaxis()->SetRangeUser(0, 1.3);
      hDrawPion->GetXaxis()->SetTitle("p (GeV)");
      hDrawPion->Draw();
      hPion3sigmaEffCent->Divide(hPionEffCent);
      hPion2sigmaEffCent->Divide(hPionEffCent);
      hPion1sigmaEffCent->Divide(hPionEffCent);
      hPion3sigmaEffCent->SetLineColor(kGreen-6);
      hPion2sigmaEffCent->SetLineColor(kBlue);
      hPion1sigmaEffCent->SetLineColor(kRed);
      hPion3sigmaEffCent->Draw("same");
      hPion2sigmaEffCent->Draw("same");
      hPion1sigmaEffCent->Draw("same");
      TLegend *lPion = new TLegend(0.7, 0.8, 0.9, 0.9);
      lPion->AddEntry(hPion3sigmaEffCent, "3 RMS", "l");
      lPion->AddEntry(hPion2sigmaEffCent, "2 RMS", "l");
      lPion->AddEntry(hPion1sigmaEffCent, "1 RMS", "l");
      lPion->Draw();
      latex->DrawLatexNDC(0.6, 0.4, "-3 < D^{0} Rapidity < 3");
      latex->DrawLatexNDC(0.6, 0.5, "central events");

      TCanvas* c2 = new TCanvas("KaonEffCent", "KaonEffCent", 500, 550);
      gStyle->SetOptStat(0);
      TH1F* hDrawKaon = new TH1F("hDrawKaonCent", "", 100, 0, 10);
      hDrawKaon->GetYaxis()->SetTitle("Kaon Yield Ratio");
      hDrawKaon->GetYaxis()->SetRangeUser(0, 1.3);
      hDrawKaon->GetXaxis()->SetTitle("p (GeV)");
      hDrawKaon->Draw();
      hKaon3sigmaEffCent->Divide(hKaonEffCent);
      hKaon2sigmaEffCent->Divide(hKaonEffCent);
      hKaon1sigmaEffCent->Divide(hKaonEffCent);
      hKaon3sigmaEffCent->SetLineColor(kGreen-6);
      hKaon2sigmaEffCent->SetLineColor(kBlue);
      hKaon1sigmaEffCent->SetLineColor(kRed);
      hKaon3sigmaEffCent->Draw("same");
      hKaon2sigmaEffCent->Draw("same");
      hKaon1sigmaEffCent->Draw("same");
      TLegend *lKaon = new TLegend(0.7, 0.8, 0.9, 0.9);
      lKaon->AddEntry(hKaon3sigmaEffCent, "3 RMS", "l");
      lKaon->AddEntry(hKaon2sigmaEffCent, "2 RMS", "l");
      lKaon->AddEntry(hKaon1sigmaEffCent, "1 RMS", "l");
		lKaon->Draw();
      latex->DrawLatexNDC(0.6, 0.4, "-3 < D^{0} Rapidity < 3");
      latex->DrawLatexNDC(0.6, 0.5, "central events");
   }
   if(drawD0Cent){
   	TCanvas* c1 = new TCanvas("hD0pTCent", "", 450, 450);
      c1->SetLeftMargin(0.16);
      gStyle->SetOptStat(0);

      TH1F* hDraw = new TH1F("hDrawD0Cent", "", 100, 0, 10);
      hDraw->GetYaxis()->SetRangeUser(0, 1.3);
      hDraw->GetYaxis()->SetTitle("D0 yield eff");
      hDraw->GetXaxis()->SetTitle("pT (GeV)");
      hDraw->Draw();

      hD0PtCent->Sumw2();
      hD0PtCent3sigma->Sumw2();
      hD0PtCent2sigma->Sumw2();
      hD0PtCent1sigma->Sumw2();

      hD0PtCent3sigma->Divide(hD0PtCent);
      hD0PtCent2sigma->Divide(hD0PtCent);
      hD0PtCent1sigma->Divide(hD0PtCent);

      hD0PtCent3sigma->SetLineColor(kGreen-6);
      hD0PtCent2sigma->SetLineColor(kBlue);
      hD0PtCent1sigma->SetLineColor(kRed);

      hD0PtCent3sigma->Draw("same");
      hD0PtCent2sigma->Draw("same");
      hD0PtCent1sigma->Draw("same");

      TLatex* latex = new TLatex();
      latex->SetTextSize(0.036);
      latex->DrawLatexNDC(0.6, 0.4, "-3 < D^{0} Rapidity < 3");
      latex->DrawLatexNDC(0.6, 0.5, "central events");

      TLegend *legend = new TLegend(0.7, 0.8, 0.9, 0.9);
      legend->AddEntry(hD0PtCent3sigma, "3 RMS", "l");
      legend->AddEntry(hD0PtCent2sigma, "2 RMS", "l");
      legend->AddEntry(hD0PtCent1sigma, "1 RMS", "l");
      legend->Draw();
   }

   TFile *f2 = new TFile("dauFrac_hyjets.root");
   TH1F* hMtdDau1Pt, *hMtdDau2Pt, *hMtdDau1Eta, *hMtdDau2Eta;
   TH1F* hNoMtdDau1Pt, *hNoMtdDau2Pt, *hNoMtdDau1Eta, *hNoMtdDau2Eta;
   TH2F* hPtVsEtaDau1Mtd, *hPtVsEtaDau1All;
   f2->GetObject("hMtdDau1Pt", hMtdDau1Pt);
   f2->GetObject("hMtdDau2Pt", hMtdDau2Pt);
   f2->GetObject("hMtdDau1Eta", hMtdDau1Eta);
   f2->GetObject("hMtdDau2Eta", hMtdDau2Eta);
   f2->GetObject("hNoMtdDau1Pt", hNoMtdDau1Pt);
   f2->GetObject("hNoMtdDau2Pt", hNoMtdDau2Pt);
   f2->GetObject("hNoMtdDau1Eta", hNoMtdDau1Eta);
   f2->GetObject("hNoMtdDau2Eta", hNoMtdDau2Eta);
   f2->GetObject("hPtVsEtaDau1Mtd", hPtVsEtaDau1Mtd);
   f2->GetObject("hPtVsEtaDau1All", hPtVsEtaDau1All);


   bool drawDauFrac = false;

   if(drawDauFrac){
   TLatex* latex = new TLatex();
   latex->SetTextSize(0.036);

   TCanvas* c1 = new TCanvas("cDau1pT", "", 450, 500);

   c1->SetLeftMargin(0.16);
   c1->SetRightMargin(0.06);
   gStyle->SetOptStat(0);

 	TH1F* hDrawDau1Pt = new TH1F("hDrawDau1Pt", "", 100, 0, 10);	
   hDrawDau1Pt->GetYaxis()->SetRangeUser(0, 1.3);
   hDrawDau1Pt->GetYaxis()->SetTitle("Dau1 yield fraction");
   hDrawDau1Pt->GetXaxis()->SetTitle("Dau1 pT (GeV)");
   hDrawDau1Pt->Draw();

   hMtdDau1Pt->SetLineColor(kBlue);
   hNoMtdDau1Pt->SetLineColor(kRed);

   hMtdDau1Pt->Draw("same");
   hNoMtdDau1Pt->Draw("same");

   latex->DrawLatexNDC(0.6, 0.68, "-3 < D^{0} Rapidity < 3");

   TLegend *l1 = new TLegend(0.5, 0.78, 0.97, 0.90);
   l1->AddEntry(hNoMtdDau1Pt, "dau1 no mtd hits", "pl");
   l1->AddEntry(hMtdDau1Pt, "dau1 has mtd", "pl");
   l1->Draw();

   TCanvas* c2 = new TCanvas("cDau2pT", "", 450, 500);

   c2->SetLeftMargin(0.16);
   c2->SetRightMargin(0.06);
   gStyle->SetOptStat(0);

 	TH1F* hDrawDau2Pt = new TH1F("hDrawDau2Pt", "", 100, 0, 10);
   hDrawDau2Pt->GetYaxis()->SetRangeUser(0, 1.3);
   hDrawDau2Pt->GetYaxis()->SetTitle("Dau2 yield fraction");
   hDrawDau2Pt->GetXaxis()->SetTitle("Dau2 pT (GeV)");
   hDrawDau2Pt->Draw();

   hMtdDau2Pt->SetLineColor(kBlue);
   hNoMtdDau2Pt->SetLineColor(kRed);

   hMtdDau2Pt->Draw("same");
   hNoMtdDau2Pt->Draw("same");

   latex->DrawLatexNDC(0.6, 0.68, "-3 < D^{0} Rapidity < 3");

   TLegend *l2 = new TLegend(0.5, 0.78, 0.97, 0.90);
   l2->AddEntry(hNoMtdDau2Pt, "dau2 no mtd hits", "pl");
   l2->AddEntry(hMtdDau2Pt, "dau2 has mtd", "pl");
   l2->Draw();

   TCanvas* c3 = new TCanvas("cDau1Eta", "", 450, 500);

   c3->SetLeftMargin(0.16);
   c3->SetRightMargin(0.06);
   gStyle->SetOptStat(0);

 	TH1F* hDrawDau1Eta = new TH1F("hDrawDau1Eta", "", 60, -3, 3);
   hDrawDau1Eta->GetYaxis()->SetRangeUser(0, 1.3);
   hDrawDau1Eta->GetYaxis()->SetTitle("Dau1 yield fraction");
   hDrawDau1Eta->GetXaxis()->SetTitle("Dau1 eta");
   hDrawDau1Eta->Draw();

   hMtdDau1Eta->SetLineColor(kBlue);
   hNoMtdDau1Eta->SetLineColor(kRed);

   hMtdDau1Eta->Draw("same");
   hNoMtdDau1Eta->Draw("same");

   latex->DrawLatexNDC(0.6, 0.68, "-3 < D^{0} Rapidity < 3");

   TLegend *l3 = new TLegend(0.5, 0.78, 0.97, 0.90);
   l3->AddEntry(hNoMtdDau1Eta, "dau1 no mtd hits", "pl");
   l3->AddEntry(hMtdDau1Eta, "dau1 has mtd", "pl");
   l3->Draw();

   TCanvas* c4 = new TCanvas("cDau2Eta", "", 450, 500);

   c4->SetLeftMargin(0.16);
   c4->SetRightMargin(0.06);
   gStyle->SetOptStat(0);

   TH1F* hDrawDau2Eta = new TH1F("hDrawDau2Eta", "", 60, -3, 3);
   hDrawDau2Eta->GetYaxis()->SetRangeUser(0, 1.3);
   hDrawDau2Eta->GetYaxis()->SetTitle("Dau2 yield fraction");
   hDrawDau2Eta->GetXaxis()->SetTitle("Dau2 eta");
   hDrawDau2Eta->Draw();

   hMtdDau2Eta->SetLineColor(kBlue);
   hNoMtdDau2Eta->SetLineColor(kRed);

   hMtdDau2Eta->Draw("same");
   hNoMtdDau2Eta->Draw("same");

   latex->DrawLatexNDC(0.6, 0.68, "-3 < D^{0} Rapidity < 3");

   TLegend *l4 = new TLegend(0.5, 0.78, 0.97, 0.90);
   l4->AddEntry(hNoMtdDau2Eta, "dau2 no mtd hits", "pl");
   l4->AddEntry(hMtdDau2Eta, "dau2 has mtd", "pl");
	l4->Draw();

   TCanvas* c5 = new TCanvas("cDau1PtVsEta", "", 450, 500);

   c5->SetLeftMargin(0.16);
   c5->SetRightMargin(0.14);
   //c5->SetLogz();
   gStyle->SetOptStat(0);
   hPtVsEtaDau1Mtd->Divide(hPtVsEtaDau1All);
   hPtVsEtaDau1Mtd->GetXaxis()->SetTitle("eta");
   hPtVsEtaDau1Mtd->GetYaxis()->SetTitle("pT (GeV)");
   hPtVsEtaDau1Mtd->SetMinimum(0.7);
   hPtVsEtaDau1Mtd->SetMaximum(1.0);
   hPtVsEtaDau1Mtd->Draw("COLZ");
   }
}
