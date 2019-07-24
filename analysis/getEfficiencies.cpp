//  getDiphotonEfficiency
//
//  Created by Jeremi Niedziela on 22/07/2019.
//
// Calculates diphoton efficiency, defined as:
// Eff^{\gamma\gamma} = N^{reco}/N^{gen}
//
// where:
//
// N^{reco}: number of reconstructed diphoton events with E_T^{reco} > 2 GeV, |\eta^{reco}|<2.4,
// passing trigger, identification and exclusivity cuts
//
// N^{gen}: number of generated events with E_T^{gen} > 2 GeV, |\eta^{gen}|<2.4

#include "Helpers.hpp"
#include "EventProcessor.hpp"
#include "ConfigManager.hpp"

string configPath = "configs/efficiencies.md";

bool HasTwoMatchingPhotons(const vector<shared_ptr<PhysObject>> &genPhotons,
                           const vector<shared_ptr<PhysObject>> &photonSCs)
{
  int nMatched=0;
  
  for(auto &genPhoton : genPhotons){
    for(auto &cluster : photonSCs){
      double deltaR = sqrt(pow(genPhoton->GetEta() - cluster->GetEta(), 2)+
                           pow(genPhoton->GetPhi() - cluster->GetPhi(), 2));
      if(deltaR < config.params["maxDeltaR"]) nMatched++;
    }
  }
  
  return nMatched >= 2;
}

void PrintEfficiency(double num, double den)
{
  cout<<(double)num/den<<"\t#pm "<<sqrt(1./num+1./den)*(double)num/den<<endl;
}

