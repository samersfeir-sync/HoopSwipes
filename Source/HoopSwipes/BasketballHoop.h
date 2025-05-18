// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HoopInterface.h"
#include "BasketballHoop.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class IGameModeInterface;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScored, bool, RandomLocation);

UCLASS()
class HOOPSWIPES_API ABasketballHoop : public AActor, public IHoopInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasketballHoop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnScored OnScored;

	virtual void ChangeHoopVelocity(FVector NewVelocity) override;

	virtual FVector GetCurrentHoopVelocity() const override;

	void DeactivateMovement();

	void ActivateMovement();

	bool IsMovementActive() const;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* TimeWidgetComponent;

	void IncreaseSpeed(float ByPercentage);

	void ResetMovementSpeed();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HoopMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* RingMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* ScoreCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UBoxComponent* LeftEdgeCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UBoxComponent* RightEdgeCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* MovementComponent;

	UFUNCTION()
	void OnScoreOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	IGameModeInterface* GameModeInterface = nullptr;

	UFUNCTION()
	void OnRingHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
	float MinMovementSpeed = 80.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
	float MaxMovementSpeed = 150.0f;
};
