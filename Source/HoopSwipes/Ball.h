// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallInterface.h"
#include "BallType.h"
#include "Ball.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallLaunched, bool, RandomLocation);

class IGameModeInterface;
class IGameInstanceInterface;

USTRUCT(BlueprintType)
struct FBallMeshSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FVector Scale = FVector(0.25f, 0.25f, 0.25f);
};

UCLASS()
class HOOPSWIPES_API ABall : public AActor, public IBallInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

	virtual FVector GetCurrentBallVelocity() const override
	{
		return GetVelocity();
	}

	virtual bool GetLaunchedBoolean() const override
	{
		return bLaunched;
	}

	virtual void SetScoredBoolean(bool Scored) override
	{
		bScored = Scored;
	}

	virtual bool GetScoredBoolean() const override
	{
		return bScored;
	}

	virtual void DeactivateBall() override;

	virtual void SetSwishBoolean(bool Swish) override
	{
		bSwish = Swish;
	}

	virtual bool GetSwishBoolean() const override
	{
		return bSwish;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	IGameModeInterface* GameModeInterface = nullptr;
	APlayerController* PlayerController = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LaunchBall(FVector2D TouchEndLocation);

	bool bLaunched = false;

	void ActivateBall(FTransform ActivateTransform);

	FOnBallLaunched OnBallLaunched;

private:

	bool bScored = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"));
	UStaticMeshComponent* BallMesh;

	UFUNCTION()
	void OnTouchBegin(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnTouchEnd(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	FVector2D BallInitialLocation = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* LaunchSound;

	bool bSwish = true;

	EBallType BallType = EBallType::Basketball;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball", meta = (AllowPrivateAccess = "true"))
	TMap<EBallType, FBallMeshSettings> BallTypeToMeshSettings;

	void ApplyBallSettings();

	IGameInstanceInterface* GameInstanceInterface = nullptr;

	virtual void OnConstruction(const FTransform& Transform) override;

};
