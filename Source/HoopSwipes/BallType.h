#pragma once

#include "CoreMinimal.h"
#include "BallType.generated.h"

UENUM(BlueprintType)
enum class EBallType : uint8
{
	Basketball     UMETA(DisplayName = "Basketball"),
	SoccerBall     UMETA(DisplayName = "Soccer Ball"),
	BeachBall      UMETA(DisplayName = "Beach Ball"),
	VolleyBall	   UMETA(DisplayName = "Volley Ball"),
	TennisBall     UMETA(DisplayName = "Tennis Ball"),
	Baseball       UMETA(DisplayName = "Baseball"),
	Globe		   UMETA(DisplayName = "Globe"),
	Cricket		   UMETA(DisplayName = "Cricket Ball"),
	BowlingBall    UMETA(DisplayName = "Bowling Ball")
};
