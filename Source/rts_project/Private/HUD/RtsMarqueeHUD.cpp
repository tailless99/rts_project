// Fill out your copyright notice in the Description page of Project Settings.


#include "RtsMarqueeHUD.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Actor/PlayerCharacter/GamePlayerCharacter.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "rts_project/rts_project.h"
#include "Widget/MainUI/BottomWidget/BottomWidgetContainer.h"
#include "Widget/MainUI/Main_Container_Widget.h"
#include "Engine/Canvas.h"
#include "EngineUtils.h"

ARtsMarqueeHUD::ARtsMarqueeHUD()
{
	PrimaryActorTick.bCanEverTick = true;

	IsDrawing = false;
}

void ARtsMarqueeHUD::BeginPlay()
{
	// 컨트롤러 초기화
	playerController = Cast<AGamePlayerController>(GetOwningPlayerController());
	playerController->SetHudComponent(this);

	// 위젯에 정보 전달
	BottomWidgetContainer = playerController->MainContainerWidget->GetBottomWidgetContainer();
	BottomWidgetContainer->SetHudComponent(this);

	SelectBoxColor = FLinearColor(0.6f, 1.0f, 0.6f, 0.4f);
}

void ARtsMarqueeHUD::DrawHUD()
{
	Super::DrawHUD();

	if (IsDrawing) {
		MarqueeHeld();
	}
}

void ARtsMarqueeHUD::MarqueePressed()
{
	IsDrawing = true;

	// 마우스 위치값 가져오기
	float mouseX, mouseY = 0;
	playerController->GetMousePosition(mouseX, mouseY);

	// 마우스 위치 값 초기화
	CurrentMousePosition = StartMousePosition = FVector2D(mouseX, mouseY);
}

void ARtsMarqueeHUD::MarqueeHeld()
{
	// 변수 초기화
	for (AFriendlyAbleCharacterBase* selectActor : Rackangle) {
		selectActor->DeSelectUnit();
	}
	Rackangle.Reset();

	// 마우스 위치값 가져오기
	float mouseX, mouseY = 0;
	playerController->GetMousePosition(mouseX, mouseY);

	// 마우스 위치 갱신
	CurrentMousePosition = FVector2D(mouseX, mouseY);

	// 선택영역 그리기
	DrawRect(SelectBoxColor,
		StartMousePosition.X,
		StartMousePosition.Y,
		(CurrentMousePosition.X - StartMousePosition.X),
		(CurrentMousePosition.Y - StartMousePosition.Y));

	// 선택 유닛들 반환
	GetActorsInSelectionRectangle(Rackangle);

	// 위젯 정보 갱신
	BottomWidgetContainer->SetUnitInfo();

	// 선택된 액터 처리 (예: 선택된 액터를 선택 상태로 변경)
	for (AFriendlyAbleCharacterBase* Actor : Rackangle)
	{
		if (Actor)
		{
			Actor->SelectUnit(); // 데칼을 활성화하여 선택됨을 표시
		}
	}	
}

void ARtsMarqueeHUD::MarqueeReleased()
{
	IsDrawing = false;
}

// 영역 안에 액터가 있다면 리스트에 추가하는 함수
void ARtsMarqueeHUD::GetActorsInSelectionRectangle(TArray<AFriendlyAbleCharacterBase*>& OutActors) const
{
	if (!GetWorld()) return;

	FIntRect SelectionRect(
		FIntPoint(FMath::Min(StartMousePosition.X, CurrentMousePosition.X), FMath::Min(StartMousePosition.Y, CurrentMousePosition.Y)),
		FIntPoint(FMath::Max(StartMousePosition.X, CurrentMousePosition.X), FMath::Max(StartMousePosition.Y, CurrentMousePosition.Y)));

	// 선택 영역 내의 액터를 찾기 위해 모든 액터를 반복
	for (TActorIterator<AFriendlyAbleCharacterBase> It(GetWorld()); It; ++It)
	{
		AFriendlyAbleCharacterBase* Actor = *It;
		if (Actor && IsActorWithinRectangle(Actor, SelectionRect) && OutActors.Num() < 12)
		{
			OutActors.Add(Actor);
		}
	}
}

// 선택 영역안에 액터가 있는지 찾는 함수
bool ARtsMarqueeHUD::IsActorWithinRectangle(AFriendlyAbleCharacterBase* Actor, const FIntRect& Rect) const
{
	if (!Actor || !GetWorld()) return false;

	FVector2D ScreenPosition;
	if (GetOwningPlayerController()->ProjectWorldLocationToScreen(Actor->GetActorLocation(), ScreenPosition))
	{
		return Rect.Contains(FIntPoint(ScreenPosition.X, ScreenPosition.Y));
	}

	return false;
}

void ARtsMarqueeHUD::UpdateSelectAreaUnit(AFriendlyAbleCharacterBase* targetUnit)
{
	Rackangle.Remove(targetUnit);
}
