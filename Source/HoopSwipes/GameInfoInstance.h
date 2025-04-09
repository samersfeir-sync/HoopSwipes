// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceInterface.h"
#include "HighScoreStruct.h"
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

private:

	virtual void Init() override;

	void FetchViewportSize();

	FVector2D ViewportSize;

	UPROPERTY()
	UGameSave* SaveGame = nullptr;

	FHighScoreData HighScores;
};
