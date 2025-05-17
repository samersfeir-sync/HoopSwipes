// Fill out your copyright notice in the Description page of Project Settings.


#include "BasketballHoop.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BallInterface.h"
#include "GameModeInterface.h"
#include "FunctionsLibrary.h"
#include "Components/WidgetComponent.h"

// Sets default values
ABasketballHoop::ABasketballHoop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HoopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoopMesh"));
	RootComponent = HoopMesh;
	HoopMesh->SetMobility(EComponentMobility::Movable);
	HoopMesh->SetNotifyRigidBodyCollision(true);

	ScoreCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScoreCollision"));
	ScoreCollision->SetupAttachment(HoopMesh);
	ScoreCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	ScoreCollision->OnComponentBeginOverlap.AddDynamic(this, &ABasketballHoop::OnScoreOverlap);

	LeftEdgeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftEdgeCollision"));
	LeftEdgeCollision->SetupAttachment(HoopMesh);

	RightEdgeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightEdgeCollision"));
	RightEdgeCollision->SetupAttachment(HoopMesh);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->InitialSpeed = MinMovementSpeed;
	MovementComponent->MaxSpeed = MaxMovementSpeed;
	MovementComponent->Velocity = FVector(1.0f, 0.0f, 0.0f);
	MovementComponent->bAutoActivate = false;

	TimeWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TimeWidget"));
	TimeWidgetComponent->SetupAttachment(RootComponent);
	TimeWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	TimeWidgetComponent->SetDrawAtDesiredSize(true);
	TimeWidgetComponent->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void ABasketballHoop::BeginPlay()
{
	Super::BeginPlay();
	
	GameModeInterface = UFunctionsLibrary::GetGameModeInterface(GetWorld());

	if (HoopMesh)
	{
		HoopMesh->OnComponentHit.AddDynamic(this, &ABasketballHoop::OnHoopHit);
	}
}

// Called every frame
void ABasketballHoop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasketballHoop::ChangeHoopVelocity(FVector NewVelocity)
{
	if (MovementComponent)
	{
		MovementComponent->Velocity = NewVelocity;
	}
}

FVector ABasketballHoop::GetCurrentHoopVelocity() const
{
	if (MovementComponent)	return MovementComponent->Velocity;
	return FVector(0.0f);
}

void ABasketballHoop::DeactivateMovement()
{
	if (MovementComponent)
	{
		MovementComponent->Deactivate();
	}
}

void ABasketballHoop::ActivateMovement()
{
	if (MovementComponent)
	{
		int Direction = FMath::RandBool() ? 1 : -1;
		float NewYVelocity = Direction * MovementComponent->InitialSpeed;
		MovementComponent->Velocity = FVector(0.0f, NewYVelocity, 0.0f);
		MovementComponent->Activate(true);
	}
}

bool ABasketballHoop::IsMovementActive() const
{
	return MovementComponent->IsActive();
}

void ABasketballHoop::IncreaseSpeed(float ByPercentage)
{
	if (MovementComponent)
	{
		float CurrentSpeed = MovementComponent->InitialSpeed;
		float NewSpeed = CurrentSpeed * (1.0f + ByPercentage / 100.0f);

		MovementComponent->InitialSpeed = NewSpeed;
		FVector CurrentVelocity = MovementComponent->Velocity;
		FVector NewVelocity = CurrentVelocity.GetSafeNormal() * NewSpeed;
		MovementComponent->Velocity = NewVelocity;
	}
}

void ABasketballHoop::ResetMovementSpeed()
{
	MovementComponent->InitialSpeed = MinMovementSpeed;
}

void ABasketballHoop::OnScoreOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UBallInterface::StaticClass()))
		{
			IBallInterface* BallInterface = Cast<IBallInterface>(OtherActor);
			if (BallInterface)
			{
				if (!BallInterface->GetScoredBoolean())
				{
					FVector BallVelocity = BallInterface->GetCurrentBallVelocity();

					if (BallVelocity.Z < 0)
					{
						BallInterface->SetScoredBoolean(true);
						OnScored.Broadcast(true);

						if (GameModeInterface)
						{
							bool Swish = BallInterface->GetSwishBoolean();
							GameModeInterface->UpdateScoreMultiplier(!Swish);
							GameModeInterface->UpdateScore();
						}

					}
				}

			}
		}

	}
}

void ABasketballHoop::OnHoopHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UBallInterface::StaticClass()))
		{
			IBallInterface* BallInterface = Cast<IBallInterface>(OtherActor);

			if (BallInterface)
			{
				BallInterface->SetSwishBoolean(false);
			}
		}

	}
}
