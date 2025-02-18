// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"


#define WLOG(_log_, ...) UE_LOG(LogTemp, Warning, _log_, ##__VA_ARGS__)

#define SLOG(_log_, ...) UKismetSystemLibrary::PrintString(GetWorld(),			\
(FString::Printf(TEXT("[LOG] ")) + FString::Printf(_log_, ##__VA_ARGS__)),		\
true, true, FLinearColor::Green, 3.0f);


// Tag Names
#define TAG_WISP		TEXT("TAG_WISP")
#define TAG_ENEMY		TEXT("TAG_ENEMY")
#define TAG_PLAYERUNIT	TEXT("TAG_PLAYERUNIT")


// Collision ProfileNames
#define COLLISIONNAME_NOCOLLISION			TEXT("NoCollision")
#define COLLISIONNAME_OVERLAPALLDYNAMIC		TEXT("OverlapAllDynamic")
#define COLLISIONNAME_BLACKALLDYNAMIC		TEXT("BlackAllDynamic")
#define COLLISIONNAME_ENEMYCHARACTER		TEXT("EnemyCharacter")
#define COLLISIONNAME_PLAYERCHARACTER		TEXT("PlayerCharacter")
#define COLLISIONNAME_PLAYERATTACK			TEXT("PlayerAttack")
#define COLLISIONNAME_WISPORPOTAL			TEXT("WispOrPotal")
#define COLLISIONNAME_BEDECTECTEDACTOR		TEXT("BeDetectedActor")

