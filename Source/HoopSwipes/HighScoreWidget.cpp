// Fill out your copyright notice in the Description page of Project Settings.


#include "HighScoreWidget.h"
#include "Components/TextBlock.h"

void UHighScoreWidget::SetHighScoreText(int HighScore)
{
	if (HighScoreText)
	{
		HighScoreText->SetText(FText::AsNumber(HighScore));
	}
}
