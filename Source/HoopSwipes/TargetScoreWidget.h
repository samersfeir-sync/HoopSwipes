// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetScoreWidget.generated.h"

class UTextBlock;

UCLASS()
class HOOPSWIPES_API UTargetScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateTargetScoreText(int NewTargetScore);

private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TargetScoreText;

};
