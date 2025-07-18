// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceInterface.h"
#include "HighScoreStruct.h"
#include "BallType.h"
#include "BallsShopStruct.h"
#include "UserProgression.h"
#include "UserPreferences.h"
#include "GameInfoInstance.generated.h"

class UGameSave;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBallTypeSet);

UCLASS()
class HOOPSWIPES_API UGameInfoInstance : public UGameInstance, public IGameInstanceInterface
{
	GENERATED_BODY()

public:

	virtual FVector2D GetViewportSize() const override;

	virtual void SaveHighScore(EGameModeType GameModeType, int NewHighScore) override;

	virtual void LoadHighScore() override;
	
	virtual FHighScoreData GetHighScoreStruct() const override;

	virtual EBallType GetBallType() const override;

	virtual void SetBallType(EBallType NewBallType) override;

	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	TArray<FBallsShopStruct> ShopItems;

	virtual TArray<FBallsShopStruct> GetShopStruct() const override;

	virtual void SaveUserProgression(FUserProgression NewUserProgression) override;

	virtual void LoadUserProgression() override;

	virtual FUserProgression& GetUserProgression() override;

	virtual void UpdateShopItemsStruct() override;

	virtual void AssignOnBallSet(const FScriptDelegate& Delegate) override;

	virtual void SaveUserPreferences(FUserPreferences NewUserPreferences) override;

	virtual void LoadUserPreferences() override;

	virtual FUserPreferences GetUserPreferences() const override;

	virtual FString GetBannerAdUnitID() const override { return BannerADUnitID; }
	virtual FString GetInterstitialAdUnitID() const override { return InterstitialADUnitID; }
	virtual FString GetRewardedAdUnitID() const override { return RewardedADUnitID; }
	virtual const TScriptInterface<IAGBannerAdInterface> GetBannerAdInterface() const override { return BannerAdInterface; }

	virtual void PlayBackgroundMusic() override;
	virtual void StopBackgroundMusic() override;

private:

	virtual void Init() override;

	void FetchViewportSize();

	FVector2D ViewportSize;

	UPROPERTY()
	UGameSave* SG_HighScore = nullptr;

	UPROPERTY()
	UGameSave* SG_UserProgression = nullptr;

	UPROPERTY()
	UGameSave* SG_UserPreferences = nullptr;

	FHighScoreData HighScores;

	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	EBallType BallType = EBallType::Basketball;

	UPROPERTY(EditDefaultsOnly, Category = "User Progression")
	FUserProgression UserProgression;

	FUserPreferences UserPreferences;

	FOnBallTypeSet OnBallTypeSet;

	void InitializeADUnits();

	FString BannerADUnitID;
	FString InterstitialADUnitID;
	FString RewardedADUnitID;

	void OnMoviePlaybackFinished();

	TScriptInterface<IAGBannerAdInterface> BannerAdInterface;

	UPROPERTY()
	UAudioComponent* MusicAudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* GameMusic;
};
