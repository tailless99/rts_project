// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTitleSecWidget.generated.h"

/**
 *  
 */ 
UCLASS()
class UGameTitleSecWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
	class UButton* Btn_GoMainGame;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* GuideText;

public:
	void InitializeWidget();

	UFUNCTION()
	void CALLBACK_GoMainGame();
};
