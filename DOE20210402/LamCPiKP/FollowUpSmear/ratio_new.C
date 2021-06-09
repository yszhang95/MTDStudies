#include "makeMultiPanelCanvas.C"

void ratio_new()
{
  gStyle->SetOptStat(0);

  double pT_ppAlice[] = {1.50,2.50,3.50,5.00,7.00};
  double ratio_ppAlice[] = {0.568,0.546,0.532,0.464,0.386};
  double pT_ppAlice_err[] = {0,0,0,0,0};
  double ratio_ppAlice_err[] = {0.118,0.075,0.066,0.048,0.076};

  double pT_pPbAlice[] = {3.00,5.00,7.00,10.00};
  double ratio_pPbAlice[] = {0.638,0.480,0.529,0.399};
  double pT_pPbAlice_err[] = {0,0,0,0};
  double ratio_pPbAlice_err[] = {0.077,0.063,0.081,0.062};

  double pT_PbPbAlice[] = {9.00};
  double ratio_PbPbAlice[] = {1.06};
  double pT_PbPbAlice_err[] = {0};
  double ratio_PbPbAlice_err[] = {0.19};

  TGraphErrors* grdata_ppAlice = new TGraphErrors(5,pT_ppAlice,ratio_ppAlice,pT_ppAlice_err,ratio_ppAlice_err);
  grdata_ppAlice->SetMarkerStyle(28);
  grdata_ppAlice->SetMarkerSize(1.7);
  grdata_ppAlice->SetLineWidth(2);

  TGraphErrors* grdata_pPbAlice = new TGraphErrors(4,pT_pPbAlice,ratio_pPbAlice,pT_pPbAlice_err,ratio_pPbAlice_err);
  grdata_pPbAlice->SetMarkerStyle(27);
  grdata_pPbAlice->SetMarkerSize(1.8);
  grdata_pPbAlice->SetLineWidth(2);

  TGraphErrors* grdata_PbPbAlice = new TGraphErrors(1,pT_PbPbAlice,ratio_PbPbAlice,pT_PbPbAlice_err,ratio_PbPbAlice_err);
  grdata_PbPbAlice->SetMarkerStyle(25);
  grdata_PbPbAlice->SetMarkerSize(1.4);
  grdata_PbPbAlice->SetLineWidth(2);

  double pT1[] = {0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.0,8.5,9.0,9.5};   // coalescence only
  double ratio1[] = {1.82,1.91,2.05,2.25,2.43,2.55,2.58,2.5,2.38,2.23,2.0,1.8,1.57,1.4,1.25,1.05,0.85,0.67,0.56};  // coalescence only
  double pT2[] = {0.25,0.75,1.,1.5,2.0,2.5,3.,3.5,4.0,4.5,5.0,5.5,6.,6.5,7.,7.5,8.0,8.5,9.,10,11,12}; // coalescence plus fragmentation
  double ratio2[] = {0.5,0.47,0.5,0.6,0.7,0.73,0.75,0.7,0.6,0.53,0.45,0.38,0.3,0.24,0.2,0.18,0.15,0.12,0.11,0.1,0.09,0.08}; // coalescence plus fragmentation

  double pT3[] = {0.5,1.,1.5,2.0,2.5,3.,3.5,4.0,4.5,5.0,5.5,6.,6.5,7.,7.5,8.0,8.5,9.,9.5}; 
  double ratio3[] = {1.15,1.21,1.32,1.48,1.58,1.65,1.59,1.52,1.42,1.3,1.15,1.02,0.89,0.79,0.7,0.58,0.48,0.39,0.33};   

  TGraph* gr1 = new TGraph(19,pT1,ratio1);
  TGraph* gr2 = new TGraph(22,pT2,ratio2);
  TGraph* gr3 = new TGraph(19,pT3,ratio3);

  gr1->SetLineWidth(2);
  gr2->SetLineWidth(2);
  gr3->SetLineWidth(1);
  gr1->SetLineColor(2);
  gr2->SetLineColor(4);
  gr3->SetLineColor(1);
//  gr1->SetLineStyle(7);
//  gr2->SetLineStyle(7);
  gr3->SetLineStyle(7);

 double pTdata[] = {1.6, 2.6,3.6,4.6,5.6,7.1,9.1};
//  double ratiodata[] = {1.58,1.59,1.42,1.15,0.79,0.39}; // average
  double ratiodata[] = {2.05, 2.43,2.58,2.38,2.0,1.4,0.67};
  double pTdata_err[] = {0, 0,0,0,0,0,0};
  double ratiodata_err[] = {0, 0,0,0,0,0,0};
  double ratiodata_y2_err[] = {0, 0,0,0,0,0,0};
  double ratiodata_y3_err[] = {0, 0,0,0,0,0,0};

  double d0sig[] = {166, 607.046,1173.28,751.291,602.09,2268.25,1378.39};
  double d0sig_y2[] = {140, 265,306,232,190,377,226};
  double d0sig_y3[] = {101, 122,132,97,86,142,97};

  double lamcsig[] = {1.7, 4.5,8,14.2,20.1,36.5,70.5};
  double lamcsig_y2[] = {5.4, 6.84,5.5,5.2,5,13,17};
  //double lamcsig_y3[] = {1.6, 2.3,2.2,1.7,2.6,6.4,7.4};
  double lamcsig_y3[] = {1.6, 2.3,2.2,1.7,2.6,6.4,9.7};

  double pTdata_womtd[] = {1.4, 2.4,3.4,4.4,5.4,6.9,8.9};
  double ratiodata_womtd[] = {2.05, 2.43,2.58,2.38,2.0,1.4,0.67};
  double pTdata_womtd_err[] = {0, 0,0,0,0,0,0};
  double ratiodata_womtd_err[] = {0, 0.,0.,0.,0.,0.,0.};
  double ratiodata_y2_womtd_err[] = {0, 0.,0.,0.,0.,0.,0.};
  double ratiodata_y3_womtd_err[] = {0, 0.,0.,0.,0.,0.,0.};

  double d0sig_womtd[] = {66, 418.557,845.593,534.19,506.072,2749.32,1708.89};
  double d0sig_y2_womtd[] = {83, 219,258,199,153,355,216};
  double d0sig_y3_womtd[] = {63, 110,126,100,88,161,98};

  double lamcsig_womtd[] = {1000, 0.721615,1.64138,4.51725,7.8896,21.7051,55.1349};
  double lamcsig_y2_womtd[] = {1000, 1.8,2.2,3.0,3.8,12.9,25.7};
  double lamcsig_y3_womtd[] = {1000, 1.5,1.69,1.63,2.1,6.3,9.7};

  for(int i=0;i<7;i++) 
  {
    ratiodata_err[i] = sqrt(1./d0sig[i]/d0sig[i]+1./lamcsig[i]/lamcsig[i]/ratiodata[i]/ratiodata[i])*ratiodata[i];
    ratiodata_womtd_err[i] = sqrt(1./d0sig_womtd[i]/d0sig_womtd[i]+1./lamcsig_womtd[i]/lamcsig_womtd[i]/ratiodata_womtd[i]/ratiodata_womtd[i])*ratiodata_womtd[i];

    ratiodata_y2_err[i] = sqrt(1./d0sig_y2[i]/d0sig_y2[i]+1./lamcsig_y2[i]/lamcsig_y2[i]/ratiodata[i]/ratiodata[i])*ratiodata[i];
    ratiodata_y2_womtd_err[i] = sqrt(1./d0sig_y2_womtd[i]/d0sig_y2_womtd[i]+1./lamcsig_y2_womtd[i]/lamcsig_y2_womtd[i]/ratiodata_womtd[i]/ratiodata_womtd[i])*ratiodata_womtd[i];

    ratiodata_y3_err[i] = sqrt(1./d0sig_y3[i]/d0sig_y3[i]+1./lamcsig_y3[i]/lamcsig_y3[i]/ratiodata[i]/ratiodata[i])*ratiodata[i];
    ratiodata_y3_womtd_err[i] = sqrt(1./d0sig_y3_womtd[i]/d0sig_y3_womtd[i]+1./lamcsig_y3_womtd[i]/lamcsig_y3_womtd[i]/ratiodata_womtd[i]/ratiodata_womtd[i])*ratiodata_womtd[i];
  }

  TGraphErrors* grdata = new TGraphErrors(7,pTdata,ratiodata,pTdata_err,ratiodata_err);
  grdata->SetMarkerStyle(20);
  grdata->SetMarkerSize(1.4);
  grdata->SetMarkerColor(kRed+3);
  grdata->SetLineColor(kRed+3);
  grdata->SetLineWidth(2);

  TGraphErrors* grdata_womtd = new TGraphErrors(7,pTdata_womtd,ratiodata_womtd,pTdata_womtd_err,ratiodata_womtd_err);
  grdata_womtd->SetMarkerStyle(24);
  grdata_womtd->SetMarkerSize(1.3);
  grdata_womtd->SetLineWidth(2);
  grdata_womtd->SetLineColor(kRed+3);
  grdata_womtd->SetMarkerColor(kRed+3);
  grdata_womtd->RemovePoint(0);
  grdata_womtd->RemovePoint(0);

  TGraphErrors* grdata_y2 = new TGraphErrors(7,pTdata,ratiodata,pTdata_err,ratiodata_y2_err);
  grdata_y2->SetMarkerStyle(20);
  grdata_y2->SetMarkerSize(1.4);
  grdata_y2->SetMarkerColor(kRed+3);
  grdata_y2->SetLineColor(kRed+3);
  grdata_y2->SetLineWidth(2);

  TGraphErrors* grdata_y2_womtd = new TGraphErrors(7,pTdata_womtd,ratiodata_womtd,pTdata_womtd_err,ratiodata_y2_womtd_err);
  grdata_y2_womtd->SetMarkerStyle(24);
  grdata_y2_womtd->SetMarkerSize(1.3);
  grdata_y2_womtd->SetLineWidth(2);
  grdata_y2_womtd->SetLineColor(kRed+3);
  grdata_y2_womtd->SetMarkerColor(kRed+3);
  grdata_y2_womtd->RemovePoint(0);
  grdata_y2_womtd->RemovePoint(0);

  TGraphErrors* grdata_y3 = new TGraphErrors(7,pTdata,ratiodata,pTdata_err,ratiodata_y3_err);
  grdata_y3->SetMarkerStyle(20);
  grdata_y3->SetMarkerSize(1.4);
  grdata_y3->SetMarkerColor(kRed+3);
  grdata_y3->SetLineColor(kRed+3);
  grdata_y3->SetLineWidth(2);

  TGraphErrors* grdata_y3_womtd = new TGraphErrors(7,pTdata_womtd,ratiodata_womtd,pTdata_womtd_err,ratiodata_y3_womtd_err);
  grdata_y3_womtd->SetMarkerStyle(24);
  grdata_y3_womtd->SetMarkerSize(1.3);
  grdata_y3_womtd->SetLineWidth(2);
  grdata_y3_womtd->SetLineColor(kRed+3);
  grdata_y3_womtd->SetMarkerColor(kRed+3);
  grdata_y3_womtd->RemovePoint(0);
  grdata_y3_womtd->RemovePoint(0);

  TH2D* htmp = new TH2D("htmp",";p_{T} [GeV];#Lambda_{c}/D^{0}",100,0.05,10.5,100,0,3.99);
  htmp->SetLineWidth(0);
  htmp->GetXaxis()->CenterTitle();
  htmp->GetYaxis()->CenterTitle();
  htmp->GetXaxis()->SetTitleSize(htmp->GetXaxis()->GetTitleSize()*1.4);  
  htmp->GetYaxis()->SetTitleSize(htmp->GetYaxis()->GetTitleSize()*1.6); 
  htmp->GetXaxis()->SetLabelSize(htmp->GetXaxis()->GetLabelSize()*1.1);
  htmp->GetYaxis()->SetLabelSize(htmp->GetYaxis()->GetLabelSize()*1.1);

  TH2D* htmp1 = (TH2D*)htmp->Clone("htmp1");
  TH2D* htmp2 = (TH2D*)htmp->Clone("htmp2");
  TH2D* htmp3 = (TH2D*)htmp->Clone("htmp3");

  TLegend* lcalc = new TLegend(0.55,0.74,0.75,0.88);
  lcalc->SetBorderSize(0);
  lcalc->SetFillStyle(0);
  lcalc->SetTextSize(0.035);
  lcalc->SetHeader("Models: 0-80% PbPb 5 TeV");
  lcalc->AddEntry(gr1,"Catania Coal. only","L");
  lcalc->AddEntry(gr2,"Catania Coal. + Frag.","L");
//  lcalc->AddEntry(gr3,"Average","L");

  TCanvas* c = new TCanvas("c","c",580,520);
  c->SetLeftMargin(0.12);
  c->SetBottomMargin(0.12);
  htmp->Draw(); 
  //grdata_ppAlice->Draw("PSAME");
  //grdata_pPbAlice->Draw("PSAME");
  //grdata_PbPbAlice->Draw("PSAME");
  gr1->Draw("CSAME");
  gr2->Draw("CSAME");
//  gr3->Draw("CSAME");
  grdata->Draw("PESAME");
  grdata_womtd->Draw("PESAME");
  lcalc->Draw();

  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.045);
  latex->DrawLatex(0.07,0.92,"#font[62]{CMS} #font[52]{Phase-2 Simulation}");
  latex->DrawLatex(0.55,0.925,"#font[42]{PbPb 3 nb^{-1} (5.5 TeV)}");
