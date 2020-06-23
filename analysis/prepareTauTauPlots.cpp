//  prepareTauTauPlots.cpp
//  Created by Matthew Nickel on 21/05/2020.
// Based off of prepareBasicPlots.cpp created by Jeremi Niedziel

#include "Helpers.hpp"
#include "EventProcessor.hpp"
#include "PhysObjectProcessor.hpp"
#include "ConfigManager.hpp"
#include "EventDisplay.hpp"
#include "Logger.hpp"


string configPath = "configs/preparePlots_default.md";
string outputPath = "results/basicPlots_test.root";

bool saveCalosFailingNEE = true;
bool saveTriphotonHists = true;
bool checkTriggers = false;
bool saveTree = false;
bool saveHists = false;
bool saveCuts = true;
int nThreePhotonEvents = 0;


// Only those datasets will be analyzed
const vector<EDataset> datasetsToAnalyze = {
  kData,
};

vector<string> suffixes = {
  ""
};

vector<tuple<string, int, double, double>> histParams = {
  // title                   nBins min   max
  {"dilepton_acoplanarity"       , 200 , 0   , 1.0   },
  {"lepton_pt"          , 100 , 0   , 100.0 },
  {"lepton_eta"         , 200   ,-2.4 , 2.4   },
  {"lepton_phi"         , 200   ,-4.0 , 4.0   },
  {"dilepton_mass"      , 2000  , 0   , 200.0 },
  {"dilepton_rapidity"  , 600   ,-2.4 , 2.4   },
  {"dilepton_pt"        , 500  , 0   , 10.0  },
  
  {"nTracks"                , 100 , 0   , 100   },
  {"track_pt"               , 5000, 0   , 100   },
  {"track_eta"              , 100 ,-3.5 , 3.5   },
  {"track_phi"              , 100 ,-3.5 , 3.5   },
  {"track_missing_hits"     , 50  , 0   , 50    },
  {"track_valid_hits"       , 50  , 0   , 50    },
  {"track_purity"           , 10  , 0   , 10    },
  {"track_charge"           , 4   ,-2   , 2     },
  {"track_chi2"             , 1000, 0   , 100   },
  {"track_dxy"              ,300000,-15  , 15    },
  {"track_dz"               ,100000,-50  , 50    },
  {"track_dxy_over_sigma"   , 1000, 0   , 100   },
  {"track_dz_over_sigma"    , 1000, 0   , 100   },
  
  {"track_dxy_from_bs"      ,300000,-15  , 15    },
  {"track_dz_from_bs"       ,100000,-50  , 50    },
  
  {"track_dxy_1_track"      ,300000,-15  , 15    },
  {"track_dxy_2_track"      ,300000,-15  , 15    },
  {"track_dxy_3_track"      ,300000,-15  , 15    },
  {"track_dxy_ge4_track"    ,300000,-15  , 15    },
  
  {"track_dz_1_track"       ,100000,-50  , 50    },
  {"track_dz_2_track"       ,100000,-50  , 50    },
  {"track_dz_3_track"       ,100000,-50  , 50    },
  {"track_dz_ge4_track"     ,100000,-50  , 50    },

  {"track_vx"               ,300000,-15  , 15    },
  {"track_vy"               ,300000,-15  , 15    },
  {"track_vz"               ,300000,-50  , 50    },
  {"Cuts"               ,10,0  , 10    },
  {"Delta_Phi"               ,1000,0  , 4    },
  
  
};

float dilepton_acoplanarity_BV;
vector<float> lepton_pt_BV;
vector<float> lepton_eta_BV;
vector<float> lepton_phi_BV;
float dilepton_mass_BV;
float dilepton_rapidity_BV;
float dilepton_pt_BV;
  
float nTracks_BV;
vector<float> track_pt_BV;
vector<float> track_eta_BV;
vector<float> track_phi_BV;
vector<float> track_missing_hits_BV;
vector<float> track_valid_hits_BV;
vector<float> track_purity_BV;
vector<float> track_charge_BV;
vector<float> track_chi2_BV;
vector<float> track_dxy_BV;
vector<float> track_dz_BV;
vector<float> track_dxy_over_sigma_BV;
vector<float> track_dz_over_sigma_BV;
  
