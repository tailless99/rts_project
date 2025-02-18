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
	// ���콺�� ��ġ�� ������
	void GetterMousePosition(float& NormalizeMousePositionX, float& NormalizeMousePositionY);

	// ���콺�� ������ ������ Ŀ���� �ٽ� �׸����� ��
	void UpdateMousePosition();

	// ���콺 Ŭ���� ��ġ���� ���ʹ� ���͸� ã�Ƽ� ��ȯ�ϴ� �Լ�
	AActor* SelectEnemyActorFromMousePoint();

	// ���콺 Ŭ���� ��ġ�� ���ͷ� ��ȯ�ϴ� �Լ�
	FVector GetMouseLeftBtnClickPointVector();

	// ���콺 Ŀ���� ����� �ʱ�ȭ �Ѵ�
	void InitializeMouseCursor();

	// Ű�Է� �Լ���
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
