// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfoInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameSave.h"

FVector2D UGameInfoInstance::GetViewportSize() const
{
	return ViewportSize;
}

void UGameInfoInstance::SaveHighScore(EGameModeType GameModeType, int NewHighScore)
{
	if (SaveGame)
	{
		switch (GameModeType)
		{
		case EGameModeType::Timed:
			HighScores.LimitedTimeScore = NewHighScore;
			SaveGame->HighScores.LimitedTimeScore = NewHighScore;
			break;

		case EGameModeType::Unlimited:
			HighScores.UnlimitedTimeScore = NewHighScore;
			SaveGame->HighScores.UnlimitedTimeScore = NewHighScore;
			break;

		default:
			break;
		}

		UGameplayStatics::SaveGameToSlot(SaveGame, "HighScoreSlot", 0);
	}
}

void UGameInfoInstance::LoadHighScore()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("HighScoreSlot"), 0))
	{
		SaveGame = Cast<UGameSave>(UGameplayStatics::LoadGameFromSlot(TEXT("HighScoreSlot"), 0));

		if (SaveGame)
		{
			HighScores = SaveGame->HighScores;
		}
	}
	else
	{
		SaveGame = Cast<UGameSave>(
			UGameplayStatics::CreateSaveGameObject(UGameSave::StaticClass()));
	}
}

FHighScoreData UGameInfoInstance::GetHighScoreStruct() const
{
	return HighScores;
}

void UGameInfoInstance::Init()
{
	FTimerHandle ViewportSizeTimer;
	GetWorld()->GetTimerManager().SetTimer(ViewportSizeTimer, this, &UGameInfoInstance::FetchViewportSize, 1.0f, false);
	LoadHighScore();
}

void UGameInfoInstance::FetchViewportSize()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
}
