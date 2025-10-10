#include "detector.hh"
#include "G4SystemOfUnits.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *R0hist)
{
    G4Track *track = aStep->GetTrack();
    track->SetTrackStatus(fStopAndKill);
    
    G4StepPoint *preStepPoint = aStep -> GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep -> GetPostStepPoint();
    
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4cout << "Photon Position" << posPhoton << G4endl;
    
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();
//    G4cout << " Copy Number: " << copyNo << G4endl;
    
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();
    
 
    G4ThreeVector ph_mom_vec = aStep -> GetDeltaMomentum();
    G4double ph_tot_ene = aStep -> GetTotalEnergyDeposit();
    G4double ph_mag_mom = ph_mom_vec.mag();
    G4double ph_wav = (1.24*eV/ph_mag_mom)*1000.0;
    
    G4cout << "Detector position: " << posDetector << G4endl;     
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0,0, evt);
    man->FillNtupleDColumn(0,1, posDetector[0]);
    man->FillNtupleDColumn(0,2, posDetector[1]);
    man->FillNtupleDColumn(0,3, posDetector[2]);
    man->AddNtupleRow(0);
    
    man->FillNtupleIColumn(1,0, evt);
    man->FillNtupleDColumn(1,1, posPhoton[0]);
    man->FillNtupleDColumn(1,2, posPhoton[1]);
    man->FillNtupleDColumn(1,3, posPhoton[2]);
    man->AddNtupleRow(1);
    
    man->FillNtupleIColumn(2,0, evt);
    man->FillNtupleDColumn(2,1, ph_mom_vec[0]);
    man->FillNtupleDColumn(2,2, ph_mom_vec[1]);
    man->FillNtupleDColumn(2,3, ph_mom_vec[2]);
    man->FillNtupleDColumn(2,4, ph_mag_mom);
    man->FillNtupleDColumn(2,5, ph_wav);
    man->FillNtupleDColumn(2,6, ph_tot_ene);
    man->AddNtupleRow(2);
    
    return true;
}
