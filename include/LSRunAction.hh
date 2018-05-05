#ifndef LSRunAction_h
#define LSRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "globals.hh"


#include "TTree.h"
#include "TBranch.h"

class G4Run;

class LSRunAction : public G4UserRunAction {
public:
  LSRunAction();
  virtual ~LSRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

  void  SetRndmFreq(G4int val) { fSaveRndm = val; }
  G4int GetRndmFreq()          { return fSaveRndm; }

  virtual void FillEventHitTree(G4int NumberOfHitInAnEvent, G4double wavelengthOfPhotonHit);

  inline void SetAutoSeed (const G4bool val) { fAutoSeed = val; }

private:
  G4int fSaveRndm;
  G4bool fAutoSeed;

  TFile* outputFilename;
  TTree* eventTree;
  G4int eventHit;
  G4double wavelengthHit;

};

#endif
