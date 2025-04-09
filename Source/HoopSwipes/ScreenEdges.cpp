// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenEdges.h"
#include "FunctionsLibrary.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "GameModeInterface.h"
#include "HoopInterface.h"
#include "GameInstanceInterface.h"

// Sets default values
AScreenEdges::AScreenEdges()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneRoot;

	LeftEdge = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftEdge"));
	LeftEdge->SetCollisionProfileName("OverlapAll");
	LeftEdge->SetGenerateOverlapEvents(true);
	LeftEdge->SetupAttachment(RootComponent);
	LeftEdge->SetVisibility(false, true);

	RightEdge = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightEdge"));
	RightEdge->SetCollisionProfileName("OverlapAll");
	RightEdge->SetGenerateOverlapEvents(true);
	RightEdge->SetupAttachment(RootComponent);
	RightEdge->SetVisibility(false, true);

	OnActorBeginOverlap.AddDynamic(this, &AScreenEdges::OnBeginOverlap);

	SetActorHiddenInGame(true);
}

void AScreenEdges::BeginPlay()
{
	Super::BeginPlay();

	GameModeInterface = UFunctionsLibrary::GetGameModeInterface(GetWorld());

	if (GameModeInterface)
	{
		FScriptDelegate CameraDelegate;
		CameraDelegate.BindUFunction(this, "SetEdgesLocations");
		GameModeInterface->AssignCameraFetchedDelegate(CameraDelegate);
	}
}


// Called every frame
void AScreenEdges::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScreenEdges::GetCameraFrustumEdges(UCameraComponent* Camera, float Distance, FVector& TopCenter, 
	FVector& LeftCenter, FVector& BottomCenter, FVector& RightCenter)
{
    FMinimalViewInfo CameraView;
    Camera->GetCameraView(0.0f, CameraView);

    float AspectRatio = CameraView.AspectRatio;
    float FoV_Y = CameraView.FOV;
    float FoV_X = FMath::Atan(FMath::Tan(FMath::DegreesToRadians(FoV_Y) * 0.5f) * AspectRatio * 2.0f);

    float HalfHeight = Distance * FMath::Tan(FMath::DegreesToRadians(FoV_Y) * 0.5f);
    float HalfWidth = HalfHeight * AspectRatio;

    FVector CamForward = Camera->GetForwardVector();
    FVector CamRight = Camera->GetRightVector();
    FVector CamUp = Camera->GetUpVector();

    FVector CamLoc = CameraView.Location;

    FVector CenterAtDistance = CamLoc + (CamForward * Distance);

    TopCenter = CenterAtDistance + (CamUp * HalfHeight);
    BottomCenter = CenterAtDistance - (CamUp * HalfHeight);
    LeftCenter = CenterAtDistance - (CamRight * HalfWidth);
    RightCenter = CenterAtDistance + (CamRight * HalfWidth);
}

void AScreenEdges::SetEdgesLocations()
{
	ACameraActor* Camera = GameModeInterface->GetCameraActor();
	UCameraComponent* CameraComponent = Camera->GetCameraComponent();

	FVector TopCenter;
	FVector LeftCenter;
	FVector BottomCenter;
	FVector RightCenter;

	IGameInstanceInterface* GameInstanceInterface = UFunctionsLibrary::GetGameInstanceInterface(GetWorld());
	FVector2D ViewportSize = GameInstanceInterface->GetViewportSize();

	float Distance = ViewportSize.X * 100.0f / 720.0f;
	GetCameraFrustumEdges(CameraComponent, Distance, TopCenter, LeftCenter, BottomCenter, RightCenter);

	FVector AdjustedLeft = FVector(-170.0f, LeftCenter.Y, 0.0f);
	LeftEdge->SetWorldLocation(AdjustedLeft);

	FVector AdjustedRight = FVector(-170.0f, RightCenter.Y, 0.0f);
	RightEdge->SetWorldLocation(AdjustedRight);
}

void AScreenEdges::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UHoopInterface::StaticClass()))
		{
			IHoopInterface* HoopInterface = Cast<IHoopInterface>(OtherActor);

			if (HoopInterface)
			{
				FVector CurrentVelocity = HoopInterface->GetCurrentHoopVelocity();
				CurrentVelocity.Y *= -1;
				HoopInterface->ChangeHoopVelocity(CurrentVelocity);
			}
		}
	}
}

