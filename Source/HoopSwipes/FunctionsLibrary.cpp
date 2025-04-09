// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionsLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameModeInterface.h"
#include "GameInstanceInterface.h"
#include"GameFramework/GameModeBase.h"

APlayerController* UFunctionsLibrary::GetPlayerController(UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	return UGameplayStatics::GetPlayerController(WorldContextObject, 0);
}

IGameModeInterface* UFunctionsLibrary::GetGameModeInterface(UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;
	
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World) return nullptr;

	AGameModeBase* GameMode = World->GetAuthGameMode();
	if (!GameMode) return nullptr;

	if (GameMode->GetClass()->ImplementsInterface(UGameModeInterface::StaticClass()))
	{
		return Cast<IGameModeInterface>(GameMode);
	}

	return nullptr;
}

IGameInstanceInterface* UFunctionsLibrary::GetGameInstanceInterface(UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World) return nullptr;

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance) return nullptr;

	if (GameInstance->GetClass()->ImplementsInterface(UGameInstanceInterface::StaticClass()))
	{
		return Cast<IGameInstanceInterface>(GameInstance);
	}

	return nullptr;
}
