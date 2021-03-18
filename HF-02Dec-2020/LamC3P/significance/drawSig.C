void drawWInEff(const std::map<std::string, TGraph*>& g,
    TGraph* gtdr, const int iy, const int where)
{
  std::string w;
  if (where == 0) w = "InnerLost";
  if (where == 1) w = "OuterLost";
  TCanvas* c = new TCanvas("c1", "", 600, 500);
  gStyle->SetOptStat(0);
  auto htemp = gtdr->GetHistogram();
  auto h = (TH1*) htemp->Clone(Form("hframe%d%d", iy, where));
  h->SetTitle(Form("%s, #Lambda_{c}: %d<|y|<%d;#Lambda_{c} pT (GeV);Significance", w.c_str(), iy, iy+1));
  h->GetXaxis()->SetLimits(2., 10.);
  auto max = h->GetMaximum();
  auto min = h->GetMinimum();
  h->GetYaxis()->SetRangeUser(min*0.9, max*1.05);
  h->Draw();
  gtdr->SetMarkerStyle(kOpenCircle);
  gtdr->Draw("PSAME");

  g.at("1layer_inEffTrue")->SetMarkerStyle(kFullCircle);
  g.at("1layer_inEffTrue")->SetMarkerColor(2);
  g.at(Form("1p5layer_%d_inEffTrue", where))->SetMarkerStyle(kOpenSquare);
  g.at(Form("0p5layer_%d_inEffTrue", where))->SetMarkerStyle(kFullSquare);
  g.at(Form("0layer"))->SetMarkerStyle(kOpenCross);
  g.at(Form("0p5layer_%d_inEffTrue", where))->SetMarkerColor(9);

  g.at("0layer")->Draw("PSAME");
  g.at("1layer_inEffTrue")->Draw("PSAME");
  g.at(Form("1p5layer_%d_inEffTrue", where))->Draw("PSAME");
  g.at(Form("0p5layer_%d_inEffTrue", where))->Draw("PSAME");

  TLegend* leg = new TLegend(0.1, 0.65, 0.45, 0.9);
  /*
  leg->AddEntry(gtdr, "TDR", "p");
  //leg->AddEntry(g.at("1layer_inEffTrue"), "1layer-WInEff.", "p");
  //leg->AddEntry(g.at(Form("1p5layer_%d_inEffTrue", where)), "1.5layer-WInEff.", "p");
  //leg->AddEntry(g.at(Form("0p5layer_%d_inEffTrue", where)), "0.5layer-WInEff.", "p");
  leg->AddEntry(g.at(Form("1p5layer_%d_inEffTrue", where)), "1.5layer", "p");
  leg->AddEntry(g.at("1layer_inEffTrue"), "1layer", "p");
  leg->AddEntry(g.at(Form("0p5layer_%d_inEffTrue", where)), "0.5layer", "p");
  leg->AddEntry(g.at("0layer"), "0layer", "p");
  */
  leg->AddEntry(gtdr, "2 ETL layers (TDR)", "p");
  leg->AddEntry(g.at(Form("1p5layer_%d_inEffTrue", where)), "1.5 ETL layers", "p");
  leg->AddEntry(g.at("1layer_inEffTrue"), "1 ETL layer", "p");
  leg->AddEntry(g.at(Form("0p5layer_%d_inEffTrue", where)), "0.5 ETL layer", "p");
  leg->AddEntry(g.at("0layer"), "No ETL layer", "p");
  leg->Draw();

  if (where == 0)c->Print(Form("drawWInEffInner_y%d.png", iy));
  if (where == 1)c->Print(Form("drawWInEffOuter_y%d.png", iy));
  delete c;
}

