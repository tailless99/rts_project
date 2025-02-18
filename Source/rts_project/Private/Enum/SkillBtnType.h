#pragma once

#include "CoreMinimal.h"
#include "SkillBtnType.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class SkillBtnType : uint8
{
	Attack			UMETA(DisplayName = "Attack"),
	Hold			UMETA(DisplayName = "Hold"),
	Petroll			UMETA(DisplayName = "Petroll"),
	Sell			UMETA(DisplayName = "Sell"),

	skill01			UMETA(DisplayName = "skill01"),
	skill02			UMETA(DisplayName = "skill02"),
	skill03			UMETA(DisplayName = "skill03"),
	skill04			UMETA(DisplayName = "skill04"),
	skill05			UMETA(DisplayName = "skill05"),
	skill06			UMETA(DisplayName = "skill06"),
	skill07			UMETA(DisplayName = "skill07"),
	skill08			UMETA(DisplayName = "skill08"),
};