// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/GameLevelScriptActor.h"
#include "Component/Level/WaveControllerComponent.h"

AGameLevelScriptActor::AGameLevelScriptActor()
{
	WaveComponent = CreateDefaultSubobject<UWaveControllerComponent>(TEXT("WAVECONTROL_COMP"));
}