vector<float> track_dxy_from_bs_BV;
vector<float> track_dz_from_bs_BV;
  
vector<float> track_dxy_1_track_BV;
vector<float> track_dxy_2_track_BV;
vector<float> track_dxy_3_track_BV;
vector<float> track_dxy_ge4_track_BV;
  
vector<float> track_dz_1_track_BV;
vector<float> track_dz_2_track_BV;
vector<float> track_dz_3_track_BV;
vector<float> track_dz_ge4_track_BV;

vector<float> track_vx_BV;
vector<float> track_vy_BV;
vector<float> track_vz_BV;

void ResetBranchVariables(){
	dilepton_acoplanarity_BV = -999999;
	lepton_pt_BV.clear();
	lepton_eta_BV.clear();
	lepton_phi_BV.clear();
	dilepton_mass_BV = -999999;
	dilepton_rapidity_BV = -999999;
	dilepton_pt_BV = -999999;
	  
	nTracks_BV = -999999;
	track_pt_BV.clear();
	track_eta_BV.clear();
	track_phi_BV.clear();
	track_missing_hits_BV.clear();
	track_valid_hits_BV.clear();
	track_purity_BV.clear();
	track_charge_BV.clear();
	track_chi2_BV.clear();
	track_dxy_BV.clear();
	track_dz_BV.clear();
	track_dxy_over_sigma_BV.clear();
	track_dz_over_sigma_BV.clear();
	  
	track_dxy_from_bs_BV.clear();
	track_dz_from_bs_BV.clear();
	  
	track_dxy_1_track_BV.clear();
	track_dxy_2_track_BV.clear();
	track_dxy_3_track_BV.clear();
	track_dxy_ge4_track_BV.clear();
	  
	track_dz_1_track_BV.clear();
	track_dz_2_track_BV.clear();
	track_dz_3_track_BV.clear();
	track_dz_ge4_track_BV.clear();

	track_vx_BV.clear();
	track_vy_BV.clear();
	track_vz_BV.clear();	
}

void InitializeBranches(TTree *Tree){
	
	Tree->Branch("dilepton_acoplanarity" , &dilepton_acoplanarity_BV);
	Tree->Branch("lepton_pt" , &lepton_pt_BV);
	Tree->Branch("lepton_eta" , &lepton_eta_BV);
	Tree->Branch("lepton_phi" , &lepton_phi_BV);
	Tree->Branch("dilepton_mass" , &dilepton_mass_BV);
	Tree->Branch("dilepton_rapidity" , &dilepton_rapidity_BV);
	Tree->Branch("dilepton_pt" , &dilepton_pt_BV);
	Tree->Branch("nTracks" , &nTracks_BV);
	Tree->Branch("track_pt" , &track_pt_BV);
	Tree->Branch("track_eta" , &track_eta_BV);
	Tree->Branch("track_phi" , &track_phi_BV);
	Tree->Branch("track_missing_hits" , &track_missing_hits_BV);
	Tree->Branch("track_valid_hits" , &track_valid_hits_BV);
	Tree->Branch("track_purity" , &track_purity_BV);
	Tree->Branch("track_charge" , &track_charge_BV);
	Tree->Branch("track_chi2" , &track_chi2_BV);
	Tree->Branch("track_dxy" , &track_dxy_BV);
	Tree->Branch("track_dz" , &track_dz_BV);
	Tree->Branch("track_dxy_over_sigma" , &track_dxy_over_sigma_BV);
	Tree->Branch("track_dz_over_sigma" , &track_dz_over_sigma_BV);
	Tree->Branch("track_dxy_from_bs" , &track_dxy_from_bs_BV);
	Tree->Branch("track_dz_from_bs" , &track_dz_from_bs_BV);
	Tree->Branch("track_dxy_1_track" , &track_dxy_1_track_BV);
	Tree->Branch("track_dxy_2_track" , &track_dxy_2_track_BV);
	Tree->Branch("track_dxy_3_track" , &track_dxy_3_track_BV);
	Tree->Branch("track_dxy_ge4_track" , &track_dxy_ge4_track_BV);
	Tree->Branch("track_dz_1_track" , &track_dz_1_track_BV);
	Tree->Branch("track_dz_2_track" , &track_dz_2_track_BV);
	Tree->Branch("track_dz_3_track" , &track_dz_3_track_BV);
	Tree->Branch("track_dz_ge4_track" , &track_dz_ge4_track_BV);
	Tree->Branch("track_vx" , &track_vx_BV);
	Tree->Branch("track_vy" , &track_vy_BV);
	Tree->Branch("track_vz", &track_vz_BV);
	
	
}

