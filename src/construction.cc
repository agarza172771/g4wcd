#include "construction.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4OpticalPhysics.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();
    
    G4Material *detecMat = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");     //Detector
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");                 //Mundo
    G4Material *radiaMat = nist->FindOrBuildMaterial("G4_WATER");               //Agua 
    G4Material *tyvekMat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");        //Tyvek
    G4Material *alumiMat = nist->FindOrBuildMaterial("G4_Al");                  //Capa de Aluminio
//    G4Material *rotopMat = nist->FindOrBuildMaterial("G4_POLYCARBONATE");       //Rotoplas
    
//Superficie optica de Tyvek
    G4OpticalSurface* tyvekSurface = new G4OpticalSurface("TyvekSurface");
    tyvekSurface->SetType(dielectric_dielectric);
    tyvekSurface->SetFinish(groundfrontpainted);                                //puede ser ground
    tyvekSurface->SetModel(unified);

//Rotoplas  
    // Elementos necesarios
    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* O = nist->FindOrBuildElement("O");
    G4Element* N = nist->FindOrBuildElement("N");
    
    // 1. Primero crear el policarbonato base (C16H14O3)
    G4Material* polycarbonate = new G4Material("POLYCARBONATE", 1.20*g/cm3, 3);
    polycarbonate->AddElement(C, 16);
    polycarbonate->AddElement(H, 14);
    polycarbonate->AddElement(O, 3);
    
    // 2. Crear negro de humo (carbono casi puro)
    G4Material* carbonBlack = new G4Material("CARBON_BLACK", 1.8*g/cm3, 1);
    carbonBlack->AddElement(C, 1);
    
    // 3. Material para estabilizantes UV (ejemplo: benzotriazol)
    G4Material* uvStabilizer = new G4Material("UV_STABILIZER", 1.2*g/cm3, 3);
    uvStabilizer->AddElement(C, 6);
    uvStabilizer->AddElement(H, 5);
    uvStabilizer->AddElement(N, 3);
    
    // 4. Crear material Rotoplas compuesto
    G4Material* rotopMat = new G4Material("ROTOPMAT", 1.22*g/cm3, 3);
    rotopMat->AddMaterial(polycarbonate, 0.88);    // 88% policarbonato
    rotopMat->AddMaterial(carbonBlack, 0.04);      // 4% negro de humo
    rotopMat->AddMaterial(uvStabilizer, 0.08);     // 8% estabilizantes   
    
    
//https://refractiveindex.info/
//Indice refractivo de agua
  G4double refractiveindex[] =
  { 
    1.3435, 1.344,  1.3445, 1.345,  1.3455,
    1.346,  1.3465, 1.347,  1.3475, 1.348,
    1.3485, 1.3492, 1.35,   1.3505, 1.351,
    1.3518, 1.3522, 1.3530, 1.3535, 1.354,
    1.3545, 1.355,  1.3555, 1.356,  1.3568,
    1.3572, 1.358,  1.3585, 1.359,  1.3595,
    1.36,   1.3608
  };

//Energias de fotones
  G4double photonEnergy[] =
  { 
    2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
    2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
    2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
    2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
    2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
    3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
    3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
    3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV 
  };

//Absorción agua (para tono azulado Cherenkov)
  G4double absorption[] = 
  {
    1.0e-7*m, 1.0e-6*m, 1.0e-5*m, 1.0e-4*m,
    1.0e-3*m, 0.005*m,  0.01*m,   0.02*m,
    0.05*m,   0.1*m,    0.2*m,    0.5*m,
    1.*m,     2*m,      5*m,      10*m,
    20*m,     40*m,     60*m,     80*m,
    100*m,    120*m,    140*m,    160*m,
    180*m,    200*m,    220*m,    240*m,
    260*m,    280*m,    300*m,    500*m
  };

