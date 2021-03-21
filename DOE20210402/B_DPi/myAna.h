#include "TF1.h"
#include "TH3.h"
#include <array>
#include <vector>

#ifndef _ANA_h_
#define _ANA_h_
namespace ana{
   // generic use
   const double c_cm_ns = 2.99792458e1; //[cm/ns]
   template<typename T>
   size_t whichBin(const T& vals, const double val) {
     size_t n = vals.size() - 1;
     for (size_t i=0; i<n; ++i) {
         if( val < vals.at(i+1) && val > vals.at(i)) return i;
     }
      return size_t(-1);
   }

   // for tracks
   const float massPion = 0.139570;
   const float massKaon = 0.493677;

   bool passTrackKinematicCuts(double eta, double pT) {
      if (std::abs(eta) > 3) return false;
      if (std::abs(eta) < 1.5 ? pT < 0.8 : pT * std::cosh(eta) < 0.7) return false;
      return true;
   }

   inline float invBetaPion(const float& p){
      return std::sqrt(1 + std::pow(massPion/p,2));
   }
   inline float invBetaKaon(const float& p){
      return std::sqrt(1 + std::pow(massKaon/p,2));
   }

   TF1 fExpBTL("fExpBTL_dInvBetaRMS","0.005 + 0.016*exp(-x/4.4)");
   TF1 fExpETL("fExpETL_dInvBetaRMS","0.003 + 0.006*exp(-x/7.6)");

   TF1 fPionResBTL("fPionResBTL", "0.0012*(x-2)*(x-2) + 0.0004", 0.7, 2);
   TF1 fPionResETL("fPionResETL", "0.00025*(x-2)*(x-2) + 0.0005", 0.7, 2);

   TF1 fKaonResBTL("fKaonResBTL", "0.003*(x-2)*(x-2) + 0.0001", 0.7, 2);
   TF1 fKaonResETL("fKaonResETL", "0.00025*(x-2)*(x-2) + 0.0003", 0.7, 2);

   float meanPion(const float eta, const float pT){
      const float p = std::cosh(eta) * pT;
      const float res = std::abs(eta) < 1.5 ? fPionResBTL.Eval(p) : fPionResETL(p);
      return p < 2 ? res : 0;
   }
   float meanKaon(const float eta, const float pT){
      const float p = std::cosh(eta) * pT;
      const float res = std::abs(eta) < 1.5 ? fKaonResBTL.Eval(p) : fKaonResETL(p);
      return p < 2 ? res : 0;
   }

   // for B
   const int nuOfY = 3; // for B meson
   const std::array<double, nuOfY+1> ybin = {0, 1.0, 2.0, 3.0}; // for B meson

   const int npt = 200;
   const double ptMin = 0;
   const double ptMax = 20;

   const int nyAbs = 30;
   const double yAbsMin = 0;
   const double yAbsMax = 3;

   const int nmass = 100;
   const double massMin = 5.06;
   const double massMax = 5.56;

   const float mass_lw[nuOfY] = {5.199, 5.199, 5.199}; // for B
   const float mass_up[nuOfY] = {5.359, 5.359, 5.359}; // for B

   bool isFWHM(double mass, double y) {
     size_t iy = whichBin(ybin, y);
     if(iy == size_t(-1)) return false;
     if( mass < mass_up[iy] && mass > mass_lw[iy]) return true;
     return false;
   }

   // for D
   const int nuOfDY = 3;
   const std::array<double, nuOfDY+1> Dybin{0, 1.0, 2.0, 3.0};
   const int nuOfDPt = 10;
   const std::array<double, nuOfDPt+1> Dptbin{0., 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 1000};

   // for D
  //|y|<1,   1<|y|<2, 2|y|<3
   const double Dagl3DCut[nuOfDPt][nuOfDY] = {
      2.0, 0.3, 0.2,       // 0.0 - 0.5 GeV
      0.6, 0.2, 0.2,       // 0.5 - 1.0 GeV
      0.3, 0.2, 0.2,       // 1.0 - 2.0 GeV
      0.2, 0.15, 0.15,     // 2.0 - 3.0 GeV
      0.1, 0.1, 0.1,       // 3.0 - 4.0 GeV
      0.1, 0.1, 0.1,       // 4.0 - 5.0 GeV
      0.1, 0.1, 0.1,       // 5.0 - 6.0 GeV
      0.05, 0.05, 0.05,    // 6.0 - 8.0 GeV
      0.05, 0.05, 0.05,     // 8.0 - 10.0 GeV
      0.05, 0.05, 0.05,     // 10.0 - 1000.0 GeV
   };

   // for D
   const double DdlSig3DCut[nuOfDPt][nuOfDY] = {
      0.0, 0.0, 0.0,
      0.0, 0.0, 0.0,
      0.0, 0.0, 0.0,
      3.0, 3.0, 3.0,
      3.5, 3.5, 3.5,
      3.5, 3.5, 3.5,
      4.0, 4.0, 4.0,
      5.0, 5.0, 5.0,
      5.0, 5.0, 5.0,
      5.0, 5.0, 5.0
   };

   // for D
   bool passDTopoCuts(double pT, double y, double angle, double dlSig){
      const size_t ipt = whichBin(Dptbin, pT);
      const size_t iy  = whichBin(Dybin,  y );
      if(ipt == size_t(-1)) return false;
      if(iy  == size_t(-1)) return false;
      if(std::fabs(angle) > Dagl3DCut[ipt][iy]) return false;
      if(dlSig < DdlSig3DCut[ipt][iy]) return false;
      return true;
   }

   // need to update
   const double evts_bkg_MB = 4799;
   const double evts_bkg_central = 673;

   const double evts_data_MB = 500e6;

   const double TAA0_100 = 5.61; // mb^-1
   const double TAA0_10 = 23.2; // mb^-1
   const double pbOvermb = 1e-12/1e-3;
   const double GeV = 1;
   const double BR = 0.0393 * 1.1;

   const double evts_sim_MB = 25e9;
   const double evts_sim_central = evts_sim_MB * evts_bkg_central / evts_bkg_MB;

};
#endif
