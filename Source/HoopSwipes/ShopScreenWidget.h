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
class UTotalCoinsWidget;

UCLASS()
class HOOPSWIPES_API UShopScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	TArray<FBallsShopStruct> BallShopItems;

	void UpdateCoinsText();

	TArray<UShopItemWidget*> ShopItemWidgets;

private:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* ItemContainer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UShopItemWidget> ShopItemWidgetClass;

	UFUNCTION()
	void FillItemContainer();

public:

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UTotalCoinsWidget* TotalCoinsWidget;

	IGameInstanceInterface* GameInstanceInterface = nullptr;
};
