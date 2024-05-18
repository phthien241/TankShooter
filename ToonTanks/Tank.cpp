// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "HealthComponent.h"

ATank::ATank(){
    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("TankHealth"));

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward",this,&ATank::Move);
    PlayerInputComponent->BindAxis("Turn",this,&ATank::Turn);
    PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&ATank::Fire);
}

void ATank::Tick(float DeltaTime){
    Super::Tick(DeltaTime);

    if(TankPlayerController){
        FHitResult TraceHitResult;
        TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, TraceHitResult);
        //DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint, 25.f, 12, FColor::Red, false, -1.f, 0, 1.f);
        RotateTurret(TraceHitResult.ImpactPoint);
    }
   
}

void ATank::BeginPlay(){
    Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());
}

void ATank::Move(float Value){
    FVector DeltaLocation(0.f);
    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation,true);
}
void ATank::Turn(float Value){
    FRotator DeltaRotation(0.f);
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDestruction() {
    Super::HandleDestruction();
	SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bAlive = false;
}

float ATank::GetCurrentHealth() const {
	return Health->GetHealth();
}



