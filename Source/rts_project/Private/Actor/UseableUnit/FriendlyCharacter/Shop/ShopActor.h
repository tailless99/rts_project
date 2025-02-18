// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "ShopActor.generated.h"

/**
 * 
 */
UCLASS()
class AShopActor : public AFriendlyAbleCharacterBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UAudioComponent* AudioComponent;

	UPROPERTY()
	class USoundWave* SoundWave;

public:
	AShopActor();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	// 인터페이스 상속 함수
	virtual bool SkillBtn05() override;
	virtual bool SkillBtn06() override;
};
