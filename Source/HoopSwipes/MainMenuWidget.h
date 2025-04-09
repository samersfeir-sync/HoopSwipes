// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class HOOPSWIPES_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta = (BindWidget))
	UButton* UnlimitedTimeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LimitedTimeButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EndlessModeHighScore;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeModeHighScore;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitGameButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void UnlimitedTimeButtonClicked();

	UFUNCTION()
	void LimitedTimeButtonClicked();

	UWorld* World = nullptr;

	UFUNCTION()
	void ExitGame();

	void SetHighScoresTexts(UTextBlock* HighScoreTextBlock, int HighScore);
};