void cutflow_hist(Event &event, const map<string, TH1D*> &hists,  EDataset dataset, string suffix="")
{
  string name = datasetName.at(dataset);
  int cutThrough=0;
  string lepton_type;
  int lepton_index;
  int matched_track_index = -1;
  int unmatched_track_index = -1;
  int nMuons = (int)event.GetPhysObjects(kMuon).size(); 
  int nTracks = (int)event.GetPhysObjects(kGeneralTrack).size();
  int nElectrons = (int)event.GetPhysObjects(kElectron).size();
  float Max_Muon_Pt = 0.0;
  float Max_Electron_Pt = 0.0;
  int Max_Muon_index = -1;
  int Max_Electron_index = -1;
  int nUnmatchedTracks = 0;

  for(int i = 0; i < nMuons; i++){
	if(event.GetPhysObjects(kMuon)[i]->GetPt() > Max_Muon_Pt){
	  Max_Muon_Pt = event.GetPhysObjects(kMuon)[i]->GetPt();
	  Max_Muon_index = i;
	}
  }

  
  for(int i = 0; i < nElectrons; i++){
	if(event.GetPhysObjects(kElectron)[i]->GetPt() > Max_Electron_Pt){
	  Max_Electron_Pt = event.GetPhysObjects(kElectron)[i]->GetPt();
	  Max_Electron_index = i;
	}
  }
  
  if(Max_Electron_Pt > Max_Muon_Pt){
	lepton_index = Max_Electron_index;
	lepton_type = "electron";
  }
  
	
  if(Max_Electron_Pt < Max_Muon_Pt){
	lepton_index = Max_Muon_index;
	lepton_type = "muon";
  }

  hists.at("Cuts_"+suffix+name)->Fill(cutThrough); // 0
  if(lepton_type == "muon"){
	auto lepton = event.GetPhysObjects(kMuon)[lepton_index];
	for(int i = 0; i < nTracks; i++){
	  auto track = event.GetPhysObjects(kGeneralTrack)[i];
	  float diff_phi = fabs(lepton->GetPhi() - track->GetPhi());
 	  float diff_eta = fabs(lepton->GetEta() - track->GetEta());
      if( diff_eta < .01 and diff_phi < .01){
		matched_track_index = i;
	  }
      if(!( diff_eta < .01 and diff_phi < .01)){
		unmatched_track_index = i;
		nUnmatchedTracks ++;
	  }
	}
	cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);  // 1
	if((unmatched_track_index != -1 and matched_track_index != -1
	and nUnmatchedTracks == 1)){
		
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);  // 2
    auto unmatch_track = event.GetPhysObjects(kGeneralTrack)[unmatched_track_index];
    auto match_track = event.GetPhysObjects(kGeneralTrack)[matched_track_index];
    if(!(lepton->GetPt() > 4.5 and fabs(lepton->GetEta())<2.5)){
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);  // 3
    if((unmatch_track->GetPt() > .5 and fabs(unmatch_track->GetEta())<2.5 
	and match_track->GetPt() > .5 and fabs(match_track->GetEta())<2.5 )){
		
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);  // 4
	float abs_delta_phi = fabs(lepton->GetPhi() - unmatch_track->GetPhi()); 
	hists.at("Delta_Phi_"+suffix+name)->Fill(abs_delta_phi);
	if((abs_delta_phi <3)){
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);   // 5
    //Mass selection cut 
    if((true)){
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);   // 6
	if((lepton->GetPt() > 6)){
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);   // 7
	}
	}
	}
	}
	}
	}	
  }
 
	
 
  if(lepton_type == "electron"){
	auto lepton = event.GetPhysObjects(kElectron)[lepton_index];
	for(int i = 0; i < nTracks; i++){
	  auto track = event.GetPhysObjects(kGeneralTrack)[i];
	  float diff_phi = fabs(lepton->GetPhi() - track->GetPhi());
 	  float diff_eta = fabs(lepton->GetEta() - track->GetEta());
      if( diff_eta < .01 and diff_phi < .01){
		matched_track_index = i;
	  }
      if(!( diff_eta < .01 and diff_phi < .01)){
		unmatched_track_index = i;
		nUnmatchedTracks ++;
	  }
	}
	cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough); // 1
	if((unmatched_track_index != -1 and matched_track_index != -1
	and nUnmatchedTracks == 1)){
		
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough); // 2
    auto unmatch_track = event.GetPhysObjects(kGeneralTrack)[unmatched_track_index];
    auto match_track = event.GetPhysObjects(kGeneralTrack)[matched_track_index];
    if((lepton->GetPt() > 3 and fabs(lepton->GetEta())<2.4)){
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);  // 3
    if((unmatch_track->GetPt() > .5 and fabs(unmatch_track->GetEta())<2.5 
	and match_track->GetPt() > .5 and fabs(match_track->GetEta())<2.5 )){
		
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);  // 4
	float abs_delta_phi = fabs(lepton->GetPhi() - unmatch_track->GetPhi()); 
	hists.at("Delta_Phi_"+suffix+name)->Fill(abs_delta_phi);
	if((abs_delta_phi <3)){
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);  // 5
    //Mass selection cut 
    if((true)){
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);  // 6
	if((lepton->GetPt() > 6)){
    cutThrough++;
    hists.at("Cuts_"+suffix+name)->Fill(cutThrough);  // 7
	}
	}
	}
	}
	}
	}  
  }
	
  
}

