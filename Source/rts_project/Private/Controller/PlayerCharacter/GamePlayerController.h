// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/MarqueeInterface.h"
#include "Enum/MouseLeftBtnActionType.h"
#include "GamePlayerController.generated.h"



UCLASS()
class AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class AGamePlayerCharacter* GamePlayer;

	UPROPERTY()
	class ARtsMarqueeHUD* hudComponent;

	UPROPERTY()
	TSubclassOf<class UMain_Container_Widget> WidgetBP_MainContainer;

	UPROPERTY()
	class UMain_Container_Widget* MainContainerWidget;

	UPROPERTY()
	class UUserWidget* NormalCursur;

	UPROPERTY()
	class UUserWidget* HoverCursur;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMouseLeftBtnActionType MouseLeftBtnActionType;


	FTimerHandle DisableClickBtnTimer;
	AActor* TargetActor;
	FVector TargetLocation;

private:
	bool MouseCursorCheck;

private:
	void CheckMousePosition();

public:
	AGamePlayerController();

public:	
	virtual void SetupInputComponent() override;
	virtual void OnPossess(class APawn* aPawn) override;
	virtual void Tick(float deltaTime) override;


public:
	// 마우스의 위치를 가져옴
	void GetterMousePosition(float& NormalizeMousePositionX, float& NormalizeMousePositionY);

	// 마우스를 강제로 움직여 커서를 다시 그리도록 함
	void UpdateMousePosition();

	// 마우스 클릭된 위치에서 에너미 액터를 찾아서 반환하는 함수
	AActor* SelectEnemyActorFromMousePoint();

	// 마우스 클릭된 위치를 벡터로 반환하는 함수
	FVector GetMouseLeftBtnClickPointVector();

	// 마우스 커서의 모양을 초기화 한다
	void InitializeMouseCursor();

	// 키입력 함수들
	void CALLBACK_OnLeftMouseBtn_ClickHoldPressed();
	void CALLBACK_OnLeftMouseBtnPressed();
	void CALLBACK_OnLeftMouseBtnReleased();
	void CALLBACK_OnRightMouseBtnPressed();
	void CALLBACK_OnAttackBtnPressed();
	void CALLBACK_OnPetrollBtnPressed();
	void CALLBACK_OnHoldBtnPressed();
	void CALLBACK_OnSellPressed();

	void SetHudComponent(ARtsMarqueeHUD* HudComponent);
};
