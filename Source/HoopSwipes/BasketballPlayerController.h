// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasketballPlayerController.generated.h"

class IGameModeInterface;

UCLASS()
class HOOPSWIPES_API ABasketballPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ABasketballPlayerController();
	
protected:

	virtual void BeginPlay() override;

private:

	virtual void SetupInputComponent() override;
	void OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location);

	IGameModeInterface* GameModeInterface = nullptr;
};
