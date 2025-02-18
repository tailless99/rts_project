// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MouseLeftBtnActionType.generated.h"

UENUM(BlueprintType)
enum class EMouseLeftBtnActionType : uint8
{
	None			UMETA(DisplayName = "None"),
	Petroll			UMETA(DisplayName = "Petroll"),
	Attack			UMETA(DisplayName = "Attack")
};