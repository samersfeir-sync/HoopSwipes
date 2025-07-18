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
class UTotalCoinsWidget;
class USecondChanceWidget;
class UButton;
class USizeBox;
class IGameModeInterface;

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

	UPROPERTY(meta = (BindWidget))
	UTotalCoinsWidget* TotalCoinsWidget;

	UPROPERTY(meta = (BindWidget))
	USecondChanceWidget* SecondChanceWidget;

	UPROPERTY(meta = (BindWidget))
	class UTotalGemsWidget* TotalGemsWidget;

	void ShowSecondChanceWidget(bool bShow);

	UFUNCTION()
	void PauseButtonClicked();

	void EnablePauseButton(bool bEnable);
	void ShowHomeButton(bool bShow);
	void ShowSettingsButton(bool bShow);

private:

	virtual void NativeConstruct() override;

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

	UPROPERTY(meta = (BindWidget))
	USizeBox* PauseButtonSizeBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* ResumeButtonSizeBox;

	UPROPERTY(meta = (BindWidget))
	UButton* PauseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UFUNCTION()
	void ResumeButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;

	UFUNCTION()
	void SettingsButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class USettingsWidget* SettingsWidget;

	IGameModeInterface* GameModeInterface = nullptr;
};
