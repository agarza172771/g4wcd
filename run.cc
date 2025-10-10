#include "run.hh"
//#include "OpNoviceRun.hh"

#include "G4ParticleDefinition.hh"
#include "G4Run.hh"
#include "G4UnitsTable.hh"


MyRunAction::MyRunAction()
{
}
// Nota
MyRunAction::~MyRunAction()
{}



//********************* ORIGINAL**MIO ***************
void MyRunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    man->OpenFile("output.root");   
    man->CreateNtuple("Hits","Hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(0);
    
    man->CreateNtuple("Hitsph","Hitsph");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(1);   
    
    man->CreateNtuple("HitsEne","HitsEne");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fmom");
    man->CreateNtupleDColumn("fwav");
    man->CreateNtupleDColumn("tene");
    man->FinishNtuple(2);  
    

}
  //Esto lo metí en **** con todo y lo que esta entre parentesis
void MyRunAction::EndOfRunAction(const G4Run*)
{

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    man->Write();
    man->CloseFile();
}

