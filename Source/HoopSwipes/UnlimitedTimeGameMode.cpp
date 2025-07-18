// Fill out your copyright notice in the Description page of Project Settings.


#include "UnlimitedTimeGameMode.h"
#include "BasketballHoop.h"
#include "BallInterface.h"
#include "GameInstanceInterface.h"
#include "GamePlayWidget.h"
#include "Ground.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/AGRewardedAdInterface.h"

AUnlimitedTimeGameMode::AUnlimitedTimeGameMode()
{
	GameModeType = EGameModeType::Unlimited;
}

void AUnlimitedTimeGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (BasketballHoop)
	{
		BasketballHoop->OnScored.AddDynamic(this, &AParentGameMode::ActivateNextBall);
	}

	AGround* Ground = Cast<AGround>(UGameplayStatics::GetActorOfClass(World, AGround::StaticClass()));

	if (Ground)
	{
		Ground->OnBallHitGround.AddDynamic(this, &AParentGameMode::EndGame);
	}

	HighScore = HighScoresData.UnlimitedTimeScore;
	GamePlayWidgetInstance->UpdateHighScoreUI(HighScore);
}

void AUnlimitedTimeGameMode::OnTriggerOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnTriggerOverlap(OverlappedActor, OtherActor);

	if (BallInterface)
	{
		if (!BallInterface->GetScoredBoolean())
		{
			EndGame();
		}

		BallInterface->SetScoredBoolean(false);
	}
}

void AUnlimitedTimeGameMode::RestartGame()
{
	Super::RestartGame();

	ActivateNextBall(false);
	LastMilestoneScore = 0;

	if (BasketballHoop)
	{
		BasketballHoop->DeactivateMovement();
		BasketballHoop->SetActorTransform(OriginalHoopTransform);
		BasketballHoop->ResetMovementSpeed();
	}
}

void AUnlimitedTimeGameMode::UpdateScore()
{
	Super::UpdateScore();

	if (CurrentScore / 5 > LastMilestoneScore / 5)
	{
		LastMilestoneScore = CurrentScore;

		if (!BasketballHoop->IsMovementActive())
		{
			BasketballHoop->ActivateMovement();
		}
		else
		{
			BasketballHoop->IncreaseSpeed(10);
		}
	}
}

void AUnlimitedTimeGameMode::GrantSecondChance(FRewardItem Reward)
{
	Super::GrantSecondChance(Reward);
	
	ActivateNextBall(true);
}
