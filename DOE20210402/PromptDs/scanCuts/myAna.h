#include "TF1.h"
#include <array>
#include <vector>

#ifndef _DsANA_h_
#define _DsANA_h_
namespace ana{
   // for Ds
   const int nuOfY = 3; // for Ds meson
   const std::array<double, nuOfY+1> ybin = {0, 1.0, 2.0, 3.0}; // for Ds meson
   const int nuOfPt = 10;
   const std::array<double, nuOfPt+1> ptbin{0., 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 1000};

   const int npt = 200;
   const double ptMin = 0;
   const double ptMax = 20;

   const int nyAbs = 30;
   const double yAbsMin = 0;
   const double yAbsMax = 3;

   const int nmass = 120;
   const double massMin = 1.91;
   const double massMax = 2.03;

   const float mass_lw[nuOfY] = {1.96, 1.95, 1.95}; // for Ds
   const float mass_up[nuOfY] = {1.976, 1.986, 1.986}; // for Ds

   bool isFWHM(const float mass, size_t iy) {
     return mass > mass_lw[iy] && mass < mass_up[iy];
   }

   const double DsDL[npt][nyAbs] {
        {0., 0., 0.}, // 0-0.5
        {0., 0., 0.}, // 0.5-1
        {0., 0., 0.}, // 1-2
        {2., 1., 0.}, // 2-3
        {2.4, 1., 1}, // 3-4
        {2.8, 2.4, 2.4}, // 4-5
        {2.8, 2.6, 2.4}, // 5-6
        {1.0, 2.6, 2.}, // 6-8
        {0., 1., 1.6}, // 8-10
        {0., 0., 0.}  // >10
     };
   const double DsAngle[npt][nyAbs] {
        {4., 4., 4.}, // 0-0.5
        {4., 4., 4.}, // 0.5-1
        {4., 4., 4.}, // 1-2
        {4., 4., 0.18}, // 2-3
        {0.42, 0.24, 0.1}, // 3-4
        {0.28, 0.16, 0.1}, // 4-5
        {0.28, 0.16, 0.08}, // 5-6
        {0.24, 0.16, 0.22}, // 6-8 // lack of statistics in forward
        {0.24, 0.12, 0.26}, // 8-10
        {4., 4., 4.}  // >10
     };
   const double PiPt[npt][nyAbs] {
        {0., 0., 0.}, // 0-0.5
        {0., 0., 0.}, // 0.5-1
        {0., 0., 0.}, // 1-2
        {0.7, 0.7, 0.}, // 2-3
        {0.8, 0.7, 0.}, // 3-4
        {0., 0.7, 0.}, // 4-5
        {0., 0.7, 0.}, // 5-6
        {0., 0., 0.}, // 6-8
        {0., 0., 0.7}, // 8-10
        {0., 0., 0.}  // >10
     };
   const double NegKPt[npt][nyAbs] {
        {0., 0., 0.}, // 0-0.5
        {0., 0., 0.}, // 0.5-1
        {0., 0., 0.}, // 1-2
        {0., 0., 0.}, // 2-3
        {0., 0., 0.}, // 3-4
        {0., 0., 0.}, // 4-5
        {0., 0., 0.}, // 5-6
        {0., 0., 0.}, // 6-8
        {0., 0., 0.}, // 8-10
        {0., 0., 0.}  // >10
     };
   const double PosKPt[npt][nyAbs] {
        {0., 0., 0.}, // 0-0.5
        {0., 0., 0.}, // 0.5-1
        {0., 0., 0.}, // 1-2
        {0., 0., 0.}, // 2-3
        {0., 0., 0.}, // 3-4
        {0., 0., 0.}, // 4-5
        {0., 0., 0.}, // 5-6
        {0., 0., 0.}, // 6-8
        {0., 0., 0.}, // 8-10
        {0., 0., 0.}  // >10
     };

   bool DsCuts(size_t ipt, size_t iy, Double_t *par)
   {
     // Ds DL
     if (par[0] < DsDL[ipt][iy]) return false;
     // Ds Angle
     if (par[1] > DsAngle[ipt][iy]) return false;
     // Pi Pt
     if (par[2] < PiPt[ipt][iy]) return false;
     // NegK pT
     if (par[3] < NegKPt[ipt][iy]) return false;
     // PosK pT
     if (par[4] < PosKPt[ipt][iy]) return false;
     return true;
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
      const size_t iy  = whichBin(Dybin,  std::abs(y) );
      if(ipt == size_t(-1)) return false;
      if(iy  == size_t(-1)) return false;
      if(std::fabs(angle) > Dagl3DCut[ipt][iy]) return false;
      if(dlSig < DdlSig3DCut[ipt][iy]) return false;
      return true;
   }

   // need to update

   // const double evts_data_MB = 500e6; // not useful

   const double BR = 0.0225 * 1.1; // for Ds

   bool reject = true;
   Double_t f2ndpoly(Double_t *x, Double_t *par)
   {
     if ( reject && x[0] < 1.989 && x[0] > 1.947 ) {
       TF1::RejectPoint();
       return 0;
     }
     return (par[0] + par[1] * x[0] + par[2] * x[0] * x[0])*par[3];
   }

   const double fitRangeLw = 1.93;
   const double fitRangeUp = 2.00;

};
#endif
