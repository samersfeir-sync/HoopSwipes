// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayWidget.generated.h"

class URestartButtonWidget;
class UScoreWidget;
class UHomeButtonWidget;
class UHighScoreWidget;
class UTargetScoreWidget;

UCLASS()
class HOOPSWIPES_API UGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	void ShowRestartButton(bool bShow);
	void ShowScoreWidget(bool bShow);
	void UpdateScoreUI(int NewScore);
	void UpdateHighScoreUI(int NewHighScore);
	void UpdateTargetScoreUI(int NewTargetScore);
	void ShowTargetScoreUI(bool Show);

private:

	UPROPERTY(meta = (BindWidget))
	URestartButtonWidget* RestartButtonWidget;

	UPROPERTY(meta = (BindWidget))
	UScoreWidget* ScoreWidget;

	UPROPERTY(meta = (BindWidget))
	UHomeButtonWidget* HomeButtonWidget;

	UPROPERTY(meta = (BindWidget))
	UHighScoreWidget* HighScoreWidget;

	UPROPERTY(meta = (BindWidget))
	UTargetScoreWidget* TargetScoreWidget;
};
