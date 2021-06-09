void makeMultiPanelCanvas(TCanvas* canv, const Int_t columns, const Int_t rows,
    const Float_t leftOffset=0.02,
    const Float_t bottomOffset=0.02,
    const Float_t leftMargin=0.2,
    const Float_t bottomMargin=0.2,
    const Float_t edge=0.0)
{
  if (canv==0) {
    Error("makeMultiPanelCanvas","Got null canvas.");
    return;
  }
  canv->Clear();

  TPad* pad[columns][rows];

  Float_t Xlow[columns];
  Float_t Xup[columns];
  Float_t Ylow[rows];
  Float_t Yup[rows];
  Float_t PadWidth = 
    (1.0-leftOffset)/((1.0/(1.0-leftMargin)) +
        (1.0/(1.0-edge))+(Float_t)columns-2.0);
  Float_t PadHeight =
    (1.0-bottomOffset)/((1.0/(1.0-bottomMargin)) +
        (1.0/(1.0-edge))+(Float_t)rows-2.0);
  Xlow[0] = leftOffset;
  Xup[0] = leftOffset + PadWidth/(1.0-leftMargin);
  Xup[columns-1] = 1;
  Xlow[columns-1] = 1.0-PadWidth/(1.0-edge);

  Yup[0] = 1;
  Ylow[0] = 1.0-PadHeight/(1.0-edge);
  Ylow[rows-1] = bottomOffset;
  Yup[rows-1] = bottomOffset + PadHeight/(1.0-bottomMargin);

  for(Int_t i=1;i<columns-1;i++) {
    Xlow[i] = Xup[0] + (i-1)*PadWidth;
    Xup[i] = Xup[0] + (i)*PadWidth;
  }
  Int_t ct = 0;
  for(Int_t i=rows-2;i>0;i--) {
    Ylow[i] = Yup[rows-1] + ct*PadHeight;
    Yup[i] = Yup[rows-1] + (ct+1)*PadHeight;
    ct++;
  }

  TString padName;
  for(Int_t i=columns-1;i>=0;i--) {
    for(Int_t j=0;j<rows;j++) {
      canv->cd();
      padName = Form("p_%d_%d",i,j);
      pad[i][j] = new TPad(padName.Data(),padName.Data(),
          Xlow[i],Ylow[j],Xup[i],Yup[j]);
      if(i==0) pad[i][j]->SetLeftMargin(leftMargin);
      else pad[i][j]->SetLeftMargin(0);

      if(i==(columns-1)) pad[i][j]->SetRightMargin(edge);
      else pad[i][j]->SetRightMargin(0);

      if(j==0) pad[i][j]->SetTopMargin(edge);
      else pad[i][j]->SetTopMargin(0);

      if(j==(rows-1)) pad[i][j]->SetBottomMargin(bottomMargin);
      else pad[i][j]->SetBottomMargin(0);

      pad[i][j]->Draw();
      pad[i][j]->cd();
      pad[i][j]->SetNumber(columns*j+i+1);
    }
  }
}


