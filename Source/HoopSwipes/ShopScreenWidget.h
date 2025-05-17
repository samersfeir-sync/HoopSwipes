// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BallsShopStruct.h"
#include "ShopScreenWidget.generated.h"

class UButton;
class UShopItemWidget;
class UUniformGridPanel;
class IGameInstanceInterface;
struct FBallsShopStruct;
class UTextBlock;

UCLASS()
class HOOPSWIPES_API UShopScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	TArray<FBallsShopStruct> BallShopItems;

	void UpdateCoinsText();

private:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* ItemContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	TSubclassOf<UShopItemWidget> ShopItemWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalCoins;

	UFUNCTION()
	void FillItemContainer();

public:

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	IGameInstanceInterface* GameInstanceInterface = nullptr;
};
