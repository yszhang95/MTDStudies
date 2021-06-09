#include <vector>
#include <list>
#include <map>
#include <set>
#include <tuple>

#include <memory>

#include <algorithm>
#include <climits>
#include <numeric>

#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
#include "Math/GenVector/VectorUtil.h"
#include "TMath.h"
#include "TChain.h"
#include "TFileCollection.h"
#include "THashList.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TString.h"
#include "TSystem.h"

#include "TreeReader/ParticleTreeMC2.hxx"
#include "Ana/Common.h"
#include "TreeHelpers.h"

#include "myAna.h"

using std::cerr;
using std::cout;
using std::endl;

using std::vector;
using std::string;
using std::list;
using std::map;
using std::tuple;

using std::shared_ptr;

using Hist1DMaps = std::map<std::string, std::unique_ptr<TH1D>>;
using Hist2DMaps = std::map<std::string, std::unique_ptr<TH2D>>;
using Hist3DMaps = std::map<std::string, std::unique_ptr<TH3D>>;
 
using MatchPairInfo =  tuple<size_t, size_t, double, double>;

int genMatchDsMass(const TString& inputList, const TString& treeDir,
    Particle Dsmeson,
    Long64_t nentries=-1, TString type="")
{
  type.ToLower();
  bool sortByPt(false), sortByR(false);
  if (type == "dpt") sortByPt = true;
  else if (type == "dr") sortByR = true;
  else if (type != "") { cout << "Wrong input variable for sort type" << endl; return -1; }
  TString basename(gSystem->BaseName(inputList));
  const auto firstPos = basename.Index(".list");
  basename.Replace(firstPos, 5, "_");
  basename += nentries > 0 ? Form("%s%lld_", treeDir.Data(), nentries) : (treeDir + "_AllEntries_");

  basename += sortByPt ? "sortBydPt_" : "";
  basename += sortByR  ? "sortBydR_"  : "";
  basename += "Ds_PhiPi.root";
  TFile ofile("output/"+basename, "recreate");
  cout << "Created " << ofile.GetName() << endl;

  TFileCollection tf("tf", "", inputList);
  TChain t(treeDir+"/ParticleTree");
  t.AddFileInfoList(tf.GetList());
  ParticleTreeMC2 p(&t);

  if(nentries < 0) nentries = p.GetEntries();
  cout << "Tree " << treeDir << "/ParticleTree in " << inputList
    << " has " << nentries << " entries." << endl;;

  MatchCriterion matchCriterion(0.03, 0.5);

  Hist1DMaps hDsMassMatch;
  hDsMassMatch["dR0.03"]= std::move(std::unique_ptr<TH1D>(
        new TH1D("hDsMassMatch0p03", "D^{#pm}_{S}, matching FS momenta, dR<0.03, dPt<0.5;Mass (GeV);Events", 200, 1.91, 2.04)));

  Long64_t nMultipleMatch = 0;
  for (Long64_t ientry=0; ientry<nentries; ientry++) {
    if (ientry % 50000 == 0) cout << "pass " << ientry << endl;
    //cout << "Events " << ientry << endl;
    p.GetEntry(ientry);
    auto gensize = p.gen_mass().size();
    auto recosize = p.cand_mass().size();

    auto pdgId = p.cand_pdgId();
    auto gen_pdgId = p.gen_pdgId();

    int nmatch = 0;
    for (size_t ireco=0; ireco<recosize; ireco++) {
      // begin Ds
      if (pdgId[ireco] == 431) {
        //cout << "record matchGEN " << p.cand_matchGEN().at(ireco) << endl;
        //cout << "record cand_genIdx " << p.cand_genIdx().at(ireco) << endl;
        //cout << "record cand_idx " << ireco << endl;
        bool matchGEN = false;
        auto dauIdx = p.cand_dauIdx().at(ireco);
        // 0 for pi, 1 for phi

        std::vector<PtEtaPhiM_t> p4FS;
        p4FS.push_back(getRecoDauP4(ireco, 0, p)); // pi from Ds
        p4FS.push_back(getRecoDauP4(dauIdx.at(1), 0, p)); // K- from phi
        p4FS.push_back(getRecoDauP4(dauIdx.at(1), 1, p)); // k+ from phi

        for (size_t igen=0; igen<gensize; igen++) {
          if (abs(gen_pdgId[igen]) != 431) continue;
          auto gen_dauIdx = p.gen_dauIdx().at(igen);
          /*
          cout << "gen Ds idx: " << igen << endl;
          cout << "gen Ds chain" << endl;
          for (auto& e : gen_dauIdx) {
            cout << p.gen_pdgId().at(e) << endl;
            if(abs(p.gen_pdgId().at(e)) == 421 ) {
              for (const auto & ee : p.gen_dauIdx().at(e))
                cout << p.gen_pdgId().at(ee) << endl;
            }
          }
          */
          Particle Dsmeson_copy(Dsmeson);
          bool sameChain = checkDecayChain(Dsmeson_copy, igen, p);
          if (!sameChain) continue;
          std::vector<PtEtaPhiM_t> p4GenFS;
          p4GenFS.push_back(getGenP4(Dsmeson_copy.daughter(0)->treeIdx(), p)); // pi from Ds
          p4GenFS.push_back(getGenP4(Dsmeson_copy.daughter(1)->daughter(0)->treeIdx(), p)); // K- from Phi
          p4GenFS.push_back(getGenP4(Dsmeson_copy.daughter(1)->daughter(1)->treeIdx(), p)); // K+ from Phi
          if (!matchGEN) {
            bool matchFS = true;
            for (size_t i=0; i<p4GenFS.size(); i++) {
              bool match = matchCriterion.match(p4FS[i], p4GenFS[i]);
              matchFS = matchFS && match;
            }
            matchGEN = matchFS;
          }
          if(matchGEN) break;
        }
        if (matchGEN) hDsMassMatch["dR0.03"]->Fill(p.cand_mass().at(ireco));
        if (matchGEN) nmatch++;
      }
    }
    if (nmatch > 1) { ++nMultipleMatch; cout << "Find one multiple match event" << endl; }
  }
  cout << nMultipleMatch << " events has more than one matched RECO Ds meson" << endl;
  ofile.cd();
  for (const auto& e : hDsMassMatch) e.second->Write();

  return 0;
}