bool endsWith(const std::string &mainStr, const std::string &toMatch)
{
   if(mainStr.size() >= toMatch.size() &&
   mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0)
   return true;
   else
   return false;
}

void fillMuonHists(Event &event, const map<string, TH1D*> &hists, string datasetName, string suffix="")
{
  for(auto muon : event.GetPhysObjects(kMuon)){
    hists.at("lepton_pt_" +suffix+datasetName)->Fill(muon->GetPt());
    hists.at("lepton_eta_"+suffix+datasetName)->Fill(muon->GetEta());
    hists.at("lepton_phi_"+suffix+datasetName)->Fill(muon->GetPhi());
  }
}

void fillDimuonHists(Event &event, const map<string, TH1D*> &hists, string datasetName, string suffix="")
{
  
  TLorentzVector dimuon = physObjectProcessor.GetDimuon(*event.GetPhysObjects(kMuon)[0],
                                                                *event.GetPhysObjects(kMuon)[1]);
  
  hists.at("dilepton_mass_"     +suffix+datasetName)->Fill(dimuon.M());
  hists.at("dilepton_rapidity_" +suffix+datasetName)->Fill(dimuon.Rapidity());
  hists.at("dilepton_pt_"       +suffix+datasetName)->Fill(dimuon.Pt());
  
}

