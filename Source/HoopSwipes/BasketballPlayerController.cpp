// Fill out your copyright notice in the Description page of Project Settings.


#include "BasketballPlayerController.h"
#include "FunctionsLibrary.h"
#include "GameModeInterface.h"
#include "Ball.h"

ABasketballPlayerController::ABasketballPlayerController()
{
	bShowMouseCursor = true;
	bEnableTouchEvents = true;
}

void ABasketballPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameModeInterface = UFunctionsLibrary::GetGameModeInterface(GetWorld());
}

void ABasketballPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindTouch(IE_Released, this, &ABasketballPlayerController::OnTouchReleased);
}

void ABasketballPlayerController::OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (GameModeInterface)
	{
		ABall* TouchedBall = GameModeInterface->GetTouchedBall();
		
		if (TouchedBall)
		{
			float TouchEndX, TouchEndY;
			bool bIsCurrentlyPressed;
			GetInputTouchState(FingerIndex, TouchEndX, TouchEndY, bIsCurrentlyPressed);
			FVector2D TouchEndLocation = FVector2D(TouchEndX, TouchEndY);
			TouchedBall->LaunchBall(TouchEndLocation);
			GameModeInterface->SetTouchedBall(nullptr);
		}
	}
}