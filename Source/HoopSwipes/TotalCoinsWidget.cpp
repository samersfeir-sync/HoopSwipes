// Fill out your copyright notice in the Description page of Project Settings.


#include "TotalCoinsWidget.h"
#include "Components/TextBlock.h"
#include "GameInstanceInterface.h"

void UTotalCoinsWidget::UpdateCoinsText(int CoinsAmount)
{
    FString FormattedCoins = FString::Printf(TEXT("%04d"), CoinsAmount);
    TotalCoins->SetText(FText::FromString(FormattedCoins));
}

void UTotalCoinsWidget::SetGameInstanceInterface(IGameInstanceInterface* NewGameInstanceInterface)
{
    GameInstanceInterface = NewGameInstanceInterface;

    UpdateCoinsText(GameInstanceInterface->GetUserProgression().TotalCoins);
}
