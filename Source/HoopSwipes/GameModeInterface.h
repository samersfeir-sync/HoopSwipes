// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameModeEnum.h"
#include "GameModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

class ABall;
class ACameraActor;

class HOOPSWIPES_API IGameModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetTouchedBall(ABall* NewTouchedBall) = 0;
	virtual ABall* GetTouchedBall() const = 0;
	virtual void ActivateNextBall(bool RandomLocation) = 0;
	virtual void UpdateScore() = 0;
	virtual void RestartGame() = 0;
	virtual ACameraActor* GetCameraActor() const = 0;
	virtual void AssignCameraFetchedDelegate(const FScriptDelegate& Delegate) = 0;
	virtual EGameModeType GetGameModeType() const = 0;
	virtual void UpdateScoreMultiplier(bool Reset) = 0;
	virtual void AddCoins() = 0;
	virtual void AddActiveBall(ABall* ActiveBall) = 0;
	virtual void RemoveActiveBall(ABall* ActiveBall) = 0;
};