//Reflectividad del Tyvek
  G4double reflectivity[] =
  {  
    0.75,  0.78,  0.82,  0.85,
    0.88,  0.91,  0.93,  0.94,
    0.95,  0.96,  0.97,  0.975,
    0.98,  0.985, 0.99,  0.992,
    0.994, 0.995, 0.995, 0.994,
    0.993, 0.992, 0.99,  0.988,
    0.985, 0.982, 0.978, 0.975,
    0.97,  0.965, 0.96,  0.95
  };
  
  G4double refracindexrotop[] =
  { 
    1.62, 1.61, 1.60, 1.60, 1.59,
    1.59, 1.59, 1.59, 1.59, 1.59,
    1.59, 1.59, 1.59, 1.59, 1.59,
    1.59, 1.59, 1.59, 1.59, 1.59,
    1.59, 1.59, 1.59, 1.59, 1.59,
    1.59, 1.59, 1.59, 1.59, 1.59,
    1.59, 1.59
  };
  
  G4double absorbindexrotop[] =
  {
   0.005, 0.008, 0.012, 0.018,
   0.025, 0.035, 0.045, 0.055,
   0.065, 0.075, 0.085, 0.095,
   0.105, 0.115, 0.125, 0.135,
   0.145, 0.155, 0.165, 0.175,
   0.185, 0.195, 0.205, 0.215,
   0.225, 0.235, 0.245, 0.255,
   0.265, 0.275, 0.285, 0.300
  };
    
//Propiedades agua
    G4MaterialPropertiesTable *mptWater = new G4MaterialPropertiesTable();
    mptWater->AddProperty("ABSLENGTH", photonEnergy, absorption, 32);
    mptWater->AddProperty("RINDEX", photonEnergy, refractiveindex, 32);
    radiaMat->SetMaterialPropertiesTable(mptWater);
    G4cout << "***** Propiedades del agua..." << G4endl;
    G4cout << "***** Absorción " << absorption[31] << G4endl;

//Propiedades rotoplas
    G4MaterialPropertiesTable *mptRotop = new G4MaterialPropertiesTable();
    mptRotop->AddProperty("ABSLENGTH", photonEnergy, absorbindexrotop, 32);
    mptRotop->AddProperty("RINDEX", photonEnergy, refracindexrotop, 32);
    rotopMat->SetMaterialPropertiesTable(mptRotop);
    G4cout << "***** Propiedades del rotopla..." << G4endl;
    
//Dimensiones
    G4double rminAgua = 0.0*m;
    G4double rmaxAgua = 1.0*m;
    G4double highAlls = 1.0*m;
    
    G4double thicTyvek = 0.02*mm;
    G4double thicAlumi = 0.02*mm;
    G4double thicRotop = 0.5*cm;
    
    G4double rminRotop = rmaxAgua+thicTyvek+thicAlumi;
    G4double rmaxRotop = rmaxAgua+thicTyvek+thicAlumi+thicRotop;
    
    G4double rminAlumi = rmaxAgua+thicTyvek;
    G4double rmaxAlumi = rmaxAgua+thicTyvek+thicAlumi;
    
    G4double rminTyvek = rmaxAgua;
    G4double rmaxTyvek = rmaxAgua+thicTyvek;
    
    G4cout << "Dimensiones...\n" <<
        "rminRotop =" << rminRotop <<"\n" <<
        "rmaxRotop =" << rmaxRotop << G4endl;

//Mundo    
    G4Box *solidWorld = new G4Box("solidWorld", 1.0*m, 1.0*m, 1.0*m);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); 
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, true);
    
//Rotoplas
    G4cout << "***** Creando capa Rotoplas..." << G4endl;
    G4Tubs *solidRotop = new G4Tubs("solidRotop", rminRotop/2., rmaxRotop/2., highAlls/2., 0.,360*deg);
    G4LogicalVolume *logicRotop = new G4LogicalVolume(solidRotop, rotopMat, "logicRotop");
    G4VPhysicalVolume *physRotop = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicRotop, "physRotop", logicWorld, false, 0, true);

