// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball.h"
#include "FunctionsLibrary.h"
#include "GameModeInterface.h"
#include "GameInstanceInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABall::ABall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	RootComponent = BallMesh;
	BallMesh->SetUsingAbsoluteScale(true);
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetNotifyRigidBodyCollision(true);
	BallMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	BallMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
}

void ABall::ActivateBall(FTransform ActivateTransform)
{
	SetActorHiddenInGame(false);
	SetActorTransform(ActivateTransform);
	BallMesh->SetSimulatePhysics(true);
}

void ABall::DeactivateBall()
{
	BallMesh->SetSimulatePhysics(false);
	SetActorHiddenInGame(true);
	bLaunched = false;
	bSwish = true;

	if (GameModeInterface)
	{
		GameModeInterface->RemoveActiveBall(this);
	}
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World)
	{
		GameModeInterface = UFunctionsLibrary::GetGameModeInterface(World);
		PlayerController = UFunctionsLibrary::GetPlayerController(World);
		GameInstanceInterface = UFunctionsLibrary::GetGameInstanceInterface(World);
	}

	if (GameModeInterface)
	{
		if (GameModeInterface->GetGameModeType() == EGameModeType::Timed)
		{
			OnBallLaunched.AddDynamic(GameModeInterface, &IGameModeInterface::ActivateNextBall);
		}
	}
	
	if (BallMesh)
	{
		ApplyBallSettings();
		BallMesh->OnInputTouchBegin.AddDynamic(this, &ABall::OnTouchBegin);
		BallMesh->OnInputTouchEnd.AddDynamic(this, &ABall::OnTouchEnd);
	}

}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABall::LaunchBall(FVector2D TouchEndLocation)
{
	FVector2D SwipeDirection = TouchEndLocation - BallInitialLocation;	
	float SwipeStrength = SwipeDirection.Size();

	FVector Impulse;
	Impulse.X = SwipeDirection.Y;
	Impulse.Y = SwipeDirection.X;
	Impulse.Z = FMath::Clamp(SwipeStrength * 1.2f, 0.0f, 1000.0f);
	BallMesh->AddImpulse(FVector(Impulse.GetAbs().X, Impulse.Y, Impulse.GetAbs().Z), NAME_None, true); 

	if (LaunchSound)
	{
		UGameplayStatics::PlaySound2D(this, LaunchSound);
	}

	bLaunched = true;
	OnBallLaunched.Broadcast(false);

	if (GameModeInterface)
	{
		GameModeInterface->AddActiveBall(this);
	}
}

void ABall::OnTouchBegin(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{

	if (!bLaunched)
	{
		if (GameModeInterface)
		{
			GameModeInterface->SetTouchedBall(this);

			if (PlayerController)
			{
				double TouchX, TouchY;
				bool bIsTouching;
				PlayerController->GetInputTouchState(FingerIndex, TouchX, TouchY, bIsTouching);
				BallInitialLocation = FVector2D(TouchX, TouchY);
			}

		}
	}
}

void ABall::OnTouchEnd(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	if (GameModeInterface)
	{
		GameModeInterface->SetTouchedBall(nullptr);
	}
}

void ABall::ApplyBallSettings()
{
	if (GameInstanceInterface)
	{
		BallType = GameInstanceInterface->GetBallType();

		BallSettings = BallTypeToMeshSettings.Find(BallType);

		if (BallSettings)
		{
			if (BallSettings->Mesh)
			{
				BallMesh->SetStaticMesh(BallSettings->Mesh);
				BallMesh->SetWorldScale3D(BallSettings->Scale);
			}
		}
	}
}
