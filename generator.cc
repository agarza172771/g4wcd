#include "generator.hh"
#include "Randomize.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}



void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName="mu+";
	G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
	
	G4ThreeVector pos(0.,0.,0.); //Original
	G4ThreeVector mom(0.,0.,1.);

/*
//Agregue	
    G4double x0 = 100.*(G4UniformRand()-0.5) *cm;
    G4double y0 = 100.*(G4UniformRand()-0.5) *cm;
    G4double z0 = 100.*(G4UniformRand()-0.6) *cm;
 //   G4double z0 = -0.6 *m;
    
    G4cout << "x0 =" << x0 << G4endl;
    G4cout << "y0 =" << y0 << G4endl;
    G4cout << "z0 =" << z0 << G4endl;
*/
	
//	fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
	fParticleGun->SetParticlePosition(pos); //Original
	fParticleGun->SetParticleMomentumDirection(mom);
//    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x0,y0,z0));
	fParticleGun->SetParticleMomentum(1000.*GeV);
	fParticleGun->SetParticleDefinition(particle);
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

