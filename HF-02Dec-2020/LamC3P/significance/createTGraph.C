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
      sF.push_back( l + "layer");
    }
    else {
      for (const auto w : WhereLost) {
        for (const auto& e : InEff) {
          if (l != "1") {
            sF.push_back( l + "layer_" + w + "_inEff" + e);
            bF.push_back(l + "layer_" + w + "_inEff" + e);
          } else {
            sF.push_back(  l + "layer_inEff" + e);
            bF.push_back( l + "layer_inEff" + e );
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
  const double ptedges[n+1] = {0, 0.5, 1., 2., 3., 4., 5., 6., 8., 10.};
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
    for (int k=0; k<3; k++){
      for (int j=0; j<3; j++){
        sig[j][k] = 0;
      }
    }
    for (int iy=0; iy<3; iy++) {
      vg[iy].push_back( new TGraph(n, pt, sig[iy]));
    }
  }

  for (int i=0; i<3; i++) {
    for (auto & g : vg[i]) {
      g->SetMarkerSize(1.4);
      //g->Print();
    }
  }

  TFile* ofile[fsF.size()];
  for (int i=0; i<fsF.size(); i++) {
    ofile[i] = TFile::Open(Form("sigMB_%s.root", sF[i].c_str()), "recreate");
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
    sig[iy][0] = 0; sig[iy][1] = 0; sig[iy][2] = 0;
    g[iy] = new TGraph(n, pt, sig[iy]);
    g[iy]->SetMarkerSize(1.4);
    g[iy]->Write(Form("gy%d", iy));
    delete g[iy];
  }
  otdr.Close();
    
  for (size_t i=0; i<fsF.size(); i++){
    fsF[i].close();
  }
}
