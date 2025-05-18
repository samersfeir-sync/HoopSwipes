// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "GameInstanceInterface.h"
#include "UserProgression.h"
#include "Components/Border.h"
#include "ShopScreenWidget.h"

void UShopItemWidget::SetItemImage(UTexture2D* Image)
{
	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(Image, false);
	}
}

void UShopItemWidget::SetItemPriceText(const FString& PriceText, bool AdjustFont)
{
	if (ItemPriceText)
	{
		ItemPriceText->SetText(FText::FromString(PriceText));

		if (AdjustFont)
		{
			ItemPriceText->SetRenderTranslation(FVector2D(-53, 4));
			ItemPriceText->Font.Size = 40;
		}
	}
}

void UShopItemWidget::SetCoinImageVisibility(bool Visible)
{
	ESlateVisibility SizeBoxVisibility = Visible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
	
	CoinImage->SetVisibility(SizeBoxVisibility);
}

void UShopItemWidget::SetBallType(EBallType NewBallType)
{
	BallType = NewBallType;
}

void UShopItemWidget::SetGameInstanceInterface(IGameInstanceInterface* NewGameInstanceInterface)
{
	GameInstanceInterface = NewGameInstanceInterface;
}

void UShopItemWidget::SetItemPrice(int Price)
{
	ItemPrice = Price;
}

void UShopItemWidget::UpdateBorderColor()
{
	const FLinearColor DefaultOutlineColor(0.38f, 0.15f, 0.0f, 1.0f);

	EBallType SelectedBallType = GameInstanceInterface->GetBallType();

	for (UShopItemWidget* ShopItem : ParentWidget->ShopItemWidgets)
	{
		FLinearColor DesiredColor = (ShopItem->BallType == SelectedBallType) ? SelectedOutlineColor : DefaultOutlineColor;
		FSlateBrush ShopItemBrush = ShopItem->MainBorder->Background;
		ShopItemBrush.OutlineSettings.Color = DesiredColor;
		ShopItem->MainBorder->SetBrush(ShopItemBrush);
	}
}

void UShopItemWidget::SetParentWidgetReference(UShopScreenWidget* NewParentWidget)
{
	ParentWidget = NewParentWidget;
}

void UShopItemWidget::BuyButtonClicked()
{
	if (GameInstanceInterface)
	{
		FUserProgression UserProgression = GameInstanceInterface->GetUserProgression();

		if (UserProgression.PurchasedBalls.Find(BallType) == INDEX_NONE)
		{
			if (UserProgression.TotalCoins >= ItemPrice)
			{
				UserProgression.TotalCoins -= ItemPrice;
				UserProgression.PurchasedBalls.Add(BallType);
				GameInstanceInterface->SaveUserProgression(UserProgression);
				GameInstanceInterface->UpdateShopItemsStruct();
				GameInstanceInterface->SetBallType(BallType);
				OnBallPurchased.Broadcast();
			}
		}

		else GameInstanceInterface->SetBallType(BallType);
	}
}
void UShopItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BuyButton)
	{
		BuyButton->OnClicked.AddDynamic(this, &UShopItemWidget::BuyButtonClicked);
	}

	if (GameInstanceInterface)
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, FName("UpdateBorderColor"));
		GameInstanceInterface->AssignOnBallSet(Delegate);
	}

	UpdateBorderColor();
}
