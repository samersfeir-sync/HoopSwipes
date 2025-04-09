// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetScoreWidget.h"
#include "Components/TextBlock.h"

void UTargetScoreWidget::UpdateTargetScoreText(int NewTargetScore)
{
	if (TargetScoreText)
	{
		FString FormattedString = FString::Printf(TEXT("Target: %d"), NewTargetScore);
		TargetScoreText->SetText(FText::FromString(FormattedString));
	}
}
