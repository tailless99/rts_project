// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class UActorNameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ActorNameTextBlock;

public:
	void InitializeWidget(FName actorName);
};
