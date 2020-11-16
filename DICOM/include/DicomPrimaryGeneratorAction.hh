
#ifndef DicomPrimaryGeneratorAction_h
#define DicomPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

class G4GeneralParticleSource;
class G4Event;

class DicomPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  DicomPrimaryGeneratorAction();
  ~DicomPrimaryGeneratorAction();
  
  public:
		//method from base class
		virtual void GeneratePrimaries(G4Event*);

		//method to access particle gun
		const G4GeneralParticleSource* GetParticleGun() const { return particleGun; }
		
private:

      G4GeneralParticleSource* particleGun;
};

#endif


