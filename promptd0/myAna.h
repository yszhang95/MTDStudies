#include "TF1.h"
#include "D0Cand.h"
#include "HyJets.h"

#ifndef _ANA_h_
#define _ANA_h_
namespace ana{
   const float massPion = 0.139570;
   const float massKaon = 0.493677;

   const int nuOfY = 3;
   const double ybin[nuOfY+1] = {0, 1.0, 2.0, 3.0};
   const int nuOfPt = 9;
   const double ptbin[nuOfPt+1] = {0., 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0};

   const int npt = 100;
   const double ptMin = 0;
   const double ptMax = 10;

   const int nyAbs = 60;
   const double yAbsMin = 0;
   const double yAbsMax = 3;

   const int nmass = 60;
   const double massMin = 1.7;
   const double massMax = 2.0;

   const int nVtxProb = 60;
   const double VtxProbMin = 0.0;
   const double VtxProbMax = 0.3;

   const int nagl3D = 120;
   const double agl3DMin = 0.;
   const double agl3DMax = 3.;

   const int ndlSig3D = 200;
   const double dlSig3DMin = 0.;
   const double dlSig3DMax = 5.;

   const double evts_bkg_MB = 966;
   const double evts_bkg_central = 139;

   const double evts_data_MB = 500e6;

   const double TAA0_100 = 5.61; // mb^-1
   const double TAA0_10 = 23.2; // mb^-1
   const double pbOvermb = 1e-12/1e-3;
   const double GeV = 1;
   const double BR = 0.0393 * 1.1;

   const double evts_sim_MB = 25e9;
   const double evts_sim_central = evts_sim_MB * evts_bkg_central / evts_bkg_MB;

   const float mass_lw[nuOfY] = {1.85,  1.84, 1.82};
   const float mass_up[nuOfY] = {1.88,  1.89, 1.91};

   bool isCentralEvt(const HyJets& t){ return t.centrality < 20;}

   bool passKinematicCuts(D0Cand* t ) {
      if ( std::fabs(t->etaD1()) > 3  ||  std::fabs(t->etaD2()) > 3  ||  std::fabs(t->Y()) > 3 ) return false;
      if ( std::fabs(t->etaD1()) < 1.5 ? t->PtD1() <= 0.8 : (t->PtD1() * std::cosh(t->etaD1())) <= 0.7 ) return false;
      if ( std::fabs(t->etaD2()) < 1.5 ? t->PtD2() <= 0.8 : (t->PtD2() * std::cosh(t->etaD2())) <= 0.7 ) return false;
      return true;
   }

   int whichY(D0Cand* t){
      for(int iy=0; iy<nuOfY; iy++){
         if(t->Y() < ybin[iy+1] && t->Y() > ybin[iy]) return iy;
      }
      return -1;
   }

   bool isFWHM(D0Cand* t){
      int iy = whichY(t);
      if(iy == -1) return false;
      if( t->Mass() < mass_up[iy] && t->Mass() > mass_lw[iy]) return true;
      return false;
   }

   bool passTopoCuts(D0Cand *t){
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

   TF1 fPionResBTL("fPionResBTL", "0.0012*(x-2)*(x-2) + 0.0004", 0.7, 2);
   TF1 fPionResETL("fPionResETL", "0.00025*(x-2)*(x-2) + 0.0005", 0.7, 2);

   TF1 fKaonResBTL("fKaonResBTL", "0.003*(x-2)*(x-2) + 0.0001", 0.7, 2);
   TF1 fKaonResETL("fKaonResETL", "0.00025*(x-2)*(x-2) + 0.0003", 0.7, 2);

   float meanPion(D0Cand* t, const int& dau){

      if(dau == 1) {
         const float pD1 = std::cosh(t->etaD1()) * t->PtD1() ;
         if(std::fabs(t->etaD1()) < 1.5) return pD1 < 2 ? fPionResBTL.Eval(pD1) : 0;
         if(std::fabs(t->etaD1()) >= 1.5) return pD1 < 2 ? fPionResETL.Eval(pD1) : 0;
      }
      if(dau == 2) {
         const float pD2 = std::cosh(t->etaD2()) * t->PtD2() ;
         if(std::fabs(t->etaD2()) < 1.5) return pD2 < 2 ? fPionResBTL.Eval(pD2) : 0;
         if(std::fabs(t->etaD2()) >= 1.5) return pD2 < 2 ? fPionResETL.Eval(pD2) : 0;
      }
      return -99.;
   }

   float meanKaon(D0Cand* t, const int& dau){

      if(dau == 1) {
         const float pD1 = std::cosh(t->etaD1()) * t->PtD1() ;
         if(std::fabs(t->etaD1()) < 1.5) return pD1 < 2 ? fKaonResBTL.Eval(pD1) : 0;
         if(std::fabs(t->etaD1()) >= 1.5) return pD1 < 2 ? fKaonResETL.Eval(pD1) : 0;
      }
      if(dau == 2) {
         const float pD2 = std::cosh(t->etaD2()) * t->PtD2() ;
         if(std::fabs(t->etaD2()) < 1.5) return pD2 < 2 ? fKaonResBTL.Eval(pD2) : 0;
         if(std::fabs(t->etaD2()) >= 1.5) return pD2 < 2 ? fKaonResETL.Eval(pD2) : 0;
      }
      return -99.;
   }
};
#endif
