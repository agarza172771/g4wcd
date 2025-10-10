// PMTSD.cc
#include "PMTSD.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "Randomize.hh"

PMTSD::PMTSD(const G4String& name, G4int id)
: G4VSensitiveDetector(name), 
  fPmtID(id),
  fQuantumEfficiency(0.25),  // 25% QE típica
  fGain(1.0e6),              // Ganancia de 1 millón
  fDarkCountRate(1000.0)     // 1000 Hz de ruido oscuro
{
    ResetCounters();
}

PMTSD::~PMTSD()
{}

void PMTSD::Initialize(G4HCofThisEvent*)
{
    ResetCounters();
    
    // Simular conteo oscuro (opcional)
    G4double darkCountProbability = fDarkCountRate * 1.0e-9; // Para eventos de 1 ns
    if (G4UniformRand() < darkCountProbability) {
        fPECount++;
        fAmplitude += fGain * 1.0; // 1 fotoelectrón amplificado
    }
}

G4bool PMTSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    // Solo procesar fotones ópticos
    G4Track* track = step->GetTrack();
    if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
        return false;
    }
    
    G4double energy = track->GetKineticEnergy();
    G4ThreeVector position = track->GetPosition();
    G4double time = track->GetGlobalTime();
    
    // Registrar fotón incidente
    fPhotonCount++;
    fTotalEnergy += energy;
    fPhotonEnergies.push_back(energy);
    fPhotonPositions.push_back(position);
    fPhotonTimes.push_back(time);
    
    // Simular eficiencia cuántica
    if (simulateQuantumEfficiency()) {
        fPECount++;
        
        // Simular amplificación (ganancia del PMT)
        G4double peAmplitude = calculateAmplification();
        fAmplitude += peAmplitude;
    }
    
    // Matar el fotón después de la detección
    track->SetTrackStatus(fStopAndKill);
    
    return true;
}

void PMTSD::EndOfEvent(G4HCofThisEvent*)
{
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
    G4cout << "PMT " << fPmtID << " - Event " << eventID << ": " 
           << fPhotonCount << " photons, " 
           << fPECount << " photoelectrons, "
           << "Amplitude: " << fAmplitude / fGain << " PE-equivalent"
           << G4endl;
}

void PMTSD::ResetCounters()
{
    fPhotonCount = 0;
    fPECount = 0;
    fTotalEnergy = 0.0;
    fAmplitude = 0.0;
    fPhotonEnergies.clear();
    fPhotonPositions.clear();
    fPhotonTimes.clear();
}

G4bool PMTSD::simulateQuantumEfficiency()
{
    // Eficiencia cuántica dependiente de la longitud de onda
    G4double wavelength = (1240.0 / (fPhotonEnergies.back()/eV)) * nm;
    G4double qe = fQuantumEfficiency;
    
    // Modulación típica de QE vs longitud de onda
    if (wavelength < 300*nm) qe *= 0.5;
    else if (wavelength > 600*nm) qe *= 0.3;
    
    return (G4UniformRand() < qe);
}

G4double PMTSD::calculateAmplification()
{
    // Distribución de ganancia con fluctuaciones
    G4double meanGain = fGain;
    G4double sigmaGain = meanGain * 0.3; // 30% de fluctuación
    
    // Distribución gaussiana de ganancia
    G4double amplifiedSignal = G4RandGauss::shoot(meanGain, sigmaGain);
    return std::max(amplifiedSignal, 0.0);
}
