// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeInterface.h"
#include "GameModeEnum.h"
#include "HighScoreStruct.h"
#include "ParentGameMode.generated.h"

class ABall;
class APoolManager;
class ABasketballHoop;
class IBallInterface;
class UGamePlayWidget;
class ACameraActor;
class IGameInstanceInterface;
class IAGRewardedAdInterface;
class USecondChanceWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraFetched);

UCLASS()
class HOOPSWIPES_API AParentGameMode : public AGameModeBase, public IGameModeInterface
{
	GENERATED_BODY()

public:

	AParentGameMode();

	virtual ABall* GetTouchedBall() const override
	{
		return TouchedBall;
	}

	virtual void SetTouchedBall(ABall* NewTouchedBall) override
	{
		TouchedBall = NewTouchedBall;
	}

	virtual EGameModeType GetGameModeType() const override
	{
		return GameModeType;
	}

	virtual void UpdateScore() override;

	virtual void RestartGame() override;

	virtual ACameraActor* GetCameraActor() const override;

	virtual void AssignCameraFetchedDelegate(const FScriptDelegate& Delegate) override;

	UFUNCTION()
	virtual void ActivateNextBall(bool RandomLocation) override;

	UFUNCTION()
	virtual void EndGame() override;

	virtual void UpdateScoreMultiplier(bool Reset) override;

	virtual void AddCoins(int32 CoinsAmount) override;

	virtual void LoadRewardedAd() override;

	virtual FOnCoinAdded& GetOnCoinAddedDelegate() override { return OnCoinAddedDelegate; }

protected:

	virtual void BeginPlay() override;
	
	APlayerController* PlayerController = nullptr;

	ABall* TouchedBall = nullptr;

	APoolManager* PoolManager = nullptr;

	FTransform OriginalBallTransform = FTransform::Identity;

	int ScoreMultiplier = 1;
	int CurrentScore = 0;

	UFUNCTION()
	virtual void OnTriggerOverlap(AActor* OverlappedActor, AActor* OtherActor);

	ABasketballHoop* BasketballHoop = nullptr;

	IBallInterface* BallInterface = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UGamePlayWidget> GamePlayWidgetClass;

	UGamePlayWidget* GamePlayWidgetInstance = nullptr;

	EGameModeType GameModeType;

	IGameInstanceInterface* GameInstanceInterface = nullptr;

	int HighScore = 0;

	FHighScoreData HighScoresData;

	UWorld* World = nullptr;

	FTransform OriginalHoopTransform = FTransform::Identity;

	virtual void AddActiveBall(ABall* ActiveBall) override
	{
		//override in timed game mode 
	}

	virtual void RemoveActiveBall(ABall* ActiveBall) override
	{
		//override in timed game mode 
	}

	UFUNCTION()
	virtual void GrantSecondChance(FRewardItem Reward);

	bool bCanWatchAd = true;

	void ShowSecondChanceWidget();

	FOnCoinAdded OnCoinAddedDelegate;

private:

	ACameraActor* Camera = nullptr;
	FOnCameraFetched CameraFetchedDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
	float NewBallSpawnOffset = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* ScoreSound;

	int CollectedCoins = 0;

	UPROPERTY()
	USecondChanceWidget* SecondChanceWidgetInstance;

	TScriptInterface< IAGRewardedAdInterface> RewardedAdInterface;

	UFUNCTION()
	void ShowRewardedAdIfAvailable();
};
