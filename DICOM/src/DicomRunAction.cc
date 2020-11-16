
#include "DicomRunAction.hh"
#include "DicomRun.hh"

//-- In order to obtain detector information.
#include <fstream>
#include <iomanip>
#include "G4THitsMap.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"


#include "DicomPrimaryGeneratorAction.hh" // mod
#include "G4Run.hh"// mod
#include "G4UImanager.hh"// mod
#include "G4VVisManager.hh"// mod
#include "G4ios.hh"// mod
#include "HistoManager.hh"// mod
#include "G4UImessenger.hh"// mod
#include "G4GeneralParticleSource.hh"// mod

extern std::ofstream output;





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DicomRunAction* DicomRunAction::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DicomRunAction* DicomRunAction::Instance() { return fInstance; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/// Constructor
DicomRunAction::DicomRunAction(HistoManager* histo)
:   G4UserRunAction(), fDcmrun(0), fFieldValue(14), histoManager(histo) 
{
    // - Prepare data member for DicomRun.
    //   vector represents a list of MultiFunctionalDetector names.
    fSDName.push_back(G4String("phantomSD"));
    fInstance = this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/// Destructor.
DicomRunAction::~DicomRunAction()
{
  fSDName.clear();
  fInstance = 0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Run* DicomRunAction::GenerateRun()
{
  // Generate new RUN object, which is specially
  // dedicated for MultiFunctionalDetector scheme.
  //  Detail description can be found in DicomRun.hh/cc.
  //return new DicomRun(fSDName);
  return fDcmrun = new DicomRun(fSDName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DicomRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  histoManager->book(aRun->GetRunID()); //mod
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DicomRunAction::EndOfRunAction(const G4Run* aRun)
{
	
	
  G4cout<<"*********************************         OOOOOOOOOOOOOOOOOOOooooooooooooooooooOOOOOOOOOOOOOOOOO   End Run."<<G4endl; //mod
  histoManager->save();//mod
	
	
	
  G4int nofEvents = aRun->GetNumberOfEvent();
  if (nofEvents == 0) return;
  //print
  //
  
  static double local_total_dose = 0;
  double total_dose = 0;
  
  const DicomRun* reRun = static_cast<const DicomRun*>(aRun);
  //--- Dump all scored quantities involved in DicomRun.
  for ( G4int i = 0; i < (G4int)fSDName.size(); i++ ){
    //
    //---------------------------------------------
    // Dump accumulated quantities for this RUN.
    //  (Display only central region of x-y plane)
    //      0       ConcreteSD/DoseDeposit
    //---------------------------------------------
    G4THitsMap<G4double>* DoseDeposit = 
      reRun->GetHitsMap(fSDName[i]+"/DoseDeposit");
    
    if( DoseDeposit && DoseDeposit->GetMap()->size() != 0 ) {
      std::map<G4int,G4double*>::iterator itr = 
        DoseDeposit->GetMap()->begin();
      for(; itr != DoseDeposit->GetMap()->end(); itr++) {
        if(!IsMaster()) { local_total_dose += *(itr->second); }
        total_dose += *(itr->second);
      }
    }

  }
  
  if (IsMaster())
    {
      G4cout
        << "\n--------------------End of Global Run-----------------------"
        << " \n The run was " << nofEvents << " events " << G4endl;
      G4cout << "LOCAL TOTAL DOSE : \t" << local_total_dose/gray 
             << " Gy" << std::endl;
      G4cout << "      TOTAL DOSE : \t" << total_dose/gray 
             << " Gy" << std::endl;
      
    }
  else
    {
      G4cout
        << "\n--------------------End of Local Run------------------------"
        << " \n The run was " << nofEvents << G4endl;
      G4cout << "LOCAL TOTAL DOSE : \t" << local_total_dose/gray 
             << " Gy" << std::endl;
      G4cout << "      TOTAL DOSE : \t" << total_dose/gray 
             << " Gy" << std::endl;
      
    }
  
  if(IsMaster()) {
    G4cout << " ###### EndOfRunAction ###### " << G4endl;
    //- DicomRun object.
    const DicomRun* re02Run = static_cast<const DicomRun*>(aRun);
    //--- Dump all scored quantities involved in DicomRun.
    
    for ( G4int i = 0; i < (G4int)fSDName.size(); i++ ){
      //
      //---------------------------------------------
      // Dump accumulated quantities for this RUN.
      //  (Display only central region of x-y plane)
      //      0       ConcreteSD/DoseDeposit
      //---------------------------------------------
      G4THitsMap<G4double>* DoseDeposit = 
        re02Run->GetHitsMap(fSDName[i]+"/DoseDeposit");
      
      G4cout << "============================================================="
             <<G4endl;
      G4cout << " Number of event processed : "
             << aRun->GetNumberOfEvent() << G4endl;
      G4cout << "============================================================="
             <<G4endl;
      
      std::ofstream fileout;
      G4String fname = "dicom.out";
      fileout.open(fname);
      G4cout << " opened file " << fname << " for dose output" << G4endl;
      
      if( DoseDeposit && DoseDeposit->GetMap()->size() != 0 ) {
        std::ostream *myout = &G4cout;
        PrintHeader(myout);
        std::map<G4int,G4double*>::iterator itr =DoseDeposit->GetMap()->begin();
        for(; itr != DoseDeposit->GetMap()->end(); itr++) {
          fileout <<  itr->first
                  << "     "  << *(itr->second)/CLHEP::gray
                  << G4endl;
          G4cout << "    " << itr->first
                 << "     " << std::setprecision(6) 
                 << *(itr->second)/CLHEP::gray << " Gy"
                 << G4endl;
        }
        G4cout << "============================================="<<G4endl;
            } else {
        G4Exception("DicomRunAction", "000", JustWarning, 
      "DoseDeposit HitsMap is either a null pointer of the HitsMap was empty");
      }
      fileout.close();
      G4cout << " closed file " << fname << " for dose output" << G4endl;
      
    }
  }
  
  G4cout << "Finished : End of Run Action " << aRun->GetRunID() << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DicomRunAction::PrintHeader(std::ostream *out)
{
  std::vector<G4String> vecScoreName;
  vecScoreName.push_back("DoseDeposit");
  
  // head line
  //
  std::string vname;
  *out << std::setw(10) << "Voxel" << " |";
  for (std::vector<G4String>::iterator it = vecScoreName.begin();
       it != vecScoreName.end(); it++) {
    //vname = FillString((*it),
    //                       ' ',
    //                       FieldValue+1,
    //                       false);
    //    *out << vname << '|';
    *out << std::setw(fFieldValue) << (*it) << "  |";
  }
  *out << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::string DicomRunAction::FillString(const std::string &name,
                                       char c, G4int n, G4bool back)
{
  std::string fname("");
  G4int k = n - name.size();
  if (k > 0) {
    if (back) {
      fname = name;
      fname += std::string(k,c);
    }
    else {
      fname = std::string(k,c);
      fname += name;
    }
  }
  else {
    fname = name;
  }
  return fname;
}
