// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeButtonWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UHomeButtonWidget::NativeConstruct()
{
	if (HomeButton)
	{
		HomeButton->OnClicked.AddDynamic(this, &UHomeButtonWidget::ReturnHome);
	}
}

void UHomeButtonWidget::ReturnHome()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuMap");
}
