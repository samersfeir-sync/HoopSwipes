// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayWidget.h"
#include "RestartButtonWidget.h"
#include "ScoreWidget.h"
#include "HighScoreWidget.h"
#include "TargetScoreWidget.h"
#include "SecondChanceWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SizeBox.h"
#include "HomeButtonWidget.h"
#include "SettingsWidget.h"
#include "TotalGemsWidget.h"
#include "GameModeInterface.h"
#include "FunctionsLibrary.h"

void UGamePlayWidget::ShowRestartButton(bool bShow)
{
	if (RestartButtonWidget)
	{
		ESlateVisibility RestartButtonVisibility = bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

		RestartButtonWidget->SetVisibility(RestartButtonVisibility);
	}
}

void UGamePlayWidget::ShowScoreWidget(bool bShow)
{
	ESlateVisibility ScoreWidgetVisibility = bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
	
	ScoreWidget->SetVisibility(ScoreWidgetVisibility);
}

void UGamePlayWidget::UpdateScoreUI(int NewScore)
{
	if (ScoreWidget)
	{
		ScoreWidget->SetScoreText(NewScore);
	}
}

void UGamePlayWidget::UpdateHighScoreUI(int NewHighScore)
{
	if (HighScoreWidget)
	{
		HighScoreWidget->SetHighScoreText(NewHighScore);
	}
}

void UGamePlayWidget::UpdateTargetScoreUI(int NewTargetScore)
{
	if (TargetScoreWidget)
	{
		TargetScoreWidget->UpdateTargetScoreText(NewTargetScore);
	}
}

void UGamePlayWidget::ShowTargetScoreUI(bool Show)
{
	ESlateVisibility NewVisibility = Show ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;

	TargetScoreWidget->SetVisibility(NewVisibility);
}

void UGamePlayWidget::ShowSecondChanceWidget(bool bShow)
{
	ESlateVisibility SecondChanceWidgetVisibility = bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	SecondChanceWidget->SetVisibility(SecondChanceWidgetVisibility);

	if (!bShow)
	{
		SecondChanceWidget->ResetWidget();
		return;
	}

	bool bCanWatchAd = GameModeInterface->GetCanWatchAd();
	SecondChanceWidget->WatchAdButton->SetIsEnabled(bCanWatchAd);

	int32 GemsNeeded = GameModeInterface->GetGemsNeededForSecondChance();
	int32 TotalGems = GameModeInterface->GetTotalGems();
	bool bCanUseGems = TotalGems >= GemsNeeded;
	SecondChanceWidget->GemButton->SetIsEnabled(bCanUseGems);

	FString GemText = FString::Printf(TEXT("%d gem%s"), GemsNeeded, GemsNeeded == 1 ? TEXT("") : TEXT("s"));
	FString UseGemsOnly = FString::Printf(TEXT("Don't give up yet! Use <CyanText>%s</> for a second shot!"), *GemText);
	FString BothOptions = FString::Printf(TEXT("Don't give up yet! Watch an ad or use <CyanText>%s</> for a second shot!"), *GemText);
	FString WatchAdOnly = FString::Printf(TEXT("Don't give up yet! Watch an ad for a second shot!"));

	SecondChanceWidget->ChangeMainText(
		bCanWatchAd && TotalGems >= GemsNeeded ? BothOptions :
		bCanWatchAd ? WatchAdOnly :
		UseGemsOnly,
		false
	);

	SecondChanceWidget->TotalGemsWidget->UpdateGemsText(TotalGems);
	SecondChanceWidget->StartSkipTimer();
}

void UGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameModeInterface = UFunctionsLibrary::GetGameModeInterface(this);

	SecondChanceWidget->SetGamePlayWigetInstance(this);

	PauseButton->OnClicked.AddDynamic(this, &UGamePlayWidget::PauseButtonClicked);
	ResumeButton->OnClicked.AddDynamic(this, &UGamePlayWidget::ResumeButtonClicked);
	SettingsButton->OnClicked.AddDynamic(this, &UGamePlayWidget::SettingsButtonClicked);
}

void UGamePlayWidget::PauseButtonClicked()
{
	UGameplayStatics::SetGamePaused(this, true);
	ResumeButtonSizeBox->SetVisibility(ESlateVisibility::Visible);
	PauseButton->SetIsEnabled(false);
	ShowHomeButton(true);
	ShowSettingsButton(true);
}

void UGamePlayWidget::EnablePauseButton(bool bEnable)
{
	PauseButton->SetIsEnabled(bEnable);
}

void UGamePlayWidget::ShowHomeButton(bool bShow)
{
	ESlateVisibility HomeButtonVisibility = bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	HomeButtonWidget->SetVisibility(HomeButtonVisibility);
}

void UGamePlayWidget::ShowSettingsButton(bool bShow)
{
	ESlateVisibility SettingsButtonVisibility = bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	SettingsButton->SetVisibility(SettingsButtonVisibility);
}

void UGamePlayWidget::ResumeButtonClicked()
{
	UGameplayStatics::SetGamePaused(this, false);
	ResumeButtonSizeBox->SetVisibility(ESlateVisibility::Hidden);
	PauseButton->SetIsEnabled(true);
	ShowHomeButton(false);
	ShowSettingsButton(false);
}

void UGamePlayWidget::SettingsButtonClicked()
{
	SettingsWidget->SetVisibility(ESlateVisibility::Visible);
}