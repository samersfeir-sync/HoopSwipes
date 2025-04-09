// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionsLibrary.generated.h"

class IGameModeInterface;
class IGameInstanceInterface;

UCLASS()
class HOOPSWIPES_API UFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	static APlayerController* GetPlayerController(UObject* WorldContextObject);

	static IGameModeInterface* GetGameModeInterface(UObject* WorldContextObject);

	static IGameInstanceInterface* GetGameInstanceInterface(UObject* WorldContextObject);

};
