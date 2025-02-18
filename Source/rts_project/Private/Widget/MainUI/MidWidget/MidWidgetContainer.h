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
	// 초기화
	void InitializeWidget();

	// 위치 값 찾기
	void SerchStartPointLocations();

	// 위습을 찾습니다
	void SerchWisp();

	// 위습 버튼의 바인딩 함수
	// 클릭 시, 위습이 있는 곳으로 시점 이동
	UFUNCTION()
	void CALLBACK_ShoWWispLocation();
};
