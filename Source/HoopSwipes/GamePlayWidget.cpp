// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayWidget.h"
#include "RestartButtonWidget.h"
#include "ScoreWidget.h"
#include "HighScoreWidget.h"
#include "TargetScoreWidget.h"

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