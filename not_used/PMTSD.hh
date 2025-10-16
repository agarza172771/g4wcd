// PMTSD.hh
#ifndef PMTSD_h
#define PMTSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4OpticalPhoton.hh"
#include "G4Track.hh"

#include <vector>
#include <map>

class PMTSD : public G4VSensitiveDetector
{
public:
    PMTSD(const G4String& name, G4int id);
    virtual ~PMTSD();
    
    virtual void Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);
    
    // Métodos para obtener datos
    G4int GetPhotonCount() const { return fPhotonCount; }
    G4int GetPECount() const { return fPECount; } // Fotoelectrones
    G4double GetTotalEnergy() const { return fTotalEnergy; }
    G4double GetAmplitude() const { return fAmplitude; } // Señal amplificada
    
    void SetQuantumEfficiency(G4double efficiency) { fQuantumEfficiency = efficiency; }
    void SetGain(G4double gain) { fGain = gain; }
    void SetDarkCountRate(G4double rate) { fDarkCountRate = rate; }
    
    void ResetCounters();
    
private:
    G4int fPmtID;
    G4int fPhotonCount;
    G4int fPECount;
    G4double fTotalEnergy;
    G4double fAmplitude;
    G4double fQuantumEfficiency; // Eficiencia cuántica (0-1)
    G4double fGain;              // Ganancia del PMT (típicamente 1e6-1e7)
    G4double fDarkCountRate;     // Tasa de conteo oscuro (Hz)
    
    std::vector<G4double> fPhotonEnergies;
    std::vector<G4ThreeVector> fPhotonPositions;
    std::vector<G4double> fPhotonTimes;
    
    G4bool simulateQuantumEfficiency();
    G4double calculateAmplification();
};

#endif