int main()
{
  config = ConfigManager(configPath);
  unique_ptr<EventProcessor> eventProcessor(new EventProcessor(kMClbl));
  
  int nGenEvents                               = 0;
  int nEventsPassingAll                        = 0;
  int nEventsIDmatched                         = 0;
  int nEventsPassingID                         = 0;
  int nEventsPassingIDandTrigger               = 0;
  int nEventsPassingIDandTriggerAndChargedExcl = 0;
  int nEventsPassingIDandTriggerAndBothExcl    = 0;
  
  int iEvent;
  
  float singleEtBins[] = { 0, 2, 3, 4, 5, 6, 8, 12, 16, 20 };
  TH1D *recoEffNum = new TH1D("reco_id_eff_num", "reco_id_eff_num", 9, singleEtBins);
  TH1D *recoEffDen = new TH1D("reco_id_eff_den", "reco_id_eff_den", 9, singleEtBins);
  recoEffNum->Sumw2();
  recoEffDen->Sumw2();
  
  float mInvBins[] = { 2, 3, 4, 5, 6, 8, 10, 13, 16, 20 };
  TH1D *triggerEffNum = new TH1D("trigger_eff_num", "trigger_eff_num", 9, mInvBins);
  TH1D *triggerEffDen = new TH1D("trigger_eff_den", "trigger_eff_den", 9, mInvBins);
  triggerEffNum->Sumw2();
  triggerEffDen->Sumw2();
  
  TH1D *triggerSingleEffNum = new TH1D("trigger_single_eff_num", "trigger_single_eff_num", 9, mInvBins);
  TH1D *triggerDoubleEffNum = new TH1D("trigger_double_eff_num", "trigger_double_eff_num", 9, mInvBins);
  triggerSingleEffNum->Sumw2();
  triggerDoubleEffNum->Sumw2();
  
  TH1D *chargedExclEffNum = new TH1D("charged_excl_eff_num", "charged_excl_eff_num", 9, mInvBins);
  TH1D *chargedExclEffDen = new TH1D("charged_excl_eff_den", "charged_excl_eff_den", 9, mInvBins);
  chargedExclEffNum->Sumw2();
  chargedExclEffDen->Sumw2();
  
  TH1D *neutralExclEffNum = new TH1D("neutral_excl_eff_num", "neutral_excl_eff_num", 9, mInvBins);
  TH1D *neutralExclEffDen = new TH1D("neutral_excl_eff_den", "neutral_excl_eff_den", 9, mInvBins);
  neutralExclEffNum->Sumw2();
  neutralExclEffDen->Sumw2();
  
  for(iEvent=0; iEvent<eventProcessor->GetNevents(); iEvent++){
    if(iEvent%10000 == 0) cout<<"Processing event "<<iEvent<<endl;
    if(iEvent >= config.params["maxEvents"]) break;
    
    auto event = eventProcessor->GetEvent(iEvent);
    auto goodGenPhotons  = event->GetGoodGenPhotons();
    auto photonSCpassing = event->GetGoodPhotonSCs();
    
    double diphotonMinv = event->GetDiphotonInvMass();
    
    // Check properties of this event
    bool hasTwoGoodGenPhotons     = goodGenPhotons.size() == 2;
    bool hasLbLTrigger            = event->HasLbLTrigger();
    bool hasTwoPhotonsPassingID   = photonSCpassing.size() == 2;
    bool passesNeutralExclusivity = ! event->HasAdditionalTowers();
    bool passesChargedExclusivity = ! event->HasChargedTracks();
    bool passesDiphotonCuts       = ! event->DiphotonPtAboveThreshold();
    bool hasTwoMachingPhotons     = HasTwoMatchingPhotons(goodGenPhotons, event->GetPhotonSCs());
    
    // Increment counters of different types of events
    if(hasTwoGoodGenPhotons){
      nGenEvents++;
      recoEffDen->Fill(goodGenPhotons.front()->GetEt());
    }
    
    if(hasLbLTrigger &&
       hasTwoPhotonsPassingID &&
       passesNeutralExclusivity &&
       passesChargedExclusivity &&
       passesDiphotonCuts){
      nEventsPassingAll++;
    }

    if(hasLbLTrigger &&
       hasTwoPhotonsPassingID &&
       passesChargedExclusivity){
      nEventsPassingIDandTriggerAndChargedExcl++;
      chargedExclEffNum->Fill(diphotonMinv);
      neutralExclEffDen->Fill(diphotonMinv);
    }
    
    if(hasLbLTrigger &&
       hasTwoPhotonsPassingID &&
       passesChargedExclusivity &&
       passesNeutralExclusivity){
      nEventsPassingIDandTriggerAndBothExcl++;
      neutralExclEffNum->Fill(diphotonMinv);
    }
    
    if(hasTwoPhotonsPassingID){
      nEventsPassingID++;
      triggerEffDen->Fill(diphotonMinv);
    }
    
    if(hasTwoPhotonsPassingID &&
       hasLbLTrigger){
      nEventsPassingIDandTrigger++;
      triggerEffNum->Fill(diphotonMinv);
      chargedExclEffDen->Fill(diphotonMinv);
    }
    
    if(hasTwoPhotonsPassingID &&
       event->HasSingleEG3Trigger()){
      triggerSingleEffNum->Fill(diphotonMinv);
    }
    
    if(hasTwoPhotonsPassingID &&
       event->HasDoubleEG2Trigger()){
      triggerDoubleEffNum->Fill(diphotonMinv);
    }
    
    if(hasTwoMachingPhotons){
      nEventsIDmatched++;
      recoEffNum->Fill(goodGenPhotons.front()->GetEt());
    }
  }
  
  cout<<"\n\n------------------------------------------------------------------------"<<endl;
  cout<<"N event analyzed: "<<iEvent<<endl;
  cout<<"N gen events within limits: "<<nGenEvents<<endl;
  cout<<"N events passing all selections: "<<nEventsPassingAll<<endl;
  cout<<"N events with matched photons passing selection: "<<nEventsIDmatched<<endl;
  cout<<"N events passing ID: "<<nEventsPassingID<<endl;
  cout<<"N events passing ID and trigger: "<<nEventsPassingIDandTrigger<<endl;
  cout<<"N events passing ID, trigger and charged excl: "<<nEventsPassingIDandTriggerAndChargedExcl<<endl;
  cout<<"N events passing ID, trigger and both excl: "<<nEventsPassingIDandTriggerAndBothExcl<<endl;
  
  cout<<"Diphoton efficiency: "; PrintEfficiency(nEventsPassingAll, nGenEvents);
  cout<<"Reco+ID efficiency: "; PrintEfficiency(nEventsIDmatched, nGenEvents);
  cout<<"Trigger efficiency: "; PrintEfficiency(nEventsPassingIDandTrigger, nEventsPassingID);
  cout<<"Cherged exclusivity efficiency: "; PrintEfficiency(nEventsPassingIDandTriggerAndChargedExcl, nEventsPassingIDandTrigger);
  cout<<"Neutral exclusivity efficiency: "; PrintEfficiency(nEventsPassingIDandTriggerAndBothExcl, nEventsPassingIDandTriggerAndChargedExcl);
  
  cout<<"------------------------------------------------------------------------\n\n"<<endl;
  
  TH1D *recoEff = new TH1D(*recoEffNum);
  recoEff->SetTitle("reco_id_eff");
  recoEff->SetName("reco_id_eff");
  recoEff->Divide(recoEffNum, recoEffDen, 1, 1, "B");
  
  TH1D *triggerEff = new TH1D(*triggerEffNum);
  triggerEff->SetTitle("trigger_eff");
  triggerEff->SetName("trigger_eff");
  triggerEff->Divide(triggerEffNum, triggerEffDen, 1, 1, "B");
  
  TH1D *triggerSingleEff = new TH1D(*triggerSingleEffNum);
  triggerSingleEff->SetTitle("trigger_single_eff");
  triggerSingleEff->SetName("trigger_single_eff");
  triggerSingleEff->Divide(triggerSingleEffNum, triggerEffDen, 1, 1, "B");
  
  TH1D *triggerDoubleEff = new TH1D(*triggerDoubleEffNum);
  triggerDoubleEff->SetTitle("trigger_double_eff");
  triggerDoubleEff->SetName("trigger_double_eff");
  triggerDoubleEff->Divide(triggerDoubleEffNum, triggerEffDen, 1, 1, "B");
  
  TH1D *chargedExclEff = new TH1D(*chargedExclEffNum);
  chargedExclEff->SetTitle("charged_excl_eff");
  chargedExclEff->SetName("charged_excl_eff");
  chargedExclEff->Divide(chargedExclEffNum, chargedExclEffDen, 1, 1, "B");
  
  TH1D *neutralExclEff = new TH1D(*neutralExclEffNum);
  neutralExclEff->SetTitle("neutral_excl_eff");
  neutralExclEff->SetName("neutral_excl_eff");
  neutralExclEff->Divide(neutralExclEffNum, neutralExclEffDen, 1, 1, "B");
  
  TFile *outFile = new TFile("results/efficiencies.root", "recreate");
  outFile->cd();
  recoEffNum->Write();
  recoEffDen->Write();
  recoEff->Write();
  triggerEffNum->Write();
  triggerEffDen->Write();
  triggerEff->Write();
  triggerSingleEffNum->Write();
  triggerSingleEff->Write();
  triggerDoubleEffNum->Write();
  triggerDoubleEff->Write();
  chargedExclEffNum->Write();
  chargedExclEffDen->Write();
  chargedExclEff->Write();
  neutralExclEffNum->Write();
  neutralExclEffDen->Write();
  neutralExclEff->Write();
  
  outFile->Close();
  
  return 0;
}

