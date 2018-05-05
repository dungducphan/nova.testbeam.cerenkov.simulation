//
// Created by Dung Phan on 3/13/18.
//

#include "LSTrackerSD.hh"

#include "G4SDManager.hh"
#include "G4OpticalPhoton.hh"

LSTrackerSD::LSTrackerSD(const G4String name)
    : G4VSensitiveDetector(name), fPhotonDetHitCollection(0) {
  collectionName.insert("PhotonDetHitCollection");
}

LSTrackerSD::~LSTrackerSD() {
}

void LSTrackerSD::Initialize(G4HCofThisEvent* hce) {
  // Create hits collection
  fPhotonDetHitCollection = new LSTrackerHitsCollection(SensitiveDetectorName,collectionName[0]);

  // Add this collection in hce

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fPhotonDetHitCollection );
}


G4bool LSTrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  return false;
}

G4bool LSTrackerSD::ProcessHits_constStep(const G4Step * aStep, G4TouchableHistory *) {
//// Generates a hit and uses the postStepPoint; PostStepPoint because the hit
//// is generated manually when the photon hits the detector
  if (aStep == NULL) return false;
  G4Track* theTrack = aStep->GetTrack();

  // Need to know if this is an optical photon
  if(theTrack->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

  // Find out information regarding the hit
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(thePostPoint->GetTouchable());

  G4ThreeVector photonArrive = thePostPoint -> GetPosition();
  G4double      arrivalTime  = theTrack -> GetGlobalTime();
  G4double      energy       = theTrack -> GetTotalEnergy();


  // Convert the global coordinate for arriving photons into
  // the local coordinate of the detector
  photonArrive = theTouchable->GetHistory()->
      GetTopTransform().TransformPoint(photonArrive);

  // Creating the hit and add it to the collection
  fPhotonDetHitCollection->insert(new LSTrackerHit(photonArrive, arrivalTime, energy));

  return true;
}
