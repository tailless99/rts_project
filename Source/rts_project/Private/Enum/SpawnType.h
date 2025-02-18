// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnType.generated.h"

UENUM(BlueprintType)
enum class ESpawnType : uint8
{
	RandomUnit			UMETA(DisplayName = "RandomUnit"),
	Money				UMETA(DisplayName = "Money"),
	Wood				UMETA(DisplayName = "Wood"),
	UncommonUnit_1		UMETA(DisplayName = "UncommonUnit_1"),
	UncommonUnit_2		UMETA(DisplayName = "UncommonUnit_2"),
	UncommonUnit_3		UMETA(DisplayName = "UncommonUnit_3"),
	UncommonUnit_4		UMETA(DisplayName = "UncommonUnit_4"),
	UncommonUnit_5		UMETA(DisplayName = "UncommonUnit_5"),
	UncommonUnit_6		UMETA(DisplayName = "UncommonUnit_6")
};