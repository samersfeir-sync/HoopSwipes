// GameModeEnum.h

#pragma once

#include "CoreMinimal.h"
#include "GameModeEnum.generated.h"

UENUM(BlueprintType)
enum class EGameModeType : uint8
{
	Timed      UMETA(DisplayName = "Timed"),
	Unlimited  UMETA(DisplayName = "Unlimited")
};
