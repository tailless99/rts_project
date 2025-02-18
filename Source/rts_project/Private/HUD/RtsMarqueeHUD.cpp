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
	// ��Ʈ�ѷ� �ʱ�ȭ
	playerController = Cast<AGamePlayerController>(GetOwningPlayerController());
	playerController->SetHudComponent(this);

	// ������ ���� ����
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

	// ���콺 ��ġ�� ��������
	float mouseX, mouseY = 0;
	playerController->GetMousePosition(mouseX, mouseY);

	// ���콺 ��ġ �� �ʱ�ȭ
	CurrentMousePosition = StartMousePosition = FVector2D(mouseX, mouseY);
}

void ARtsMarqueeHUD::MarqueeHeld()
{
	// ���� �ʱ�ȭ
	for (AFriendlyAbleCharacterBase* selectActor : Rackangle) {
		selectActor->DeSelectUnit();
	}
	Rackangle.Reset();

	// ���콺 ��ġ�� ��������
	float mouseX, mouseY = 0;
	playerController->GetMousePosition(mouseX, mouseY);

	// ���콺 ��ġ ����
	CurrentMousePosition = FVector2D(mouseX, mouseY);

	// ���ÿ��� �׸���
	DrawRect(SelectBoxColor,
		StartMousePosition.X,
		StartMousePosition.Y,
		(CurrentMousePosition.X - StartMousePosition.X),
		(CurrentMousePosition.Y - StartMousePosition.Y));

	// ���� ���ֵ� ��ȯ
	GetActorsInSelectionRectangle(Rackangle);

	// ���� ���� ����
	BottomWidgetContainer->SetUnitInfo();

	// ���õ� ���� ó�� (��: ���õ� ���͸� ���� ���·� ����)
	for (AFriendlyAbleCharacterBase* Actor : Rackangle)
	{
		if (Actor)
		{
			Actor->SelectUnit(); // ��Į�� Ȱ��ȭ�Ͽ� ���õ��� ǥ��
		}
	}	
}

void ARtsMarqueeHUD::MarqueeReleased()
{
	IsDrawing = false;
}

// ���� �ȿ� ���Ͱ� �ִٸ� ����Ʈ�� �߰��ϴ� �Լ�
void ARtsMarqueeHUD::GetActorsInSelectionRectangle(TArray<AFriendlyAbleCharacterBase*>& OutActors) const
{
	if (!GetWorld()) return;

	FIntRect SelectionRect(
		FIntPoint(FMath::Min(StartMousePosition.X, CurrentMousePosition.X), FMath::Min(StartMousePosition.Y, CurrentMousePosition.Y)),
		FIntPoint(FMath::Max(StartMousePosition.X, CurrentMousePosition.X), FMath::Max(StartMousePosition.Y, CurrentMousePosition.Y)));

	// ���� ���� ���� ���͸� ã�� ���� ��� ���͸� �ݺ�
	for (TActorIterator<AFriendlyAbleCharacterBase> It(GetWorld()); It; ++It)
	{
		AFriendlyAbleCharacterBase* Actor = *It;
		if (Actor && IsActorWithinRectangle(Actor, SelectionRect) && OutActors.Num() < 12)
		{
			OutActors.Add(Actor);
		}
	}
}

// ���� �����ȿ� ���Ͱ� �ִ��� ã�� �Լ�
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
