// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget){
	/*FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LooKAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	TurretMesh->SetWorldRotation(LooKAtRotation);*/
	FVector ToTarget = LookAtTarget - ProjectileSpawnPoint->GetComponentLocation();
	FRotator LookAtRotation = ToTarget.Rotation();
	float MinDistance = 175.f;
	if (ToTarget.Size() < MinDistance)	return;
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();

	FRotator NewRotation = FRotator(LookAtRotation.Pitch, LookAtRotation.Yaw, CurrentRotation.Roll);
	TurretMesh->SetWorldRotation(NewRotation);
}

void ABasePawn::Fire(){
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,Location,Rotation);
	
	if(Projectile) Projectile->SetOwner(this);
}

void ABasePawn::HandleDestruction() {
	if (DeathParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation(), GetActorRotation());
	}
	if (DeathSound) {
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
	if (DeathCameraShakeClass) {
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}