void drawInOut(const std::map<std::string, TGraph*>& g, TGraph* gtdr,
    const int iy, const char* layer)
{
  TCanvas* c = new TCanvas("c1", "", 600, 500);
  gStyle->SetOptStat(0);
  auto htemp = gtdr->GetHistogram();
  auto h = (TH1*) htemp->Clone(Form("hframe%d%s", iy, layer));
  h->SetTitle(Form("#Lambda_{c}: %d<|y|<%d;#Lambda_{c} pT (GeV);Significance", iy, iy+1));
  h->GetXaxis()->SetLimits(2., 10.);
  auto max = h->GetMaximum();
  auto min = h->GetMinimum();
  h->GetYaxis()->SetRangeUser(min*0.9, max*1.05);
  h->Draw();

  g.at(Form("%slayer_0_inEffTrue", layer))->SetMarkerStyle(kOpenCircle);
  g.at(Form("%slayer_0_inEffTrue", layer))->SetMarkerColor(kRed);
  g.at(Form("%slayer_1_inEffTrue", layer))->SetMarkerStyle(kFullCircle);
  g.at(Form("%slayer_1_inEffTrue", layer))->SetMarkerColor(kBlue);

  g.at(Form("%slayer_0_inEffTrue", layer))->Draw("PSAME");
  g.at(Form("%slayer_1_inEffTrue", layer))->Draw("PSAME");

  TLegend* leg = new TLegend(0.1, 0.65, 0.45, 0.9);
  TString l(layer);
  if (l == "1p5") l = "1.5 ETL layers";
  else if (l == "0p5") l = "0.5 ETL layer";
  else {
    delete c;
    return;
  }
  leg->AddEntry(g.at(Form("%slayer_0_inEffTrue", layer)), Form("%s, outer kept", l.Data()), "p");
  leg->AddEntry(g.at(Form("%slayer_1_inEffTrue", layer)), Form("%s, inner kept", l.Data()), "p");
  leg->Draw();

  c->Print(Form("drawInOut_y%d_%slayer.png", iy, layer));
  delete c;
}

void drawWInEffLowPt(const std::map<std::string, TGraph*>& g,
    TGraph* gtdr, const int iy, const int where)
{
  std::string w;
  if (where == 0) w = "InnerLost";
  if (where == 1) w = "OuterLost";
  TCanvas* c = new TCanvas("c1", "", 600, 500);
  gStyle->SetOptStat(0);
  auto htemp = gtdr->GetHistogram();
  auto h = (TH1*) htemp->Clone(Form("hframe%d%d", iy, where));
  h->SetTitle(Form("%s, #Lambda_{c}: %d<|y|<%d;#Lambda_{c} pT (GeV);Significance", w.c_str(), iy, iy+1));
  //h->GetXaxis()->SetLimits(0., 10.);
  h->GetXaxis()->SetLimits(0., 3.);
  auto max = gtdr->GetY()[3];
  auto min = gtdr->GetY()[0];
  h->GetYaxis()->SetRangeUser(min*0., max*1.3);
  h->Draw();
  gtdr->SetMarkerStyle(kOpenCircle);
  gtdr->Draw("PSAME");

  g.at("1layer_inEffTrue")->SetMarkerStyle(kFullCircle);
  g.at("1layer_inEffTrue")->SetMarkerColor(2);
  g.at(Form("1p5layer_%d_inEffTrue", where))->SetMarkerStyle(kOpenSquare);
  g.at(Form("0p5layer_%d_inEffTrue", where))->SetMarkerStyle(kFullSquare);
  g.at(Form("0p5layer_%d_inEffTrue", where))->SetMarkerColor(9);

  g.at("1layer_inEffTrue")->Draw("PSAME");
  g.at(Form("1p5layer_%d_inEffTrue", where))->Draw("PSAME");
  g.at(Form("0p5layer_%d_inEffTrue", where))->Draw("PSAME");

  TLegend* leg = new TLegend(0.1, 0.65, 0.45, 0.9);
  leg->AddEntry(gtdr, "TDR", "p");
  leg->AddEntry(g.at("1layer_inEffTrue"), "1layer-WInEff.", "p");
  leg->AddEntry(g.at(Form("1p5layer_%d_inEffTrue", where)), "1.5layer-WInEff.", "p");
  leg->AddEntry(g.at(Form("0p5layer_%d_inEffTrue", where)), "0.5layer-WInEff.", "p");
  leg->Draw();

  if (where == 0)c->Print(Form("drawWInEffInner_y%d_LowPt.png", iy));
  if (where == 1)c->Print(Form("drawWInEffOuter_y%d_LowPt.png", iy));
  delete c;
}

