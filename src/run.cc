#include "run.hh"
#include "G4AnalysisManager.hh"

MyRunAction::MyRunAction()
{

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    
//    man->CreateH1("Photons", "Photons", 100, 0., 1.1 * MeV);
    man->CreateNtuple("Photons", "Photons");
    man->CreateNtupleIColumn("iEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fGlobalTime");
    man->CreateNtupleDColumn("fWlen");
    man->FinishNtuple(0);
    
    
//    man->CreateH1("Hitsphotons", "Hitsphotons", 100, 0., 1. * MeV);
    man->CreateNtuple("Hitsphotons","Hitsphotons");
    man->CreateNtupleIColumn("iEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(1);   
    
    man->CreateH1("Edep","Energy deposit",100,0.*MeV,1.*GeV);
    man->CreateNtuple("HitsEnergy","HitsEnergy");
    man->CreateNtupleIColumn("iEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fmom");
    man->CreateNtupleDColumn("fwav");
    man->CreateNtupleDColumn("fene");
    man->FinishNtuple(2);

}

MyRunAction::~MyRunAction()
{}

//********************* ORIGINAL**MIO ***************
void MyRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    G4int runID = run->GetRunID();                                      
    std::stringstream strRunID;                                         
    strRunID << runID;                                                  
    man->OpenFile("output" + strRunID.str() + ".root");
    
}

void MyRunAction::EndOfRunAction(const G4Run *run)
{

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    G4int runID = run->GetRunID();
//    man->FillH1(0, ph_wav);  
//    man->FillH1(1, ph_mag_mom);
//    man->FillH1(2, ph_tot_ene);

    man->Write();
    man->CloseFile();
    

    G4cout << "****** Finishing run ******" << runID << G4endl;
    
}

/*

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(0, fEnergyAbs);
  analysisManager->FillH1(1, fEnergyGap);





*/






