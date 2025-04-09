// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolManager.generated.h"

class ABall;

UCLASS()
class HOOPSWIPES_API APoolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ABall* GetBallFromPool(FTransform BallTransform);

	void AddBallToArray(ABall* NewBall);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	TSubclassOf<ABall> BallClass;

	TArray<ABall*> BallsArray;

	ABall* SpawnNewBall(FTransform BallTransform);
};
