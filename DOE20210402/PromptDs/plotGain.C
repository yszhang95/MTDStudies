void plotGain()
{
  auto c = TCanvas("gain", "", 1500, 500);
  c.Divide(3, 1);

  double pTdata[] = {2.6,3.6,4.6,5.6,7.1,9.1};
  double lamcsig[] = {4.5,8,14.2,20.1,36.5,70.5};
  double lamcsig_y2[] = {6.84,5.5,5.2,5,13,17};
  double lamcsig_y3[] = {2.3,2.2,1.7,2.6,6.4,7.4};

  double lamcsig_womtd[] = {0.721615,1.64138,4.51725,7.8896,21.7051,55.1349};
  double lamcsig_y2_womtd[] = {1.8,2.2,3.0,3.8,12.9,25.7};
  double lamcsig_y3_womtd[] = {1.5,1.69,1.63,2.1,6.3,9.7};

  TGraph* glamc[3];
  glamc[0] = new TGraph(6);
  for (int i=0; i<6; i++) {
    glamc[0]->SetPoint(i, pTdata[i], lamcsig[i]/lamcsig_womtd[i]);
    glamc[0]->SetMarkerStyle(kOpenCircle);
  }
  glamc[1] = new TGraph(6);
  for (int i=0; i<6; i++) {
    glamc[1]->SetPoint(i, pTdata[i], lamcsig_y2[i]/lamcsig_y2_womtd[i]);
    glamc[1]->SetMarkerStyle(kOpenCircle);
  }
  glamc[2] = new TGraph(6);
  for (int i=0; i<6; i++) {
    glamc[2]->SetPoint(i, pTdata[i], lamcsig_y3[i]/lamcsig_y3_womtd[i]);
    glamc[2]->SetMarkerStyle(kOpenCircle);
  }


  auto f = TFile("sig_MB.root");
  const char* yRanges[] = {"0to1", "1to2", "2to3"};
  int iy=1;
  for (const auto y : yRanges) {
    TGraphErrors* g = (TGraphErrors*) f.Get(Form("gSigVsPt_%s", y));
    TGraphErrors* gNoMtd = (TGraphErrors*) f.Get(Form("gSigVsPt_%s_noMTD", y));
    c.cd(iy);
    auto gRatio = TGraphErrors(*g);
    gRatio.SetName(Form("DsSigRatio_y%s", y));
    for (int i=0; i<gRatio.GetN(); i++) {
      auto x = g->GetX()[i];
      auto ratio = g->GetY()[i]/gNoMtd->GetY()[i];
      auto ratioErr = g->GetEY()[i]/g->GetY()[i];
      auto xErr = g->GetEX()[i];
      gRatio.SetPoint(i, x, ratio);
      gRatio.SetPointError(i, xErr, ratioErr);
    }
    gRatio.SetTitle(Form("wMTD/woMTD y%s;pT;Gain",  y));
    gRatio.SetMarkerStyle(20);
    glamc[iy-1]->SetTitle(gRatio.GetTitle());
    glamc[iy-1]->Draw("AP");
    gRatio.DrawClone("PSAME");
    //gRatio.Print();
    iy++;
  }

  c.Print("plots/centMB_gain.pdf");
  c.Print("plots/centMB_gain.png");
}
