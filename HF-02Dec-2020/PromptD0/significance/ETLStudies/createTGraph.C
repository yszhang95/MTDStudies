#include <fstream>
void readFromTxt(ifstream& fIn, float sig[][9])
{
  std::string line;
  int j=0;
  while (std::getline(fIn, line)) {
    std::stringstream  linestream(line);
    //std::cout << line << std::endl;
    vector<string> tokens;
    copy(istream_iterator<string>(linestream),
        istream_iterator<string>(),
        back_inserter(tokens));
    for(int i=0; i<3; i++){
      float temp = std::stof(tokens[i].substr(0, tokens[i].size()-1));
      sig[i][j] = temp;
    }
    j++;
  }
}
void createTGraph()
{
  std::string WhereLost[] = {"0", "1"};
  std::string Layers[] = {"0p5", "1", "1p5", "0"};
  std::string InEff[] = {"True", "False"};
  std::vector<std::string> sF, bF;
  for (const auto& l : Layers) {
    if (l == "0") {
      sF.push_back("PromptDMassHists_reRECO_all_" + l + "layer.root");
      bF.push_back("HyJetsMassHists_reRECO_tdr.list_" + l + "layer.root");
    } else {
      for (const auto w : WhereLost) {
        for (const auto& e : InEff) {
          if (l != "1") {
            sF.push_back( "PromptDMassHists_reRECO_all_" + l + "layer_" + w + "_inEff" + e + ".root");
            bF.push_back("HyJetsMassHists_reRECO_tdr.list_" + l + "layer_" + w + "_inEff" + e + ".root");
          } else {
            sF.push_back( "PromptDMassHists_reRECO_all_" + l + "layer_inEff" + e + ".root");
            bF.push_back("HyJetsMassHists_reRECO_tdr.list_" + l + "layer_inEff" + e + ".root");
          }
        }
      }
    }
  }
  std::vector<ifstream> fsF(sF.size());
  for (size_t i=0; i<fsF.size(); i++) {
    fsF[i].open("sigMB_"+sF.at(i)+".txt");
  }

  ifstream fTDR("sigMBOld.txt");

  const unsigned int n = 9;
  const double ptedges[n+1] = {0, 0.5, 1, 2., 3., 4., 5., 6., 8., 10.};
  const float pt[n] = {0.25, 0.75, 1.5, 2.5, 3.5, 4.5, 5.5, 7., 9.};

  float sig[3][n] = {0.};

  std::vector<TGraph*> vg[3];
  for (size_t i=0; i<fsF.size(); i++){
    readFromTxt(fsF[i], sig);
    for (int k=0; k<9; k++){
      for (int j=0; j<3; j++){
        //std::cout << sig[j][k] << ", ";
      }
      //std::cout << std::endl;
    }
    for (int iy=0; iy<3; iy++) {
      vg[iy].push_back( new TGraph(n, pt, sig[iy]));
    }
  }

  for (int i=0; i<3; i++) {
    for (auto & g : vg[i]) {
      g->SetMarkerSize(1.4);
    }
  }

  TFile* ofile[fsF.size()];
  for (int i=0; i<fsF.size(); i++) {
    ofile[i] = TFile::Open(Form("sigMB_%s", sF[i].c_str()), "recreate");
    ofile[i]->cd();
    for (int jy=0; jy<3; jy++) {
      vg[jy].at(i)->Write(Form("gy%d", jy));
      delete vg[jy].at(i);
    }
    ofile[i]->Close();
  }

  readFromTxt(fTDR, sig);
  TGraph* g[3];
  TFile otdr ("sigTDR.root", "recreate");
  for (int iy=0; iy<3; iy++) {
    g[iy] = new TGraph(n, pt, sig[iy]);
    g[iy]->SetMarkerSize(1.4);
    g[iy]->Write(Form("gy%d", iy));
    delete g[iy];
  }
  otdr.Close();
    
  /*
  TH1D* hSigTDR[3];
  TH1D* hSigWoInEff[3];
  TH1D* hSigWInEff[3];
  TCanvas* c[3];
  TLegend* leg[3];
  for(int i=0; i<3; i++){
    gStyle->SetOptStat(0);
    c[i] = new TCanvas(Form("c%d", i), "", 600, 500);
    hSigTDR[i] = new TH1D(Form("hy%d_TDR", i),";pT (GeV);Significance", n, ptedges);
    hSigWoInEff[i] = new TH1D(Form("hy%d_WoInEff", i),";pT (GeV);Significance", n, ptedges);
    hSigWInEff[i] = new TH1D(Form("hy%d_WInEff", i),";pT (GeV);Significance", n, ptedges);
    hSigTDR[i]->SetLineColor(kBlack);
    hSigWoInEff[i]->SetMarkerSize(1.5);
    hSigWoInEff[i]->SetMarkerStyle(kFullCircle);
    hSigWoInEff[i]->SetLineColor(kRed);
    hSigWInEff[i]->SetMarkerSize(1.5);
    hSigWInEff[i]->SetMarkerStyle(kOpenCircle);
    for(int j=0; j<n; j++){
      hSigTDR[i]->SetBinContent(j+1, sigTDR[i].at(j));
      hSigWoInEff[i]->SetBinContent(j+1, sigWoInEff[i].at(j));
      hSigWInEff[i]->SetBinContent(j+1, sigWInEff[i].at(j));
      hSigTDR[i]->SetBinError(j+1, 0);
      hSigWoInEff[i]->SetBinError(j+1, 0);
      hSigWInEff[i]->SetBinError(j+1, 0);
    }
    hSigTDR[i]->SetTitle(Form("D0: %.1d<|y|<%.1d", i, i+1));
    hSigTDR[i]->Draw();
    hSigWoInEff[i]->Draw("P SAME");
    hSigWInEff[i]->Draw("P SAME");
    leg[i] = new TLegend(0.1, 0.75, 0.4, 0.9);
    leg[i]->AddEntry(hSigTDR[i], "TDR", "l");
    leg[i]->AddEntry(hSigWoInEff[i], "SmearWoInEff", "p");
    leg[i]->AddEntry(hSigWInEff[i], "SmearWInEff", "p");
    leg[i]->Draw();
    c[i]->Print(Form("y%d.png", i));
  }
  */

  for (size_t i=0; i<fsF.size(); i++){
    fsF[i].close();
  }
}
