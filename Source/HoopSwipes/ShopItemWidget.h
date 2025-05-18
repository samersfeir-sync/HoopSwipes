// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BallType.h"
#include "ShopItemWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class IGameInstanceInterface;
class UBorder;
class UShopScreenWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBallPurchased);

UCLASS()
class HOOPSWIPES_API UShopItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetItemImage(UTexture2D* Image);
	
	void SetItemPriceText(const FString& PriceText, bool AdjustFont);

	void SetCoinImageVisibility(bool Visible);

	void SetBallType(EBallType NewBallType);

	void SetGameInstanceInterface(IGameInstanceInterface* NewGameInstanceInterface);

	void SetItemPrice(int Price);

	FOnBallPurchased OnBallPurchased;

	UFUNCTION()
	void UpdateBorderColor();

	void SetParentWidgetReference(UShopScreenWidget* NewParentWidget);

private:

	EBallType BallType;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemPriceText;

	UPROPERTY(meta = (BindWidget))
	UImage* CoinImage;

	UPROPERTY(meta = (BindWidget))
	UButton* BuyButton;

	UFUNCTION()
	void BuyButtonClicked();

	virtual void NativeConstruct() override;

	IGameInstanceInterface* GameInstanceInterface;

	int ItemPrice;

	UPROPERTY(meta = (BindWidget))
	UBorder* MainBorder;

	UShopScreenWidget* ParentWidget;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor SelectedOutlineColor;
};
