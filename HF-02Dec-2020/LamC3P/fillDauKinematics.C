#include "myAna.h"
#include "HyJets.h"
#include "TMath.h"
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "THashList.h"
#include "TLatex.h"
#include "TFileCollection.h"
#include "TVector3.h"
#include "TF1.h"
#include "TH3D.h"
#include "TStopwatch.h"
#include "TSystem.h"

void fillDauKinematics(std::string list = "")
{
  TChain chain("lamc3pana_mc/VertexCompositeNtuple");
  TFileCollection* fc = new TFileCollection("dum", "", list.c_str());
  chain.AddFileInfoList(fc->GetList()); 
  HyJets* t = new HyJets(&chain);
  std::cout << t->GetEntries() << std::endl;

  std::map<std::string, TH3D*> h3DDauPVsDauEtaVsLcPt;
  std::map<std::string, TH3D*> h3DMtdDauPVsDauEtaVsLcPt;
  std::map<std::string, TH3D*> h3DDauPVsDauEtaVsLcPt;
  std::map<std::string, TH3D*> h3DMtdDauPVsDauEtaVsLcPt;
  std::array<std::string, 3> yranges = {"y0to1", "y1to2", "y2to3"};
  for (const auto n : yranges) {
    h3DDauPVsDauEtaVsLcPt[n] = new TH3D(Form("h3DDauPVsDauEtaVsLcPt%s", n.c_str()),
        Form("%s;#lambda_{c}^{#pm} p_{T} (GeV);Dau #eta;Dau p(GeV)", n.c_str()),
        10, 0, 10, 600, -3, 3, 800, 0, 8);
    h3DMtdDauPVsDauEtaVsLcPt[n] = new TH3D(Form("h3DMtdDauPVsDauEtaVsLcPt%s", n.c_str()),
        Form("%s;#lambda_{c}^{#pm} p_{T} (GeV);Dau #eta;Dau p(GeV)", n.c_str()),
        10, 0, 10, 600, -3, 3, 800, 0, 8);
  }

  TStopwatch ts;
  ts.Start();

  for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
    t->GetEntry(ientry);

    if(!ana::passKinematicCuts(t)) continue;

    const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
    const float pD2 = t->pTD2 * std::cosh(t->EtaD2);
    const float pD3 = t->pTD3 * std::cosh(t->EtaD3);

    bool isCentral = ana::isCentralEvt(*t);

    const float dInvBetaCut1 = std::fabs(t->EtaD1<1.5) ? ana::fExpBTL.Eval(pD1) : ana::fExpETL.Eval(pD1);
    const float dInvBetaCut2 = std::fabs(t->EtaD2<1.5) ? ana::fExpBTL.Eval(pD2) : ana::fExpETL.Eval(pD2);
    const float dInvBetaCut3 = std::fabs(t->EtaD3<1.5) ? ana::fExpBTL.Eval(pD3) : ana::fExpETL.Eval(pD3);

    if(!ana::passTopoCuts(t)) continue;
    //if(t->pT < 1) continue;

    bool isFWHM = ana::isFWHM(t);

    auto iy = ana::whichY(t);
    if (iy < 0) continue;
    h3DDauPVsDauEtaVsLcPt[yranges.at(iy)]->Fill(t->pT, t->EtaD1, pD1);
    h3DDauPVsDauEtaVsLcPt[yranges.at(iy)]->Fill(t->pT, t->EtaD2, pD2);
    h3DDauPVsDauEtaVsLcPt[yranges.at(iy)]->Fill(t->pT, t->EtaD3, pD3);
    /*
    if(isCentral) hMassVsPtVsYCent->Fill(std::fabs(t->y), t->pT, t->mass);
    if(t->centrality <100 && t->centrality > 60) hMassVsPtVsY30_50->Fill(std::fabs(t->y), t->pT, t->mass);
    if( isFWHM ) {
      hVtxProbVsPtVsY->Fill(std::fabs(t->y), t->pT, t->VtxProb);
      hagl3DVsPtVsY->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
      hdlSig3DVsPtVsY->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
    }
    if( isFWHM && isCentral ) {
      hVtxProbVsPtVsYCent->Fill(std::fabs(t->y), t->pT, t->VtxProb);
      hagl3DVsPtVsYCent->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
      hdlSig3DVsPtVsYCent->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
    }
    */

    bool is1sigmaPionDau1 = false;
    bool is1sigmaKaonDau1 = false;
    bool is1sigmaProtonDau1 = false;
    bool is1sigmaPionDau2 = false;
    bool is1sigmaKaonDau2 = false;
    bool is1sigmaProtonDau2 = false;
    bool is1sigmaPionDau3 = false;
    bool is1sigmaKaonDau3 = false;
    bool is1sigmaProtonDau3 = false;

    if(t->isMtdDau1) is1sigmaPionDau1 = std::fabs(1./t->beta1_PV - ana::invBetaPion(pD1) - ana::meanPion(t, 1) ) < 1.0 * dInvBetaCut1;
    if(t->isMtdDau1) is1sigmaKaonDau1 = std::fabs(1./t->beta1_PV - ana::invBetaKaon(pD1) - ana::meanKaon(t, 1) ) < 1.0 * dInvBetaCut1;
    if(t->isMtdDau1) is1sigmaProtonDau1 = std::fabs(1./t->beta1_PV - ana::invBetaProton(pD1) - ana::meanProton(t, 1) ) < 1.0 * dInvBetaCut1;
    if(t->isMtdDau2) is1sigmaPionDau2 = std::fabs(1./t->beta2_PV - ana::invBetaPion(pD2) - ana::meanPion(t, 2) ) < 1.0 * dInvBetaCut2;
    if(t->isMtdDau2) is1sigmaKaonDau2 = std::fabs(1./t->beta2_PV - ana::invBetaKaon(pD2) - ana::meanKaon(t, 2) ) < 1.0 * dInvBetaCut2;
    if(t->isMtdDau2) is1sigmaProtonDau2 = std::fabs(1./t->beta2_PV - ana::invBetaProton(pD2) - ana::meanProton(t, 2) ) < 1.0 * dInvBetaCut2;
    if(t->isMtdDau3) is1sigmaPionDau3 = std::fabs(1./t->beta3_PV - ana::invBetaPion(pD3) - ana::meanPion(t, 3) ) < 1.0 * dInvBetaCut3;
    if(t->isMtdDau3) is1sigmaKaonDau3 = std::fabs(1./t->beta3_PV - ana::invBetaKaon(pD3) - ana::meanKaon(t, 3)) < 1.0 * dInvBetaCut3;
    if(t->isMtdDau3) is1sigmaProtonDau3 = std::fabs(1./t->beta3_PV - ana::invBetaProton(pD3) - ana::meanProton(t, 3) ) < 1.0 * dInvBetaCut3;

    if((t->flavor == 1 && is1sigmaKaonDau2 && ((is1sigmaPionDau1 && is1sigmaProtonDau3) || (is1sigmaPionDau3 && is1sigmaProtonDau1))) ||
        (t->flavor == -1 && is1sigmaKaonDau1 && ((is1sigmaPionDau2 && is1sigmaProtonDau3) || (is1sigmaPionDau3 && is1sigmaProtonDau2)))
      )
    {
      /*
      hMassVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->mass);
      if(t->centrality <100 && t->centrality > 60) hMassVsPtVsY30_50Mtd->Fill(std::fabs(t->y), t->pT, t->mass);
      if( isFWHM ) {
        hVtxProbVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->VtxProb);
        hagl3DVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
        hdlSig3DVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
      }
      if(isCentral) {
        hMassVsPtVsYCentMtd->Fill(std::fabs(t->y), t->pT, t->mass);
        if(isFWHM){
          hVtxProbVsPtVsYCentMtd->Fill(std::fabs(t->y), t->pT, t->VtxProb);
          hagl3DVsPtVsYCentMtd->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
          hdlSig3DVsPtVsYCentMtd->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
        }
      }
      */
      h3DMtdDauPVsDauEtaVsLcPt[yranges.at(iy)]->Fill(t->pT, t->EtaD1, pD1);
      h3DMtdDauPVsDauEtaVsLcPt[yranges.at(iy)]->Fill(t->pT, t->EtaD2, pD2);
      h3DMtdDauPVsDauEtaVsLcPt[yranges.at(iy)]->Fill(t->pT, t->EtaD3, pD3);
    }
  }

  TString newlist(gSystem->BaseName(list.c_str()));
  auto pos = newlist.Index(".list");
  newlist.Replace(pos, 5, "");
  TFile fout(Form("DauKinHists_reRECO_all_%s.root", newlist.Data()), "recreate");
  for (auto & e : h3DDauPVsDauEtaVsLcPt) { e.second->Write(); delete e.second; e.second = nullptr; }
  for (auto & e : h3DMtdDauPVsDauEtaVsLcPt) { e.second->Write(); delete e.second; e.second = nullptr; }
  delete t;

  ts.Stop();
  ts.Print();
}
