
// Geant4 Libraries
//
#include "Randomize.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"


// Local Libraries
//
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), 
  enerPart(1.),
  fParticleGun(0),
  parPosDir()
{
  //G4cout << "...PrimaryGeneratorAction..." << G4endl;

  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  //create a messenger for this class
  fGunMessenger = new PrimaryGeneratorMessenger(this);

  //default kinematic
  //Para seleccionar una única partícula
  /*G4ParticleTable* particleTable 
   = G4ParticleTable::GetParticleTable();

  G4ParticleDefinition* particle 
   = particleTable->FindParticle("mu-");

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleTime(0.0*ns);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*cm, 20.0*cm, 61.*cm));// 
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.)); //VEMz 120 cm
  //fParticleGun->SetParticlePosition(G4ThreeVector(0.*cm, 60.0*cm, 0.*cm));// 
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., -1., 0.)); //VEMy 120 cm
  //fParticleGun->SetParticlePosition(G4ThreeVector(60.0*cm, 60.0*cm, 60.*cm));//   
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(-0.577, -0.577, -0.577)); //Diagonal 
  //fParticleGun->SetParticleEnergy(enerPart);//500.0*keV);*/
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	
	//Para incluir el flujo de CORSIKA
		
	parPosDir.primaryPosition();
	parPosDir.primaryMomento();

	G4ThreeVector position = parPosDir.particlePosition;
	G4ThreeVector direction = parPosDir.particleDirection;
	//G4double totalMomentum = parPosDir.pTotal;
	G4String parid = parPosDir.parId;

	G4ParticleTable* particleTable 
		= G4ParticleTable::GetParticleTable();
	
	G4ParticleDefinition* particle 
		= particleTable->FindParticle(parid);


	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticlePosition(position);
	fParticleGun->SetParticleMomentumDirection(direction);
	//fParticleGun->SetParticleMomentum(direction);

	fParticleGun->GeneratePrimaryVertex(anEvent);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetOptPhotonPolar()
{
 G4double angle = G4UniformRand() * 360.0*deg;
 SetOptPhotonPolar(angle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
{
 if (fParticleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton")
   {
     /*G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
               "the particleGun is not an opticalphoton" << G4endl;*/
     return;
   }

 G4ThreeVector normal (1., 0., 0.);
 G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
 G4ThreeVector product = normal.cross(kphoton);
 G4double modul2       = product*product;
 
 G4ThreeVector e_perpend (0., 0., 1.);
 if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
 G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
 
 G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
 fParticleGun->SetParticlePolarization(polar);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
