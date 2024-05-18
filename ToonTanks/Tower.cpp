// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
    if(InFireRange()){
        RotateTurret(Tank->GetActorLocation());
    }
}

void ATower::BeginPlay(){
    Super::BeginPlay();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0));
    GetWorldTimerManager().SetTimer(FireRateTimeHandle,this,&ATower::CheckFireCondition,FireRate,true);
}

bool ATower::InFireRange(){
    if(Tank){
        float Distance = FVector::Dist(GetActorLocation(),Tank->GetActorLocation());
        if(Distance <= FireRange){
            return true;
        }
    }
    return false;
}

void ATower::CheckFireCondition(){
    if(!Tank) return;
    if(InFireRange() && Tank->bAlive){
        Fire();
    }
    
}

void ATower::HandleDestruction() {
	Super::HandleDestruction();
    Destroy();
}