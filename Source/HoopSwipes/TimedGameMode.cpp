// Fill out your copyright notice in the Description page of Project Settings.


#include "TimedGameMode.h"
#include "GamePlayWidget.h"
#include "BallInterface.h"
#include "BasketballHoop.h"
#include "Components/WidgetComponent.h"
#include "TimerWidget.h"
#include "Interface/AGRewardedAdInterface.h"

void ATimedGameMode::AddActiveBall(ABall* ActiveBall)
{
	ActiveBalls.AddUnique(ActiveBall);
}

void ATimedGameMode::RemoveActiveBall(ABall* ActiveBall)
{
	if (ActiveBalls.Contains(ActiveBall))
	{
		ActiveBalls.Remove(ActiveBall);
	}

	if (bWaitingForBalls && ActiveBalls.IsEmpty())
	{
		if (IsTargetScoreReached(TempScoreTarget))
		{
			World->GetTimerManager().UnPauseTimer(TimerHandle);
			PlayerController->bEnableTouchEvents = true;
			bWaitingForBalls = false;
		}
		else 
			EndGame();
	}
}

ATimedGameMode::ATimedGameMode()
{
	GameModeType = EGameModeType::Timed;
}

void ATimedGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (BasketballHoop && BasketballHoop->TimeWidgetComponent)
	{
		UUserWidget* RawWidget = BasketballHoop->TimeWidgetComponent->GetUserWidgetObject();

		TimerWidget = Cast<UTimerWidget>(RawWidget);

		if (TimerWidget)
		{
			BasketballHoop->TimeWidgetComponent->SetHiddenInGame(false);
			TimerWidget->UpdateTime(TotalSeconds);

			World->GetTimerManager().SetTimer(TimerHandle, this, &ATimedGameMode::ReduceGameTime, 1.0f, true);
		}
	}

	HighScore = HighScoresData.LimitedTimeScore;
	GamePlayWidgetInstance->UpdateHighScoreUI(HighScore);
	GamePlayWidgetInstance->ShowTargetScoreUI(true);
}

void ATimedGameMode::OnTriggerOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnTriggerOverlap(OverlappedActor, OtherActor);

	if (BallInterface)
	{
		BallInterface->SetScoredBoolean(false);
	}
}


void ATimedGameMode::RestartGame()
{
	Super::RestartGame();

	bWaitingForBalls = false;
	ActiveBalls.Empty();
	TempScoreTarget = 0;
	ScoreTarget = 10;
	TotalSeconds = 20;
	TargetMultiplier = 2;
	GamePlayWidgetInstance->UpdateTargetScoreUI(ScoreTarget);
	TimerWidget->UpdateTime(TotalSeconds);
	PlayerController->bEnableTouchEvents = true;
	World->GetTimerManager().SetTimer(TimerHandle, this, &ATimedGameMode::ReduceGameTime, 1.0f, true);
}

void ATimedGameMode::ReduceGameTime()
{
	--TotalSeconds;
	TimerWidget->UpdateTime(TotalSeconds);

	if (TotalSeconds <= 0)
	{
		World->GetTimerManager().PauseTimer(TimerHandle);
		PlayerController->bEnableTouchEvents = false;

		if (ActiveBalls.Num() == 0)
		{
			if (bCanWatchAd)
			{
				bCanWatchAd = false;
				ShowSecondChanceWidget();
			}

			else
			{
				EndGame();
			}
		}

		else
		{
			bWaitingForBalls = true;
			TempScoreTarget = ScoreTarget;
		}
	}
}

void ATimedGameMode::EndGame()
{
	Super::EndGame();

	World->GetTimerManager().ClearTimer(TimerHandle);
}

void ATimedGameMode::UpdateScore()
{
	Super::UpdateScore();

	if (IsTargetScoreReached(ScoreTarget))
	{
		ScoreTarget = ScoreTarget + (10 * TargetMultiplier);
		TargetMultiplier++;
		GamePlayWidgetInstance->UpdateTargetScoreUI(ScoreTarget);
		TotalSeconds += 15;
		TimerWidget->UpdateTime(TotalSeconds);
	}
}

bool ATimedGameMode::IsTargetScoreReached(int TargetScore) const
{
	return CurrentScore >= TargetScore;
}

void ATimedGameMode::GrantSecondChance(FRewardItem Reward)
{
	Super::GrantSecondChance(Reward);

	PlayerController->bEnableTouchEvents = true;
	TotalSeconds = 15;
	World->GetTimerManager().UnPauseTimer(TimerHandle);	
}