int genMatchFSDsMass(const TString& inputList, const TString& treeDir,
    Particle Dsmeson, ana::TopoCut topo, ana::KineCut kins,
    Long64_t nentries=-1, const bool doRecoGenMatch=true,
    const bool doTrkQA=true, const bool saveGen=true, TString type="")
{
  type.ToLower();
  bool sortByPt(false), sortByR(false);
  if (type == "dpt") sortByPt = true;
  else if (type == "dr") sortByR = true;
  else if (type != "") { cout << "Wrong input variable for sort type" << endl; return -1; }
  TString basename(gSystem->BaseName(inputList));
  const auto firstPos = basename.Index(".list");
  basename.Replace(firstPos, 5, "_");
  basename += nentries > 0 ? Form("%s%lld_", treeDir.Data(), nentries) : (treeDir + "_AllEntries_");

  basename += sortByPt ? "sortBydPt_" : "";
  basename += sortByR  ? "sortBydR_"  : "";
  basename += "Ds_PhiPi_BottomToTop.root";
  TFile ofile("output/"+basename, "recreate");
  cout << "Created " << ofile.GetName() << endl;

  TFileCollection tf("tf", "", inputList);
  TChain t(treeDir+"/ParticleTree");
  t.AddFileInfoList(tf.GetList());
  ParticleTreeMC2 p(&t);

  if(nentries < 0) nentries = p.GetEntries();
  cout << "Tree " << treeDir << "/ParticleTree in " << inputList
    << " has " << nentries << " entries." << endl;;

  MatchCriterion  matchCriterion(0.03, 0.5);

  Hist1DMaps hDsMassMatch;
  hDsMassMatch["dR0.03"]= std::move(std::unique_ptr<TH1D>(
        new TH1D("hDsMassMatch0p03", "D^{#pm}_{S}, matching FS momenta, dR<0.03, dPt<0.5;M_{#phi#pi^{#pm}} (GeV);Events", 200, 1.91, 2.04)));

  std::unique_ptr<TH1D> hCent = std::unique_ptr<TH1D>(new TH1D("hCent", "Centrality:centrality", 200, 0, 100));

  std::unique_ptr<TH2D> hGenYVsPt = std::unique_ptr<TH2D>(new TH2D("hGenYVsPt", ";p_{T} (GeV);y", 200, 0, 20, 60, -3, 3));

  std::string fsNames[3] = {"PionFromDs", "KaonNegFromPhi", "KaonPosFromPhi"};

  Hist2DMaps hTrkBetaInvVsP;
  if (doRecoGenMatch) {
    hTrkBetaInvVsP["PionFromDs"] = std::move(std::unique_ptr<TH2D>(
          new TH2D("hTrkBetaInvVsPVsPPion", "Pion;p (GeV);1/#beta", 200, 0., 8., 200, 0.9, 1.9)));
    hTrkBetaInvVsP["KaonPosFromPhi"] = std::move(std::unique_ptr<TH2D>(
          new TH2D("hTrkBetaInvVsPVsPPosK", "Positive Kaon;p (GeV);1/#beta", 200, 0., 8., 200, 0.9, 1.9)));
    hTrkBetaInvVsP["KaonNegFromPhi"] = std::move(std::unique_ptr<TH2D>(
          new TH2D("hTrkBetaInvVsPVsPNegK", "Negative Kaon;p (GeV);1/#beta", 200, 0., 8., 200, 0.9, 1.9)));
  }
  hTrkBetaInvVsP["AllTracks"] = std::move(std::unique_ptr<TH2D>(
        new TH2D("hTrkBetaInvVsPVsP", "Tracks;p (GeV);1/#beta", 200, 0., 8., 200, 0.9, 1.9)));

  Hist3DMaps hDauPVsDauEtaVsDsPt[ana::nyAbs];
  for (size_t iy=0; iy<ana::nyAbs; iy++) {
    hDauPVsDauEtaVsDsPt[iy].insert( Hist3DMaps::value_type(
        "PionFromDs", std::unique_ptr<TH3D>(
          new TH3D(Form("hDauPVsDauEtaVsDsPtY%zuPion", iy), "Pion;D_{s} p_{T} (GeV);Pion #eta;Pion p (GeV)",
            10, 0, 10, 300, -3, 3, 1000, 0, 10))));
    hDauPVsDauEtaVsDsPt[iy].insert( Hist3DMaps::value_type(
        "KaonPosFromPhi", std::unique_ptr<TH3D>(
          new TH3D(Form("hDauPVsDauEtaVsDsPtY%zuPosK", iy), "K^+;D_{s} p_{T} (GeV);K^{+} #eta;K^{+} p (GeV)",
            10, 0, 10, 300, -3, 3, 1000, 0, 10))));
    hDauPVsDauEtaVsDsPt[iy].insert( Hist3DMaps::value_type(
        "KaonNegFromPhi", std::unique_ptr<TH3D>(
          new TH3D(Form("hDauPVsDauEtaVsDsPtY%zuNegK", iy), "K^-;D_{s} p_{T} (GeV);K^{-} #eta;K^{-} p (GeV)",
            10, 0, 10, 300, -3, 3, 1000, 0, 10))));
  }
  Hist3DMaps hTopoVsDsPtVsDsY;
  hTopoVsDsPtVsDsY["angleDs"] = std::make_unique<TH3D>("hAngleDsVsDsPtVsDsY", "3D angle;Ds y;Ds pT;Ds angle3D",
      6, 0, 3, 20, 0, 20, 80, 0, 4);
  hTopoVsDsPtVsDsY["anglePhi"] = std::make_unique<TH3D>("hAnglePhiVsDsPtVsDsY", "3D angle;Ds y;Ds pT;Phi angle3D",
      6, 0, 3, 20, 0, 20, 50, 0, 1);
  hTopoVsDsPtVsDsY["DsDL"] = std::make_unique<TH3D>("hDLDsVsDsPtVsDsY", "3D DL;Ds y;Ds pT;Ds DL sig.",
      6, 0, 3, 20, 0, 20, 80, 0, 4);
  hTopoVsDsPtVsDsY["VtxProb"] = std::make_unique<TH3D>("hVtxProbVsDsPtVsDsY", "VtxProb;Ds y;Ds pT;Ds VtxProb.",
      6, 0, 3, 20, 0, 20, 100, 0, 1);
  hTopoVsDsPtVsDsY["PhiDL"] = std::make_unique<TH3D>("hDLPhiVsDsPtVsDsY", "3D DL;Ds y;Ds pT;Phi DL sig.",
      6, 0, 3, 20, 0, 20, 80, 0, 4);
  hTopoVsDsPtVsDsY["PionPt"] = std::make_unique<TH3D>("hPionPtVsDsPtVsDsY", "Pion Pt;Ds y;Ds pT;Pion Pt",
      6, 0, 3, 20, 0, 20, 6, 0.4, 1.0);
  hTopoVsDsPtVsDsY["PosKPt"] = std::make_unique<TH3D>("hPosKPtVsDsPtVsDsY", "PosK Pt;Ds y;Ds pT;PosK Pt",
      6, 0, 3, 20, 0, 20, 6, 0.4, 1.0);
  hTopoVsDsPtVsDsY["NegKPt"] = std::make_unique<TH3D>("hNegKPtVsDsPtVsDsY", "NegK Pt;Ds y;Ds pT;NegK Pt",
      6, 0, 3, 20, 0, 20, 6, 0.4, 1.0);

  Hist3DMaps hMassVsPtVsY;
  hMassVsPtVsY["WoMTD"] = std::move(std::unique_ptr<TH3D>(new TH3D("hMassVsPtVsY", "hMassVsPtVsY", 
      ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax)));
  hMassVsPtVsY["WMTD" ] = std::move(std::unique_ptr<TH3D>(new TH3D("hMassVsPtVsYMtd", "hMassVsPtVsYMtd", 
      ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax)));

  std::unique_ptr<TH1D> hFSpT[3];
  hFSpT[0] = std::move(std::unique_ptr<TH1D>(new TH1D("hPi_pT", "Pi;pT;", 100, 0, 10)));
  hFSpT[1] = std::move(std::unique_ptr<TH1D>(new TH1D("hNegK_pT", "Pi;pT;", 100, 0, 10)));
  hFSpT[2] = std::move(std::unique_ptr<TH1D>(new TH1D("hPosK_pT", "Pi;pT;", 100, 0, 10)));
  std::unique_ptr<TH1D> hFSp[3];
  hFSp[0] = std::move(std::unique_ptr<TH1D>(new TH1D("hPi_p", "Pi;p;", 100, 0, 10)));
  hFSp[1] = std::move(std::unique_ptr<TH1D>(new TH1D("hNegK_p", "Pi;p;", 100, 0, 10)));
  hFSp[2] = std::move(std::unique_ptr<TH1D>(new TH1D("hPosK_p", "Pi;p;", 100, 0, 10)));

  std::unique_ptr<TH1D> hPhiMass(new TH1D("hPhiMass", "PhiMass;Mass;", 100, 1.0195-0.01, 1.0195+0.01));

  Long64_t nMultipleMatch = 0;
  for (Long64_t ientry=0; ientry<nentries; ientry++) {
    if (ientry % 20000 == 0) cout << "pass " << ientry << endl;
    p.GetEntry(ientry);
    // centrality
    hCent->Fill(p.centrality()/2.);
    // track QA, doTrkQA should be true
    size_t nTrk = p.trk_candIdx().size();
    if (!doTrkQA) nTrk = 0;
    for (size_t iTrk=0; iTrk<nTrk; iTrk++) {
      if (p.trk_tMTDErr().at(iTrk) < 0) continue;
      double dT = p.trk_tMTD().at(iTrk) - p.bestvtxT();
      double betaInv = dT * ana::c_cm_ns/ p.trk_pathLength().at(iTrk); // 
      if (abs(betaInv - 1./p.trk_beta().at(iTrk)) > 1e-2) cerr << "Wrong beta value" << endl;
      double pT = p.cand_pT().at(p.trk_candIdx().at(iTrk).at(0));
      double eta = p.cand_eta().at(p.trk_candIdx().at(iTrk).at(0));
      hTrkBetaInvVsP["AllTracks"]->Fill(pT * std::cosh(eta), betaInv);
    }
    // reco-gen matching, if you do not want to do, set doRecoGenMatch to false, and it will make p4GenFS and p4RecoFS empty
    auto gensize = p.gen_mass().size();
    auto recosize = p.cand_mass().size();

    auto pdgId = p.cand_pdgId();
    auto gen_pdgId = p.gen_pdgId();

    map<size_t, PtEtaPhiM_t> p4GenFS;
    for (size_t igen=0; igen<gensize; igen++) {
      if (!doRecoGenMatch) break;
      if (abs(gen_pdgId[igen]) == 211) p4GenFS[igen] = getGenP4(igen, p);
      if (abs(gen_pdgId[igen]) != std::abs(Dsmeson.id())) continue;
      auto gen_dauIdx = p.gen_dauIdx().at(igen);
      Particle Dsmeson_copy(Dsmeson);
      bool sameChain = checkDecayChain(Dsmeson_copy, igen, p);
      if (!sameChain) continue;
      p4GenFS[Dsmeson_copy.daughter(0)->treeIdx()] = getGenP4(Dsmeson_copy.daughter(0)->treeIdx(), p); // pi from Ds
      p4GenFS[Dsmeson_copy.daughter(1)->daughter(0)->treeIdx()] = getGenP4(Dsmeson_copy.daughter(1)->daughter(0)->treeIdx(), p); // K- from Phi
      p4GenFS[Dsmeson_copy.daughter(1)->daughter(1)->treeIdx()] = getGenP4(Dsmeson_copy.daughter(1)->daughter(1)->treeIdx(), p); // K+ from Phi
    }
    map<size_t, PtEtaPhiM_t> p4RecoFS;
    for (size_t ireco=0; ireco<recosize; ireco++) {
      if (!doRecoGenMatch) break;
      if (p.cand_status().at(ireco) != 1) continue; // stable
      p4RecoFS[ireco] = getRecoP4(ireco, p);
          //cout << "A stable" << p.cand_pdgId().at(ireco) << endl;
    }

    vector<bool> recoLock(recosize, 0);
    vector<bool> genLock (gensize,  0);

    vector<MatchPairInfo> matchGenInfo;
    for (const auto& gen : p4GenFS) {
      for (const auto& reco : p4RecoFS) {
        if (matchCriterion.match(reco.second, gen.second)) {
          const auto dR = ROOT::Math::VectorUtil::DeltaR(reco.second, gen.second);
          const auto dRelPt = TMath::Abs(gen.second.Pt() - reco.second.Pt())/gen.second.Pt();
          matchGenInfo.push_back( std::make_tuple(reco.first, gen.first, dR, dRelPt) );
        }
      }
    }
    // sort by pT
    std::sort(matchGenInfo.begin(), matchGenInfo.end(), [](MatchPairInfo i, MatchPairInfo j) { return std::get<3>(i) < std::get<3>(j); } );

    map<size_t, size_t> matchPairs;

    for (auto e : matchGenInfo) {
      const auto ireco = std::get<0>(e);
      const auto igen  = std::get<1>(e);
      if (!recoLock.at(ireco) && !genLock.at(igen)) {
        recoLock.at(ireco) = 1;
        genLock.at(igen)   = 1;
        matchPairs.insert(map<size_t, size_t>::value_type(ireco, igen));
      }
    }

    // loop over particles
    for (size_t ireco=0; ireco<recosize; ireco++) {
      // begin Ds
      if (pdgId[ireco] == std::abs(Dsmeson.id())) {
        auto iy = ana::whichBin(ana::ybin, std::abs(p.cand_y().at(ireco)));
        if (iy == size_t(-1)) continue;
        auto ipt = ana::whichBin(ana::ptbin, p.cand_pT().at(ireco));
        if (ipt == size_t(-1)) continue;

        auto dauIdx = p.cand_dauIdx().at(ireco);
        // 0 for pi, 1 for phi

        // check reco-gen match
        bool matchGEN = true;
        bool isSwap = false;
        if (doRecoGenMatch) {
          matchGEN = matchGEN && ( matchPairs.find( dauIdx.at(0) ) != matchPairs.end() );
          matchGEN = matchGEN && ( matchPairs.find( dauIdx.at(1) ) != matchPairs.end() );
          matchGEN = matchGEN && ( matchPairs.find( dauIdx.at(2) ) != matchPairs.end() );
        }
        if (doRecoGenMatch && matchGEN) {
          isSwap = isSwap || abs(ana::massPion - p.gen_mass().at(matchPairs.at(dauIdx.at(0)))) > 0.05;
          isSwap = isSwap || abs(ana::massKaon - p.gen_mass().at(matchPairs.at(dauIdx.at(1)))) > 0.05;
          isSwap = isSwap || abs(ana::massKaon - p.gen_mass().at(matchPairs.at(dauIdx.at(2)))) > 0.05;
        }
        if (doRecoGenMatch && isSwap) continue;
        if (!matchGEN) continue;
        if (doRecoGenMatch) {
          hDsMassMatch["dR0.03"]->Fill(p.cand_mass().at(ireco));
          if (doTrkQA) {
            // track QA
            {// pi from Ds
              double mom = p.cand_pT().at(dauIdx.at(0)) * std::cosh(p.cand_eta().at(dauIdx.at(0)));
              auto trkIdx = p.cand_trkIdx().at(dauIdx.at(0));
              hTrkBetaInvVsP["PionFromDs"]->Fill(mom, 1./p.trk_beta().at(trkIdx));
            }
            {// K+
              double mom = p.cand_pT().at(dauIdx.at(2)) * std::cosh(p.cand_eta().at(dauIdx.at(2)));
              auto trkIdx = p.cand_trkIdx().at(dauIdx.at(2));
              hTrkBetaInvVsP["KaonPosFromPhi"]->Fill(mom, 1./p.trk_beta().at(trkIdx));
            }
            {// K-
              double mom = p.cand_pT().at(dauIdx.at(1)) * std::cosh(p.cand_eta().at(dauIdx.at(1)));
              auto trkIdx = p.cand_trkIdx().at(dauIdx.at(1));
              hTrkBetaInvVsP["KaonNegFromPhi"]->Fill(mom, 1./p.trk_beta().at(trkIdx));
            }
            // track QA end
          }
        }
        // prepare p4 and MTD info
        std::vector<ParticleInfo> fsPars; fsPars.reserve(3); // pi from Ds, K- from Phi, K+ from Phi
        unsigned short trkIdx[3] = {0};
        unsigned int   fsIdx[3]  = {0};
        int ids[3] = {211, -321, 321}; // optimize later
        double masses[3] = {ana::massPion, ana::massKaon, ana::massKaon}; // optimize later
        // pi from Ds
        fsIdx [0] = dauIdx.at(0); // pi from Ds
        fsIdx [1] = dauIdx.at(1); // K- from  phi
        fsIdx [2] = dauIdx.at(2); // K+ from  phi
        for (int i=0; i<3; i++) {
          trkIdx[i] = p.cand_trkIdx().at(fsIdx[i]);
          fsPars.push_back(std::make_tuple(PtEtaPhiM_t(p.cand_pT().at(fsIdx[i]), p.cand_eta().at(fsIdx[i]), p.cand_phi().at(fsIdx[i]), masses[i]), ids[i],
              p.trk_beta().at(trkIdx[i]), p.trk_tMTD().at(trkIdx[i]), p.trk_tMTDErr().at(trkIdx[i]), p.trk_pathLength().at(trkIdx[i])));
        }
        // check PID
        bool passMTD = true;
        for (size_t i=0; i<3; i++) {
          passMTD = passMTD && checkPassPID(fsPars[i], 1.0);
          //passMTD = passMTD && checkPassPID(fsPars[i], 1.0) && std::get<0>(fsPars[i]).Pt() > 0.5;
        }
        if (!ana::passKinematic(p.cand_pT().at(ireco), p.cand_eta().at(ireco), kins)) continue; // a sharp cut during background tree production
        if (!ana::passTopoCuts(p.cand_decayLength3D().at(ireco)/p.cand_decayLengthError3D().at(ireco),
              p.cand_angle3D().at(ireco), p.cand_vtxProb().at(ireco), topo)) continue; // a sharp cut during background tree production

        if (passMTD) hMassVsPtVsY["WMTD"]->Fill(p.cand_y().at(ireco), p.cand_pT().at(ireco), p.cand_mass().at(ireco));
        hMassVsPtVsY["WoMTD"]->Fill(p.cand_y().at(ireco), p.cand_pT().at(ireco), p.cand_mass().at(ireco));
        // topo cuts studies begin
        if (passMTD && ana::isFWHM(p.cand_mass().at(ireco), iy)) {
          hTopoVsDsPtVsDsY["angleDs"]->Fill(p.cand_y().at(ireco), p.cand_pT().at(ireco), p.cand_angle3D().at(ireco));
          hTopoVsDsPtVsDsY["anglePhi"]->Fill(p.cand_y().at(ireco), p.cand_pT().at(ireco), std::abs(std::cos(p.cand_angle3D().at(dauIdx.at(1)))));
          hTopoVsDsPtVsDsY["DsDL"]->Fill(p.cand_y().at(ireco), p.cand_pT().at(ireco),
              p.cand_decayLength3D().at(ireco)/p.cand_decayLengthError3D().at(ireco));
          hTopoVsDsPtVsDsY["VtxProb"]->Fill(p.cand_y().at(ireco), p.cand_pT().at(ireco),
              p.cand_vtxProb().at(ireco));
          hTopoVsDsPtVsDsY["PhiDL"]->Fill(p.cand_y().at(ireco), p.cand_pT().at(ireco),
              p.cand_decayLength3D().at(dauIdx.at(1))/p.cand_decayLengthError3D().at(dauIdx.at(1)));
          hTopoVsDsPtVsDsY["PionPt"]->Fill(p.cand_y().at(ireco), p.cand_pT().at(ireco), p.cand_pT().at(dauIdx.at(0)));
          hTopoVsDsPtVsDsY["NegKPt"]->Fill(p.cand_y().at(ireco), p.cand_pT().at(ireco), p.cand_pT().at(dauIdx.at(1)));
          hTopoVsDsPtVsDsY["PosKPt"]->Fill(p.cand_y().at(ireco), p.cand_pT().at(ireco), p.cand_pT().at(dauIdx.at(2)));
        }
        // topo cuts studies end
        // pT QR
        for (size_t i=0; i<3; i++) {
          hFSpT[i]->Fill(std::get<0>(fsPars[i]).Pt());
          hFSp[i]->Fill(std::get<0>(fsPars[i]).P());
        } // pT QT end
        // begin dau kinematic
        for (size_t i=0; i<3; i++) {
          (hDauPVsDauEtaVsDsPt[iy])[fsNames[i]]->Fill(p.cand_pT().at(ireco), std::get<0>(fsPars[i]).Eta(), std::get<0>(fsPars[i]).P());
        }
        // end dau kinematic
        // phi mass
        auto temp1 = std::get<0>(fsPars[1]);
        auto temp2 = std::get<0>(fsPars[2]);
        auto tempE = temp1.E()+temp2.E();
        auto tempPx = temp1.Px()+temp2.Px();
        auto tempPy = temp1.Py()+temp2.Py();
        auto tempPz = temp1.Pz()+temp2.Pz();
        auto tempM = std::sqrt(tempE*tempE - tempPx*tempPx - tempPy*tempPy - tempPz*tempPz);
        hPhiMass->Fill(tempM);
        // end phi mass
      } // end Ds
    }
    // begin saveGen
    if (doRecoGenMatch && saveGen) {
      for (size_t i=0; i<gensize; ++i) {
        if (std::abs(p.gen_pdgId().at(i)) != std::abs(Dsmeson.id())) continue;
        Particle Dsmeson_copy(Dsmeson);
        bool sameChain = checkDecayChain(Dsmeson_copy, i, p);
        if (!sameChain) continue;
        hGenYVsPt->Fill(p.gen_pT().at(i), p.gen_y().at(i));
      }
    } // end saveGen
  } // end
  cout << nMultipleMatch << " events has more than one matched RECO Ds meson" << endl;
  ofile.cd();
  for (const auto& e : hDsMassMatch) e.second->Write();
  for (const auto& e : hTrkBetaInvVsP) e.second->Write();
  for (const auto& e : hMassVsPtVsY) e.second->Write();
  for (size_t i=0; i<3; i++) {
    hFSpT[i]->Write();
    hFSp[i]->Write();
    for (const auto& e : hDauPVsDauEtaVsDsPt[i]) e.second->Write();
  }
  for (const auto& e : hTopoVsDsPtVsDsY) e.second->Write();
  hCent->Write();
  if (saveGen) hGenYVsPt->Write();
  hPhiMass->Write();

  return 0;
}