//  latex->DrawLatex(0.6,0.5, #font[52]{Preliminary}");

  TLegend* lc = new TLegend(0.14,0.7,0.36,0.88);
  lc->SetBorderSize(0);
  lc->SetFillStyle(0);
  lc->SetTextSize(0.045);
  lc->SetHeader("0-100%, |y|<1");
  lc->AddEntry(grdata,"MTD","P");
  lc->AddEntry(grdata_womtd,"no MTD","P");
  lc->Draw();

  TLegend* lc2 = new TLegend(0.04,0.73,0.3,0.91);
  lc2->SetBorderSize(0);
  lc2->SetFillStyle(0);
  lc2->SetTextSize(0.065);
  lc2->SetHeader("0-100%, 1<|y|<2");
  lc2->AddEntry(grdata,"MTD","P");
  lc2->AddEntry(grdata_womtd,"no MTD","P");

  TLegend* lc3 = new TLegend(0.03,0.73,0.29,0.91);
  lc3->SetBorderSize(0);
  lc3->SetFillStyle(0);
  lc3->SetTextSize(0.06);
  lc3->SetHeader("0-100%, 2<|y|<3");
  lc3->AddEntry(grdata,"MTD","P");
  lc3->AddEntry(grdata_womtd,"no MTD","P");

  TLegend* lcc = new TLegend(0.54,0.56,0.8,0.73);
  lcc->SetBorderSize(0);
  lcc->SetFillStyle(0);
  lcc->SetTextSize(0.036);
  lcc->SetHeader("ALICE Run 2");
  lcc->AddEntry(grdata_ppAlice,"pp 7 TeV","P");
  lcc->AddEntry(grdata_pPbAlice,"pPb 5 TeV","P");
  lcc->AddEntry(grdata_PbPbAlice,"0-80% PbPb 5 TeV","P");
  //lcc->Draw();


  c->Print("LamCD0Ratio_new.pdf");
  c->Print("LamCD0Ratio_new.png");
