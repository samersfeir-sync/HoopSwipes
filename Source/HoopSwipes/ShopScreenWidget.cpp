// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopScreenWidget.h"
#include "BallType.h"
#include "Components/UniformGridPanel.h"
#include "ShopItemWidget.h"
#include "BallsShopStruct.h"
#include "GameInstanceInterface.h"
#include "Components/Button.h"
#include "FunctionsLibrary.h"
#include "Components/TextBlock.h"

void UShopScreenWidget::UpdateCoinsText()
{
    if (TotalCoins && GameInstanceInterface)
    {
        TotalCoins->SetText(FText::AsNumber(GameInstanceInterface->GetUserProgression().TotalCoins));
    }
}

void UShopScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameInstanceInterface = UFunctionsLibrary::GetGameInstanceInterface(GetWorld());

    FillItemContainer();
}

void UShopScreenWidget::FillItemContainer()
{
    if (GameInstanceInterface)
    {
        BallShopItems = GameInstanceInterface->GetShopStruct();
        ItemContainer->ClearChildren();
        const int ItemsPerRow = 3;

        for (int32 i = 0; i < BallShopItems.Num(); ++i)
        {
            const FBallsShopStruct& BallItem = BallShopItems[i];

            UShopItemWidget* NewItem = CreateWidget<UShopItemWidget>(this, ShopItemWidgetClass);

            if (NewItem)
            {
                NewItem->OnBallPurchased.AddDynamic(this, &UShopScreenWidget::FillItemContainer);
                NewItem->SetGameInstanceInterface(GameInstanceInterface);

                bool bIsPurchased = BallItem.IsPurchased;
                NewItem->SetItemImage(BallItem.BallTexture);
                NewItem->SetCoinImageVisibility(!bIsPurchased);
                NewItem->SetBallType(BallItem.BallType);
                FString FormattedPrice = bIsPurchased ? "OWNED" : FString::FromInt(BallItem.Price);
                NewItem->SetItemPriceText(FormattedPrice, bIsPurchased);

                if (!bIsPurchased)
                    NewItem->SetItemPrice(BallItem.Price);

                int32 Row = i / ItemsPerRow;
                int32 Column = i % ItemsPerRow;
                ItemContainer->AddChildToUniformGrid(NewItem, Row, Column);
            }
        }

        UpdateCoinsText();
    }
}
