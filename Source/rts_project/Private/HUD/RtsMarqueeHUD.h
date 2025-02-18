// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/MarqueeInterface.h"
#include "RtsMarqueeHUD.generated.h"

/**
 * 
 */
UCLASS()
class ARtsMarqueeHUD : public AHUD, public IMarqueeInterface
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	class AGamePlayerController* playerController;

	UPROPERTY()
	class UBottomWidgetContainer* BottomWidgetContainer;

private:
	FLinearColor SelectBoxColor;
	FVector2D StartMousePosition;
	FVector2D CurrentMousePosition;
	TArray<class AFriendlyAbleCharacterBase*> Rackangle;
	bool IsDrawing;

public:
	ARtsMarqueeHUD();

private:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

public:
	// 인터페이스 함수
	virtual void MarqueePressed() override;
	virtual void MarqueeHeld() override;
	virtual void MarqueeReleased() override;

private:
	void GetActorsInSelectionRectangle(TArray<AFriendlyAbleCharacterBase*>& OutActors) const;
	bool IsActorWithinRectangle(AFriendlyAbleCharacterBase* Actor, const FIntRect& Rect) const;

public:
	void UpdateSelectAreaUnit(AFriendlyAbleCharacterBase* targetUnit);

	FORCEINLINE TArray<AFriendlyAbleCharacterBase*> GetGrabSelectedUnit() { return Rackangle; }
	FORCEINLINE void SetGrabSelectedUnit(TArray<AFriendlyAbleCharacterBase*> actors) { 
		Rackangle.Empty();
		Rackangle = actors;
	}

};