//Aluminio capa lateral
    G4cout << "***** Creando capa Aluminio..." << G4endl;
    G4Tubs *solidAlumi = new G4Tubs("solidAlumi", rminAlumi/2., rmaxAlumi/2., highAlls/2., 0.,360*deg);
    G4LogicalVolume *logicAlumi = new G4LogicalVolume(solidAlumi, alumiMat, "logicAlumi");
    G4VPhysicalVolume *physAlumi = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicAlumi, "physAlumi", logicWorld, false, 0, true);
//    new G4LogicalSkinSurface("TyvekAluminio", logicAlumi, alumiSurface);

    
//Tyvek capa lateral
    G4cout << "***** Creando capa Tyvek..." << G4endl;
    G4Tubs *solidTyvek = new G4Tubs("solidTyvek", rminTyvek/2., rmaxTyvek/2., highAlls/2., 0.,360*deg);
    G4LogicalVolume *logicTyvek = new G4LogicalVolume(solidTyvek, worldMat, "logicTyvek");
    G4VPhysicalVolume *physTyvek = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicTyvek, "physTyvek", logicWorld, false, 0, true);
    new G4LogicalSkinSurface("TyvekSurface", logicTyvek, tyvekSurface);
    
//Agua
    G4cout << "***** Creando agua..." << G4endl; 
    G4Tubs *solidRadiator = new G4Tubs("solidRadiator",rminAgua/2., rmaxAgua/2.,highAlls/2.,0.,360*deg);
    G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, radiaMat, "logicRadiator");
    G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicRadiator, "physRadiator", logicWorld, false, 0, true);
  
    

   G4double rminDetec = 0.0*cm;
   G4double rmaxDetec = 20.0*cm;
   G4double highDetec = 0.010*m;
   G4cout << "***** Creando detector..." << G4endl; 
   G4Tubs *solidDetector = new G4Tubs("solidDetector", rminDetec/2., rmaxDetec/2., highDetec, 0.,360*deg);
//   G4Tubs *solidDetector = new G4Tubs("solidDetector", 0.*m, 0.10*m, 0.01*m, 0.,360*deg);
   logicDetector = new G4LogicalVolume(solidDetector, worldMat,"logicDetector");
   G4VPhysicalVolume *physDetector = new G4PVPlacement(0,G4ThreeVector(0, 0,0.51*m),logicDetector,"physDetector", logicWorld, false, 1, true);
   
//highAlls/2.+highDetec/2.

// Atributos visuales (Colores)
    G4VisAttributes* worldVis = new G4VisAttributes(G4Colour(0.9,0.9,0.9,0.1));
    worldVis->SetVisibility(false);                                     //Borra el cubo del mundo
    logicWorld->SetVisAttributes(worldVis);  
    
    G4VisAttributes* waterVis = new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.5));
    logicRadiator->SetVisAttributes(waterVis);                          //Agua (azul) 0.3 
    
    G4VisAttributes* detectorVis = new G4VisAttributes(G4Colour(1.0,0.0,0.0,1.0));
    detectorVis->SetForceSolid(true);
    logicDetector->SetVisAttributes(detectorVis);                       //~PMT (donde va a ir, rojo) 0.8 
    
   
// Impresiones para que no aparezca "variables unused..." 
   G4cout << "**** detecMat created: " << detecMat->GetName() <<G4endl;
   G4cout << "**** tyvekMat created: " << tyvekMat->GetName() <<G4endl;
   G4cout << "**** reflectivity created: " << *reflectivity <<G4endl;
   G4cout << "**** physRotop created: " << physRotop->GetName() <<G4endl;
   G4cout << "**** physAlumi created: " << physAlumi->GetName() <<G4endl;
   G4cout << "**** physTyvek created: " << physTyvek->GetName() <<G4endl;
   G4cout << "**** physRadiator created: " << physRadiator->GetName() <<G4endl;
   G4cout << "**** physDetector created: " << physDetector->GetName() <<G4endl;
   G4cout << "**** logicAlumi: " << logicAlumi->GetName() <<G4endl;
   G4cout << "**** logicRotop: " << logicRotop->GetName() <<G4endl;
   
    
    
    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    logicDetector->SetSensitiveDetector(sensDet);
}

