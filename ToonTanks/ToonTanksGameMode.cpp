// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Tank.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksPlayerController.h"
#include "TimerManager.h"

void AToonTanksGameMode::BeginPlay() {
    Super::BeginPlay();
	HandleGameStart();

}

void AToonTanksGameMode::ActorDied(AActor* DeadActor) {
    if (DeadActor == Tank) {
		UE_LOG(LogTemp, Warning, TEXT("Tank died"));
        Tank->HandleDestruction();
        if (ToonTanksPlayerController) {
			ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    }
    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor)) {
        DestroyedTower->HandleDestruction();
        TargetTowers--;

        if (TargetTowers == 0) {
            GameOver(true);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("A Pawn Died"));
}

void AToonTanksGameMode::HandleGameStart() {
    TargetTowers = GetTargetTowersCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    StartGame();
    if (ToonTanksPlayerController) {
		ToonTanksPlayerController->SetPlayerEnabledState(false);
		FTimerHandle PlayerEnableHandle;
		FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
		GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
    }
}

int32 AToonTanksGameMode::GetTargetTowersCount() {
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}