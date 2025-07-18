// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopScreenWidget.h"
#include "BallType.h"
#include "Components/UniformGridPanel.h"
#include "ShopItemWidget.h"
#include "BallsShopStruct.h"
#include "GameInstanceInterface.h"
#include "Components/Button.h"
#include "FunctionsLibrary.h"
#include "TotalCoinsWidget.h"
#include "TotalGemsWidget.h"
#include "Components/ScrollBox.h"
#include "GemShopWidget.h"
#include "InsufficientCoinsWidget.h"

void UShopScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameInstanceInterface = UFunctionsLibrary::GetGameInstanceInterface(GetWorld());

    if (GameInstanceInterface)
    {
        if (TotalCoinsWidget)
            TotalCoinsWidget->SetGameInstanceInterface(GameInstanceInterface);

        TArray<FBallsShopStruct> BallsStruct = GameInstanceInterface->GetShopStruct();
        FillItemsContainer(BallsStruct);
    }

    InsufficientCoinsWidget->SetShopWidget(this);

    BallsButton->OnClicked.AddDynamic(this, &UShopScreenWidget::BallButtonClicked);
    GemsButton->OnClicked.AddDynamic(this, &UShopScreenWidget::GemButtonClicked);
}

void UShopScreenWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (IsDesignTime())
    {
        FillItemsContainer(BallShopItems);
    }

    FillGemsContainer();
}

void UShopScreenWidget::FillItemsContainer(TArray<FBallsShopStruct> BallsShopStruct)
{
    ItemsContainer->ClearChildren();
    const int ItemsPerRow = 3;

    for (int32 i = 0; i < BallsShopStruct.Num(); ++i)
    {
        const FBallsShopStruct& BallItem = BallsShopStruct[i];

        UShopItemWidget* NewItem = CreateWidget<UShopItemWidget>(this, ShopItemWidgetClass);

        if (NewItem)
        {
            bool bIsPurchased = BallItem.IsPurchased;
            FString FormattedPrice = bIsPurchased ? "OWNED" : FString::FromInt(BallItem.Price);

            ShopItemWidgets.Add(NewItem);
            NewItem->SetParentWidgetReference(this);
            NewItem->SetGameInstanceInterface(GameInstanceInterface);
            // NewItem->SetGameModeInterface(GameModeInterface);
            NewItem->SetBallType(BallItem.BallType);
            NewItem->SetItemImage(BallItem.BallTexture);
            NewItem->SetCoinImageVisibility(!bIsPurchased);
            NewItem->SetItemPriceText(FormattedPrice, bIsPurchased);
            NewItem->SetItemPrice(BallItem.Price);
            NewItem->OnBallPurchased.AddDynamic(this, &UShopScreenWidget::FillItemsContainer);

            int32 Row = i / ItemsPerRow;
            int32 Column = i % ItemsPerRow;
            ItemsContainer->AddChildToUniformGrid(NewItem, Row, Column);
        }
    }

    if (IsDesignTime())
        return;

    int32 TotalCoins = GameInstanceInterface->GetUserProgression().TotalCoins;
    TotalCoinsWidget->UpdateCoinsText(TotalCoins);
    int32 TotalGems = GameInstanceInterface->GetUserProgression().TotalGems;
    TotalGemsWidget->UpdateGemsText(TotalGems);
}

void UShopScreenWidget::FillGemsContainer()
{
    if (GemShopInfo.IsEmpty())
        return;

    const int GemsPerRow = 2;

    for (int32 i = 0; i < GemShopInfo.Num(); ++i)
    {
        UGemShopWidget* NewGemItem = CreateWidget<UGemShopWidget>(this, GemShopWidgetClass);

        if (NewGemItem)
        {
            NewGemItem->SetParentWidget(this);
            NewGemItem->SetGemPrice(GemShopInfo[i].Price);
            NewGemItem->SetGemAmount(GemShopInfo[i].Amount);
            NewGemItem->SetGemImage(GemShopInfo[i].Image);
            NewGemItem->SetProductId(GemShopInfo[i].ProductId);

            int32 Row = i / GemsPerRow;
            int32 Column = i % GemsPerRow;
            GemsContainer->AddChildToUniformGrid(NewGemItem, Row, Column);
        }
    }
}

void UShopScreenWidget::GemButtonClicked()
{
    ItemsContainer->SetVisibility(ESlateVisibility::Collapsed);
    GemsContainer->SetVisibility(ESlateVisibility::Visible);
    BallsButton->SetIsEnabled(true);
    GemsButton->SetIsEnabled(false);
    ScrollBox->ScrollToStart();
}

void UShopScreenWidget::BallButtonClicked()
{
    ItemsContainer->SetVisibility(ESlateVisibility::Visible);
    GemsContainer->SetVisibility(ESlateVisibility::Collapsed);
    BallsButton->SetIsEnabled(false);
    GemsButton->SetIsEnabled(true);
    ScrollBox->ScrollToStart();
}
