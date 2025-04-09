// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartButtonWidget.h"
#include "GameModeInterface.h"
#include "FunctionsLibrary.h"
#include "Components/Button.h"

void URestartButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameModeInterface = UFunctionsLibrary::GetGameModeInterface(GetWorld());

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &URestartButtonWidget::RestartGame);
	}
}

void URestartButtonWidget::RestartGame()
{
	if (GameModeInterface)
	{
		GameModeInterface->RestartGame();
	}
}