void fillTracksHists(Event &event, const map<string, TH1D*> &hists, EDataset dataset, string suffix="")
{
  string name = datasetName.at(dataset);
  
  int nTracks = (int)event.GetPhysObjects(kGeneralTrack).size();
  
  hists.at("nTracks_"+suffix+name)->Fill(nTracks);
  
  
  for(auto track : event.GetPhysObjects(kGeneralTrack)){
    double trackPt = track->GetPt();
    hists.at("track_pt_"+suffix+name)->Fill(trackPt);
    hists.at("track_eta_"+suffix+name)->Fill(track->GetEta());
    hists.at("track_phi_"+suffix+name)->Fill(track->GetPhi());
    
    hists.at("track_missing_hits_"  + suffix + name)->Fill(track->GetNmissingHits());
    hists.at("track_valid_hits_"    + suffix + name)->Fill(track->GetNvalidHits());
    hists.at("track_purity_"        + suffix + name)->Fill(track->GetPurity());
    hists.at("track_charge_"        + suffix + name)->Fill(track->GetCharge());
    hists.at("track_chi2_"          + suffix + name)->Fill(track->GetChi2());
    hists.at("track_dxy_"           + suffix + name)->Fill(track->GetDxy());
    
    hists.at("track_dxy_from_bs_"   + suffix + name)->Fill(track->GetXYdistanceFromBeamSpot(dataset));
    hists.at("track_dz_from_bs_"    + suffix + name)->Fill(track->GetZdistanceFromBeamSpot(dataset));
    
    if(nTracks==1){
      hists.at("track_dxy_1_track_" + suffix + name)->Fill(track->GetDxy());
      hists.at("track_dz_1_track_"  + suffix + name)->Fill(track->GetDz());
    }
    if(nTracks==2){
      hists.at("track_dxy_2_track_" + suffix + name)->Fill(track->GetDxy());
      hists.at("track_dz_2_track_"  + suffix + name)->Fill(track->GetDz());
    }
    if(nTracks==3){
      hists.at("track_dxy_3_track_" + suffix + name)->Fill(track->GetDxy());
      hists.at("track_dz_3_track_"  + suffix + name)->Fill(track->GetDz());
    }
    if(nTracks>=4){
      hists.at("track_dxy_ge4_track_" + suffix + name)->Fill(track->GetDxy());
      hists.at("track_dz_ge4_track_"  + suffix + name)->Fill(track->GetDz());
    }
    
    hists.at("track_dz_"            + suffix + name)->Fill(track->GetDz());
    hists.at("track_dxy_over_sigma_"+ suffix + name)->Fill(fabs(track->GetDxy()/track->GetDxyErr()));
    hists.at("track_dz_over_sigma_" + suffix + name)->Fill(fabs(track->GetDz()/track->GetDzErr()));
    hists.at("track_vx_"            + suffix + name)->Fill(track->GetVertexX());
    hists.at("track_vy_"            + suffix + name)->Fill(track->GetVertexY());
    hists.at("track_vz_"            + suffix + name)->Fill(track->GetVertexZ());
  }
  
}

void fillTauTauHistograms(Event &event, const map<string, TH1D*> &hists, EDataset dataset, vector<string> suffix_list)
{
  string name = datasetName.at(dataset);
  
  double aco = physObjectProcessor.GetAcoplanarity(*event.GetPhysObjects(kMuon)[0],
                                                   *event.GetPhysObjects(kMuon)[1]);
  for(string suffix : suffix_list){
  if(suffix != "") suffix += "_";  
	

  
  hists.at("dilepton_acoplanarity_"+suffix+name)->Fill(aco);
  fillMuonHists(  event, hists, name, suffix);
  fillDimuonHists(event, hists, name, suffix);
  fillTracksHists(    event, hists, dataset, suffix);
  }
}

void fillMuonBranchVariables(Event &event)
{
  for(auto muon : event.GetPhysObjects(kMuon)){
    lepton_pt_BV.push_back(muon->GetPt());
    lepton_eta_BV.push_back(muon->GetEta());
    lepton_phi_BV.push_back(muon->GetPhi());
  }
}

void fillDimuonBranchVariables(Event &event)
{
  
  TLorentzVector dimuon = physObjectProcessor.GetDimuon(*event.GetPhysObjects(kMuon)[0],
                                                                *event.GetPhysObjects(kMuon)[1]);
  
  dilepton_mass_BV = (dimuon.M());
  dilepton_rapidity_BV = (dimuon.Rapidity());
  dilepton_pt_BV = (dimuon.Pt());
  
}


