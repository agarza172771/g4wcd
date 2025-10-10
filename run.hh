#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"



class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction(); //MIO
    ~MyRunAction(); //MIO
    

    
    virtual void BeginOfRunAction(const G4Run*); //MIO
    virtual void EndOfRunAction(const G4Run*); //MIO
};



#endif