//  c->Print("../figures/LamCD0Ratio_new_preliminary.pdf");
//  c->Print("../figures/LamCD0Ratio_new_preliminary.png");

/////// 3-panel plot //////////////////

  TCanvas* c1 = new TCanvas("c1","c1",1200,480);
  makeMultiPanelCanvas(c1,3,1,0.01,0.0,0.15,0.14,0.07);

  c1->GetPad(1)->SetFillStyle(4000);
  c1->GetPad(2)->SetFillStyle(4000);
  c1->GetPad(3)->SetFillStyle(4000);

  c1->cd(1);
  htmp1->GetXaxis()->SetLabelSize(htmp1->GetXaxis()->GetLabelSize()*1.25);
  htmp1->GetXaxis()->SetTitleSize(htmp1->GetXaxis()->GetTitleSize()*1.25);
  htmp1->GetYaxis()->SetLabelSize(htmp1->GetYaxis()->GetLabelSize()*1.25);
  htmp1->GetYaxis()->SetTitleSize(htmp1->GetYaxis()->GetTitleSize()*1.25);
  htmp1->GetXaxis()->SetTitleOffset(htmp1->GetXaxis()->GetTitleOffset()*0.95);
  htmp1->Draw();
  //grdata_ppAlice->Draw("PSAME");
  //grdata_pPbAlice->Draw("PSAME");
  //grdata_PbPbAlice->Draw("PSAME");
  gr1->Draw("CSAME");
  gr2->Draw("CSAME");
  grdata->Draw("PESAME");
  grdata_womtd->Draw("PESAME");

  TLegend* llc = new TLegend(0.18,0.73,0.41,0.91);
  llc->SetBorderSize(0);
  llc->SetFillStyle(0);
  llc->SetTextSize(0.055);
  llc->SetHeader("0-100%, |y|<1");
  llc->AddEntry(grdata,"MTD","P");
  llc->AddEntry(grdata_womtd,"no MTD","P");
  llc->Draw();

  TLegend* llcalc = new TLegend(0.52,0.77,0.72,0.91);
  llcalc->SetBorderSize(0);
  llcalc->SetFillStyle(0);
  llcalc->SetTextSize(0.04);
  llcalc->SetHeader("Models: 0-80% PbPb 5 TeV");
  llcalc->AddEntry(gr1,"Catania Coal. only","L");
  llcalc->AddEntry(gr2,"Catania Coal. + Frag.","L");
  llcalc->Draw();

  TLegend* llcc = new TLegend(0.6,0.58,0.86,0.75);
  llcc->SetBorderSize(0);
  llcc->SetFillStyle(0);
  llcc->SetTextSize(0.041);
  llcc->SetHeader("ALICE Run 2");
  llcc->AddEntry(grdata_ppAlice,"pp 7 TeV","P");
  llcc->AddEntry(grdata_pPbAlice,"pPb 5 TeV","P");
  llcc->AddEntry(grdata_PbPbAlice,"0-80% PbPb 5 TeV","P");
  //llcc->Draw();

  //TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.065);
  latex->DrawLatex(0.14,0.955,"#font[62]{CMS} #font[52]{Phase-2 Simulation}");
