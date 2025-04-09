// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameModeEnum.h"
#include "GameInstanceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

struct FHighScoreData;

class HOOPSWIPES_API IGameInstanceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FVector2D GetViewportSize() const = 0;

	virtual void SaveHighScore(EGameModeType GameModeType, int NewHighScore) = 0;

	virtual void LoadHighScore() = 0;

	virtual FHighScoreData GetHighScoreStruct() const = 0;

};
