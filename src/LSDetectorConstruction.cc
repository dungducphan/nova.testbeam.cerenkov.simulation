#include "LSDetectorConstruction.hh"
#include "LSTrackerSD.hh"

#include "G4SDManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include <iostream>
#include <fstream>
#include <vector>

LSDetectorConstruction::LSDetectorConstruction() : G4VUserDetectorConstruction() {
}

LSDetectorConstruction::~LSDetectorConstruction() {
}

void LSDetectorConstruction::BuildMaterial() {
  G4NistManager * nist = G4NistManager::Instance();

  fAir = nist->FindOrBuildMaterial("G4_AIR");
  fAl = nist->FindOrBuildMaterial("G4_Al");
  fCO2 = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");

  /**
   * Optical Properties of CO2
   * */
  G4double photonEnery_CO2[4] = {0.001*eV, 1*eV, 10*eV, 100*eV};
  G4double refractiveIndex_CO2[4] = {1.0004, 1.0004, 1.0004, 1.0004};

  G4double photonEnery_Air[4] = {0.001*eV, 1*eV, 10*eV, 100*eV};
  G4double refractiveIndex_Air[4] = {1.0000, 1.0000, 1.0000, 1.0000};

  G4MaterialPropertiesTable* MPT_CO2 = new G4MaterialPropertiesTable();
  MPT_CO2->AddProperty("RINDEX", photonEnery_CO2, refractiveIndex_CO2, 4)->SetSpline(true);
  fCO2->SetMaterialPropertiesTable(MPT_CO2);

  G4MaterialPropertiesTable* MPT_Air = new G4MaterialPropertiesTable();
  MPT_Air->AddProperty("RINDEX", photonEnery_Air, refractiveIndex_Air, 4)->SetSpline(true);
  fAir->SetMaterialPropertiesTable(MPT_Air);
}

