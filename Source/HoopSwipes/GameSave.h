// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HighScoreStruct.h"
#include "UserProgression.h"
#include "GameSave.generated.h"

/**
 * 
 */
UCLASS()
class HOOPSWIPES_API UGameSave : public USaveGame
{
	GENERATED_BODY()


public:

	UPROPERTY()
	FHighScoreData HighScores;

	UPROPERTY()
	FUserProgression UserProgression;
};
