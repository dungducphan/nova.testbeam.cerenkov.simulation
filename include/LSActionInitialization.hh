#ifndef LSActionInitialization_h
#define LSActionInitialization_h 1

#include "LSPrimaryGeneratorAction.hh"
#include "LSEventAction.hh"
#include "LSRunAction.hh"
#include "LSSteppingAction.hh"

#include "G4VUserActionInitialization.hh"

class LSDetectorConstruction;

class LSActionInitialization : public G4VUserActionInitialization {
public:
  LSActionInitialization(LSDetectorConstruction*);
  virtual ~LSActionInitialization();

  virtual void Build() const;
  virtual void BuildForMaster() const;

private:
  LSDetectorConstruction* fDetector;
};

#endif
