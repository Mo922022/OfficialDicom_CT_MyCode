//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: DicomActionInitialization.cc 68058 2013-07-22 14:47:43Z jmadsen $
//
/// \file DicomActionInitialization.cc
/// \brief Implementation of the DicomActionInitialization class

#include "DicomActionInitialization.hh"

#include "DicomActionInitialization.hh"
#include "DicomPrimaryGeneratorAction.hh"
#include "DicomRunAction.hh"
#include "DicomEventAction.hh"
#include "HistoManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DicomActionInitialization::DicomActionInitialization(DicomDetectorConstruction* detector)
: G4VUserActionInitialization(),
fDetector(detector)
{  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DicomActionInitialization::~DicomActionInitialization()
{  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DicomActionInitialization::BuildForMaster() const
{
    HistoManager*  histo = new HistoManager(fDetector);
    SetUserAction(new DicomRunAction(histo));
}

//DetectorConstruction* detector = new DetectorConstruction(); 
//HistoManager*  histo = new HistoManager(detector);
//RunAction* runaction = new RunAction(histo);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DicomActionInitialization::Build() const
{
	HistoManager*  histo = new HistoManager(fDetector);
	
    SetUserAction(new DicomPrimaryGeneratorAction);
    SetUserAction(new DicomRunAction(histo));
    SetUserAction(new DicomEventAction);
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
