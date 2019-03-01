#ifndef _ANA_h_
#define _ANA_h_
namespace ana{
   const float massPion = 0.139570;
   const float massKaon = 0.493677;

   const int nuOfY = 12;
   const float ybin[nuOfY+1] = {-3, -2.5, -2., -1.5, -1., -0.5, 0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
   const int nuOfPt = 10;
   const float ptbin[nuOfPt+1] = {0., 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0};
};
#endif
