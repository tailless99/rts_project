// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorSpawnLocationBase.generated.h"

UCLASS()
class AActorSpawnLocationBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AActorSpawnLocationBase();

protected:
	virtual void BeginPlay() override;


};
