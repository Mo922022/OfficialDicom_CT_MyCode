#ifndef DicomEventAction_h
#define DicomEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include <TVector3.h>
#include <iostream>

#include "HistoManager.hh"

class G4Event;

class DicomEventAction : public G4UserEventAction
{
public:
  DicomEventAction(HistoManager* histo);
  virtual~DicomEventAction();

 public:
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event* anEvent);
  
  G4int GetEventno();

  //void SetDrawFlag (G4String val) { fDrawFlag = val; }
    
private:
 // G4String fDrawFlag;
	
	HistoManager* dataManager;
		
    		int EventNo;    
    		std::vector<double>* Energy;
    		std::vector<double>* Time;
    		std::vector<TVector3>* Position;
    		std::vector<TVector3>* Momentum;
	std::vector<int>* particleID;
};
#endif


	

	
	
