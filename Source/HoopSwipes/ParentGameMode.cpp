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

	if (GamePlayWidgetInstace)
	{
		GamePlayWidgetInstace->UpdateScoreUI(CurrentScore);

		if (CurrentScore > HighScore)
		{
			GamePlayWidgetInstace->UpdateHighScoreUI(CurrentScore);
		}

	}
}

void AParentGameMode::RestartGame()
{
	//shared functionality for all game modes, override the rest in each game mode child class

	CurrentScore = 0;
	UpdateScoreMultiplier(true);

	if (GamePlayWidgetInstace)
	{
		GamePlayWidgetInstace->ShowRestartButton(false);
		//GamePlayWidgetInstace->ShowScoreWidget(true);
		GamePlayWidgetInstace->UpdateScoreUI(CurrentScore);
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

void AParentGameMode::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

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
		ABall* DefaultBall = Cast<ABall>(UGameplayStatics::GetActorOfClass(World, ABall::StaticClass()));

		if (DefaultBall)
		{
			OriginalBallTransform = DefaultBall->GetActorTransform();

			if (PoolManager)
			{
				PoolManager->AddBallToArray(DefaultBall);
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
			GamePlayWidgetInstace = CreateWidget<UGamePlayWidget>(World, GamePlayWidgetClass);

			if (GamePlayWidgetInstace)
			{
				GamePlayWidgetInstace->AddToViewport();
				GamePlayWidgetInstace->TotalCoinsWidget->SetGameInstanceInterface(GameInstanceInterface);
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

void AParentGameMode::EndGame()
{
	if (GamePlayWidgetInstace)
	{
		GamePlayWidgetInstace->ShowRestartButton(true);
		//GamePlayWidgetInstace->ShowScoreWidget(false);
	}

	if (CurrentScore > HighScore)
	{
		HighScore = CurrentScore;
		GameInstanceInterface->SaveHighScore(GameModeType, CurrentScore);
	}

	FUserProgression UserProgression = GameInstanceInterface->GetUserProgression();
	GameInstanceInterface->SaveUserProgression(UserProgression);
}

void AParentGameMode::UpdateScoreMultiplier(bool Reset)
{
	//ScoreMultiplier = Reset ? 1 : ScoreMultiplier * 2;
	ScoreMultiplier = Reset ? 1 : 2;

}

void AParentGameMode::AddCoins()
{
	GameInstanceInterface->GetUserProgression().TotalCoins += ScoreMultiplier;
	GamePlayWidgetInstace->TotalCoinsWidget->UpdateCoinsText();
}
