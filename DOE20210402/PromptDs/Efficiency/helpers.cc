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
#include "Math/Vector4Dfwd.h"
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
#include "Ana/TreeHelpers.h"

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

  std::unique_ptr<TH2D> hGenYVsPt = std::unique_ptr<TH2D>(new TH2D("hGenYVsPt", ";p_{T} (GeV);y", 40, 0, 40, 6, -3, 3));
  std::unique_ptr<TH2D> hAccGenYVsPt = std::unique_ptr<TH2D>(new TH2D("hAccGenYVsPt", ";p_{T} (GeV);y", 40, 0, 40, 6, -3, 3));
  std::unique_ptr<TH2D> hRecoYVsPt = std::unique_ptr<TH2D>(new TH2D("hRecoYVsPt", ";p_{T} (GeV);y", 40, 0, 40, 6, -3, 3));
  std::unique_ptr<TH2D> hLooseRecoYVsPt = std::unique_ptr<TH2D>(new TH2D("hLooseRecoYVsPt", ";p_{T} (GeV);y", 40, 0, 40, 6, -3, 3));
  std::unique_ptr<TH2D> hFinalRecoYVsPt = std::unique_ptr<TH2D>(new TH2D("hFinalRecoYVsPt", ";p_{T} (GeV);y", 40, 0, 40, 6, -3, 3));

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

  Hist3DMaps hMassVsPtVsY, hMassVsPtVsY_0_10, hMassVsPtVsY_0_20, hMassVsPtVsY_30_50;
  hMassVsPtVsY["WoMTD"] = std::move(std::unique_ptr<TH3D>(new TH3D("hMassVsPtVsY", "hMassVsPtVsY", 
      ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax)));
  hMassVsPtVsY_0_10["WoMTD"] = std::move(std::unique_ptr<TH3D>(new TH3D("hMassVsPtVsY_0_10", "hMassVsPtVsY_0_10", 
      ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax)));
  hMassVsPtVsY_0_20["WoMTD"] = std::move(std::unique_ptr<TH3D>(new TH3D("hMassVsPtVsY_0_20", "hMassVsPtVsY_0_20", 
      ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax)));
  hMassVsPtVsY_30_50["WoMTD"] = std::move(std::unique_ptr<TH3D>(new TH3D("hMassVsPtVsY_30_50", "hMassVsPtVsY_30_50", 
      ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax)));

  hMassVsPtVsY["WMTD"] = std::move(std::unique_ptr<TH3D>(new TH3D("hMassVsPtVsYMtd", "hMassVsPtVsYMtd", 
      ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax)));
  hMassVsPtVsY_0_10["WMTD"] = std::move(std::unique_ptr<TH3D>(new TH3D("hMassVsPtVsYMtd_0_10", "hMassVsPtVsYMtd_0_10", 
      ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax)));
  hMassVsPtVsY_0_20["WMTD"] = std::move(std::unique_ptr<TH3D>(new TH3D("hMassVsPtVsYMtd_0_20", "hMassVsPtVsYMtd_0_20", 
      ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax)));
  hMassVsPtVsY_30_50["WMTD"] = std::move(std::unique_ptr<TH3D>(new TH3D("hMassVsPtVsYMtd_30_50", "hMassVsPtVsYMtd_30_50", 
      ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax)));

  std::unique_ptr<TH1D> hFSpT[3];
  hFSpT[0] = std::move(std::unique_ptr<TH1D>(new TH1D("hPi_pT", "Pi;pT;", 100, 0, 10)));
  hFSpT[1] = std::move(std::unique_ptr<TH1D>(new TH1D("hNegK_pT", "Pi;pT;", 100, 0, 10)));
  hFSpT[2] = std::move(std::unique_ptr<TH1D>(new TH1D("hPosK_pT", "Pi;pT;", 100, 0, 10)));
  std::unique_ptr<TH1D> hFSp[3];
  hFSp[0] = std::move(std::unique_ptr<TH1D>(new TH1D("hPi_p", "Pi;p;", 100, 0, 10)));
  hFSp[1] = std::move(std::unique_ptr<TH1D>(new TH1D("hNegK_p", "Pi;p;", 100, 0, 10)));
  hFSp[2] = std::move(std::unique_ptr<TH1D>(new TH1D("hPosK_p", "Pi;p;", 100, 0, 10)));

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
        auto gDauIdx = p.cand_dauIdx().at(dauIdx.at(1));

        // check tracks
        bool passTrack = ana::passTrackKinematicCuts(p.cand_eta().at(dauIdx.at(0)), p.cand_pT().at(dauIdx.at(0)))
                        && ana::passTrackKinematicCuts(p.cand_eta().at(gDauIdx.at(0)), p.cand_pT().at(gDauIdx.at(0)))
                        && ana::passTrackKinematicCuts(p.cand_eta().at(gDauIdx.at(1)), p.cand_pT().at(gDauIdx.at(1)));
        if (!passTrack) continue;

        // check topo variable
        double topos[5];
        topos[0] = p.cand_decayLength3D().at(ireco)/p.cand_decayLengthError3D().at(ireco);
        topos[1] = p.cand_angle3D().at(ireco);
        topos[2] = p.cand_pT().at(dauIdx.at(0));
        topos[3] = p.cand_pT().at(gDauIdx.at(0));
        topos[4] = p.cand_pT().at(gDauIdx.at(1));

        // check reco-gen match
        bool matchGEN = true;
        bool isSwap = false;
        if (doRecoGenMatch) {
          matchGEN = matchGEN && ( matchPairs.find( dauIdx.at(0) ) != matchPairs.end() );
          matchGEN = matchGEN && ( matchPairs.find( gDauIdx.at(0) ) != matchPairs.end() );
          matchGEN = matchGEN && ( matchPairs.find( gDauIdx.at(1) ) != matchPairs.end() );
        }
        if (doRecoGenMatch && matchGEN) {
          isSwap = isSwap || abs(ana::massPion - p.gen_mass().at(matchPairs.at(dauIdx.at(0)))) > 0.05;
          isSwap = isSwap || abs(ana::massKaon - p.gen_mass().at(matchPairs.at(gDauIdx.at(0)))) > 0.05;
          isSwap = isSwap || abs(ana::massKaon - p.gen_mass().at(matchPairs.at(gDauIdx.at(1)))) > 0.05;
        }
        if (doRecoGenMatch && isSwap) continue;
        if (!matchGEN) continue;
        hRecoYVsPt->Fill(p.cand_pT().at(ireco), p.cand_y().at(ireco));
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
              double mom = p.cand_pT().at(gDauIdx.at(1)) * std::cosh(p.cand_eta().at(gDauIdx.at(1)));
              auto trkIdx = p.cand_trkIdx().at(gDauIdx.at(1));
              hTrkBetaInvVsP["KaonPosFromPhi"]->Fill(mom, 1./p.trk_beta().at(trkIdx));
            }
            {// K-
              double mom = p.cand_pT().at(gDauIdx.at(0)) * std::cosh(p.cand_eta().at(gDauIdx.at(0)));
              auto trkIdx = p.cand_trkIdx().at(gDauIdx.at(0));
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
        fsIdx [1] = gDauIdx.at(0); // K- from  phi
        fsIdx [2] = gDauIdx.at(1); // K+ from  phi
        for (int i=0; i<3; i++) {
          trkIdx[i] = p.cand_trkIdx().at(fsIdx[i]);
          fsPars.push_back(std::make_tuple(PtEtaPhiM_t(p.cand_pT().at(fsIdx[i]), p.cand_eta().at(fsIdx[i]), p.cand_phi().at(fsIdx[i]), masses[i]), ids[i],
              p.trk_beta().at(trkIdx[i]), p.trk_tMTD().at(trkIdx[i]), p.trk_tMTDErr().at(trkIdx[i]), p.trk_pathLength().at(trkIdx[i])));
        }
        // check PID
        bool passMTD = true;
        for (size_t i=0; i<3; i++) {
          passMTD = passMTD && checkPassPID(fsPars[i], 1.0);
          //passMTD = passMTD && checkPassPID(fsPars[i], 1.0) && i !=0 ? std::get<0>(fsPars[i]).Pt() > 0.8 : true;
          //passMTD = passMTD && checkPassPID(fsPars[i], 1.0) && std::get<0>(fsPars[i]).Pt() > 0.7;
        }
        if (!ana::passKinematic(p.cand_pT().at(ireco), p.cand_y().at(ireco), kins)) continue; // a sharp cut during background tree production
        if (!ana::passTopoCuts(p.cand_decayLength3D().at(ireco)/p.cand_decayLengthError3D().at(ireco),
              p.cand_angle3D().at(ireco), p.cand_vtxProb().at(ireco), topo)) continue; // a sharp cut during background tree production
        hLooseRecoYVsPt->Fill(p.cand_pT().at(ireco), p.cand_y().at(ireco));
        // check phi mass
        //ROOT::Math::PtEtaPhiMVector pNegK(std::get<0>(fsPars[1]));
        //ROOT::Math::PtEtaPhiMVector pPosK(std::get<0>(fsPars[2]));
        //if (std::abs((pNegK+pPosK).M()-1.0195) > 0.005) continue;
        
        if (std::abs(p.cand_mass().at(dauIdx.at(1))-1.0195) > 0.005) continue;
        //if ( p.cand_pT().at(ireco) < 6 &&
            //std::abs(p.cand_mass().at(dauIdx.at(1))-1.0195) > 0.005) continue;
        if ( p.cand_pT().at(ireco) < 4 &&
            std::abs(p.cand_mass().at(dauIdx.at(1))-1.0195) > 0.004) continue;
        if ( p.cand_pT().at(ireco) < 3 &&
            std::abs(p.cand_mass().at(dauIdx.at(1))-1.0195) > 0.003) continue;
        // track pT cut
        if (p.cand_pT().at(ireco)<3 && std::abs(p.cand_y().at(ireco))<1) {
          if (p.cand_pT().at(dauIdx.at(0)) < 0.7) continue;
          if (p.cand_pT().at(gDauIdx.at(0)) < 0.7) continue;
          if (p.cand_pT().at(gDauIdx.at(1)) < 0.7) continue;
        }

        // topo cuts
        if (!ana::DsCuts(ipt, iy, topos)) continue;

        hFinalRecoYVsPt->Fill(p.cand_pT().at(ireco), p.cand_y().at(ireco));

        if (passMTD) hMassVsPtVsY["WMTD"]->Fill(std::abs(p.cand_y().at(ireco)), p.cand_pT().at(ireco), p.cand_mass().at(ireco));
        hMassVsPtVsY["WoMTD"]->Fill(std::abs(p.cand_y().at(ireco)), p.cand_pT().at(ireco), p.cand_mass().at(ireco));

        // 0-10
        if (p.centrality() < 20) {
          if (passMTD) hMassVsPtVsY_0_10["WMTD"]->Fill(std::abs(p.cand_y().at(ireco)), p.cand_pT().at(ireco), p.cand_mass().at(ireco));
          hMassVsPtVsY_0_10["WoMTD"]->Fill(std::abs(p.cand_y().at(ireco)), p.cand_pT().at(ireco), p.cand_mass().at(ireco));
        }
        // 0-20
        if (p.centrality() < 40) {
          if (passMTD) hMassVsPtVsY_0_20["WMTD"]->Fill(std::abs(p.cand_y().at(ireco)), p.cand_pT().at(ireco), p.cand_mass().at(ireco));
          hMassVsPtVsY_0_20["WoMTD"]->Fill(std::abs(p.cand_y().at(ireco)), p.cand_pT().at(ireco), p.cand_mass().at(ireco));
        }
        // 30-50
        if (p.centrality() < 100 && p.centrality() >= 60) {
          if (passMTD) hMassVsPtVsY_30_50["WMTD"]->Fill(std::abs(p.cand_y().at(ireco)), p.cand_pT().at(ireco), p.cand_mass().at(ireco));
          hMassVsPtVsY_30_50["WoMTD"]->Fill(std::abs(p.cand_y().at(ireco)), p.cand_pT().at(ireco), p.cand_mass().at(ireco));
        }

        // topo cuts studies begin
        // deleted
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
        // pi
        auto piIdx = Dsmeson_copy.daughter(0)->treeIdx();
        double piPt = p.gen_pT().at(piIdx);
        double piEta = p.gen_eta().at(piIdx);
        if (!ana::passTrackKinematicCuts(piEta, piPt)) continue;
        // K-
        auto negKIdx = Dsmeson_copy.daughter(0)->treeIdx();
        double negKPt = p.gen_pT().at(negKIdx);
        double negKEta = p.gen_eta().at(negKIdx);
        if (!ana::passTrackKinematicCuts(negKEta, negKPt)) continue;
        // K+
        auto posKIdx = Dsmeson_copy.daughter(1)->treeIdx();
        double posKPt = p.gen_pT().at(posKIdx);
        double posKEta = p.gen_eta().at(posKIdx);
        if (!ana::passTrackKinematicCuts(posKEta, posKPt)) continue;
        hAccGenYVsPt->Fill(p.gen_pT().at(i), p.gen_y().at(i));
      }
    } // end saveGen
  } // end
  cout << nMultipleMatch << " events has more than one matched RECO Ds meson" << endl;
  ofile.cd();
  for (const auto& e : hDsMassMatch) e.second->Write();
  for (const auto& e : hTrkBetaInvVsP) e.second->Write();
  for (const auto& e : hMassVsPtVsY) e.second->Write();
  for (const auto& e : hMassVsPtVsY_0_10) e.second->Write();
  for (const auto& e : hMassVsPtVsY_0_20) e.second->Write();
  for (const auto& e : hMassVsPtVsY_30_50) e.second->Write();
  for (size_t i=0; i<3; i++) {
    hFSpT[i]->Write();
    hFSp[i]->Write();
    for (const auto& e : hDauPVsDauEtaVsDsPt[i]) e.second->Write();
  }
  hCent->Write();
  if (saveGen) hGenYVsPt->Write();
  if (saveGen) hAccGenYVsPt->Write();
  hRecoYVsPt->Write();
  hLooseRecoYVsPt->Write();
  hFinalRecoYVsPt->Write();

  return 0;
}
