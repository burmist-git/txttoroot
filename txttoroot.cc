//
#include <TH1D.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>

//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// waveform 0
// dt 4.000000E-10
// Npoints 5000
// Vbias 30.000
// Ibias -2.036386E-16
// Diode Current -1.108700E-14
// Diode Current Std 7.637303E-14
// 0.001739        0.001388        0.000685        -0.000018       -0.001073 
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//Int_t waveformID;
//Float_t dt;
//Int_t NPoints;
//Float_t Vbias;
//Float_t Ibias;
//Float_t IDiode;
//Float_t IDiodeStd;
//Float_t Volt[5000]; //[NPoints]
//////////////////////////////////////////////////////////////////////////////

void convtoroot( TString txtFile, TString rootFile, Int_t NP, Int_t Nwf);

int main(int argc, char *argv[]){
  if(argc == 4){
    TString txtFile = argv[1];
    TString rootFile = txtFile + ".root";
    Int_t Npoints = atoi(argv[2]);
    Int_t Nwf = atoi(argv[3]);
    cout<<"txtFile  : "<<txtFile<<endl
	<<"rootFile : "<<txtFile<<endl
	<<"Npoints  : "<<Npoints<<endl
	<<"Nwf      : "<<Npoints<<endl;
    convtoroot( txtFile, rootFile, Npoints, Nwf);
  }
  else{
    cout<<"  ERROR ---> in input arguments "<<endl
	<<"             [1] : name of a ASCI file containing waveforms"<<endl
	<<"             [2] : number of points in waveforms"<<endl
	<<"             [3] : number of waveforms"<<endl;
  }
  return 0;
}

void convtoroot(TString txtFile, TString rootFile, Int_t NP, Int_t Nwf){
  const Int_t nnPoints = 5000;
  //
  if(nnPoints != NP){
    cout<<" ---> ERROR : nnPoints != NP"<<endl
	<<"nnPoints = "<<nnPoints<<endl
      	<<"NP       = "<<NP<<endl;
    assert(0);
  }
  //
  Int_t waveformID;
  Float_t dt;
  Int_t NPoints;
  Float_t Vbias;
  Float_t Ibias;
  Float_t IDiode;
  Float_t IDiodeStd;
  Float_t Volt[nnPoints];
  //
  vector<TGraph*> v_gr;
  //
  ///////////////////Root file with data/////////////////
  TFile *hfile = new TFile( rootFile, "RECREATE", "USBWC Data", 1);
  if (hfile->IsZombie()) {
    cout<<" ---> ERROR : PROBLEM with the initialization of the output ROOT file : "<<endl 
        <<rootFile
        <<endl;
    assert(0);
  }
  TTree *tree = new TTree("T", "SiPM data");
  hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);  
  // Create new event
  TTree::SetBranchStyle(0);
  ///////////////////////////////////////////////////////
  //  
  //Event////////////////////////////////////////////////
  tree->Branch("waveformID",&waveformID, "waveformID/I");
  tree->Branch("dt",&dt, "dt/F");
  tree->Branch("NPoints",&NPoints, "NPoints/I");
  tree->Branch("Vbias",&Vbias, "Vbias/F");
  tree->Branch("Ibias",&Ibias, "Ibias/F");
  tree->Branch("IDiode",&IDiode, "IDiode/F");
  tree->Branch("IDiodeStd",&IDiodeStd, "IDiodeStd/F");
  tree->Branch("Volt", Volt, "Volt[NPoints]/F"); 
  ///////////////////////////////////////////////////////
  //
  string mot;
  Float_t Volt_val;
  //  
  ifstream indata;
  indata.open(txtFile.Data()); 
  assert(indata.is_open());
  cout<<" ---> Conversion of "<<txtFile<<endl;
  while(indata>>mot){
    if(mot == "waveform"){
      indata>>waveformID
	    >>mot>>dt
	    >>mot>>NPoints
	    >>mot>>Vbias
	    >>mot>>Ibias
	    >>mot>>mot>>IDiode
	    >>mot>>mot>>mot>>IDiodeStd;
      if(waveformID%100==0){
	cout<<"waveformID = "<<waveformID<<endl
	    <<"dt         = "<<dt<<endl
	    <<"NPoints    = "<<NPoints<<endl
	    <<"Vbias      = "<<Vbias<<endl
	    <<"Ibias      = "<<Ibias<<endl
	    <<"IDiode     = "<<IDiode<<endl
	    <<"IDiodeStd  = "<<IDiodeStd<<endl;
      }
      TGraph *gr = new TGraph();
      TString gr_nt = "gr_";
      gr_nt += waveformID;
      gr->SetNameTitle(gr_nt.Data(),gr_nt.Data());
      for(Int_t j = 0;j<NPoints;j++){
	indata>>Volt_val;
	Volt[j] = Volt_val;
	gr->SetPoint(j,j*dt*1.0e+9,Volt_val);
      }
      v_gr.push_back(gr);
      tree->Fill();
    }
  }
  indata.close();
  hfile = tree->GetCurrentFile();
  hfile->Write();
  hfile->Close();
  ///////////////
  TString nameF = rootFile;
  nameF += "_gr.root";
  TFile* gr_rootFile = new TFile(nameF.Data(), "RECREATE", " Histograms", 1);
  gr_rootFile->cd();
  if (gr_rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<nameF.Data()<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<nameF.Data()<<endl;
  for(unsigned int i = 0;i<v_gr.size();i++)
    v_gr.at(i)->Write();
  gr_rootFile->Close();
  ///////////////
}
