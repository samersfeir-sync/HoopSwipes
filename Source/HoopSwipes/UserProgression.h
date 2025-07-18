// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallType.h"
#include "UserProgression.generated.h"

USTRUCT(BlueprintType)
struct FUserProgression
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    TArray<EBallType> PurchasedBalls;

    UPROPERTY(EditDefaultsOnly)
    int TotalCoins;

    UPROPERTY(EditDefaultsOnly)
    int TotalGems;

    FUserProgression()
        : TotalCoins(0),
        TotalGems(0)
    {
        PurchasedBalls.Add(EBallType::Basketball);
    }
};