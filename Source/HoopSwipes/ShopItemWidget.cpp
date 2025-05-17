// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "GameInstanceInterface.h"
#include "UserProgression.h"
#include "Components/Border.h"

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

void UShopItemWidget::UpdateBorderColor(FLinearColor NewColor)
{
	if (MainBorder)
	{
		FSlateBrush CurrentBrush = MainBorder->Background;
		CurrentBrush.OutlineSettings.Color = NewColor;
		MainBorder->SetBrush(CurrentBrush);
	}
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
}
