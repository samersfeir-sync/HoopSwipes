// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RestartButtonWidget.generated.h"

class UButton;
class IGameModeInterface;

UCLASS()
class HOOPSWIPES_API URestartButtonWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	IGameModeInterface* GameModeInterface = nullptr;
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;

	UFUNCTION()
	void RestartGame();
	
};
