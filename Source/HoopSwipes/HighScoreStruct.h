#pragma once

#include "CoreMinimal.h"
#include "HighScoreStruct.generated.h"

USTRUCT(BlueprintType)
struct FHighScoreData
{
	GENERATED_BODY()

	UPROPERTY()
	int UnlimitedTimeScore;

	UPROPERTY()
	int LimitedTimeScore;

	FHighScoreData()
		: UnlimitedTimeScore(0), LimitedTimeScore(0)
	{
	}
};
