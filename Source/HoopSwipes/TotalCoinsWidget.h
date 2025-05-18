// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TotalCoinsWidget.generated.h"

class UTextBlock;
class IGameInstanceInterface;

UCLASS()
class HOOPSWIPES_API UTotalCoinsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateCoinsText(int CoinsAmount);

	void SetGameInstanceInterface(IGameInstanceInterface* NewGameInstanceInterface);

private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalCoins;

	IGameInstanceInterface* GameInstanceInterface = nullptr;

};
