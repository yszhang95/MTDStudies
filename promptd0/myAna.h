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
};
#endif
