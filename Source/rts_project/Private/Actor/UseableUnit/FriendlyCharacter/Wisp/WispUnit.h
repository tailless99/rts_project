// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "WispUnit.generated.h"

/**
 * 
 */
UCLASS()
class AWispUnit : public AFriendlyAbleCharacterBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* Wisp_Body;
		
public:
	virtual void BeginPlay() override;

	void DestroyWisp();

private:
	AWispUnit();

};