G4VPhysicalVolume * LSDetectorConstruction::Construct() {
  BuildMaterial();
  
  // Overlapping check
  G4bool checkOverlaps = true;

  /**
   * [detector Volume description]
   */
  //detectorBox = new G4Box("DetectorBox", detector_hx/2., detector_hy/2., detector_hz/2.);
  detectorBox = new G4Tubs("DetectorBox",
                         0*cm  , //pRMin,
                         detector_radius , //pRMax,
                         detector_hz   , //Dz, Half length in z
                         0     , //pSPhi,
                         twopi);   //pDPhi)
  detectorLog = new G4LogicalVolume(detectorBox, fAl, "DetectorLog", 0, 0, 0);

  /**
   * [mirror Volume description]
   */
  mirrorBox = new G4Box("MirrorBox", mirror_hx/2., mirror_hy/2., mirror_hz/2.);
  mirrorLog = new G4LogicalVolume(mirrorBox, fAl, "MirrorLog", 0, 0, 0);

  /**
   * [cone Volume description]
   */
  cone = new G4Cons("Cone",
                         15*cm  , //pRMin1,
                         16*cm  , //pRMax1,
                         3.5*2.54*cm  , //pRMin2,
                         3.5*2.54*cm + 1*cm , //pRMax2,
                         30*cm   , //Dz,
                         0     , //pSPhi,
                         twopi);   //pDPhi)
  coneLog = new G4LogicalVolume(cone, fAl, "ConeLog", 0, 0, 0);

  // Set Visual Properties
  G4VisAttributes* mirrorVA = new G4VisAttributes();
  mirrorVA->SetColor(G4Colour(0.6, 0.6, 0.6));
  mirrorVA->SetForceWireframe(true);
  mirrorLog->SetVisAttributes(mirrorVA);

  /**
   * [cherenkovEnviroment description]
   */
  targetBox = new G4Tubs("TargetTubs",
                          14*cm  , //pRMin,
                          15*cm  , //pRMax,
                          1*m   , //Dz, Half length in z
                          0     , //pSPhi,
                          twopi);   //pDPhi)
  targetLog = new G4LogicalVolume(targetBox, fAl,"TargetLog", 0, 0, 0);

  CO2Box = new G4Tubs("CO2Tubs",
                         0*cm  , //pRMin,
                         14*cm  , //pRMax,
                         1*m   , //Dz, Half length in z
                         0     , //pSPhi,
                         twopi);   //pDPhi)
  CO2Log = new G4LogicalVolume(CO2Box, fCO2,"CO2Log", 0, 0, 0);

  // Set Visual Properties
  G4VisAttributes* targetVA = new G4VisAttributes();
  targetVA->SetColor(G4Colour(0.8, 0.8, 0.8));
  targetVA->SetForceWireframe(true);
  targetLog->SetVisAttributes(targetVA);

  /**
   * [World description]
   */
  worldBox = new G4Box("WorldBox", world_hx, world_hy, world_hz);
  worldLog = new G4LogicalVolume(worldBox, fAir, "WorldLog");


  /**
   * Placing volumes in WORLD
   * Remember to define all the needed logical volumes before arranging place hierarchy!
   */
  G4double phi = 90*deg;
  G4ThreeVector u = G4ThreeVector(0, 0, -1);
  G4ThreeVector v = G4ThreeVector(-std::sin(phi), std::cos(phi),0.);
  G4ThreeVector w = G4ThreeVector( std::cos(phi), std::sin(phi),0.);
  G4RotationMatrix* rotm1  = new G4RotationMatrix(u, v, w);

  G4double phi2 = -45*deg;
  G4ThreeVector u2 = G4ThreeVector(-std::sin(phi2),0., std::cos(phi2));
  G4ThreeVector v2 = G4ThreeVector(0., -1., 0.);
  G4ThreeVector w2 = G4ThreeVector( std::cos(phi2),0., std::sin(phi2));
  G4RotationMatrix* rotm2 = new G4RotationMatrix(u2, v2, w2);


  targetPhy = new G4PVPlacement(0, G4ThreeVector(0,0,0), targetLog, "TargetPhy", worldLog, false, 0);
  CO2Phy = new G4PVPlacement(0, G4ThreeVector(0,0,0), CO2Log, "CO2Phy", worldLog, false, 0);
  detectorPhy = new G4PVPlacement(rotm1, G4ThreeVector(-85*cm,0,-1.2*m), detectorLog, "DetectorPhy", worldLog, false, 0);
  mirrorPhy = new G4PVPlacement(rotm2, G4ThreeVector(0,0,-1.2*m), mirrorLog, "MirrorPhy", worldLog, false, 0);
  conePhy = new G4PVPlacement(rotm1, G4ThreeVector(-52*cm,0,-1.2*m), coneLog, "ConePhy", worldLog, false, 0);
  worldPhy = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldPhy", 0, false, 0, checkOverlaps);



  //---------------------------------------------------------------------------------------------------
  /**
   * Surface Properties of TargetHousing and CO2
   * */
  G4OpticalSurface* opHousingCO2Surface = new G4OpticalSurface("HousingCO2Surface");
  opHousingCO2Surface->SetType(dielectric_metal);
  opHousingCO2Surface->SetFinish(ground);
  opHousingCO2Surface->SetModel(unified);
  G4LogicalBorderSurface* housingCO2Surface = new G4LogicalBorderSurface("HousingCO2Surface", CO2Phy, targetPhy, opHousingCO2Surface);

  /**
   * Surface Properties of World and CO2
   * */
  G4OpticalSurface* opWorldCO2Surface = new G4OpticalSurface("WorldCO2Surface");
  opWorldCO2Surface->SetType(dielectric_dielectric);
  opWorldCO2Surface->SetFinish(polished);
  opWorldCO2Surface->SetModel(unified);
  G4LogicalBorderSurface* worldCO2Surface = new G4LogicalBorderSurface("WorldCO2Surface", CO2Phy, worldPhy, opWorldCO2Surface);

  /**
   * Surface Properties of PMT (Detector)
   * */
  G4OpticalSurface* opDetectorSurface = new G4OpticalSurface("DetectorSurface");
  opDetectorSurface->SetType(dielectric_metal);
  opDetectorSurface->SetFinish(ground);
  opDetectorSurface->SetModel(glisur);
  G4LogicalSkinSurface* detectorSurface = new G4LogicalSkinSurface("DetectorSurface", detectorLog, opDetectorSurface);

  /**
   * Surface Properties of Mirror
   * */
  G4OpticalSurface* opMirrorSurface = new G4OpticalSurface("MirrorSurface");
  opMirrorSurface->SetType(dielectric_metal);
  opMirrorSurface->SetFinish(polished);
  opMirrorSurface->SetModel(unified);
  G4LogicalSkinSurface* mirrorSurface = new G4LogicalSkinSurface("DetectorSurface", mirrorLog, opMirrorSurface);

  /**
   * Surface Properties of Cone
   * */
  G4OpticalSurface* opConeSurface = new G4OpticalSurface("ConeSurface");
  opMirrorSurface->SetType(dielectric_metal);
  opMirrorSurface->SetFinish(polished);
  opMirrorSurface->SetModel(unified);
  G4LogicalSkinSurface* coneSurface = new G4LogicalSkinSurface("DetectorSurface", coneLog, opConeSurface);


  //---------------------------------------------------------------------------------------------------
  /**
   * Photon Transport Properties for Housing-CO2 Surface
   * */
  G4double ephoton_housingCO2[2]      = {0.0001*eV, 100*eV};
  G4double reflectivity_housingCO2[2] = {0.5, 0.5}; // Not realistic
  G4double efficiency_housingCO2[2]   = {0.0, 0.0};
  G4MaterialPropertiesTable *housingCO2MPT = new G4MaterialPropertiesTable();
  housingCO2MPT->AddProperty("REFLECTIVITY", ephoton_housingCO2, reflectivity_housingCO2, 2)->SetSpline(true);
  housingCO2MPT->AddProperty("EFFICIENCY",   ephoton_housingCO2, efficiency_housingCO2,   2)->SetSpline(true);
  opHousingCO2Surface->SetMaterialPropertiesTable(housingCO2MPT);

  /**
   * Photon Transport Properties for Detector Surface
   * */

  std::ifstream m16data("/Users/dphan/Documents/CO2Cherenkov/Geant4Skeleton/M16Corrected2-3.csv");
  std::vector<G4double> wavelength, qe;
  double x, y;
  while (m16data >> x >> y) {
    wavelength.push_back(x);
    qe.push_back(y);
    x = 0;
    y = 0;
  }
  m16data.close();

  G4double *ephoton_pmt       = (G4double*) malloc(wavelength.size()* sizeof(G4double));
  G4double *reflectivity_pmt  = (G4double*) malloc(wavelength.size()* sizeof(G4double));
  G4double *efficiency_pmt    = (G4double*) malloc(wavelength.size()* sizeof(G4double));

  for (unsigned short int idx = 0; idx < wavelength.size(); idx++) {
    ephoton_pmt[idx] = (1242.38 / wavelength.at(idx))*eV;
    reflectivity_pmt[idx] = 0.;
    efficiency_pmt[idx] = 1;//(qe.at(idx))/100.;
  }

//  std::cout << wavelength.size() << std::endl;
//  for (unsigned short int idx = 0; idx < wavelength.size(); idx++) {
//    std::cout << ephoton_pmt[idx]/eV << " : " << efficiency_pmt[idx] << std::endl;
//  }

  G4MaterialPropertiesTable *detectorMPT = new G4MaterialPropertiesTable();
  detectorMPT->AddProperty("REFLECTIVITY", ephoton_pmt, reflectivity_pmt, wavelength.size())->SetSpline(true);
  detectorMPT->AddProperty("EFFICIENCY",   ephoton_pmt, efficiency_pmt,   wavelength.size())->SetSpline(true);
  opDetectorSurface->SetMaterialPropertiesTable(detectorMPT);

  /**
   * Photon Transport Properties for Mirror Surface
   * */
  G4double ephoton_mirror[4]      = {0.0001*eV, 1*eV, 10*eV, 100*eV};
  G4double reflectivity_mirror[4] = {0.8,0.8,0.8,0.8};
  G4double efficiency_mirror[4]   = {0.0,0.0,0.0,0.0};
  G4MaterialPropertiesTable *mirrorMPT = new G4MaterialPropertiesTable();
  mirrorMPT->AddProperty("REFLECTIVITY", ephoton_mirror, reflectivity_mirror, 4)->SetSpline(true);
  mirrorMPT->AddProperty("EFFICIENCY",   ephoton_mirror, efficiency_mirror,   4)->SetSpline(true);
  opMirrorSurface->SetMaterialPropertiesTable(mirrorMPT);

  return worldPhy;
}

void LSDetectorConstruction::ConstructSDandField() {
  // Sensitive detectors

  LSTrackerSD* aTrackerSD = new LSTrackerSD("TrackerChamberSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);

  // Setting aTrackerSD to all logical volumes with the same name of "Chamber_LV".
  SetSensitiveDetector("DetectorLog", aTrackerSD, true);
}


