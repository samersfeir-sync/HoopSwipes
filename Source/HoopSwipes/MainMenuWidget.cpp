// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FunctionsLibrary.h"
#include "Components/TextBlock.h"
#include "GameInstanceInterface.h"
#include "HighScoreStruct.h"
#include "ShopScreenWidget.h"
#include "Components/CanvasPanel.h"

void UMainMenuWidget::NativeConstruct()
{
	World = GetWorld();

	IGameInstanceInterface* GameInstanceInterface = UFunctionsLibrary::GetGameInstanceInterface(World);

	if (World)
	{
		if (GameInstanceInterface)
		{
			FHighScoreData HighScoresData = GameInstanceInterface->GetHighScoreStruct();

			if (EndlessModeHighScore)
			{
				SetHighScoresTexts(EndlessModeHighScore, HighScoresData.UnlimitedTimeScore);
			}

			if (TimeModeHighScore)
			{
				SetHighScoresTexts(TimeModeHighScore, HighScoresData.LimitedTimeScore);
			}
		}
	}

	if (UnlimitedTimeButton)
	{
		UnlimitedTimeButton->OnClicked.AddDynamic(this, &UMainMenuWidget::UnlimitedTimeButtonClicked);
	}

	if (LimitedTimeButton)
	{
		LimitedTimeButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LimitedTimeButtonClicked);
	}

	if (ExitGameButton)
	{
		ExitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ExitGame);
	}

	if (ShopButton)
	{
		ShopButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ShopButtonClicked);
	}

	if (ShopScreen)
	{
		if (ShopScreen->BackButton)
		{
			ShopScreen->BackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnShopBackButtonClicked);
		}
	}
}


void UMainMenuWidget::OnShopBackButtonClicked()
{
	MainMenuScreenContainer->SetVisibility(ESlateVisibility::Visible);
	ShopScreen->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuWidget::UnlimitedTimeButtonClicked()
{
	if (World)
	{
		OnGameStarted.Broadcast();
		UGameplayStatics::OpenLevel(World, "UnlimitedTimeMap");
	}
}

void UMainMenuWidget::LimitedTimeButtonClicked()
{
	if (World)
	{
		OnGameStarted.Broadcast();
		UGameplayStatics::OpenLevel(World, "LimitedTimeMap");
	}
}

void UMainMenuWidget::ExitGame()
{
	if (World)
	{
		APlayerController* PlayerController = UFunctionsLibrary::GetPlayerController(World);
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
	}
}

void UMainMenuWidget::SetHighScoresTexts(UTextBlock* HighScoreTextBlock, int HighScore)
{
	HighScoreTextBlock->SetText(FText::AsNumber(HighScore));
}

void UMainMenuWidget::ShopButtonClicked()
{
	MainMenuScreenContainer->SetVisibility(ESlateVisibility::Hidden);
	ShopScreen->SetVisibility(ESlateVisibility::Visible);
}
