#include "DicomEventAction.hh"
#include "G4Event.hh"
#include "DicomRunAction.hh"
#include "DicomRun.hh"

// New Add
#include <G4EventManager.hh>
#include <G4VVisManager.hh>
#include "G4SDManager.hh"
#include <G4UImanager.hh>
#include <G4ios.hh>
#include <vector>
#include <map>
#include <TVector3.h>
#include <TString.h>
#include <G4SystemOfUnits.hh>
#include "PhantomHit.hh"
#include "HistoManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DicomEventAction::DicomEventAction(HistoManager* histo):dataManager(histo),
 EventNo(0) 
   //fDrawFlag("all")
{ 
	Energy= new std::vector<double>;
	Time= new std::vector<double>;
	particleID=new std::vector<int>;
	Position = new std::vector<TVector3>;
	Momentum = new std::vector<TVector3>;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DicomEventAction::~DicomEventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DicomEventAction::BeginOfEventAction(const G4Event* evt)
{
    EventNo=GetEventno();
    //std::cout << "Event no " << EventNo << std::endl;
	Energy->clear();
	Time->clear();
	particleID->clear();
	Position->clear();
	Momentum->clear();
	dataManager->IncreasePrimaries();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DicomEventAction::EndOfEventAction(const G4Event* evt)
{
	
G4SDManager * SDman = G4SDManager::GetSDMpointer();

	PhantomHitsCollection* SHC = 0;

	G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
	
	if (HCE){
	int nPhantom=HCE->GetNumberOfCollections();	
		//G4cout << "Number of Collections" << nScins << G4endl;
		for(int j=0;j<nPhantom;j++){

			//G4cout << "Current Collection ID" << ScinID << G4endl;
			SHC = (PhantomHitsCollection*)(HCE->GetHC(j));

			if (SHC){

				int phantom_hit = SHC->entries();
				for (G4int i=0;i<phantom_hit;i++){
					Energy->push_back((*SHC)[i]->GetEkin());
					Time->push_back((*SHC)[i]->GetTime());
					particleID->push_back((*SHC)[i]->GetParticleID());
					Position->push_back((*SHC)[i]->GetPosition());
					Momentum->push_back((*SHC)[i]->GetMomentum());
				}
				if(phantom_hit!=0)dataManager->SaveSecondaries(particleID,Energy,Time,Position,Momentum);
			}


		}
	}	
	
	
}
//***********************Helper Functions ***************

G4int DicomEventAction::GetEventno() {
	G4int evno = fpEventManager->GetConstCurrentEvent()->GetEventID();
	return evno ;
}

