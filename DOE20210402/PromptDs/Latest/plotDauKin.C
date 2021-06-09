void setPalette()
{
  gStyle->SetPalette(55);
  const Int_t NRGBs = 5;
  //const Int_t NCont = 255;
  const Int_t NCont = 16;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
}
void plotDauKin(TString file, const char* par)
{
  setPalette();
  gStyle->SetOptStat(0);
  const size_t ny = 3;
  std::array<std::string, ny> yranges = {"0", "1", "2"};
  
  const size_t npt = 7;
  std::array<double, npt+1> ptbins{0, 2., 3., 4., 5., 6., 8., 10.};

  std::map<std::string, TH3D*> h3DDauPVsDauEtaVsDsPt;
  std::map<std::string, TH2D*> h2DDauPVsDauEta;

  TFile* f = TFile::Open(file, "READ");

  TLatex tex;
  tex.SetTextSize(0.04);
  tex.SetTextAlign(22);
  tex.SetTextFont(42);

  for (const auto iy : yranges) {
    f->GetObject(Form("hDauPVsDauEtaVsDsPtY%s%s", iy.c_str(), par), h3DDauPVsDauEtaVsDsPt[iy]);
    for (size_t ipt=0; ipt<npt; ipt++) {
      auto beg = h3DDauPVsDauEtaVsDsPt[iy]->GetXaxis()->FindBin(ptbins.at(ipt));
      auto end = h3DDauPVsDauEtaVsDsPt[iy]->GetXaxis()->FindBin(ptbins.at(ipt+1)) - 1;
      h3DDauPVsDauEtaVsDsPt[iy]->GetXaxis()->SetRange(beg, end);
      const std::string iptiy( Form("Ds_pT%gto%g_%s", ptbins.at(ipt), ptbins.at(ipt+1), iy.c_str()) );
      h2DDauPVsDauEta[iptiy] = (TH2D*) h3DDauPVsDauEtaVsDsPt[iy]->Project3D("zy");
      h2DDauPVsDauEta[iptiy]->SetName(iptiy.c_str());
      h2DDauPVsDauEta[iptiy]->SetTitle(iptiy.c_str());
      TCanvas c("c", "", 600, 450);
      c.SetLogz();
      c.SetLeftMargin(0.14);
      c.SetBottomMargin(0.14);
      h2DDauPVsDauEta[iptiy]->GetXaxis()->SetTitleSize(0.05);
      h2DDauPVsDauEta[iptiy]->GetYaxis()->SetTitleSize(0.05);
      h2DDauPVsDauEta[iptiy]->Draw("COLZ");
      tex.DrawLatex(0, 6, Form("Entries:%g", h2DDauPVsDauEta[iptiy]->GetEntries()));
      c.Print(Form("plots/%s%s.pdf", iptiy.c_str(), par));
      c.Print(Form("plots/%s%s.png", iptiy.c_str(), par));
    }
  }
  f->Close();
}
