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
#include "TNtuple.h"

#include "myAna.h"

inline float invBetaPion(const float p){
    return std::sqrt(1 + std::pow(ana::massPion/p,2));
}

inline float invBetaKaon(const float p){
    return std::sqrt(1 + std::pow(ana::massKaon/p,2));
}

void fillMatchNtuple()
{
    TH1F::SetDefaultSumw2(true);

    TChain *chain = new TChain("d0ana_mc/VertexCompositeNtuple");
    TFileCollection* fc = new TFileCollection("dum", "", "newd0signal.list");
    chain->AddFileInfoList(fc->GetList()); 
    PromptD *t = new PromptD(chain);
    std::cout << "total entries: " << t->GetEntries() << std::endl;

    TH2F* hInvBetaVsPDau1 = new TH2F("hInvBetaVsPDau1", "hInvBetaVsPDau1", 1000, 0, 5, 1000, 0.9, 1.7);
    TH2F* hInvBetaVsPDau2 = new TH2F("hInvBetaVsPDau2", "hInvBetaVsPDau2", 1000, 0, 5, 1000, 0.9, 1.7);

    TH2F* hdInvBetaPionVsPDau1 = new TH2F("hdInvBetaPionVsPDau1", "hdInvBetaPionVsPDau1", 1000, 0, 5, 1000, 0.9, 1.7);
    TH2F* hdInvBetaPionVsPDau2 = new TH2F("hdInvBetaPionVsPDau2", "hdInvBetaPionVsPDau2", 1000, 0, 5, 1000, 0.9, 1.7);

    TH2F* hdInvBetaKaonVsPDau1 = new TH2F("hdInvBetaKaonVsPDau1", "hdInvBetaKaonVsPDau1", 1000, 0, 5, 1000, 0.9, 1.7);
    TH2F* hdInvBetaKaonVsPDau2 = new TH2F("hdInvBetaKaonVsPDau2", "hdInvBetaKaonVsPDau2", 1000, 0, 5, 1000, 0.9, 1.7);

    TNtuple *ntp = new TNtuple("PromptD", "PromptD", 
            "pT:y:mass:flavor:eta:isSwap:matchGEN:pTD1:EtaD1:pTD2:EtaD2:beta1_PV:beta2_PV:beta1_PVerr:beta2_PVerr:isMtdDau1:isMtdDau2:sigmatmtd1:sigmatmtd2:isGoodMtdDau1:isGoodMtdDau2");

    int isMtdWrong = 0;
    Long64_t nD0 = 0;
    for(Long64_t ientry=0; ientry<t->GetEntries(); ++ientry){
        t->GetEntry(ientry);
        //bool trkQuality = t->pTD1 > 0.77 && t->pTD2 > 0.77;
        //if(!trkQuality) continue;

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
        float var[256];
        int j=0;
        var[j++] = t->pT;
        var[j++] = t->y;
        var[j++] = t->mass;
        var[j++] = t->flavor;
        var[j++] = t->eta; 
        var[j++] = t->isSwap;
        var[j++] = t->matchGEN;
        var[j++] = t->pTD1;
        var[j++] = t->EtaD1;
        var[j++] = t->pTD2;
        var[j++] = t->EtaD2;
        var[j++] = t->beta1_PV;
        var[j++] = t->beta2_PV;
        var[j++] = t->beta1_PVerr;
        var[j++] = t->beta2_PVerr;
        var[j++] = t->isMtdDau1;
        var[j++] = t->isMtdDau2;
        var[j++] = t->sigmatmtd1;
        var[j++] = t->sigmatmtd2;
        var[j++] = t->isGoodMtdDau1;
        var[j++] = t->isGoodMtdDau2;

        ntp->Fill(var);

    }
    std::cout << "counts of sigmatmtd >= 0 but fabs(eta)>3: " << isMtdWrong << std::endl;
    std::cout << "number of D0 passing selection: " << nD0 << std::endl;
    TFile* fout;
    fout = new TFile("matchPromptD0_fullSample_reRECO.root", "recreate");
    ntp->Write();
    hInvBetaVsPDau1->Write();
    hInvBetaVsPDau2->Write();
    hdInvBetaPionVsPDau1->Write();
    hdInvBetaPionVsPDau2->Write();
    hdInvBetaKaonVsPDau1->Write();
    hdInvBetaKaonVsPDau2->Write();
}
