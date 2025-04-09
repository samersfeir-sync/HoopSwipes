// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BallInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBallInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HOOPSWIPES_API IBallInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FVector GetCurrentBallVelocity() const = 0;

	virtual bool GetLaunchedBoolean() const = 0;

	virtual void SetScoredBoolean(bool Scored) = 0;

	virtual bool GetScoredBoolean() const = 0;

	virtual void DeactivateBall() = 0;

	virtual void SetSwishBoolean(bool Swish) = 0;

	virtual bool GetSwishBoolean() const = 0;
};
