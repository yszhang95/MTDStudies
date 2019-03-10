#include "TF1.h"
#include "D0Cand.h"
#include "HyJets.h"

#ifndef _ANA_h_
#define _ANA_h_
namespace ana{
   const float massPion = 0.139570;
   const float massKaon = 0.493677;

   const int nuOfY = 12;
   const double ybin[nuOfY+1] = {-3, -2.5, -2., -1.5, -1., -0.5, 0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
   const int nuOfPt = 10;
   const double ptbin[nuOfPt+1] = {0., 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0};

   //const double evts_bkg_MB = 1273;
   //const double evts_bkg_central = 121;
   const double evts_bkg_MB = 960;
   const double evts_bkg_central = 151;

   const double evts_data_MB = 500e6;

   const double TAA0_100 = 5.61; // mb^-1
   const double TAA0_10 = 23.2; // mb^-1
   const double pbOvermb = 1e-12/1e-3;
   const double GeV = 1;
   const double BR = 0.0393;

   const double evts_sim_MB = 25e9;
   const double evts_sim_central = evts_sim_MB * evts_bkg_central / evts_bkg_MB;

   const float mass_lw[nuOfY] = {1.82, 1.825, 1.835, 1.845, 1.85, 1.85, 1.85, 1.85, 1.845, 1.835, 1.825, 1.82};
   const float mass_up[nuOfY] = {1.91, 1.905, 1.895, 1.885, 1.88, 1.88, 1.88, 1.88, 1.885, 1.895, 1.905, 1.91};

   const bool isKeepNoMtd = false;

   bool isCentralEvt(const HyJets& t){ return t.centrality < 20;}

   bool passKinematicCuts(D0Cand* t ) {
      //if ( std::fabs(t.EtaD1) > 3  ||  std::fabs(t.EtaD2) > 3  ||  std::fabs(t.y) > 3 ) return false;
      //if ( std::fabs(t.EtaD1) < 1.5 ? t.pTD1 <= 0.8 : (t.pTD1 * std::cosh(t.EtaD1)) <= 0.7 ) return false;
      //if ( std::fabs(t.EtaD2) < 1.5 ? t.pTD2 <= 0.8 : (t.pTD2 * std::cosh(t.EtaD2)) <= 0.7 ) return false;
      if ( std::fabs(t->etaD1()) > 3  ||  std::fabs(t->etaD2()) > 3  ||  std::fabs(t->Y()) > 3 ) return false;
      if ( std::fabs(t->etaD1()) < 1.5 ? t->PtD1() <= 0.8 : (t->PtD1() * std::cosh(t->etaD1())) <= 0.7 ) return false;
      if ( std::fabs(t->etaD2()) < 1.5 ? t->PtD2() <= 0.8 : (t->PtD2() * std::cosh(t->etaD2())) <= 0.7 ) return false;
      return true;
   }
   inline float invBetaPion(const float& p){
      return std::sqrt(1 + std::pow(ana::massPion/p,2));
   }
   inline float invBetaKaon(const float& p){
      return std::sqrt(1 + std::pow(ana::massKaon/p,2));
   }
   TF1 fExpBTL("fExpBTL_dInvBetaRMS","0.005 + 0.016*exp(-x/4.4)");
   TF1 fExpETL("fExpETL_dInvBetaRMS","0.003 + 0.006*exp(-x/7.6)");
};
#endif
