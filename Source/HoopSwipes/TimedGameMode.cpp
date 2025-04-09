// Fill out your copyright notice in the Description page of Project Settings.


#include "TimedGameMode.h"
#include "GamePlayWidget.h"
#include "BallInterface.h"
#include "BasketballHoop.h"
#include "Components/WidgetComponent.h"
#include "TimerWidget.h"

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
	GamePlayWidgetInstace->UpdateHighScoreUI(HighScore);
	GamePlayWidgetInstace->ShowTargetScoreUI(true);
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

	ScoreTarget = 10;
	TotalSeconds = 20;
	GamePlayWidgetInstace->UpdateTargetScoreUI(ScoreTarget);
	TimerWidget->UpdateTime(TotalSeconds);
	PlayerController->bEnableTouchEvents = true;
}

void ATimedGameMode::ReduceGameTime()
{
	--TotalSeconds;
	TimerWidget->UpdateTime(TotalSeconds);

	if (TotalSeconds <= 0)
	{
		EndGame();
	}
}

void ATimedGameMode::EndGame()
{
	Super::EndGame();

	PlayerController->bEnableTouchEvents = false;

}

void ATimedGameMode::UpdateScore()
{
	Super::UpdateScore();

	if (CurrentScore >= ScoreTarget)
	{
		ScoreTarget = ScoreTarget + (10 * TargetMultiplier);
		TargetMultiplier++;
		GamePlayWidgetInstace->UpdateTargetScoreUI(ScoreTarget);
		TotalSeconds += 15;
	}
}
