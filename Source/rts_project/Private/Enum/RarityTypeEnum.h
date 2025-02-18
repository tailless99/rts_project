// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RarityTypeEnum.generated.h"

UENUM(BlueprintType)
enum class ERarityTypeEnum : uint8
{
	Uncomon			UMETA(DisplayName = "Uncomon"),
	Comon			UMETA(DisplayName = "Comon"),
	Rare			UMETA(DisplayName = "Rare"),
	Unique			UMETA(DisplayName = "Unique"),
	Legend			UMETA(DisplayName = "Legend"),
	Hidden			UMETA(DisplayName = "Hidden"),
	Changed			UMETA(DisplayName = "Changed"),
	Limited			UMETA(DisplayName = "Limited"),
	Transcendence	UMETA(DisplayName = "Transcendence"),
	Imperishability UMETA(DisplayName = "Imperishability"),
	Eternal			UMETA(DisplayName = "Eternal")
};