void plotRaaCMS()
{
  gStyle->SetOptStat(0);
  TH2D* htmp = new TH2D("htmp",";p_{T} [GeV];R_{AA}", 100, 0.05, 10.5, 100, 0, 2.3);
  htmp->SetLineWidth(0);
  htmp->GetXaxis()->CenterTitle();
  htmp->GetYaxis()->CenterTitle();
  htmp->GetXaxis()->SetTitleSize(htmp->GetXaxis()->GetTitleSize()*1.3);  
  htmp->GetYaxis()->SetTitleSize(htmp->GetYaxis()->GetTitleSize()*1.45); 
  htmp->GetXaxis()->SetLabelSize(htmp->GetXaxis()->GetLabelSize()*1.1);
  htmp->GetYaxis()->SetLabelSize(htmp->GetYaxis()->GetLabelSize()*1.1);

  TH2D* htmp1 = (TH2D*)htmp->Clone("htmp1");
  TH2D* htmp2 = (TH2D*)htmp->Clone("htmp2");
  TH2D* htmp3 = (TH2D*)htmp->Clone("htmp3");

  std::unique_ptr<TCanvas> c(new TCanvas("cRaa", "", 1200, 480));
  makeMultiPanelCanvas(c.get(), 3, 1, 0.01, 0.08, 0.15, 0.14, 0.07);
  c->GetPad(1)->SetFillStyle(4000);
  c->GetPad(2)->SetFillStyle(4000);
  c->GetPad(3)->SetFillStyle(4000);

  c->cd(1);
  htmp1->GetXaxis()->SetLabelSize(htmp1->GetXaxis()->GetLabelSize()*1.25);
  htmp1->GetXaxis()->SetTitleSize(htmp1->GetXaxis()->GetTitleSize()*1.25);
  htmp1->GetYaxis()->SetLabelSize(htmp1->GetYaxis()->GetLabelSize()*1.25);
  htmp1->GetYaxis()->SetTitleSize(htmp1->GetYaxis()->GetTitleSize()*1.25);
  htmp1->GetXaxis()->SetTitleOffset(htmp1->GetXaxis()->GetTitleOffset()*0.95);
  htmp1->Draw();
  c->cd(2);
  htmp2->GetXaxis()->SetLabelSize(htmp2->GetXaxis()->GetLabelSize()*1.5);
  htmp2->GetXaxis()->SetTitleSize(htmp2->GetXaxis()->GetTitleSize()*1.5);
  htmp2->GetYaxis()->SetLabelSize(htmp2->GetYaxis()->GetLabelSize()*1.45);
  htmp2->GetYaxis()->SetTitleSize(htmp2->GetYaxis()->GetTitleSize()*1.45);
  htmp2->GetXaxis()->SetTitleOffset(htmp2->GetXaxis()->GetTitleOffset()*0.8);
  htmp2->GetXaxis()->SetLabelOffset(htmp2->GetXaxis()->GetLabelOffset()*0.2);
  htmp2->Draw();
  c->cd(3);
  htmp3->GetXaxis()->SetLabelSize(htmp3->GetXaxis()->GetLabelSize()*1.3);
  htmp3->GetXaxis()->SetTitleSize(htmp3->GetXaxis()->GetTitleSize()*1.3);
  htmp3->GetYaxis()->SetLabelSize(htmp3->GetYaxis()->GetLabelSize()*1.3);
  htmp3->GetYaxis()->SetTitleSize(htmp3->GetYaxis()->GetTitleSize()*1.3);
  htmp3->GetXaxis()->SetTitleOffset(htmp3->GetXaxis()->GetTitleOffset()*0.92);
  htmp3->Draw();

  double ratio[7] = {0.};
  // I use the numbers calculated from plotRaa.C, the one 7-8 is removed
  // ratio refers to Ds/D0
  ratio[0] = 0.717949;
  ratio[1] = 0.683203;
  ratio[2] = 0.475954;
  ratio[3] = 0.391199;
  ratio[4] = 0.299434;
  ratio[5] = 0.282127;

  double Raa_Ds[] = {
    1.848, 1.267, 0.7858, 0.5334, 0.4034, 0.3607
  };

  double Raa_D0[] =  {
    0.801,  // 0 -1
    0.652, // 1-2
    0.5148, // 2-3
    0.3709, // 3-4
    0.3302, // 4-5
    0.2727, // 5-6
    0.2828, // 6-8
    0.2557 // 8-10
  };
  double pT_D0[] = { 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 7, 9};

  double d0_sig[] = {
    //10.1914, 12.8875, 9.91204, //0.0 < pT < 0.5
    1.4*12.4927, 1.4*17.9512, 1.4*14.328, //0.5 < pT < 1.0
    34.3085, 32.0491, 22.2383, //1.0 < pT < 2.0
    140.847, 60.664, 27.9527, //2.0 < pT < 3.0
    216.384, 62.8964, 26.5243, //3.0 < pT < 4.0
    152.325, 50.307, 20.6779, //4.0 < pT < 5.0
    112.94, 39.2561, 16.3622, //5.0 < pT < 6.0
    285.798, 57.5856, 20.0804, //6.0 < pT < 8.0
    814.593, 47.597, 19.5243 //8.0 < pT < 10.0
  };

  double d0_sig_womtd[] =
  {
     //4.93013, 9.76968, 9.6791, //0.0 < pT < 0.5
    1.4*6.74405, 1.4*13.504, 1.4*12.7195, //0.5 < pT < 1.0
    20.2974, 25.2167, 19.1001, //1.0 < pT < 2.0
    94.9378, 49.6801, 25.1166, //2.0 < pT < 3.0
    167.688, 52.6112, 25.0888, //3.0 < pT < 4.0
    113.102, 41.7145, 20.6368, //4.0 < pT < 5.0
    87.504, 32.0433, 16.9828, //5.0 < pT < 6.0
    213.727, 50.6908, 22.3479, //6.0 < pT < 8.0
    196.06, 45.4001, 19.7766 //8.0 < pT < 10.0
  };


  TGraphErrors* gd0[3];
  TGraphErrors* gd0_womtd[3];
  for (int i=0; i<3; i++) {
    gd0[i] = new TGraphErrors(8);
    gd0_womtd[i] = new TGraphErrors(8);
    for (int j=0; j<8; j++) {
      gd0[i]->SetPoint(j, pT_D0[j]+0.1, Raa_D0[j]);
      gd0[i]->SetPointError(j, 0, Raa_D0[j]/d0_sig[i*3+j]);
      gd0_womtd[i]->SetPoint(j, pT_D0[j]-0.1, Raa_D0[j]);
      gd0_womtd[i]->SetPointError(j, 0, Raa_D0[j]/d0_sig_womtd[i*3+j]);
    }
    gd0[i]->SetMarkerStyle(kFullSquare);
    gd0[i]->SetMarkerSize(1.3);
    gd0_womtd[i]->SetMarkerStyle(kOpenSquare);
    gd0_womtd[i]->SetMarkerSize(1.3);
  }

  TFile infile("Latest/sig_0_10.root");
  TGraphErrors* gSigVsPt[3];
  TGraphErrors* gSigVsPtNoMTD[3];

  for (int iy=0; iy<3; iy++) {
    gSigVsPt[iy] = (TGraphErrors*) infile.Get(Form("gSigVsPt_%dto%d", iy, iy+1));
    gSigVsPtNoMTD[iy] = (TGraphErrors*) infile.Get(Form("gSigVsPt_%dto%d_noMTD", iy, iy+1));
  }

  std::unique_ptr<TGraphErrors> Ds_CMS[3];
  std::unique_ptr<TGraphErrors> Ds_CMSNoMTD[3];
  for (int iy=0; iy<3; iy++) {
    Ds_CMS[iy] = std::make_unique<TGraphErrors>(6);
    Ds_CMSNoMTD[iy] = std::make_unique<TGraphErrors>(6);
  }
  for (int iy=0; iy<3; iy++) {
    for (int i=0; i<6; i++) {
      auto sig = gSigVsPt[iy]->GetY()[i];
      sig *= ratio[i];
      auto sigNoMTD = gSigVsPtNoMTD[iy]->GetY()[i];
      cout << sigNoMTD << endl;
      sigNoMTD *= ratio[i];
      Ds_CMS[iy]->SetPoint(i, gSigVsPt[iy]->GetX()[i]+0.1, Raa_Ds[i]);
      Ds_CMS[iy]->SetPointError(i, 0, Raa_Ds[i]/sig);
      Ds_CMSNoMTD[iy]->SetPoint(i, gSigVsPtNoMTD[iy]->GetX()[i]-0.1, Raa_Ds[i]);
      Ds_CMSNoMTD[iy]->SetPointError(i, 0, Raa_Ds[i]/sigNoMTD);
    }
    Ds_CMS[iy]->SetMarkerStyle(kFullCircle);
    Ds_CMS[iy]->SetMarkerSize(1.3);
    Ds_CMS[iy]->SetMarkerColor(kRed+3);
    Ds_CMSNoMTD[iy]->SetMarkerStyle(kOpenCircle);
    Ds_CMSNoMTD[iy]->SetMarkerSize(1.3);
    Ds_CMSNoMTD[iy]->SetMarkerColor(kRed+3);
    Ds_CMSNoMTD[iy]->RemovePoint(0);
  }
  for (int i=0; i<3; i++) {
    c->cd(i+1);
    Ds_CMS[i]->Draw("SAMEP");
    Ds_CMSNoMTD[i]->Draw("SAMEP");
    gd0[i]->Draw("SAMEP");
    gd0_womtd[i]->Draw("SAMEP");
  }

  c->cd(1);
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.065);
  latex->DrawLatex(0.14,0.955,"#font[62]{CMS} #font[52]{Phase-2 Simulation}");

  TLatex* tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(0.05);
  tex->SetTextFont(42);
  tex->SetTextAlign(22);

  std::unique_ptr<TLegend> leg(new TLegend(0.6, 0.59, 0.89, 0.89));
  leg->SetHeader("Cent. 0-10%");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.05);
  leg->AddEntry(Ds_CMS[0].get(), "D_{S}^{#pm}, MTD", "p");
  leg->AddEntry(Ds_CMSNoMTD[0].get(), "D_{S}^{#pm}, MTD", "p");
  leg->AddEntry(gd0[0], "D^{0}/#bar{D^{0}}}, MTD", "p");
  leg->AddEntry(gd0_womtd[0], "D^{0}/#bar{D^{0}}, MTD", "p");
  leg->Draw();

  tex->DrawLatex(0.72, 0.5, "|y| < 1");

  c->cd(2);
  tex->DrawLatex(0.72, 0.5, "1 < |y| < 2");

  c->cd(3);
  tex->DrawLatex(0.72, 0.5, "2 < |y| < 3");
  latex->DrawLatex(0.35,0.955,"#font[42]{PbPb 3 nb^{-1} (5.5 TeV)}");

  c->Print("plots/RaaCentralCMS.pdf");
  c->Print("plots/RaaCentralCMS.png");
}
