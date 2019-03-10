#include "myAna.h"
#include "matchD.h"

inline float invBetaPion(const float& p){
      return std::sqrt(1 + std::pow(ana::massPion/p,2));
}

inline float invBetaKaon(const float& p){
      return std::sqrt(1 + std::pow(ana::massKaon/p,2));
}

bool passTopoCuts(matchD* t)
{
   return true;
}


void fillPromptDMass()
{
   TH1::SetDefaultSumw2(true);

   TF1 *fExpPion, *fExpKaon;

   TF1* fExpBTL = new TF1("fExpBTL_dInvBetaRMS","0.005 + 0.016*exp(-x/4.4)");
   TF1* fExpETL = new TF1("fExpETL_dInvBetaRMS","0.003 + 0.006*exp(-x/7.6)");

   TFile* f1 = new TFile("matchPromptD0_fullSample_reRECO.root");
   TNtuple* tp = (TNtuple*) f1->Get("PromptD");
   matchD* t = new matchD(tp);
   std::cout << t->GetEntries() << std::endl;

   double massbin[61];
   for(int i=0; i<61; i++){
      massbin[i] = (float)i * 0.005 + 1.7;
   }

   TH3F* hMassVsPtVsY = new TH3F("hMassVsPtVsY", "hMassVsPtVsY", 60, -3, 3, 100, 0, 10, 60, 1.7, 2.0);

   TH3F* hMassVsPtVsYMtd = new TH3F("hMassVsPtVsYMtd", "hMassVsPtVsYMtd", 60, -3, 3, 100, 0, 10, 60, 1.7, 2.0);

   std::cout << hMassVsPtVsY->GetZaxis()->FindBin(1.7) << std::endl;;
   
   for(Long64_t ientry=0; ientry<t->GetEntries(); ientry++){
      t->GetEntry(ientry);

      if(std::fabs(t->EtaD1) > 3) continue;
      if(std::fabs(t->EtaD2) > 3) continue;

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);

      if(std::fabs(t->EtaD1) < 1.4 ? t->pTD1 <= 0.8 : pD1 <= 0.7) continue;
      if(std::fabs(t->EtaD2) < 1.4 ? t->pTD2 <= 0.8 : pD2 <= 0.7) continue;

      const float dInvBetaCut1 = std::fabs(t->EtaD1<1.5) ? fExpBTL->Eval(pD1) : fExpETL->Eval(pD1);
      const float dInvBetaCut2 = std::fabs(t->EtaD2<1.5) ? fExpBTL->Eval(pD2) : fExpETL->Eval(pD2);

      if(!passTopoCuts(t)) continue;

      if(!(t->matchGEN && !t->isSwap)) continue;
      hMassVsPtVsY->Fill(t->y, t->pT, t->mass);

      bool is1sigmaPionDau1 = true;
      bool is1sigmaKaonDau1 = true;
      bool is1sigmaPionDau2 = true;
      bool is1sigmaKaonDau2 = true;

      if(t->isMtdDau1) is1sigmaPionDau1 = std::fabs(1./t->beta1_PV - invBetaPion(pD1)) < 1.0 * dInvBetaCut1;
      if(t->isMtdDau1) is1sigmaKaonDau1 = std::fabs(1./t->beta1_PV - invBetaKaon(pD1)) < 1.0 * dInvBetaCut1;
      if(t->isMtdDau2) is1sigmaPionDau2 = std::fabs(1./t->beta2_PV - invBetaPion(pD2)) < 1.0 * dInvBetaCut2;
      if(t->isMtdDau2) is1sigmaKaonDau2 = std::fabs(1./t->beta2_PV - invBetaKaon(pD2)) < 1.0 * dInvBetaCut2;
      if((t->flavor == 1 && is1sigmaPionDau1 && is1sigmaKaonDau2) || (t->flavor == -1 && is1sigmaKaonDau1 && is1sigmaPionDau2)) {
         hMassVsPtVsYMtd->Fill(t->y, t->pT, t->mass);
      }
   }

   TFile fout("promptd0MassHists_reRECO_all.root", "recreate");
   hMassVsPtVsY->Write();
   hMassVsPtVsYMtd->Write();
}
