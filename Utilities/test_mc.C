#include <iostream>

#include "TreeReader/ParticleTreeMC.cxx"
#include "TreeReader/ParticleTree.cxx"
#include "TString.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TFileCollection.h"
#include "THashList.h"
#include "TH1D.h"
#include "TH2D.h"

#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
#include "Math/GenVector/VectorUtil.h"
#include "TMath.h"

using namespace std; 

float DeltaR(float eta1,float phi1,float eta2,float phi2)
{
	float deltaPhi = TMath::Abs(phi1-phi2);
	float deltaEta = eta1-eta2;
	if(deltaPhi > TMath::Pi())
	deltaPhi = TMath::TwoPi() - deltaPhi;
		return TMath::Sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi);
}

void test_mc()
{
  using PtEtaPhiM_t = ROOT::Math::PtEtaPhiM4D<double>;

  std::unique_ptr<TH2D> hKsRecoAll(new TH2D("hKsRecoAll", "All channels, Ks p3;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));

  std::unique_ptr<TH2D> hKsRecoAll_mass(new TH2D("hKsRecoAll_mass", "All channels, Ks p3;pT (GeV);mass (GeV);",
          40, 0, 8, 80, 0.44, 0.55));

  std::unique_ptr<TH2D> hKsRecoMatch(new TH2D("hKsRecoMatch", "All channels, Ks p3;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));

  std::unique_ptr<TH2D> hKsRecoMatch_mass(new TH2D("hKsRecoMatch_mass", "All channels, Ks p3;pT (GeV);mass (GeV);",
          40, 0, 8, 80, 0.44, 0.55));

  std::unique_ptr<TH2D> hLamRecoAll(new TH2D("hLamRecoAll", "All channels, Lam p3;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));

  std::unique_ptr<TH2D> hLamRecoAll_mass(new TH2D("hLamRecoAll_mass", "All channels, Lam p3;pT (GeV);mass (GeV);",
          40, 0, 8, 80, 1.08, 1.16));

  std::unique_ptr<TH2D> hLamRecoMatch(new TH2D("hLamRecoMatch", "All channels, Lam p3;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));

  std::unique_ptr<TH2D> hLamRecoMatch_mass(new TH2D("hLamRecoMatch_mass", "All channels, Lam p3;pT (GeV);mass (GeV);",
          40, 0, 8, 80, 1.08, 1.16));

  std::unique_ptr<TH2D> hXiRecoAll(new TH2D("hXiRecoAll", "All channels, Xi p3;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));

  std::unique_ptr<TH2D> hXiRecoAll_mass(new TH2D("hXiRecoAll_mass", "All channels, Xi p3;pT (GeV);mass (GeV);",
          40, 0, 8, 80, 1.26, 1.39));

  std::unique_ptr<TH2D> hXiRecoMatch(new TH2D("hXiRecoMatch", "All channels, Xi p3;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));

  std::unique_ptr<TH2D> hXiRecoMatch_mass(new TH2D("hXiRecoMatch_mass", "All channels, Xi p3;pT (GeV);mass (GeV);",
          40, 0, 8, 80, 1.26, 1.39));

  std::unique_ptr<TH2D> hOmRecoAll(new TH2D("hOmRecoAll", "All channels, Om p3;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));

  std::unique_ptr<TH2D> hOmRecoAll_mass(new TH2D("hOmRecoAll_mass", "All channels, Om p3;pT (GeV);mass (GeV);",
          40, 0, 8, 80, 1.61, 1.73));

  std::unique_ptr<TH2D> hOmRecoMatch(new TH2D("hOmRecoMatch", "All channels, Om p3;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));

  std::unique_ptr<TH2D> hOmRecoMatch_mass(new TH2D("hOmRecoMatch_mass", "All channels, Om p3;pT (GeV);mass (GeV);",
          40, 0, 8, 80, 1.61, 1.73));

  std::unique_ptr<TH1D> hKsRecoAll_trkdcaxy1(new TH1D("hKsRecoAll_trkdcaxy1", "All channels, ks p3;dca xy",
          1000, 0, 100));  
  std::unique_ptr<TH1D> hKsRecoAll_trkdcaxy2(new TH1D("hKsRecoAll_trkdcaxy2", "All channels, ks p3;dca xy",
          1000, 0, 100));          
  std::unique_ptr<TH1D> hKsRecoAll_trkdcaz1(new TH1D("hKsRecoAll_trkdcaz1", "All channels, ks p3;dca z",
          1000, 0, 100));              
  std::unique_ptr<TH1D> hKsRecoAll_trkdcaz2(new TH1D("hKsRecoAll_trkdcaz2", "All channels, ks p3;dca z",
          1000, 0, 100));
  std::unique_ptr<TH1D> hKsRecoAll_angle3D(new TH1D("hKsRecoAll_angle3D", "All channels, ks p3;angle3D",
          100, 0, 1));
  std::unique_ptr<TH1D> hKsRecoAll_dl3D(new TH1D("hKsRecoAll_dl3D", "All channels, ks p3;dl3D",
          200, 0, 20));
  std::unique_ptr<TH1D> hKsRecoMatch_trkdcaxy1(new TH1D("hKsRecoMatch_trkdcaxy1", "All channels, ks p3;dca xy",
          1000, 0, 100));        
  std::unique_ptr<TH1D> hKsRecoMatch_trkdcaxy2(new TH1D("hKsRecoMatch_trkdcaxy2", "All channels, ks p3;dca xy",
          1000, 0, 100));
  std::unique_ptr<TH1D> hKsRecoMatch_trkdcaz1(new TH1D("hKsRecoMatch_trkdcaz1", "All channels, ks p3;dca z",
          1000, 0, 100));
  std::unique_ptr<TH1D> hKsRecoMatch_trkdcaz2(new TH1D("hKsRecoMatch_trkdcaz2", "All channels, ks p3;dca z",
          1000, 0, 100));
  std::unique_ptr<TH1D> hKsRecoMatch_angle3D(new TH1D("hKsRecoMatch_angle3D", "All channels, ks p3;angle3D",
          100, 0, 1));
  std::unique_ptr<TH1D> hKsRecoMatch_dl3D(new TH1D("hKsRecoMatch_dl3D", "All channels, ks p3;dl3D",
          200, 0, 20));

  std::unique_ptr<TH2D> hKsGen(new TH2D("hKsGen", "All channels;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));
  std::unique_ptr<TH2D> hLamGen(new TH2D("hLamGen", "All channels;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));
  std::unique_ptr<TH2D> hXiGen(new TH2D("hXiGen", "All channels;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));
  std::unique_ptr<TH2D> hOmGen(new TH2D("hOmGen", "All channels;pT (GeV);y;",
          40, 0, 8, 10, -2.5, 2.5));

  const TString& inputList = "test.list";
  const TString& treeDir_ks = "kshortana";
  const TString& treeDir_lam = "lambdaana";
  const TString& treeDir_xi = "xiana";
  const TString& treeDir_om = "omegaana";

  TFileCollection tf("tf", "", inputList);

  TChain t_ks(treeDir_ks+"/ParticleTree");
  t_ks.AddFileInfoList(tf.GetList());
  ParticleTreeMC p_ks(&t_ks);

  TChain t_lam(treeDir_lam+"/ParticleTree");
  t_lam.AddFileInfoList(tf.GetList());
  ParticleTreeMC p_lam(&t_lam);

  TChain t_xi(treeDir_xi+"/ParticleTree");
  t_xi.AddFileInfoList(tf.GetList());
  ParticleTreeMC p_xi(&t_xi);

  TChain t_om(treeDir_om+"/ParticleTree");
  t_om.AddFileInfoList(tf.GetList());
  ParticleTreeMC p_om(&t_om);

  auto nentries = p_ks.GetEntries()/10;
  //auto nentries = 1000L;

  cout << "Tree lambdaana/ParticleTree in " << inputList
       << " has " << nentries << " entries." << endl;;

  for (Long64_t ientry=0; ientry<nentries; ientry++) {

    if( !(ientry % 10000) ) std::cout<<"Processing "<<ientry<<" events"<<std::endl;
   
    p_ks.GetEntry(ientry);

    auto gensize_ks = p_ks.gen_mass().size();
    auto recosize_ks = p_ks.cand_mass().size();

    auto gensize_lam = p_lam.gen_mass().size();
    auto recosize_lam = p_lam.cand_mass().size();

    auto gensize_xi = p_xi.gen_mass().size();
    auto recosize_xi = p_xi.cand_mass().size();

    auto gensize_om = p_om.gen_mass().size();
    auto recosize_om = p_om.cand_mass().size();

    auto dauIdxEvt_ks = p_ks.cand_dauIdx();
    auto dauIdxEvt_lam = p_lam.cand_dauIdx();
    auto dauIdxEvt_xi = p_xi.cand_dauIdx();
    auto dauIdxEvt_om = p_om.cand_dauIdx();

    auto pdgId_ks = p_ks.cand_pdgId();

    //if(gensize == 0 || recosize == 0) continue;

/*
    auto pdgId = p.cand_pdgId();
    auto charge = p.cand_charge();
    auto gen_pdgId = p.gen_pdgId();
    auto gen_charge = p.gen_charge();

    auto dauIdxxEvt = p.cand_dauIdx();
    auto gen_dauIdxEvt = p.gen_dauIdx();

cout<<"get here"<<endl;
*/
    for (size_t ireco=0; ireco<recosize_ks; ireco++) {

       if (pdgId_ks.at(ireco) != 310) continue;
       std::cout << ireco << std::endl;
       std::cout << (int)p_ks.cand_status().at(ireco) << std::endl;
       std::cout << pdgId_ks.at(ireco) << std::endl;
       cout << p_ks.cand_pdgId().at(ireco) << std::endl;

       auto dauIdx = dauIdxEvt_ks.at(ireco);
       cout << p_ks.cand_pT().at(ireco) << std::endl;
       cout << p_ks.cand_dauIdx().at(ireco).size() << std::endl;

cout<<dauIdx.at(1)<<endl;

if (ientry>1) return;

       //auto trkIdx1 = p_ks.cand_trkIdx().at(dauIdx.at(0));
       const unsigned short trkIdx1 = 0;
//       auto trkIdx2 = p_ks.cand_trkIdx().at(dauIdx.at(1));
//cout<<trkIdx1<<" "<<trkIdx2<<endl;

//cout<<trkIdx1<<endl;

       hKsRecoAll_trkdcaxy1->Fill(p_ks.trk_xyDCASignificance().at(trkIdx1));
//       hKsRecoAll_trkdcaxy2->Fill(p_ks.trk_xyDCASignificance().at(trkIdx2));
       hKsRecoAll_trkdcaz1->Fill(p_ks.trk_zDCASignificance().at(trkIdx1));
//       hKsRecoAll_trkdcaz2->Fill(p_ks.trk_zDCASignificance().at(trkIdx2));

       hKsRecoAll_angle3D->Fill(cos(p_ks.cand_angle3D()[ireco]));
       hKsRecoAll_dl3D->Fill(fabs(p_ks.cand_decayLength3D()[ireco]/p_ks.cand_decayLengthError3D()[ireco]));

//       if(cos(p_ks.cand_angle3D()[ireco])<0.999 || fabs(p_ks.cand_decayLength3D()[ireco]/p_ks.cand_decayLengthError3D()[ireco])<5) continue;

       hKsRecoAll->Fill(p_ks.cand_pT()[ireco], p_ks.cand_y()[ireco]);
       hKsRecoAll_mass->Fill(p_ks.cand_pT()[ireco], p_ks.cand_mass()[ireco]);

    }
      
  }
/*
  TH2D* hKsRecoUnMatch = (TH2D*)hKsRecoAll->Clone("hKsRecoUnMatch");
  TH2D* hKsRecoUnMatch_mass = (TH2D*)hKsRecoAll_mass->Clone("hKsRecoUnMatch_mass");
  TH2D* hLamRecoUnMatch = (TH2D*)hLamRecoAll->Clone("hLamRecoUnMatch");
  TH2D* hLamRecoUnMatch_mass = (TH2D*)hLamRecoAll_mass->Clone("hLamRecoUnMatch_mass");
  TH2D* hXiRecoUnMatch = (TH2D*)hXiRecoAll->Clone("hXiRecoUnMatch");
  TH2D* hXiRecoUnMatch_mass = (TH2D*)hXiRecoAll_mass->Clone("hXiRecoUnMatch_mass");
  TH2D* hOmRecoUnMatch = (TH2D*)hOmRecoAll->Clone("hOmRecoUnMatch");
  TH2D* hOmRecoUnMatch_mass = (TH2D*)hOmRecoAll_mass->Clone("hOmRecoUnMatch_mass");

  hKsRecoUnMatch->Add(hKsRecoMatch,-1);
  hKsRecoUnMatch_mass->Add(hKsRecoMatch_mass,-1);
  hLamRecoUnMatch->Add(hLamRecoMatch,-1);
  hLamRecoUnMatch_mass->Add(hLamRecoMatch_mass,-1);
  hXiRecoUnMatch->Add(hXiRecoMatch,-1);
  hXiRecoUnMatch_mass->Add(hXiRecoMatch_mass,-1);
  hOmRecoUnMatch->Add(hOmRecoMatch,-1);
  hOmRecoUnMatch_mass->Add(hOmRecoMatch_mass,-1);
*/
  TFile* fout = new TFile("output.root","recreate");

  hKsRecoAll->Write();
  hKsRecoAll_mass->Write();
  hKsRecoMatch->Write();
  hKsRecoMatch_mass->Write();
//  hKsRecoUnMatch->Write();
//  hKsRecoUnMatch_mass->Write();
  hKsGen->Write();

  hLamRecoAll->Write();
  hLamRecoAll_mass->Write();
  hLamRecoMatch->Write();
  hLamRecoMatch_mass->Write();
//  hLamRecoUnMatch->Write();
//  hLamRecoUnMatch_mass->Write();
  hLamGen->Write();

  hXiRecoAll->Write();
  hXiRecoAll_mass->Write();
  hXiRecoMatch->Write();
  hXiRecoMatch_mass->Write();
//  hXiRecoUnMatch->Write();
//  hXiRecoUnMatch_mass->Write();
  hXiGen->Write();

  hOmRecoAll->Write();
  hOmRecoAll_mass->Write();
  hOmRecoMatch->Write();
  hOmRecoMatch_mass->Write();
//  hOmRecoUnMatch->Write();
//  hOmRecoUnMatch_mass->Write();
  hOmGen->Write();


  hKsRecoAll_trkdcaxy1->Write();
  hKsRecoAll_trkdcaxy2->Write();
  hKsRecoAll_trkdcaz1->Write();
  hKsRecoAll_trkdcaz2->Write();
  hKsRecoAll_angle3D->Write();
  hKsRecoAll_dl3D->Write();
  hKsRecoMatch_trkdcaxy1->Write();
  hKsRecoMatch_trkdcaxy2->Write();
  hKsRecoMatch_trkdcaz1->Write();
  hKsRecoMatch_trkdcaz2->Write();
  hKsRecoMatch_angle3D->Write();
  hKsRecoMatch_dl3D->Write();

  fout->Close();
}
