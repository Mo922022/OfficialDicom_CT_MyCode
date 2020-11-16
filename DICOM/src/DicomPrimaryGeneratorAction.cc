#include "DicomPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "DicomRegularDetectorConstruction.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "CLHEP/Random/RandFlat.h"


#include "G4RunManager.hh"
#include "G4GeneralParticleSource.hh"


#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DicomPrimaryGeneratorAction::DicomPrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction()
  
{
  G4int nParticle = 1;
  particleGun = new G4GeneralParticleSource(); 
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="proton");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticlePosition(G4ThreeVector(0.*cm,0.*cm,-40.*cm));
}


    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DicomPrimaryGeneratorAction::~DicomPrimaryGeneratorAction()
{
   delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DicomPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  particleGun->GeneratePrimaryVertex(anEvent);
}
