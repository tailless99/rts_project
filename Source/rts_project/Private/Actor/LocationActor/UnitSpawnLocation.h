// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/LocationActor/ActorSpawnLocationBase.h"
#include "UnitSpawnLocation.generated.h"

UCLASS()
class AUnitSpawnLocation : public AActorSpawnLocationBase
{
	GENERATED_BODY()
	
public:	
	AUnitSpawnLocation();

protected:
	virtual void BeginPlay() override;

};
