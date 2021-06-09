#include "TF1.h"
#include <array>
#include <vector>

#ifndef _DsANA_h_
#define _DsANA_h_
namespace ana{
   // for Ds
   const int nuOfY = 3; // for LambdaC
   const std::array<double, nuOfY+1> ybin = {0, 1.0, 2.0, 3.0}; // for LambdaC

   const int npt = 200;
   const double ptMin = 0;
   const double ptMax = 20;

   const int nyAbs = 30;
   const double yAbsMin = 0;
   const double yAbsMax = 3;

   const int ny = 60;
   const double yMin = -3;
   const double yMax = 3;

   const int nmass = 60;
   const double massMin = 2.15;
   const double massMax = 2.45;

   const float mass_lw[nuOfY] = {2.286-0.01, 2.286-0.02, 2.286-0.03}; // for LambdaC
   const float mass_up[nuOfY] = {2.286+0.01, 2.286+0.02, 2.286+0.03}; // for LambdaC


   // need to update

   // const double evts_data_MB = 500e6; // not useful

   const double BR = 0.05 * 1.1; // for LambdaC

};
#endif
