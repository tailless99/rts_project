// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MidWidgetContainer.generated.h"

/**
 * 
 */
UCLASS()
class UMidWidgetContainer : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
	class UButton* WispBtn;

	FVector GoStartPointLocation;

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	// �ʱ�ȭ
	void InitializeWidget();

	// ��ġ �� ã��
	void SerchStartPointLocations();

	// ������ ã���ϴ�
	void SerchWisp();

	// ���� ��ư�� ���ε� �Լ�
	// Ŭ�� ��, ������ �ִ� ������ ���� �̵�
	UFUNCTION()
	void CALLBACK_ShoWWispLocation();
};
