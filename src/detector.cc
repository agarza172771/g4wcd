#include "detector.hh"
#include "G4SystemOfUnits.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    fTotalEnergyDeposited = 0.;
//    fTotalMomentum = 0;
//    fTotalWave = 0;
}

MySensitiveDetector::~MySensitiveDetector()
{}


void MySensitiveDetector::Initialize(G4HCofThisEvent *)
{
    fTotalEnergyDeposited = 0.;
//    fTotalMomentum = 0;
//    fTotalWave = 0;
}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{


    G4Track *track = aStep->GetTrack();
    track->SetTrackStatus(fStopAndKill);
    
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep -> GetPostStepPoint();
    
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4cout << "Photon Position" << posPhoton << G4endl;
    
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();
    
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();
    
    G4ThreeVector ph_mom_vec = postStepPoint->GetMomentum();
    G4double ph_tot_ene = aStep -> GetTotalEnergyDeposit(); 
    G4double ph_mag_mom = ph_mom_vec.mag();  
    G4double ph_wav = (1.24*eV/ph_mag_mom)*1000.0; 
    
    G4cout << "Detector position: " << posDetector << G4endl;   


    G4double fGlobalTime = preStepPoint->GetGlobalTime();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    G4double fMomPhotonMag = momPhoton.mag();
    G4double fWlen = (1.239841939 * eV / fMomPhotonMag) * 1E+03;

    man->FillNtupleIColumn(0, 0, eventID);
    man->FillNtupleDColumn(0, 1, posPhoton[0]);
    man->FillNtupleDColumn(0, 2, posPhoton[1]);
    man->FillNtupleDColumn(0, 3, posPhoton[2]);
    man->FillNtupleDColumn(0, 4, fGlobalTime);
    man->FillNtupleDColumn(0, 5, fWlen);
    man->AddNtupleRow(0);
    
    
    
    man->FillNtupleIColumn(1,0, eventID);
    man->FillNtupleDColumn(1,1, posDetector[0]);
    man->FillNtupleDColumn(1,2, posDetector[1]); 
    man->FillNtupleDColumn(1,3, posDetector[2]); 
    man->AddNtupleRow(1); 

    
    man->FillNtupleIColumn(2,0, eventID); 
    man->FillNtupleDColumn(2,1, ph_mom_vec[0]); 
    man->FillNtupleDColumn(2,2, ph_mom_vec[1]); 
    man->FillNtupleDColumn(2,3, ph_mom_vec[2]); 
    man->FillNtupleDColumn(2,4, ph_mag_mom); 
    man->FillNtupleDColumn(2,5, ph_wav); 
    man->FillNtupleDColumn(2,6, ph_tot_ene); 
    man->AddNtupleRow(2); 

    G4double energyDeposited = aStep->GetTotalEnergyDeposit();

    if (energyDeposited > 0)
    {
        fTotalEnergyDeposited += energyDeposited;
    }

    return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->FillH1(2, fTotalEnergyDeposited); 
//    man->FillH1(1, ph_mag_mom);
//    man->FillH1(2, ph_tot_ene);

  
}










