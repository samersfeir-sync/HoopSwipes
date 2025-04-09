// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class HOOPSWIPES_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateTime(int TotalSeconds);

private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText;
};
