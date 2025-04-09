// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolManager.h"
#include "Kismet/GameplayStatics.h"
#include "Ball.h"

// Sets default values
APoolManager::APoolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APoolManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APoolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ABall* APoolManager::GetBallFromPool(FTransform BallTransform)
{
	if (BallsArray.IsEmpty())
	{
		return SpawnNewBall(BallTransform);
	}

	else //array not empty
	{
		for (ABall* Ball : BallsArray)
		{
			if (!Ball->bLaunched)
			{
				return Ball;
			}
		}
		return SpawnNewBall(BallTransform);
	}
}

void APoolManager::AddBallToArray(ABall* NewBall)
{
	BallsArray.Add(NewBall);
}

ABall* APoolManager::SpawnNewBall(FTransform BallTransform)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform AdjustedTransform;
	AdjustedTransform.SetLocation(BallTransform.GetLocation());
	AdjustedTransform.SetRotation(BallTransform.GetRotation());
	AdjustedTransform.SetScale3D(FVector(1.0f));

	ABall* NewBall = World->SpawnActor<ABall>(BallClass, AdjustedTransform, SpawnParams);

	if (NewBall)
	{
		AddBallToArray(NewBall);
	}

	return NewBall;
}

