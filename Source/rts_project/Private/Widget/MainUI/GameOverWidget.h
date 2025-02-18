// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* GameStateText;

	UPROPERTY(Meta = (BindWidget))
	class UButton* RetryBtn;

	UPROPERTY(Meta = (BindWidget))
	class UButton* TitleBtn;

public:
	void InitializeWidget();

	// ������ ������ϴ� �Լ�
	UFUNCTION()
	void CALLBACK_RetryGame();

	// Ÿ��Ʋ�� �̵��ϴ� �Լ�
	UFUNCTION()
	void CALLBACK_GoTitle();

	void SetGameStateText(FName gameStateText);
};
