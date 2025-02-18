// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameTitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AGameTitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TSubclassOf<class UGameTitleSecWidget> WidgetBP_GameTitleSec;

	UPROPERTY(Meta = (BindWidget))
	class UGameTitleSecWidget* GameTitleSec;

public:
	AGameTitlePlayerController();

public:
	virtual void OnPossess(APawn* aPawn) override;
};
