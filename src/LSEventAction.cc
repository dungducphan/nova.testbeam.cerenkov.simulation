#include "LSEventAction.hh"
#include "LSRunAction.hh"
#include "LSTrackerHit.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

LSEventAction::LSEventAction(LSRunAction *runAction) : G4UserEventAction(),
                                                        fRunAction(runAction) {
  fMPPCCollID = 0;
}

LSEventAction::~LSEventAction() {
}

void LSEventAction::BeginOfEventAction(const G4Event *event) {
  //G4int evtNb = event->GetEventID();
  //G4cout << "<<< Event  " << evtNb << " started." << G4endl;
}

#include "G4Threading.hh"

void LSEventAction::EndOfEventAction(const G4Event *event) {
  //G4cout << "<<< Event  " << event->GetEventID() << " ended." << G4endl;

  if (fRunAction->GetRndmFreq() == 2) {
    std::ostringstream os;
    os<<"endOfEvent_"<<G4Threading::G4GetThreadId()<<".rndm";
    G4Random::saveEngineStatus(os.str().c_str());
  }

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4String colName = "PhotonDetHitCollection";
  fMPPCCollID = SDman->GetCollectionID(colName);

  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  LSTrackerHitsCollection* mppcHC = 0;

  // Get the hit collections
  if (HCE) {
    if (fMPPCCollID>=0) mppcHC = (LSTrackerHitsCollection*)(HCE->GetHC(fMPPCCollID));
  }

  // Get hit information about photons that reached the detector in this event
  if (mppcHC) {
    G4int n_hit = mppcHC->entries();
    auto HitVector = mppcHC->GetVector();
    for (unsigned short int ihit = 0; ihit < n_hit; ihit++) {
      G4double energy = HitVector->at(ihit)->GetEnergy();
      G4double wavelength = 1242.38 / (energy/eV);
      fRunAction->FillEventHitTree(n_hit, wavelength);
    }
  }
}
