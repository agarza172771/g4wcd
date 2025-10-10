#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
   RegisterPhysics(new G4EmStandardPhysics()); //Checar esto, se puede usar hadrones
   RegisterPhysics(new G4OpticalPhysics());
}

MyPhysicsList::~MyPhysicsList()
{}
