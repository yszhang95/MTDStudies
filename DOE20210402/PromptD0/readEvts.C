void readEvts()
{
   TFile* f[2048];
   //ifstream fin("hyjets_sample.list");
   //ifstream fin("newHyJets.list");
   ifstream fin("HYDJETS_ALL.list");
   //ifstream fin("hyjets_test.list");
   //ifstream fin("newHyJets_2500.list");
   //ifstream fin("hyjets_testDec2020.list");
   //char name[512];
   std::string name;
   TH1F* hCent[2048];
   int nfile=0;
   while(fin>>name){
      std::cout << name << std::endl;
      f[nfile] = new TFile(name.c_str());
      //f[nfile]->GetObject("d0ana/hCent", hCent[nfile]);
      f[nfile]->GetObject("d0ana_mc/hCent", hCent[nfile]);
      nfile++;
   }
   std::cout << nfile << " files" << std::endl;
   int MB=0;
   int central=0;
   int n30_50 = 0;
   for(int ifile=0; ifile<nfile; ifile++){
      //std::cout << "MB: " << hCent[ifile]->Integral() << std::endl;
      //std::cout << "central: " << hCent[ifile]->Integral(1, 20) << std::endl;
      MB += hCent[ifile]->Integral();
      central += hCent[ifile]->Integral(0, 19);
      n30_50 += hCent[ifile]->Integral(60, 99);
   }
   std::cout << "MB: " << MB << std::endl;
   std::cout << "central: " << central << std::endl;
   std::cout << "30 - 50 " << n30_50 << std::endl;
}
