// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class AGameLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UWaveControllerComponent* WaveComponent;


public:
	AGameLevelScriptActor();

	FORCEINLINE class UWaveControllerComponent* GetWaveComponent(){
		return WaveComponent;
	}
};
