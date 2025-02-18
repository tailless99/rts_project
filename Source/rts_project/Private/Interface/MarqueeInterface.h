// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MarqueeInterface.generated.h"

UINTERFACE(BlueprintType, NotBlueprintable)
class UMarqueeInterface : public UInterface
{
    GENERATED_BODY()
};

// Define the interface methods
class IMarqueeInterface
{
    GENERATED_BODY()

public:
    virtual void MarqueePressed() = 0;
    virtual void MarqueeHeld() = 0;
    virtual void MarqueeReleased() = 0;
};
