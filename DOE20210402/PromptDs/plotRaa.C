void plotRaa()
{
  std::unique_ptr<TGraphErrors> D0_Alice(new TGraphErrors("RAA_D0_Alice.txt"));
  std::unique_ptr<TGraphErrors> Ds_Alice(new TGraphErrors("RAA_Ds_Alice.txt"));

  std::unique_ptr<TCanvas> c(new TCanvas("cRaa", "", 600, 450));
  auto frame = c->DrawFrame(0.0, 0.0, 12.5, 2);
  Ds_Alice->SetMarkerStyle(kOpenCircle);
  D0_Alice->SetMarkerStyle(kOpenSquare);
  for (int i=0; i<Ds_Alice->GetN(); i++) {
    Ds_Alice->SetPoint(i, Ds_Alice->GetX()[i]+0.15, Ds_Alice->GetY()[i]);
  }
  Ds_Alice->Draw("P");
  D0_Alice->Draw("P");

  //std::unique_ptr<TGraphErrors> 
  //
  // calculate ratios
  // Ds start from 2 GeV, first point, index 0
  // D0 start from 0 GeV, third point, index 2
  // Ds have 6-7, 7-8, index 4, 5
  // D0 have 6-8, index 6
  // Ds have 8-10, index 6
  // D0 have 8-12, index 7
  double ratio[7] = {0.};
  for (int i=0; i<7; i++) {
    if (i<5) {
      ratio[i] = Ds_Alice->GetY()[i]/ D0_Alice->GetY()[i+2] * 0.2; // 0.2 from pp
    }
    if (i==5 || i == 6) {
      ratio[i] = Ds_Alice->GetY()[i]/ D0_Alice->GetY()[i+1] * 0.2; // 0.2 from pp
    }
    cout << ratio[i] << endl;
  }

  TFile infile("Latest/sig_0_10.root");
  TGraphErrors* gSigVsPt = (TGraphErrors*) infile.Get("gSigVsPt_0to1");

  std::unique_ptr<TGraphErrors> Ds_CMS( new TGraphErrors(*gSigVsPt));
  auto n = Ds_CMS->GetN();
  for (int i=0; i<n; i++) {
    auto sig = Ds_CMS->GetY()[i];
    sig *= ratio[i];
    cout << sig << endl;
    if (i<5) {
      Ds_CMS->SetPoint(i, Ds_CMS->GetX()[i], Ds_Alice->GetY()[i]);
      Ds_CMS->SetPointError(i, Ds_CMS->GetEX()[i], Ds_Alice->GetY()[i]/sig);
    }
    if (i>=5) {
      Ds_CMS->SetPoint(i, Ds_CMS->GetX()[i], Ds_Alice->GetY()[i+1]);
      Ds_CMS->SetPointError(i, Ds_CMS->GetEX()[i], Ds_Alice->GetY()[i+1]/sig);
    }
  }
  Ds_CMS->SetMarkerStyle(kFullCircle);
  Ds_CMS->Draw("P");

  std::unique_ptr<TLegend> leg(new TLegend(0.6, 0.6, 0.89, 0.89));
  leg->SetHeader("Cent. 0-10%");
  leg->AddEntry(Ds_CMS.get(), "D_{S}^{#pm} CMS MTD, PbPb 3nb^{-1}", "p");
  leg->AddEntry(Ds_Alice.get(), "D_{S}^{#pm} Alice, PbPb 10nb^{-1}", "p");
  leg->AddEntry(D0_Alice.get(), "D^{0} Alice, PbPb 10nb^{-1}", "p");
  leg->Draw();

  c->Print("plots/RaaAliceCMS.pdf");
  c->Print("plots/RaaAliceCMS.png");
}
