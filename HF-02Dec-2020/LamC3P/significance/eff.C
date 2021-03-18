void eff()
{
   TFile* f1 = new TFile("genPt_reRECO.root");
   TH1D* hGenPt = (TH1D*) f1->Get("hGenPtMidY");
   TFile* f2 = new TFile("lamC3PMassHists_reRECO_all.root");
   TH3D* hMassVsPtVsY = (TH3D*) f2->Get("hMassVsPtVsY");
   TH1D* hProj = hMassVsPtVsY->ProjectionY("pt", 1, 1, 1, 60);
   hProj->Divide(hGenPt);
   hProj->Draw();
   hGenPt->Draw();
}
