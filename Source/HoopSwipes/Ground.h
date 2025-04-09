// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ground.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBallHitGround);

UCLASS()
class HOOPSWIPES_API AGround : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGround();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnBallHitGround OnBallHitGround;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* GroundMesh;

	UFUNCTION()
	void OnGroundHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);
};
