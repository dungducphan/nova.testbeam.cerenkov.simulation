//
// Created by Dung Phan on 3/13/18.
//

#include "LSTrackerHit.hh"

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<LSTrackerHit>* LSTrackerHitAllocator=0;

LSTrackerHit::LSTrackerHit() {
  fArrivalTime = 0.;
  fPosArrive   = G4ThreeVector(0., 0., 0.);
}

LSTrackerHit::LSTrackerHit(G4ThreeVector pArrive, G4double pTime, G4double pEnergy) {
  fPosArrive   = pArrive;
  fArrivalTime = pTime;
  fEnergy      = pEnergy;
}

LSTrackerHit::~LSTrackerHit() {
}

LSTrackerHit::LSTrackerHit(const LSTrackerHit& right) : G4VHit() {
  *this = right;
}

const LSTrackerHit& LSTrackerHit::operator=(const LSTrackerHit& right) {
  fPosArrive   = right.fPosArrive;
  fArrivalTime = right.fArrivalTime;
  fEnergy      = right.fEnergy;

  return *this;
}

G4int LSTrackerHit::operator==(const LSTrackerHit& right) const {
  return fPosArrive   == right.fPosArrive  &&
         fArrivalTime == right.fArrivalTime &&
         fEnergy      == right.fEnergy;
}