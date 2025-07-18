// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfoInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameSave.h"
#include "MoviePlayer.h"
#include "Ads/AGAdLibrary.h"
#include "Components/AudioComponent.h"

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

void UGameInfoInstance::SaveUserPreferences(FUserPreferences NewUserPreferences)
{
	if (SG_UserPreferences)
	{
		UserPreferences = NewUserPreferences;
		SG_UserPreferences->UserPreferences = NewUserPreferences;
		UGameplayStatics::SaveGameToSlot(SG_UserPreferences, "UserPreferencesSlot", 0);
	}
}

void UGameInfoInstance::LoadUserPreferences()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("UserPreferencesSlot"), 0))
	{
		SG_UserPreferences = Cast<UGameSave>(UGameplayStatics::LoadGameFromSlot(TEXT("UserPreferencesSlot"), 0));

		if (SG_UserPreferences)
		{
			UserPreferences = SG_UserPreferences->UserPreferences;
			BallType = UserPreferences.DesiredBall;
		}
	}
	else
	{
		SG_UserPreferences = Cast<UGameSave>(
			UGameplayStatics::CreateSaveGameObject(UGameSave::StaticClass()));
	}
}

FUserPreferences UGameInfoInstance::GetUserPreferences() const
{
	return UserPreferences;
}

FUserProgression& UGameInfoInstance::GetUserProgression()
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

void UGameInfoInstance::AssignOnBallSet(const FScriptDelegate& Delegate)
{
	OnBallTypeSet.Add(Delegate);
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
	UserPreferences.DesiredBall = NewBallType;
	OnBallTypeSet.Broadcast();
	SaveUserPreferences(UserPreferences);
}

TArray<FBallsShopStruct> UGameInfoInstance::GetShopStruct() const
{
	return ShopItems;
}

void UGameInfoInstance::Init()
{
	InitializeADUnits();
	FTimerHandle ViewportSizeTimer;
	GetWorld()->GetTimerManager().SetTimer(ViewportSizeTimer, this, &UGameInfoInstance::FetchViewportSize, 0.1f, false);
	LoadHighScore();
	LoadUserProgression();
	LoadUserPreferences();
	GetMoviePlayer()->OnMoviePlaybackFinished().AddUObject(this, &UGameInfoInstance::OnMoviePlaybackFinished);

	MusicAudioComponent = NewObject<UAudioComponent>(this);

	if (MusicAudioComponent)
	{
		MusicAudioComponent->SetSound(GameMusic);
		MusicAudioComponent->bAutoActivate = false;
		MusicAudioComponent->bIsUISound = true;
		MusicAudioComponent->RegisterComponent();
	}
}

void UGameInfoInstance::FetchViewportSize()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
}

void UGameInfoInstance::InitializeADUnits()
{
#if PLATFORM_ANDROID
	BannerADUnitID = TEXT("ca-app-pub-5279001044325653/5632528607");
	InterstitialADUnitID = TEXT("ca-app-pub-5279001044325653/9080819028");
	RewardedADUnitID = TEXT("ca-app-pub-5279001044325653/6337421401");

#elif PLATFORM_IOS
	BannerADUnitID = TEXT("");
	InterstitialADUnitID = TEXT("");
	RewardedADUnitID = TEXT("");

#endif
}

void UGameInfoInstance::OnMoviePlaybackFinished()
{
	bool bMusicMuted = UserPreferences.bIsMusicMuted;

	if (!bMusicMuted)
		PlayBackgroundMusic();

	if (UserPreferences.bNoAds)
		return;

	//banner ad
	BannerAdInterface = UAGAdLibrary::MakeBannerAd(
		BannerADUnitID,
		EAdSizeType::Banner,
		EAdPosition::Bottom
	);

	if (BannerAdInterface)
	{
		BannerAdInterface->LoadAd(true);
	}
}

void UGameInfoInstance::PlayBackgroundMusic()
{
	if (MusicAudioComponent)
	{
		MusicAudioComponent->Play();
	}
}

void UGameInfoInstance::StopBackgroundMusic()
{
	if (MusicAudioComponent)
	{
		MusicAudioComponent->Stop();
	}
}