//  latex->DrawLatex(0.55,0.965,"#font[42]{PbPb 3 nb^{-1} (5.5 TeV)}");

  c1->cd(2);
  htmp2->GetXaxis()->SetLabelSize(htmp2->GetXaxis()->GetLabelSize()*1.5);
  htmp2->GetXaxis()->SetTitleSize(htmp2->GetXaxis()->GetTitleSize()*1.5);
  htmp2->GetYaxis()->SetLabelSize(htmp2->GetYaxis()->GetLabelSize()*1.45);
  htmp2->GetYaxis()->SetTitleSize(htmp2->GetYaxis()->GetTitleSize()*1.45);
  htmp2->GetXaxis()->SetTitleOffset(htmp2->GetXaxis()->GetTitleOffset()*0.8);
  htmp2->GetXaxis()->SetLabelOffset(htmp2->GetXaxis()->GetLabelOffset()*0.2);
  htmp2->Draw();
//  grdata_ppAlice->Draw("PSAME");
//  grdata_pPbAlice->Draw("PSAME");
//  grdata_PbPbAlice->Draw("PSAME");
  gr1->Draw("CSAME");
  gr2->Draw("CSAME");
  grdata_y2->Draw("PESAME");
  grdata_y2_womtd->Draw("PESAME");
  lc2->Draw();

  c1->cd(3);
  htmp3->GetXaxis()->SetLabelSize(htmp3->GetXaxis()->GetLabelSize()*1.3);
  htmp3->GetXaxis()->SetTitleSize(htmp3->GetXaxis()->GetTitleSize()*1.3);
  htmp3->GetYaxis()->SetLabelSize(htmp3->GetYaxis()->GetLabelSize()*1.3);
  htmp3->GetYaxis()->SetTitleSize(htmp3->GetYaxis()->GetTitleSize()*1.3);
  htmp3->GetXaxis()->SetTitleOffset(htmp3->GetXaxis()->GetTitleOffset()*0.92);
  htmp3->Draw();
//  grdata_ppAlice->Draw("PSAME");
//  grdata_pPbAlice->Draw("PSAME");
//  grdata_PbPbAlice->Draw("PSAME");
  gr1->Draw("CSAME");
  gr2->Draw("CSAME");
  grdata_y3->Draw("PESAME");
  grdata_y3_womtd->Draw("PESAME");
  lc3->Draw();
  latex->DrawLatex(0.35,0.955,"#font[42]{PbPb 3 nb^{-1} (5.5 TeV)}");
//  latex->DrawLatex(0.6,0.7,"#it{Preliminary}");

  c1->Print("LamCD0Ratio_new_allrapidity.pdf");
  c1->Print("LamCD0Ratio_new_allrapidity.png");
//  c1->Print("../figures/LamCD0Ratio_new_allrapidity_preliminary.pdf");
//  c1->Print("../figures/LamCD0Ratio_new_allrapidity_preliminary.png");


}

