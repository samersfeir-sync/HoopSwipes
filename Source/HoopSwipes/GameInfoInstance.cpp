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
	if (SG_HighScore)
	{
		switch (GameModeType)
		{
		case EGameModeType::Timed:
			HighScores.LimitedTimeScore = NewHighScore;
			SG_HighScore->HighScores.LimitedTimeScore = NewHighScore;
			break;

		case EGameModeType::Unlimited:
			HighScores.UnlimitedTimeScore = NewHighScore;
			SG_HighScore->HighScores.UnlimitedTimeScore = NewHighScore;
			break;

		default:
			break;
		}

		UGameplayStatics::SaveGameToSlot(SG_HighScore, "HighScoreSlot", 0);
	}
}

void UGameInfoInstance::LoadHighScore()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("HighScoreSlot"), 0))
	{
		SG_HighScore = Cast<UGameSave>(UGameplayStatics::LoadGameFromSlot(TEXT("HighScoreSlot"), 0));

		if (SG_HighScore)
		{
			HighScores = SG_HighScore->HighScores;
		}
	}
	else
	{
		SG_HighScore = Cast<UGameSave>(
			UGameplayStatics::CreateSaveGameObject(UGameSave::StaticClass()));
	}
}

void UGameInfoInstance::SaveUserProgression(FUserProgression NewUserProgression)
{
	if (SG_UserProgression)
	{
		UserProgression = NewUserProgression;
		SG_UserProgression->UserProgression = NewUserProgression;
		UGameplayStatics::SaveGameToSlot(SG_UserProgression, "UserProgressionSlot", 0);
	}
}

void UGameInfoInstance::LoadUserProgression()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("UserProgressionSlot"), 0))
	{
		SG_UserProgression = Cast<UGameSave>(UGameplayStatics::LoadGameFromSlot(TEXT("UserProgressionSlot"), 0));

		if (SG_UserProgression)
		{
			UserProgression = SG_UserProgression->UserProgression;
			UpdateShopItemsStruct();
		}
	}
	else
	{
		SG_UserProgression = Cast<UGameSave>(
			UGameplayStatics::CreateSaveGameObject(UGameSave::StaticClass()));
	}
}

FUserProgression UGameInfoInstance::GetUserProgression() const
{
	return UserProgression;
}

void UGameInfoInstance::UpdateShopItemsStruct()
{
	for (FBallsShopStruct& ShopItem : ShopItems)
	{
		int Index = UserProgression.PurchasedBalls.Find(ShopItem.BallType);

		if (Index != INDEX_NONE)
		{
			ShopItem.IsPurchased = true;
		}
	}
}

FHighScoreData UGameInfoInstance::GetHighScoreStruct() const
{
	return HighScores;
}

EBallType UGameInfoInstance::GetBallType() const
{
	return BallType;
}

void UGameInfoInstance::SetBallType(EBallType NewBallType)
{
	BallType = NewBallType;
}

TArray<FBallsShopStruct> UGameInfoInstance::GetShopStruct() const
{
	return ShopItems;
}

void UGameInfoInstance::Init()
{
	FTimerHandle ViewportSizeTimer;
	GetWorld()->GetTimerManager().SetTimer(ViewportSizeTimer, this, &UGameInfoInstance::FetchViewportSize, 1.0f, false);
	LoadHighScore();
	LoadUserProgression();
}

void UGameInfoInstance::FetchViewportSize()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
}
