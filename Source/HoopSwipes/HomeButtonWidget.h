// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HomeButtonWidget.generated.h"

class UButton;

UCLASS()
class HOOPSWIPES_API UHomeButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	

private:

	UPROPERTY(meta = (BindWidget))
	UButton* HomeButton;

	UFUNCTION()
	void ReturnHome();

	virtual void NativeConstruct() override;

};
