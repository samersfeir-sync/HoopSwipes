// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerWidget.h"
#include "Components/TextBlock.h"

void UTimerWidget::UpdateTime(int TotalSeconds)
{
	TotalSeconds = FMath::Max(0, TotalSeconds);

	int Minutes = TotalSeconds / 60;
	int Seconds = TotalSeconds % 60;

	FString FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

	if (TimeText)
	{
		TimeText->SetText(FText::FromString(FormattedTime));
	}
}