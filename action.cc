#include "action.hh"
#include "G4RunManager.hh"
#include "run.hh"

MyActionInitialization::MyActionInitialization()
{}

MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::Build() const
{
	MyPrimaryGenerator *generator = new MyPrimaryGenerator();
	SetUserAction(generator);
	
	MyRunAction *runAction = new MyRunAction();
	SetUserAction(runAction); 

//NUEVO
//	auto run = static_cast<MyRunAction*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
//    run->AddScintillation((G4double)fScintillationCounter);
//    run->AddCerenkov((G4double)fCerenkovCounter);

}
