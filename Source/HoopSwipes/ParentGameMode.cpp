// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "FunctionsLibrary.h"
#include "PoolManager.h"
#include "Ball.h"
#include "Engine/TriggerVolume.h"
#include "BallInterface.h"
#include "BasketballHoop.h"
#include "BasketballPlayerController.h"
#include "GamePlayWidget.h"
#include "GameInstanceInterface.h"
#include "TotalCoinsWidget.h"
#include "Engine/TargetPoint.h"
#include "Interface/AGRewardedAdInterface.h"
#include "Ads/AGAdLibrary.h"
#include "SecondChanceWidget.h"
#include "TotalGemsWidget.h"

AParentGameMode::AParentGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ABasketballPlayerController::StaticClass();
}

void AParentGameMode::UpdateScore()
{
	CurrentScore += ScoreMultiplier;

	if (ScoreSound)
	{
		UGameplayStatics::PlaySound2D(this, ScoreSound);
	}

	if (GamePlayWidgetInstance)
	{
		GamePlayWidgetInstance->UpdateScoreUI(CurrentScore);

		if (CurrentScore > HighScore)
		{
			GamePlayWidgetInstance->UpdateHighScoreUI(CurrentScore);
		}

	}
}

void AParentGameMode::RestartGame()
{
	//shared functionality for all game modes, override the rest in each game mode child class

	CurrentScore = 0;
	UpdateScoreMultiplier(true);
	CollectedCoins = 0;
	bCanWatchAd = true;
	RetryCount = 0;
	GemsNeededForSecondChance = BaseGemCost;
	bSkipped = false;

	if (GamePlayWidgetInstance)
	{
		GamePlayWidgetInstance->ShowRestartButton(false);
		GamePlayWidgetInstance->EnablePauseButton(true);
		GamePlayWidgetInstance->ShowHomeButton(false);
		GamePlayWidgetInstance->ShowSettingsButton(false);
		//GamePlayWidgetInstance->ShowScoreWidget(true);
		GamePlayWidgetInstance->UpdateScoreUI(CurrentScore);
	}
}

ACameraActor* AParentGameMode::GetCameraActor() const
{
	return Camera;
}

void AParentGameMode::AssignCameraFetchedDelegate(const FScriptDelegate& Delegate)
{
	CameraFetchedDelegate.Add(Delegate);
}

void AParentGameMode::ActivateNextBall(bool RandomLocation)
{
	if (PoolManager)
	{
		FTransform SpawnTransform = OriginalBallTransform;

		FRotator RandomRotation = FRotator(
			FMath::FRandRange(0.f, 360.f),
			FMath::FRandRange(0.f, 360.f),
			FMath::FRandRange(0.f, 360.f)
		);

		SpawnTransform.SetRotation(RandomRotation.Quaternion());

		if (RandomLocation)
		{
			int OffsetDirection = FMath::RandRange(-1, 1);
			float OffsetAmount = NewBallSpawnOffset;

			FVector Location = SpawnTransform.GetLocation();
			Location.Y += OffsetDirection * OffsetAmount;
			SpawnTransform.SetLocation(Location);
		}

		PoolManager->GetBallFromPool(SpawnTransform)->ActivateBall(SpawnTransform);
	}
}

void AParentGameMode::IncrementGemsNeededForSecondChance()
{
	GemsNeededForSecondChance = FMath::CeilToInt(BaseGemCost * FMath::Pow(GrowthRate, RetryCount));
}

void AParentGameMode::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	GemsNeededForSecondChance = BaseGemCost;

	if (World)
	{
		Camera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(World, ACameraActor::StaticClass()));

		if (Camera)
		{
			PlayerController = UFunctionsLibrary::GetPlayerController(GetWorld());

			if (PlayerController)
			{
				PlayerController->SetViewTargetWithBlend(Camera);
				CameraFetchedDelegate.Broadcast();
			}
		}

		PoolManager = Cast<APoolManager>(UGameplayStatics::GetActorOfClass(World, APoolManager::StaticClass()));
		ATargetPoint* BallTargetPoint = Cast<ATargetPoint>(UGameplayStatics::GetActorOfClass(World, ATargetPoint::StaticClass()));

		if (BallTargetPoint)
		{
			if (PoolManager)
			{
				FTransform TargetPointTransform = BallTargetPoint->GetActorTransform();
				ABall* DefaultBall = PoolManager->GetBallFromPool(TargetPointTransform);

				if (DefaultBall)
				{
					OriginalBallTransform.SetLocation(TargetPointTransform.GetLocation());
					OriginalBallTransform.SetRotation(TargetPointTransform.GetRotation());
					OriginalBallTransform.SetScale3D(DefaultBall->BallSettings->Scale);
				}
			}
		}

		ATriggerVolume* BallDeactivationBox = Cast<ATriggerVolume>(UGameplayStatics::GetActorOfClass(World, ATriggerVolume::StaticClass()));

		if (BallDeactivationBox)
		{
			BallDeactivationBox->OnActorBeginOverlap.AddDynamic(this, &AParentGameMode::OnTriggerOverlap);
		}

		BasketballHoop = Cast<ABasketballHoop>(UGameplayStatics::GetActorOfClass(World, ABasketballHoop::StaticClass()));

		if (BasketballHoop)
		{
			OriginalHoopTransform = BasketballHoop->GetActorTransform();
		}

		GameInstanceInterface = UFunctionsLibrary::GetGameInstanceInterface(World);

		if (GameInstanceInterface)
		{
			HighScoresData = GameInstanceInterface->GetHighScoreStruct();
		}
		
		if (GamePlayWidgetClass)
		{
			GamePlayWidgetInstance = CreateWidget<UGamePlayWidget>(World, GamePlayWidgetClass);

			if (GamePlayWidgetInstance)
			{
				GamePlayWidgetInstance->AddToViewport();
				GamePlayWidgetInstance->TotalCoinsWidget->SetGameInstanceInterface(GameInstanceInterface);
				GamePlayWidgetInstance->TotalGemsWidget->SetGameInstanceInterface(GameInstanceInterface);
				SecondChanceWidgetInstance = GamePlayWidgetInstance->SecondChanceWidget;
			}
		}
	}
}

