#include "myAna.h"
#include "LamC3P.h"

void fillLamC3PMass()
{

   TChain* tp = new TChain("LamC3P");
   tp->Add("matchLamC3PTree_fullSample_reRECO.root");
   LamC3P* t = new LamC3P(tp);
   std::cout << t->GetEntries() << std::endl;

   TH3D* hMassVsPtVsY = new TH3D("hMassVsPtVsY", "hMassVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);

   TH3D* hVtxProbVsPtVsY = new TH3D("hVtxProbVsPtVsY", "hVtxProbVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

   TH3D* hagl3DVsPtVsY = new TH3D("hagl3DVsPtVsY", "hagl3DVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);

   TH3D* hdlSig3DVsPtVsY = new TH3D("hdlSig3DVsPtVsY", "hdlSig3DVsPtVsY", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);

   TH3D* hMassVsPtVsYMtd = new TH3D("hMassVsPtVsYMtd", "hMassVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nmass, ana::massMin, ana::massMax);

   TH3D* hVtxProbVsPtVsYMtd = new TH3D("hVtxProbVsPtVsYMtd", "hVtxProbVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nVtxProb, ana::VtxProbMin, ana::VtxProbMax);

   TH3D* hagl3DVsPtVsYMtd = new TH3D("hagl3DVsPtVsYMtd", "hagl3DVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::nagl3D, ana::agl3DMin, ana::agl3DMax);

   TH3D* hdlSig3DVsPtVsYMtd = new TH3D("hdlSig3DVsPtVsYMtd", "hdlSig3DVsPtVsYMtd", 
         ana::nyAbs, ana::yAbsMin, ana::yAbsMax, ana::npt, ana::ptMin, ana::ptMax, ana::ndlSig3D, ana::dlSig3DMin, ana::dlSig3DMax);

   std::cout << hMassVsPtVsY->GetZaxis()->FindBin(2.15) << std::endl;;
   
   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      if(!ana::passKinematicCuts(t)) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);
      const float pD3 = t->pTD3 * std::cosh(t->EtaD3);

      const float dInvBetaCut1 = std::fabs(t->EtaD1<1.5) ? ana::fExpBTL.Eval(pD1) : ana::fExpETL.Eval(pD1);
      const float dInvBetaCut2 = std::fabs(t->EtaD2<1.5) ? ana::fExpBTL.Eval(pD2) : ana::fExpETL.Eval(pD2);
      const float dInvBetaCut3 = std::fabs(t->EtaD3<1.5) ? ana::fExpBTL.Eval(pD3) : ana::fExpETL.Eval(pD3);

      if(!ana::passTopoCuts(t)) continue;
      bool isFWHM = ana::isFWHM(t);

      if(!(t->matchGEN && !t->isSwap)) continue;

      bool is1sigmaPionDau1 = false;
      bool is1sigmaKaonDau1 = false;
      bool is1sigmaProtonDau1 = false;
      bool is1sigmaPionDau2 = false;
      bool is1sigmaKaonDau2 = false;
      bool is1sigmaProtonDau2 = false;
      bool is1sigmaPionDau3 = false;
      bool is1sigmaKaonDau3 = false;
      bool is1sigmaProtonDau3 = false;

      if(t->isMtdDau1) is1sigmaPionDau1 = std::fabs(1./t->beta1_PV - ana::invBetaPion(pD1) - ana::meanPion(t, 1) ) < 1.4 * dInvBetaCut1;
      if(t->isMtdDau1) is1sigmaKaonDau1 = std::fabs(1./t->beta1_PV - ana::invBetaKaon(pD1) - ana::meanKaon(t, 1) ) < 1.4 * dInvBetaCut1;
      if(t->isMtdDau1) is1sigmaProtonDau1 = std::fabs(1./t->beta1_PV - ana::invBetaProton(pD1)  ) < 1.4 * dInvBetaCut1;
      if(t->isMtdDau2) is1sigmaPionDau2 = std::fabs(1./t->beta2_PV - ana::invBetaPion(pD2) - ana::meanPion(t, 2) ) < 1.4 * dInvBetaCut2;
      if(t->isMtdDau2) is1sigmaKaonDau2 = std::fabs(1./t->beta2_PV - ana::invBetaKaon(pD2) - ana::meanKaon(t, 2) ) < 1.4 * dInvBetaCut2;
      if(t->isMtdDau2) is1sigmaProtonDau2 = std::fabs(1./t->beta2_PV - ana::invBetaProton(pD2) ) < 1.4 * dInvBetaCut2;
      if(t->isMtdDau3) is1sigmaPionDau3 = std::fabs(1./t->beta3_PV - ana::invBetaPion(pD3) - ana::meanPion(t, 3) ) < 1.4 * dInvBetaCut3;
      if(t->isMtdDau3) is1sigmaKaonDau3 = std::fabs(1./t->beta3_PV - ana::invBetaKaon(pD3) - ana::meanKaon(t, 3) ) < 1.4 * dInvBetaCut3;
      if(t->isMtdDau3) is1sigmaProtonDau3 = std::fabs(1./t->beta3_PV - ana::invBetaProton(pD3)  ) < 1.4 * dInvBetaCut3;
      if(is1sigmaProtonDau3) std::cout << std::endl;

      if((t->flavor == 1 && is1sigmaKaonDau2 && ((is1sigmaPionDau1 && is1sigmaProtonDau3) || (is1sigmaPionDau3 && is1sigmaProtonDau1))) ||
         (t->flavor == -1 && is1sigmaKaonDau1 && ((is1sigmaPionDau2 && is1sigmaProtonDau3) || (is1sigmaPionDau3 && is1sigmaProtonDau2)))
        )
      {
         hMassVsPtVsY->Fill(std::fabs(t->y), t->pT, t->mass);
         if( isFWHM ) {
            hVtxProbVsPtVsY->Fill(std::fabs(t->y), t->pT, t->VtxProb);
            hagl3DVsPtVsY->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
            hdlSig3DVsPtVsY->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
         }
      }

      is1sigmaPionDau1 = false;
      is1sigmaKaonDau1 = false;
      is1sigmaProtonDau1 = false;
      is1sigmaPionDau2 = false;
      is1sigmaKaonDau2 = false;
      is1sigmaProtonDau2 = false;
      is1sigmaPionDau3 = false;
      is1sigmaKaonDau3 = false;
      is1sigmaProtonDau3 = false;

      if(t->isMtdDau1) is1sigmaPionDau1 = std::fabs(1./t->beta1_PV - ana::invBetaPion(pD1) - ana::meanPion(t, 1) ) < 1.0 * dInvBetaCut1;
      if(t->isMtdDau1) is1sigmaKaonDau1 = std::fabs(1./t->beta1_PV - ana::invBetaKaon(pD1) - ana::meanKaon(t, 1) ) < 1.0 * dInvBetaCut1;
      if(t->isMtdDau1) is1sigmaProtonDau1 = std::fabs(1./t->beta1_PV - ana::invBetaProton(pD1) - ana::meanProton(t, 1) ) < 1.0 * dInvBetaCut1;
      if(t->isMtdDau2) is1sigmaPionDau2 = std::fabs(1./t->beta2_PV - ana::invBetaPion(pD2) - ana::meanPion(t, 2) ) < 1.0 * dInvBetaCut2;
      if(t->isMtdDau2) is1sigmaKaonDau2 = std::fabs(1./t->beta2_PV - ana::invBetaKaon(pD2) - ana::meanKaon(t, 2) ) < 1.0 * dInvBetaCut2;
      if(t->isMtdDau2) is1sigmaProtonDau2 = std::fabs(1./t->beta2_PV - ana::invBetaProton(pD2) - ana::meanProton(t, 2) ) < 1.0 * dInvBetaCut2;
      if(t->isMtdDau3) is1sigmaPionDau3 = std::fabs(1./t->beta3_PV - ana::invBetaPion(pD3) - ana::meanPion(t, 3) ) < 1.0 * dInvBetaCut3;
      if(t->isMtdDau3) is1sigmaKaonDau3 = std::fabs(1./t->beta3_PV - ana::invBetaKaon(pD3) - ana::meanKaon(t, 3) ) < 1.0 * dInvBetaCut3;
      if(t->isMtdDau3) is1sigmaProtonDau3 = std::fabs(1./t->beta3_PV - ana::invBetaProton(pD3) - ana::meanProton(t, 3) ) < 1.0 * dInvBetaCut3;

      if((t->flavor == 1 && is1sigmaKaonDau2 && ((is1sigmaPionDau1 && is1sigmaProtonDau3) || (is1sigmaPionDau3 && is1sigmaProtonDau1))) ||
         (t->flavor == -1 && is1sigmaKaonDau1 && ((is1sigmaPionDau2 && is1sigmaProtonDau3) || (is1sigmaPionDau3 && is1sigmaProtonDau2)))
        )
      {
         hMassVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->mass);
         if( isFWHM ) {
            hVtxProbVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->VtxProb);
            hagl3DVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->m3DPointingAngle);
            hdlSig3DVsPtVsYMtd->Fill(std::fabs(t->y), t->pT, t->m3DDecayLengthSignificance);
         }
      }
   }

   TFile fout("lamC3PMassHists_reRECO_all.root", "recreate");
   hMassVsPtVsY->Write();
   hMassVsPtVsYMtd->Write();
   hVtxProbVsPtVsY->Write();
   hVtxProbVsPtVsYMtd->Write();
   hagl3DVsPtVsY->Write();
   hagl3DVsPtVsYMtd->Write();
   hdlSig3DVsPtVsY->Write();
   hdlSig3DVsPtVsYMtd->Write();
}
