//
// Created by Dung Phan on 3/19/18.
//

#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "LSSteppingAction.hh"
#include "LSDetectorConstruction.hh"
#include "LSTrackerSD.hh"

#include <sstream>

LSSteppingAction::LSSteppingAction(LSDetectorConstruction * detector) : fDetector(detector) {
  fOpProcess = NULL;
}

LSSteppingAction::~LSSteppingAction() {}

void LSSteppingAction::UserSteppingAction(const G4Step * theStep) {
  G4Track* theTrack = theStep->GetTrack();

  G4StepPoint* thePrePoint  = theStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();

  G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  G4String thePrePVname  = " ";
  G4String thePostPVname = " ";

  if (thePostPV) {
    thePrePVname  = thePrePV->GetName();
    thePostPVname = thePostPV->GetName();
  }

  //Recording data for start
  if (theTrack->GetParentID()==0) {
    //This is a primary track
    if ( theTrack->GetCurrentStepNumber() == 1 ) {
        G4double x  = theTrack->GetVertexPosition().x();
        G4double y  = theTrack->GetVertexPosition().y();
        G4double z  = theTrack->GetVertexPosition().z();
        G4double pz = theTrack->GetVertexMomentumDirection().z();
    }
  }

  // Retrieve the status of the photon
  G4OpBoundaryProcessStatus theStatus = Undefined;
  G4ProcessManager* OpManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

  if (OpManager) {
    G4int MAXofPostStepLoops = OpManager->GetPostStepProcessVector()->entries();
    G4ProcessVector* fPostStepDoItVector = OpManager->GetPostStepProcessVector(typeDoIt);

    for ( G4int i=0; i<MAXofPostStepLoops; i++) {
      G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
      fOpProcess = dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
      if (fOpProcess) { theStatus = fOpProcess->GetStatus(); break;}
    }
  }

  switch (theStatus) {
    // Detected by a detector
    case Detection:
      // Check if the photon hits the detector and process the hit if it does
      if ( thePostPVname == "DetectorPhy" ) {
        //std::cout << "Photon is detected." << std::endl;
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        G4String SDname="TrackerChamberSD";
        LSTrackerSD* mppcSD = (LSTrackerSD*)SDman->FindSensitiveDetector(SDname);
        if (mppcSD) mppcSD->ProcessHits_constStep(theStep,NULL);
        // Stop Tracking when it hits the detector's surface
        theTrack->SetTrackStatus(fStopAndKill);
        return;
      }
      break;

    default: break;
  }

}