void AParentGameMode::OnTriggerOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UBallInterface::StaticClass()))
		{
			BallInterface = Cast<IBallInterface>(OtherActor);

			if (BallInterface)
			{
				if (BallInterface->GetLaunchedBoolean())
				{
					BallInterface->DeactivateBall();
				}
			}
		}
	}
}

void AParentGameMode::ShowRewardedAdIfAvailable()
{
	FOnRewardedAdUserEarhedRewardDelegate Delegate;
	Delegate.BindDynamic(this, &AParentGameMode::GrantSecondChance);
	RewardedAdInterface->BindEventToOnUserEarnedReward(Delegate);
	RewardedAdInterface->Show();
	GamePlayWidgetInstance->ShowSecondChanceWidget(false);
	bCanWatchAd = false;
}

void AParentGameMode::GrantSecondChance(FRewardItem Reward)
{
	//override in child game mode class
}

int32 AParentGameMode::GetTotalGems() const
{
	FUserProgression UserProgression = GameInstanceInterface->GetUserProgression();
	return UserProgression.TotalGems;
}

void AParentGameMode::LoadRewardedAd()
{
	RewardedAdInterface = UAGAdLibrary::MakeRewardedAd(
		GameInstanceInterface->GetRewardedAdUnitID());

	if (RewardedAdInterface)
	{
		RewardedAdInterface->LoadAd();

		FOnRewardedAdLoadedDelegate Delegate;
		Delegate.BindDynamic(this, &AParentGameMode::ShowRewardedAdIfAvailable);
		RewardedAdInterface->BindEventToOnAdLoaded(Delegate);

		FOnRewardedAdFailedToLoadDelegate FailedToLoadDelegate;
		FailedToLoadDelegate.BindDynamic(SecondChanceWidgetInstance, &USecondChanceWidget::RewardAdFailed);
		RewardedAdInterface->BindEventToOnAdFailedToLoad(FailedToLoadDelegate);

		FOnRewardedAdFailedToShowDelegate FailedToShowDelegate;
		FailedToShowDelegate.BindDynamic(SecondChanceWidgetInstance, &USecondChanceWidget::RewardAdFailed);
		RewardedAdInterface->BindEventToOnAdFailedToShow(FailedToShowDelegate);
	}
}

void AParentGameMode::EndGame()
{
	if ((bCanWatchAd || GetTotalGems() >= GetGemsNeededForSecondChance()) && !bSkipped)
	{
		ShowSecondChanceWidget();
	}

	else
	{
		if (GamePlayWidgetInstance)
		{
			GamePlayWidgetInstance->EnablePauseButton(false);
			GamePlayWidgetInstance->ShowRestartButton(true);
			GamePlayWidgetInstance->ShowHomeButton(true);
			GamePlayWidgetInstance->ShowSettingsButton(true);
			//GamePlayWidgetInstance->ShowScoreWidget(false);
		}

		if (CurrentScore > HighScore)
		{
			HighScore = CurrentScore;
			GameInstanceInterface->SaveHighScore(GameModeType, CurrentScore);
		}

		FUserProgression UserProgression = GameInstanceInterface->GetUserProgression();
		UserProgression.TotalCoins += CollectedCoins;
		GameInstanceInterface->SaveUserProgression(UserProgression);
	}
}

void AParentGameMode::UpdateScoreMultiplier(bool Reset)
{
	//ScoreMultiplier = Reset ? 1 : ScoreMultiplier * 2;
	ScoreMultiplier = Reset ? 1 : 2;

}

void AParentGameMode::AddCoins(int32 CoinsAmount)
{
	CollectedCoins += CoinsAmount;
	int CoinsToDisplay = GameInstanceInterface->GetUserProgression().TotalCoins + CollectedCoins;
	GamePlayWidgetInstance->TotalCoinsWidget->UpdateCoinsText(CoinsToDisplay);
}

void AParentGameMode::ShowSecondChanceWidget()
{
	if (SecondChanceWidgetInstance)
	{
		GamePlayWidgetInstance->ShowSecondChanceWidget(true);
	}
}