// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentGameMode.h"
#include "TimedGameMode.generated.h"

class UTimerWidget;

UCLASS()
class HOOPSWIPES_API ATimedGameMode : public AParentGameMode
{
	GENERATED_BODY()
	
private:

	ATimedGameMode();

	virtual void BeginPlay() override;

	virtual void OnTriggerOverlap(AActor* OverlappedActor, AActor* OtherActor) override;

	virtual void RestartGame() override;

	int TotalSeconds = 20;

	int ScoreTarget = 10;

	FTimerHandle TimerHandle;

	void ReduceGameTime();

	UTimerWidget* TimerWidget = nullptr;

	virtual void EndGame() override;

	virtual void UpdateScore() override;

	float TargetMultiplier = 2;
};
