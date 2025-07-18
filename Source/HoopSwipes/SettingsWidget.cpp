// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"
#include "Components/Button.h"
#include "GameInstanceInterface.h"
#include "FunctionsLibrary.h"
#include "Components/Border.h"
#include "Sound/SoundClass.h"

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstanceInterface = UFunctionsLibrary::GetGameInstanceInterface(this);

	if (GameInstanceInterface)
	{
		FUserPreferences UserPreference = GameInstanceInterface->GetUserPreferences();

		bool bMutedSFX = UserPreference.bIsSFXMuted;
		UpdateBordersVisuals(MutedSFXBorder, UnmutedSFXBorder, bMutedSFX);
		SetSoundClassVolume(SFXSoundClass, !bMutedSFX ? 1.0f : 0.0f);

		bool bMutedMusic = UserPreference.bIsMusicMuted;
		UpdateBordersVisuals(MutedMusicBorder, UnmutedMusicBorder, bMutedMusic);
		ToggleMusic(bMutedMusic);
	}

	BackButton->OnClicked.AddDynamic(this, &USettingsWidget::BackButtonClicked);
	SoundEffectButton->OnClicked.AddDynamic(this, &USettingsWidget::ToggleSoundEffects);
	MusicButton->OnClicked.AddDynamic(this, &USettingsWidget::ToggleMusic);
}

void USettingsWidget::BackButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void USettingsWidget::UpdateBordersVisuals(UBorder* MutedBorder, UBorder* UnmutedBorder, bool bMuted) const
{
	const float MutedAlpha = bMuted ? 1.0f : 0.0f;
	const float UnmutedAlpha = bMuted ? 0.0f : 1.0f;

	FLinearColor FinalMutedColor = MutedColor;   FinalMutedColor.A = MutedAlpha;
	FLinearColor FinalUnmutedColor = UnmutedColor; FinalUnmutedColor.A = UnmutedAlpha;

	if (MutedBorder)
	{
		MutedBorder->SetBrushColor(FinalMutedColor);

		FSlateBrush MutedBrush = MutedBorder->Background;
		MutedBrush.OutlineSettings.Color = FLinearColor(0, 0, 0, MutedAlpha);
		MutedBorder->SetBrush(MutedBrush);
	}

	if (UnmutedBorder)
	{
		UnmutedBorder->SetBrushColor(FinalUnmutedColor);

		FSlateBrush UnmutedBrush = UnmutedBorder->Background;
		UnmutedBrush.OutlineSettings.Color = FLinearColor(0, 0, 0, UnmutedAlpha);
		UnmutedBorder->SetBrush(UnmutedBrush);
	}
}

void USettingsWidget::ToggleSoundEffects()
{
	if (!GameInstanceInterface) return;

	FUserPreferences UserPreference = GameInstanceInterface->GetUserPreferences();
	bool bMuted = UserPreference.bIsSFXMuted;
	UserPreference.bIsSFXMuted = !bMuted;
	UpdateBordersVisuals(MutedSFXBorder, UnmutedSFXBorder, !bMuted);
	SetSoundClassVolume(SFXSoundClass, bMuted ? 1.0f : 0.0f);
	GameInstanceInterface->SaveUserPreferences(UserPreference);
}

void USettingsWidget::ToggleMusic()
{
	if (!GameInstanceInterface) return;

	FUserPreferences UserPreference = GameInstanceInterface->GetUserPreferences();
	bool bMuted = UserPreference.bIsMusicMuted;
	UserPreference.bIsMusicMuted = !bMuted;
	UpdateBordersVisuals(MutedMusicBorder, UnmutedMusicBorder, !bMuted);
	ToggleMusic(!bMuted);
	GameInstanceInterface->SaveUserPreferences(UserPreference);
}

void USettingsWidget::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
	SoundClass->Properties.Volume = Volume;
}

void USettingsWidget::ToggleMusic(bool bMuted)
{
	bMuted ? GameInstanceInterface->StopBackgroundMusic() : GameInstanceInterface->PlayBackgroundMusic();
}