void drawWInEffWoInEff(const std::map<std::string, TGraph*>& g,
    TGraph* gtdr, const int iy, const int where, const std::string layer)
{
  std::string w;
  if (where == 0) w = "InnerLost";
  if (where == 1) w = "OuterLost";

  std::string prefix;
  if(layer == "1") prefix = "1layer_inEff";
  else prefix = layer+ Form("layer_%d_inEff", where);

  TCanvas* c = new TCanvas("c1", "", 600, 500);
  gStyle->SetOptStat(0);
  auto htemp = gtdr->GetHistogram();
  auto h = (TH1*) htemp->Clone(Form("hframe%d%d", iy, where));
  h->SetTitle(Form("%s %slayer #Lambda_{c}: %d<|y|<%d;#Lambda_{c} pT (GeV);Significance",
        layer == "1" ? "" : w.c_str(), layer.c_str(), iy, iy+1));
  h->GetXaxis()->SetLimits(2., 10.);
  auto max = h->GetMaximum();
  auto min = h->GetMinimum();
  h->GetYaxis()->SetRangeUser(min*0.9, max*1.1);
  h->Draw();
  gtdr->SetMarkerStyle(kOpenCircle);
  gtdr->Draw("PSAME");

  g.at(Form("%sFalse", prefix.c_str()))->SetMarkerStyle(kFullCircle);
  g.at(Form("%sFalse", prefix.c_str()))->SetMarkerColor(2);
  g.at(Form("%sTrue", prefix.c_str()))->SetMarkerStyle(kFullSquare);
  g.at(Form("%sTrue", prefix.c_str()))->SetMarkerColor(9);

  g.at(Form("%sTrue", prefix.c_str()))->Draw("PSAME");
  g.at(Form("%sFalse", prefix.c_str()))->Draw("PSAME");

  TLegend* leg = new TLegend(0.1, 0.65, 0.45, 0.9);
  leg->AddEntry(gtdr, "TDR", "p");
  leg->AddEntry(g.at(Form("%sTrue",
          prefix.c_str())), "WInEff", "p");
  leg->AddEntry(g.at(Form("%sFalse",
          prefix.c_str())), "WoInEff", "p");
  leg->Draw();

  if(layer == "1") 
    c->Print(Form("draw_%s_y%d.png", prefix.substr(0, prefix.size()-6).c_str(), iy));
  else 
    if (where == 0)c->Print(Form("draw_%s_Inner_y%d.png", prefix.substr(0, prefix.size()-6).c_str(), iy));
  else
    if (where == 1)c->Print(Form("draw_%s_Outer_y%d.png", prefix.substr(0, prefix.size()-6).c_str(), iy));
  delete c;
}

void drawSig()
{
  std::string WhereLost[] = {"0", "1"};
  std::string Layers[] = {"0p5", "1", "1p5", "0"};
  std::string InEff[] = {"True", "False"};
  std::vector<std::string> sF, bF, labels;
  for (const auto& l : Layers) {
    if ( l == "0") {
      labels.push_back(l + "layer");
      sF.push_back(l+"layer");
    } else {
      for (const auto w : WhereLost) {
        for (const auto& e : InEff) {
          if (l != "1") {
            labels.push_back(l + "layer_" + w + "_inEff" + e );
            sF.push_back( l + "layer_" + w + "_inEff" + e);
            bF.push_back( l + "layer_" + w + "_inEff" + e);
          } else {
            labels.push_back(l + "layer_inEff" + e );
            sF.push_back( l + "layer_inEff" + e);
            bF.push_back( l + "layer_inEff" + e);
          }
        }
      }
    }
  }

  TFile* ftdr = TFile::Open("sigTDR.root");
  TGraph* gtdr[3];
  for (int iy=0; iy<3; iy++) {
    ftdr->GetObject(Form("gy%d", iy), gtdr[iy]);
  }
  std::map<std::string, TFile*> files;
  std::map<std::string, TGraph*> graphs[3];
  for (size_t i=0; i<labels.size(); i++) {
    files[labels[i]] = TFile::Open(Form("sigMB_%s.root", sF[i].c_str()));
    const auto f = files[labels[i]];
    if(!f->IsOpen()) {
      std::cout << sF[i] << "not available" << std::endl;
      return;
    }
    for (int iy=0; iy<3; iy++) {
      f->GetObject(Form("gy%d", iy), (graphs[iy])[labels[i]]);
    }
  }

  for (int iy=0; iy<3; iy++) {
    // inner
    drawWInEff(graphs[iy], gtdr[iy], iy, 0);
    //drawWInEffLowPt(graphs[iy], gtdr[iy], iy, 0);
    // outer 
    drawWInEff(graphs[iy], gtdr[iy], iy, 1);
    //drawWInEffLowPt(graphs[iy], gtdr[iy], iy, 1);
  }

  for (int iy=0; iy<3; iy++) {
    for (const auto& l : Layers) {
      // inner
      //drawWInEffWoInEff(graphs[iy], gtdr[iy], iy, 0, l);
      // outer 
      //drawWInEffWoInEff(graphs[iy], gtdr[iy], iy, 1, l);
    }
  }
  
  for (int iy=0; iy<3; iy++) {
    drawInOut(graphs[iy], gtdr[iy], iy, "1p5");
    drawInOut(graphs[iy], gtdr[iy], iy, "0p5");
  }
}
