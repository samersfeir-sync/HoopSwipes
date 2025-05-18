// Fill out your copyright notice in the Description page of Project Settings.


#include "TotalCoinsWidget.h"
#include "Components/TextBlock.h"
#include "GameInstanceInterface.h"

void UTotalCoinsWidget::UpdateCoinsText()
{
    if (TotalCoins && GameInstanceInterface)
    {
        TotalCoins->SetText(FText::AsNumber(GameInstanceInterface->GetUserProgression().TotalCoins));
    }
}

void UTotalCoinsWidget::SetGameInstanceInterface(IGameInstanceInterface* NewGameInstanceInterface)
{
    GameInstanceInterface = NewGameInstanceInterface;

    UpdateCoinsText();
}
