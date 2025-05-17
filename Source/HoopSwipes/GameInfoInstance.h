// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceInterface.h"
#include "HighScoreStruct.h"
#include "BallType.h"
#include "BallsShopStruct.h"
#include "UserProgression.h"
#include "GameInfoInstance.generated.h"

class UGameSave;

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

	virtual FUserProgression GetUserProgression() const override;

private:

	virtual void Init() override;

	void FetchViewportSize();

	FVector2D ViewportSize;

	UPROPERTY()
	UGameSave* SG_HighScore = nullptr;

	UPROPERTY()
	UGameSave* SG_UserProgression = nullptr;

	FHighScoreData HighScores;

	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	EBallType BallType = EBallType::Basketball;

	UPROPERTY(EditDefaultsOnly, Category = "User Progression")
	FUserProgression UserProgression;
};
