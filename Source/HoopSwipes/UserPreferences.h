// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallType.h"
#include "UserPreferences.generated.h"

USTRUCT(BlueprintType)
struct FUserPreferences
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    EBallType DesiredBall = EBallType::Basketball;

    UPROPERTY(SaveGame)
	bool bNoAds = false;

};