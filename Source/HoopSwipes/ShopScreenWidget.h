// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BallsShopStruct.h"
#include "GemShopInfo.h"
#include "ShopScreenWidget.generated.h"

class UButton;
class UShopItemWidget;
class UUniformGridPanel;
class IGameInstanceInterface;
struct FBallsShopStruct;
class UTotalCoinsWidget;
class UTotalGemsWidget;
class UScrollBox;

UCLASS()
class HOOPSWIPES_API UShopScreenWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* ItemsContainer;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* GemsContainer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UShopItemWidget> ShopItemWidgetClass;

	UFUNCTION()
	void FillItemsContainer(TArray<FBallsShopStruct> BallsShopStruct);

	UFUNCTION()
	void FillGemsContainer();

	UPROPERTY(meta = (BindWidget))
	UButton* BallsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* GemsButton;

	UFUNCTION()
	void GemButtonClicked();
	
	UFUNCTION()
	void BallButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGemShopInfo> GemShopInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGemShopWidget> GemShopWidgetClass;

public:

	TArray<UShopItemWidget*> ShopItemWidgets;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UTotalCoinsWidget* TotalCoinsWidget;

	UPROPERTY(meta = (BindWidget))
	UTotalGemsWidget* TotalGemsWidget;

	IGameInstanceInterface* GameInstanceInterface = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<FBallsShopStruct> BallShopItems;
};
