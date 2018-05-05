#ifndef LSPhysicsList_h
#define LSPhysicsList_h 1

#include "G4VUserPhysicsList.hh"

#include "G4Cerenkov.hh"
#include "G4OpBoundaryProcess.hh"

class G4Cerenkov;
class G4OpBoundaryProcess;

class LSPhysicsList: public G4VUserPhysicsList {
public:
  LSPhysicsList();
  virtual ~LSPhysicsList();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

  virtual void ConstructOp();
  virtual void ConstructDecay();
  virtual void ConstructEM();


  virtual void SetCuts();

private:
  static G4ThreadLocal G4int fVerboseLevel;
  static G4ThreadLocal G4int fMaxNumPhotonStep;
  static G4ThreadLocal G4Cerenkov* fCerenkovProcess;
  static G4ThreadLocal G4OpBoundaryProcess* fBoundaryProcess;
};

#endif
