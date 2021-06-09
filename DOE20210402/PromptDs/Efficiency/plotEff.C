void plotEff()
{
  TFile* f = TFile::Open("output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root");
  TH2D* hGenYVsPt, *hAccGenYVsPt, *hRecoYVsPt, *hLooseRecoYVsPt, *hFinalRecoYVsPt;
  f->GetObject("hGenYVsPt", hGenYVsPt);
  f->GetObject("hAccGenYVsPt", hAccGenYVsPt);
  f->GetObject("hRecoYVsPt", hRecoYVsPt);
  f->GetObject("hLooseRecoYVsPt", hLooseRecoYVsPt);
  f->GetObject("hFinalRecoYVsPt", hFinalRecoYVsPt);

  TLatex tex;
  tex.SetNDC();

  // gen
  auto hall = hGenYVsPt->ProjectionX("all", 3, 4);
  // acceptance
  auto hacc = hAccGenYVsPt->ProjectionX("acc", 3, 4);
  hacc->Divide(hall);
  hacc->SetTitle("Acceptance");
  hacc->GetYaxis()->SetTitle("Acceptance");
  TCanvas cacc("caac", "", 600, 480);
  hacc->Draw();
  tex.DrawLatex(0.18, 0.65, "|y|<1 PbPb 5.5TeV");
  cacc.Print("acceptance.png");
  cacc.Print("acceptance.pdf");
  // reco
  auto hreco = hRecoYVsPt->ProjectionX("reco", 3, 4);
  hreco->Divide(hall);
  hreco->SetTitle("Reconstruction efficiency");
  hreco->GetYaxis()->SetTitle("Acc. * eff.");
  TCanvas creco("creco", "", 600, 480);
  hreco->Draw();
  tex.DrawLatex(0.18, 0.65, "|y|<1 PbPb 5.5TeV");
  creco.Print("reco_eff.png");
  creco.Print("reco_eff.pdf");
  // loose
  auto hloose = hLooseRecoYVsPt->ProjectionX("loose", 3, 4);
  hloose->Divide(hall);
  hloose->SetTitle("3D Pointing angle < 0.45");
  hloose->GetYaxis()->SetTitle("Acc. * eff.");
  TCanvas cloose("cloose", "", 600, 480);
  hloose->Draw();
  tex.DrawLatex(0.18, 0.65, "|y|<1 PbPb 5.5TeV");
  cloose.Print("loose_eff.png");
  cloose.Print("loose_eff.pdf");
  // final
  auto hfinal = hFinalRecoYVsPt->ProjectionX("final", 3, 4);
  hfinal->Divide(hall);
  hfinal->SetTitle("All topo. cuts applied");
  hfinal->GetYaxis()->SetTitle("Acc. * eff.");
  TCanvas cfinal("cfinal", "", 600, 480);
  hfinal->Draw();
  tex.DrawLatex(0.18, 0.65, "|y|<1 PbPb 5.5TeV");
  cfinal.Print("final_eff.png");
  cfinal.Print("final_eff.pdf");
}
