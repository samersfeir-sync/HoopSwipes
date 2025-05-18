// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallType.h"
#include "UserPreferences.generated.h"

USTRUCT(BlueprintType)
struct FUserPreferences
{
    GENERATED_BODY()

    FUserPreferences()
        : DesiredBall(EBallType::Basketball)
    {
    }

    UPROPERTY()
    EBallType DesiredBall;

};