// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentGameMode.h"
#include "UnlimitedTimeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HOOPSWIPES_API AUnlimitedTimeGameMode : public AParentGameMode
{
	GENERATED_BODY()
	
private:

	AUnlimitedTimeGameMode();

	virtual void BeginPlay() override;

	virtual void OnTriggerOverlap(AActor* OverlappedActor, AActor* OtherActor) override;

	virtual void RestartGame() override;

	virtual void UpdateScore() override;

	int LastMilestoneScore = 0;

	virtual void GrantSecondChance(FRewardItem Reward) override;
};
