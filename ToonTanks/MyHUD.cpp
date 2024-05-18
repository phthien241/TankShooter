// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Blueprint/UserWidget.h"

void AMyHUD::BeginPlay() {
    Super::BeginPlay();

    if (HealthWidgetClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HealthWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}