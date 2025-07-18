// Fill out your copyright notice in the Description page of Project Settings.


#include "TotalGemsWidget.h"
#include "Components/TextBlock.h"
#include "GameInstanceInterface.h"

void UTotalGemsWidget::UpdateGemsText(int NewTotalGems)
{
	FString FormattedGems = FString::Printf(TEXT("%04d"), NewTotalGems);
	GemsText->SetText(FText::FromString(FormattedGems));
}

void UTotalGemsWidget::SetGameInstanceInterface(IGameInstanceInterface* NewGameInstanceInterface)
{
	GameInstanceInterface = NewGameInstanceInterface;

	UpdateGemsText(GameInstanceInterface->GetUserProgression().TotalGems);
}