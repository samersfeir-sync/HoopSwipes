// Fill out your copyright notice in the Description page of Project Settings.


#include "SplashScreenWidget.h"
#include "MediaPlayer.h"
#include "Kismet/GameplayStatics.h"

void USplashScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();
    {
        if (MediaPlayer && MediaSource)
        {
            MediaPlayer->OpenSource(MediaSource);

            if (SplashSound)
            {
                UGameplayStatics::PlaySound2D(this, SplashSound);
            }

            MediaPlayer->OnEndReached.AddDynamic(this, &USplashScreenWidget::OnVideoFinished);
        }
    }
}

void USplashScreenWidget::OnVideoFinished()
{

    FTimerHandle TimerHandle_OpenLevel;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_OpenLevel, this, &USplashScreenWidget::OpenMainMenuMap, 0.5f, false);
}


void USplashScreenWidget::OpenMainMenuMap()
{
    UGameplayStatics::OpenLevel(this, FName("MainMenuMap"));
}
