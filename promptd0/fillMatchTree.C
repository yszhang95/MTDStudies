#include "PromptD.h"
#include "TMath.h"
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "THashList.h"
#include "TLatex.h"
#include "TFileCollection.h"
#include "TVector3.h"

#include "myAna.h"

inline float invBetaPion(const float p){
    return std::sqrt(1 + std::pow(ana::massPion/p,2));
}

inline float invBetaKaon(const float p){
    return std::sqrt(1 + std::pow(ana::massKaon/p,2));
}

void fillMatchTree()
{
   TH1F::SetDefaultSumw2(true);

   TChain *chain = new TChain("d0ana_mc/VertexCompositeNtuple");
   TFileCollection* fc = new TFileCollection("dum", "", "newD0Signal.list");
   chain->AddFileInfoList(fc->GetList()); 
   PromptD *t = new PromptD(chain);
   std::cout << "total entries: " << t->GetEntries() << std::endl;

   TH2F* hInvBetaVsPDau1 = new TH2F("hInvBetaVsPDau1", "hInvBetaVsPDau1", 1000, 0, 5, 1000, 0.9, 1.7);
   TH2F* hInvBetaVsPDau2 = new TH2F("hInvBetaVsPDau2", "hInvBetaVsPDau2", 1000, 0, 5, 1000, 0.9, 1.7);

   TH2F* hdInvBetaPionVsPDau1 = new TH2F("hdInvBetaPionVsPDau1", "hdInvBetaPionVsPDau1", 1000, 0, 5, 1000, 0.9, 1.7);
   TH2F* hdInvBetaPionVsPDau2 = new TH2F("hdInvBetaPionVsPDau2", "hdInvBetaPionVsPDau2", 1000, 0, 5, 1000, 0.9, 1.7);

   TH2F* hdInvBetaKaonVsPDau1 = new TH2F("hdInvBetaKaonVsPDau1", "hdInvBetaKaonVsPDau1", 1000, 0, 5, 1000, 0.9, 1.7);
   TH2F* hdInvBetaKaonVsPDau2 = new TH2F("hdInvBetaKaonVsPDau2", "hdInvBetaKaonVsPDau2", 1000, 0, 5, 1000, 0.9, 1.7);

   TTree* tree = new TTree("PromptD", "matchedD0");

   float pt;
   float eta;
   float flavor;
   float y;
   float mass;
   int Ntrkoffline;
   int Npixel;
   float HFsumET;
   float bestvx;
   float bestvy;
   float bestvz;

   float VtxProb;
   float dlos;
   float dl;
   float dlerror;
   float agl;
   float agl_abs;
   float agl2D;
   float agl2D_abs;
   float dlos2D;
   float dl2D;
   bool isSwap;
   bool matchGEN;
   int idmom_reco;
   float dzos1;
   float dzos2;
   float dxyos1;
   float dxyos2;
   float nhit1;
   float nhit2;
   bool trkquality1;
   bool trkquality2;
   float pt1;
   float pt2;    
   float ptErr1;
   float ptErr2;
   float eta1;
   float eta2;
   float H2dedx1;
   float H2dedx2;
   
    // mtd info
   float beta1_PV;     // mtd
   float beta1_PVerr;  // mtd
   float beta2_PV;
   float beta2_PVerr;
   float tmtd1;
   float tmtd2;
   float sigmatmtd1;
   float sigmatmtd2;
   float pathLength1;
   float pathLength2;
   bool  isMtdDau1;
   bool  isMtdDau2;
   bool  isMtdDau3;
   bool  isGoodMtdDau1;
   bool  isGoodMtdDau2;

   float t0_PV;
   float sigmat0_PV;

   float pt_gen;
   float eta_gen;
   int status_gen;
   int idmom;
   float y_gen;
   int iddau1;
   int iddau2;
   int iddau3;

   tree->Branch("pT",&pt,"pT/F");
   tree->Branch("y",&y,"y/F");
   tree->Branch("mass",&mass,"mass/F");

   tree->Branch("Ntrkoffline",&Ntrkoffline,"Ntrkoffline/I");
   tree->Branch("Npixel",&Npixel,"Npixel/I");
   tree->Branch("HFsumET",&HFsumET,"HFsumET/F");
   tree->Branch("bestvtxX",&bestvx,"bestvtxX/F");
   tree->Branch("bestvtxY",&bestvy,"bestvtxY/F");
   tree->Branch("bestvtxZ",&bestvz,"bestvtxZ/F");
        
   tree->Branch("flavor",&flavor,"flavor/F");
   tree->Branch("eta",&eta,"eta/F");
   tree->Branch("VtxProb",&VtxProb,"VtxProb/F");
   tree->Branch("3DCosPointingAngle",&agl,"3DCosPointingAngleF");
   tree->Branch("3DPointingAngle",&agl_abs,"3DPointingAngle/F");
   tree->Branch("2DCosPointingAngle",&agl2D,"2DCosPointingAngle/F");
   tree->Branch("2DPointingAngle",&agl2D_abs,"2DPointingAngle/F");
   tree->Branch("3DDecayLengthSignificance",&dlos,"3DDecayLengthSignificance/F");
   tree->Branch("3DDecayLength",&dl,"3DDecayLength/F");
   tree->Branch("2DDecayLengthSignificance",&dlos2D,"2DDecayLengthSignificance/F");
   tree->Branch("2DDecayLength",&dl2D,"2DDecayLength/F");

   tree->Branch("isSwap",&isSwap,"isSwap/O");
   tree->Branch("idmom_reco",&idmom_reco,"idmom_reco/I");
   tree->Branch("matchGEN",&matchGEN,"matchGEN/O");
   tree->Branch("zDCASignificanceDaugther1",&dzos1,"zDCASignificanceDaugther1/F");
   tree->Branch("xyDCASignificanceDaugther1",&dxyos1,"xyDCASignificanceDaugther1/F");
   tree->Branch("NHitD1",&nhit1,"NHitD1/F");
   tree->Branch("HighPurityDaugther1",&trkquality1,"HighPurityDaugther1/O");
   tree->Branch("pTD1",&pt1,"pTD1/F");
   tree->Branch("pTerrD1",&ptErr1,"pTerrD1/F");
   tree->Branch("EtaD1",&eta1,"EtaD1/F");
   tree->Branch("dedxHarmonic2D1",&H2dedx1,"dedxHarmonic2D1/F");
   tree->Branch("zDCASignificanceDaugther2",&dzos2,"zDCASignificanceDaugther2/F");
   tree->Branch("xyDCASignificanceDaugther2",&dxyos2,"xyDCASignificanceDaugther2/F");
   tree->Branch("NHitD2",&nhit2,"NHitD2/F");
   tree->Branch("HighPurityDaugther2",&trkquality2,"HighPurityDaugther2/O");
   tree->Branch("pTD2",&pt2,"pTD2/F");
   tree->Branch("pTerrD2",&ptErr2,"pTerrD2/F");
   tree->Branch("EtaD2",&eta2,"EtaD2/F");
   tree->Branch("dedxHarmonic2D2",&H2dedx2,"dedxHarmonic2D2/F");
   tree->Branch("t0_PV", &t0_PV, "t0_PV/F");
   tree->Branch("sigmat0_PV", &sigmat0_PV, "sigmat0_PV/F");
   tree->Branch("beta1_PV", &beta1_PV, "beta1_PV/F");
   tree->Branch("beta2_PV", &beta2_PV, "beta2_PV/F");
   tree->Branch("beta1_PVerr", &beta1_PVerr, "beta1_PVerr/F");
   tree->Branch("beta2_PVerr", &beta2_PVerr, "beta2_PVerr/F");
   tree->Branch("tmtd1", &tmtd1, "tmtd1/F");
   tree->Branch("tmtd2", &tmtd2, "tmtd2/F");
   tree->Branch("sigmatmtd1", &sigmatmtd1, "sigmatmtd1/F");
   tree->Branch("sigmatmtd2", &sigmatmtd2, "sigmatmtd2/F");
   tree->Branch("pathLength1", &pathLength1, "pathLength1/F");
   tree->Branch("pathLength2", &pathLength2, "pathLength2/F");
   tree->Branch("isMtdDau1", &isMtdDau1, "isMtdDau1/O");
   tree->Branch("isMtdDau2", &isMtdDau2, "isMtdDau2/O");
   tree->Branch("isGoodMtdDau1", &isGoodMtdDau1, "isGoodMtdDau1/O");
   tree->Branch("isGoodMtdDau2", &isGoodMtdDau2, "isGoodMtdDau2/O");
   tree->Branch("pT_gen",&pt_gen,"pT_gen/F");
   tree->Branch("eta_gen",&eta_gen,"eta_gen/F");
   tree->Branch("y_gen",&y_gen,"y_gen/F");
   tree->Branch("status_gen",&status_gen,"status_gen/I");
   tree->Branch("MotherID_gen",&idmom,"MotherID_gen/I");

   tree->Branch("DauID1_gen",&iddau1,"DauID1_gen/I");
   tree->Branch("DauID2_gen",&iddau2,"DauID2_gen/I");
   tree->Branch("DauID3_gen",&iddau3,"DauID3_gen/I");


   int isMtdWrong = 0;
   Long64_t nD0 = 0;
   for(Long64_t ientry=0; ientry<t->GetEntries(); ++ientry){
      t->GetEntry(ientry);

      const float pD1 = t->pTD1 * std::cosh(t->EtaD1);
      const float pD2 = t->pTD2 * std::cosh(t->EtaD2);
       // fill dau1 histograms
      if(t->isGoodMtdDau1 && std::fabs(t->EtaD1)<3){
         hInvBetaVsPDau1->Fill(pD1, 1./t->beta1_PV);
         hdInvBetaPionVsPDau1->Fill(pD1, 1./t->beta1_PV - invBetaPion(pD1));
         hdInvBetaKaonVsPDau1->Fill(pD1, 1./t->beta1_PV - invBetaKaon(pD1));
      } 
       // fill dau2 histograms
      if(t->isGoodMtdDau2 && std::fabs(t->EtaD2)<3){
         hInvBetaVsPDau2->Fill(pD2, 1./t->beta2_PV);
         hdInvBetaPionVsPDau2->Fill(pD2, 1./t->beta2_PV - invBetaPion(pD2));
         hdInvBetaKaonVsPDau2->Fill(pD2, 1./t->beta2_PV - invBetaKaon(pD2));
      } 

        // look up whether there is a case track has mtd hits when eta>3
      if(t->isMtdDau1 && std::fabs(t->EtaD1)>3) isMtdWrong++;
      if(t->isMtdDau2 && std::fabs(t->EtaD2)>3) isMtdWrong++;

        // cut those unmatched and swap candidates
      if(t->matchGEN && !t->isSwap) nD0++;
      if(!t->matchGEN || t->isSwap) continue;

      pt = t->pT;
      eta = t->eta;
      flavor = t->flavor;
      y = t->y;
      mass = t->mass;
      Ntrkoffline = t->Ntrkoffline;
      Npixel = t->Npixel;
      HFsumET = t->HFsumET;
      bestvx = t->bestvtxX;
      bestvx = t->bestvtxY;
      bestvx = t->bestvtxZ;
      VtxProb = t->VtxProb;
      dlos = t->m3DDecayLengthSignificance;
      dl = t->m3DDecayLength;
      agl = t->m3DCosPointingAngle;
      agl_abs = t->m3DPointingAngle;
      agl2D = t->m2DCosPointingAngle;
      agl2D_abs = t->m2DPointingAngle;
      dlos2D = t->m2DDecayLengthSignificance;
      dl2D = t->m2DDecayLength;
      isSwap = t->isSwap;
      matchGEN = t->matchGEN;
      idmom_reco = t->idmom_reco;
      dzos1 = t->zDCASignificanceDaugther1;
      dzos2 = t->zDCASignificanceDaugther2;
      dxyos1 = t->xyDCASignificanceDaugther1;
      dxyos2 = t->xyDCASignificanceDaugther2;
      nhit1 = t->NHitD1;
      nhit2 = t->NHitD2;
      trkquality1 = t->HighPurityDaugther1;
      trkquality2 = t->HighPurityDaugther2;
      pt1 = t->pTD1;
      pt2 = t->pTD2;    
      ptErr1 = t->pTerrD1;
      ptErr2 = t->pTerrD2;
      eta1 = t->EtaD1;
      eta2 = t->EtaD2;
      H2dedx1 = t->dedxHarmonic2D1;
      H2dedx2 = t->dedxHarmonic2D2;
   
    // mtd info
      beta1_PV = t->beta1_PV;     // mtd
      beta1_PVerr = t->beta1_PVerr;  // mtd
      beta2_PV = t->beta2_PV;
      beta2_PVerr = t->beta2_PVerr;
      tmtd1 = t->tmtd1;
      tmtd2 = t->tmtd2;
      sigmatmtd1 = t->sigmatmtd1;
      sigmatmtd2 = t->sigmatmtd2;
      pathLength1 = t->pathLength1;
      pathLength2 = t->pathLength2;
      isMtdDau1 = t->isMtdDau1;
      isMtdDau2 = t->isMtdDau2;
      isGoodMtdDau1 = t->isGoodMtdDau1;
      isGoodMtdDau2 = t->isGoodMtdDau2;

      t0_PV = t->t0_PV;
      sigmat0_PV = t->sigmat0_PV;

      pt_gen = t->pT_gen;
      eta_gen = t->eta_gen;
      status_gen = t->status_gen;
      idmom = t->MotherID_gen;
      y_gen = t->y_gen;
      iddau1 = t->DauID1_gen;
      iddau2 = t->DauID2_gen;
      iddau3 = t->DauID3_gen;

      tree->Fill();
   }
   std::cout << "counts of sigmatmtd >= 0 but fabs(eta)>3: " << isMtdWrong << std::endl;
   std::cout << "number of D0 passing selection: " << nD0 << std::endl;
   TFile* fout;
   fout = new TFile("matchPromptD0Tree_fullSample_reRECO.root", "recreate");
   tree->Write();
   hInvBetaVsPDau1->Write();
   hInvBetaVsPDau2->Write();
   hdInvBetaPionVsPDau1->Write();
   hdInvBetaPionVsPDau2->Write();
   hdInvBetaKaonVsPDau1->Write();
   hdInvBetaKaonVsPDau2->Write();
}
