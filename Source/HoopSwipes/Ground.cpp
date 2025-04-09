// Fill out your copyright notice in the Description page of Project Settings.


#include "Ground.h"
#include "BallInterface.h"

// Sets default values
AGround::AGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	RootComponent = GroundMesh;
	GroundMesh->SetNotifyRigidBodyCollision(true);
	GroundMesh->SetRenderCustomDepth(true);
}

// Called when the game starts or when spawned
void AGround::BeginPlay()
{
	Super::BeginPlay();

	GroundMesh->OnComponentHit.AddDynamic(this, &AGround::OnGroundHit);
}

// Called every frame
void AGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGround::OnGroundHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UBallInterface::StaticClass()))
		{
			IBallInterface* BallInterface = Cast<IBallInterface>(OtherActor);

			if (BallInterface)
			{
				if (BallInterface->GetLaunchedBoolean())
				{
					BallInterface->DeactivateBall();
					OnBallHitGround.Broadcast();
				}
			}
		}
	}
}
