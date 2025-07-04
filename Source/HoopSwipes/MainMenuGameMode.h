// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

class UMainMenuWidget;
class IGameInstanceInterface;
class IAGInterstitialAdInterface;

UCLASS()
class HOOPSWIPES_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AMainMenuGameMode();


private:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UMainMenuWidget* MainMenuWidgetInstance = nullptr;

	IGameInstanceInterface* GameInstanceInterface = nullptr;

	void ClearInterstitialTimer();

	FTimerHandle InterstitialTimerHandle;

	UFUNCTION()
	void ShowInterstitialAd();

	void LoadInterstitialAd();

	TScriptInterface<IAGInterstitialAdInterface> InterstitialAdInterface;
};
