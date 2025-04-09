// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScreenEdges.generated.h"

class UCameraComponent;
class IGameModeInterface;

UCLASS()
class HOOPSWIPES_API AScreenEdges : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScreenEdges();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Components");
	UStaticMeshComponent* LeftEdge;

	UPROPERTY(EditDefaultsOnly, Category = "Components");
	UStaticMeshComponent* RightEdge;

	void GetCameraFrustumEdges(UCameraComponent* Camera, float Distance, FVector& TopCenter, 
		FVector& LeftCenter, FVector& BottomCenter, FVector& RightCenter);

	UFUNCTION()
	void SetEdgesLocations();

	IGameModeInterface* GameModeInterface = nullptr;

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