void fillTracksBranchVariables(Event &event, EDataset dataset)
{
  
  int nTracks = (int)event.GetPhysObjects(kGeneralTrack).size();
  
  nTracks_BV = nTracks;
  
  
  for(auto track : event.GetPhysObjects(kGeneralTrack)){
    double trackPt = track->GetPt();
    track_pt_BV.push_back(trackPt);
    track_eta_BV.push_back(track->GetEta());
    track_phi_BV.push_back(track->GetPhi());
    
    track_missing_hits_BV.push_back(track->GetNmissingHits());
    track_valid_hits_BV.push_back(track->GetNvalidHits());
    track_purity_BV.push_back(track->GetPurity());
    track_charge_BV.push_back(track->GetCharge());
    track_chi2_BV.push_back(track->GetChi2());
    track_dxy_BV.push_back(track->GetDxy());
    
    track_dxy_from_bs_BV.push_back(track->GetXYdistanceFromBeamSpot(dataset));
    track_dz_from_bs_BV.push_back(track->GetZdistanceFromBeamSpot(dataset));
    
    if(nTracks==1){
      track_dxy_1_track_BV.push_back(track->GetDxy());
      track_dz_1_track_BV.push_back(track->GetDz());
    }
    if(nTracks==2){
      track_dxy_2_track_BV.push_back(track->GetDxy());
      track_dz_2_track_BV.push_back(track->GetDz());
    }
    if(nTracks==3){
      track_dxy_3_track_BV.push_back(track->GetDxy());
      track_dz_3_track_BV.push_back(track->GetDz());
    }
    if(nTracks>=4){
      track_dxy_ge4_track_BV.push_back(track->GetDxy());
      track_dz_ge4_track_BV.push_back(track->GetDz());
    }
    
    track_dz_BV.push_back(track->GetDz());
    track_dxy_over_sigma_BV.push_back(fabs(track->GetDxy()/track->GetDxyErr()));
    track_dz_over_sigma_BV.push_back(fabs(track->GetDz()/track->GetDzErr()));
    track_vx_BV.push_back(track->GetVertexX());
    track_vy_BV.push_back(track->GetVertexY());
    track_vz_BV.push_back(track->GetVertexZ());
  }
  
}

void fillTauTauBranchVariables(Event &event, EDataset dataset)
{
  
  double aco = physObjectProcessor.GetAcoplanarity(*event.GetPhysObjects(kMuon)[0],
                                                   *event.GetPhysObjects(kMuon)[1]);
  dilepton_acoplanarity_BV = (aco);
  fillMuonBranchVariables(event);
  fillDimuonBranchVariables(event);
  fillTracksBranchVariables(event, dataset);
  
}

/// Creates histograms, cut through and event counters for given dataset name, for each
/// histogram specified in `histParams` vector.
void InitializeHistograms(map<string, TH1D*> &hists, string datasetType, string suffix="")
{
  if(suffix != "") suffix = "_" + suffix;
  
  for(auto &[histName, nBins, min, max] : histParams){
    string title = histName + suffix + "_" + datasetType;

    if(hists.find(title) != hists.end()) continue;
    hists[title] = new TH1D(title.c_str(), title.c_str(), nBins, min, max);
  }
}

