#include "LSRunAction.hh"
#include "TFile.h"

LSRunAction::LSRunAction() : G4UserRunAction(), fSaveRndm(0), fAutoSeed(false) {
  G4RunManager::GetRunManager()->SetPrintProgress(1000);
}

LSRunAction::~LSRunAction() {
}

void LSRunAction::BeginOfRunAction(const G4Run * run) {
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

  outputFilename = new TFile("CerenkovHit.root", "recreate");
  eventTree = new TTree("HitTree", "Hit Statistics");
  eventTree->Branch("eventHit", &eventHit, "eventHit/I");
  eventTree->Branch("wavelengthHit", &wavelengthHit, "wavelengthHit/D");

  eventHit = 0;

  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");

  if (1) {
    // automatic (time-based) random seeds for each run
    G4cout << "*******************" << G4endl;
    G4cout << "*** AUTOSEED ON ***" << G4endl;
    G4cout << "*******************" << G4endl;
    long seeds[2];
    time_t systime = time(NULL);
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());
    G4Random::setTheSeeds(seeds);
    G4Random::showEngineStatus();
  } else {
    G4Random::showEngineStatus();
  }

  if (fSaveRndm > 0) G4Random::saveEngineStatus("BeginOfRun.rndm");
}

void LSRunAction::EndOfRunAction(const G4Run * run) {
  eventTree->Write();
  outputFilename->Write(0, TObject::kOverwrite);
  outputFilename->Close();

  if (fSaveRndm == 1) {
    G4Random::showEngineStatus();
    G4Random::saveEngineStatus("endOfRun.rndm");
  }
}

void LSRunAction::FillEventHitTree(G4int NumberOfHitInAnEvent, G4double wavelengthOfPhotonHit) {
  eventHit = NumberOfHitInAnEvent;
  wavelengthHit = wavelengthOfPhotonHit;
  eventTree->Fill();
}
