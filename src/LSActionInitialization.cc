#include "LSActionInitialization.hh"
#include "LSDetectorConstruction.hh"

#include "LSPrimaryGeneratorAction.hh"

#include "LSRunAction.hh"
#include "LSEventAction.hh"
#include "LSSteppingAction.hh"

LSActionInitialization::LSActionInitialization(LSDetectorConstruction* det)
    : G4VUserActionInitialization(),
      fDetector(det) {
}

LSActionInitialization::~LSActionInitialization() {
}

void LSActionInitialization::BuildForMaster() const {
  SetUserAction(new LSRunAction());
}

void LSActionInitialization::Build() const {
  SetUserAction(new LSPrimaryGeneratorAction());

  LSRunAction* runAction = new LSRunAction();
  LSEventAction* eventAction = new LSEventAction(runAction);


  SetUserAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new LSSteppingAction(fDetector));
}