int main(int argc, char* argv[])
{
  if(argc != 1 && argc != 5){
    cout<<"This app requires 0 or 4 parameters.\n";
    cout<<"./prepareBasicPlots configPath inputPath outputPath datasetName[Data|QED_SC|QED_SL|LbL|CEP]\n";
    exit(0);
  }

  string inputPath = "";
  string sampleName = "";
 
  if(argc == 5){
    configPath = argv[1];
    inputPath  = argv[2];
    outputPath = argv[3];
    sampleName = argv[4];
  }

  config = ConfigManager(configPath);

  map<string, TH1D*> hists;
  TFile *outFile = new TFile(outputPath.c_str(), "recreate");
  TTree *outTree = new TTree("outTree","Tree with Histogram Variables");
  InitializeBranches(outTree);
  
  if(inputPath==""){
    for(auto dataset : datasetsToAnalyze){
      for(string suffix : suffixes){
        InitializeHistograms(hists, datasetName.at(dataset), suffix);
      }
    }
    

    for(auto dataset : datasetsToAnalyze){
      string name = datasetName.at(dataset);
      
      Log(0)<<"Creating "<<name<<" plots\n";


      auto events = make_unique<EventProcessor>(inFileNames.at(dataset), dataset);

      for(int iEvent=0; iEvent<events->GetNevents(); iEvent++){
		ResetBranchVariables();
        if(iEvent%1000 == 0)  Log(1)<<"Processing event "<<iEvent<<"\n";
        if(iEvent%10000 == 0) Log(0)<<"Processing event "<<iEvent<<"\n";
        if(iEvent >= config.params("maxEvents")) break;
        
        auto event = events->GetEvent(iEvent);


        
 
        fillTauTauHistograms(*event, hists, dataset, suffixes);
      }
      
      outFile->cd();
      for(auto &[histName, hist] : hists){
        if(histName.find(name) != string::npos) hist->Write();
      }
    }
  }

  else{
    if (endsWith(inputPath, "root")){
		cout << "root file" << endl;
		EDataset dataset = nDatasets;
		   
		if(sampleName == "Data")    dataset = kData;
		if(sampleName == "QED_SC")  dataset = kMCqedSC;
		if(sampleName == "QED_SL")  dataset = kMCqedSL;
		if(sampleName == "LbL")     dataset = kMClbl;
		if(sampleName == "CEP")     dataset = kMCcep;
		   
		auto events = make_unique<EventProcessor>(inputPath, dataset);

		for(string suffix : suffixes){
		  InitializeHistograms(hists, sampleName, suffix);
		}
		

		if(dataset == nDatasets){
		  Log(0)<<"ERROR -- unknown dataset name provided: "<<sampleName<<"\n";
		  exit(0);
		}
		
		for(int iEvent=0; iEvent<events->GetNevents(); iEvent++){
		  if(iEvent%1000 == 0)  Log(1)<<"Processing event "<<iEvent<<"\n";
		  if(iEvent%10000 == 0) Log(0)<<"Processing event "<<iEvent<<"\n";
		  if(iEvent >= config.params("maxEvents")) break;
		  auto event = events->GetEvent(iEvent);
		  if(saveHists){
			fillTauTauHistograms(*event, hists, dataset, suffixes);
		  }
		  if(saveCuts){
			cutflow_hist(*event, hists, dataset, "");
		  }
		  if(saveTree){
			ResetBranchVariables();
			fillTauTauBranchVariables(*event, dataset);
			outTree->Fill();
		  }
		}
	}
    if (endsWith(inputPath, "txt")){
		cout << "txt file" << endl;
		EDataset dataset = nDatasets;
		   
		if(sampleName == "Data")    dataset = kData;
		if(sampleName == "QED_SC")  dataset = kMCqedSC;
		if(sampleName == "QED_SL")  dataset = kMCqedSL;
		if(sampleName == "LbL")     dataset = kMClbl;
		if(sampleName == "CEP")     dataset = kMCcep;
		ifstream file(inputPath);
		string inputFile;
		for(string suffix : suffixes){
		  InitializeHistograms(hists, sampleName, suffix);
		}
		while(getline(file, inputFile)){
			auto events = make_unique<EventProcessor>(inputFile, dataset);

			

			if(dataset == nDatasets){
			  Log(0)<<"ERROR -- unknown dataset name provided: "<<sampleName<<"\n";
			  exit(0);
			}
			
			for(int iEvent=0; iEvent<events->GetNevents(); iEvent++){
			  if(iEvent%1000 == 0)  Log(1)<<"Processing event "<<iEvent<<"\n";
			  if(iEvent%10000 == 0) Log(0)<<"Processing event "<<iEvent<<"\n";
			  if(iEvent >= config.params("maxEvents")) break;

			  auto event = events->GetEvent(iEvent);
			  
			  // run this here just to save electron cut flow hist
			  if(saveHists){
			    fillTauTauHistograms(*event, hists, dataset, suffixes);
			  }
			  if(saveCuts){
			    cutflow_hist(*event, hists, dataset, "");
			  }
			  if(saveTree){
				ResetBranchVariables();
				fillTauTauBranchVariables(*event, dataset);
				outTree->Fill();
			  }
			}
		}
	}
	if (!(endsWith(inputPath, "txt")) && !(endsWith(inputPath, "root"))){
		cout << "not valid input file" << endl;
		exit(0);
		}
	
    cout << "Writing Histograms" <<endl;
    outFile->cd();
    for(auto &[histName, hist] : hists) hist->Write();
	if(saveTree) outTree->Write();
  }

  outFile->Close();
  cout << "Finished" << endl;
}

