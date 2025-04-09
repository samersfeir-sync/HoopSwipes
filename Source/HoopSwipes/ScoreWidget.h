// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

class UTextBlock;

UCLASS()
class HOOPSWIPES_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetScoreText(int Score);

private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;
	
};
