// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "MainMenuWidget.h"
#include "Ads/AGAdLibrary.h"
#include "Interface/AGBannerAdInterface.h"
#include "Interface/AGInterstitialAdInterface.h"
#include "GameInstanceInterface.h"
#include "FunctionsLibrary.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
	GameStateClass = nullptr;
	PlayerStateClass = nullptr;
	SpectatorClass = nullptr;
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);

		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
			MainMenuWidgetInstance->OnGameStarted.AddUObject(this, &AMainMenuGameMode::ClearInterstitialTimer);
		}

		GameInstanceInterface = UFunctionsLibrary::GetGameInstanceInterface(this);

		if (GameInstanceInterface)
		{
			//banner ad
			if (TScriptInterface<IAGBannerAdInterface> BannerAdInterface = UAGAdLibrary::MakeBannerAd(
				GameInstanceInterface->GetBannerAdUnitID(),
				EAdSizeType::Banner,
				EAdPosition::Bottom
			))
			{
				BannerAdInterface->LoadAd(true);
			}

			//interstitial ad

			GetWorld()->GetTimerManager().SetTimer(InterstitialTimerHandle, this, &AMainMenuGameMode::LoadInterstitialAd, 15.0f, true);
		}
	}
}

void AMainMenuGameMode::ClearInterstitialTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(InterstitialTimerHandle);
}

void AMainMenuGameMode::LoadInterstitialAd()
{
	InterstitialAdInterface = UAGAdLibrary::MakeInterstitialAd(
		GameInstanceInterface->GetInterstitialAdUnitID()
	);

	if (InterstitialAdInterface)
	{
		InterstitialAdInterface->LoadAd();
		FOnInterstitialAdLoadedDelegate OnAdLoadedDelegate;
		OnAdLoadedDelegate.BindDynamic(this, &AMainMenuGameMode::ShowInterstitialAd);
		InterstitialAdInterface->BindEventToOnAdLoaded(OnAdLoadedDelegate);
	}
}

void AMainMenuGameMode::ShowInterstitialAd()
{
	InterstitialAdInterface->Show();
}