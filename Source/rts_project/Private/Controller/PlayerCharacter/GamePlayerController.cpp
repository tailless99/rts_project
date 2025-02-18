#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Actor/PlayerCharacter/GamePlayerCharacter.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"
#include "HUD/RtsMarqueeHUD.h"
#include "../rts_project.h"
#include "Widget/MainUI/Main_Container_Widget.h"
#include "Widget/MainUI/BottomWidget/BottomButtonWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Enum/MouseLeftBtnActionType.h"


AGamePlayerController::AGamePlayerController()
{
	static ConstructorHelpers::FClassFinder<UMain_Container_Widget> WIDGET_MAINCONTAINER(
		TEXT("/Game/Blueprints/Widget/MainUI/BP_MainContainer.BP_MainContainer_C"));

	if (WIDGET_MAINCONTAINER.Succeeded())
	{
		WidgetBP_MainContainer = WIDGET_MAINCONTAINER.Class;
	}

	static ConstructorHelpers::FObjectFinder<UUserWidget> WIDGET_NORMAL_CORSUR(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/Comon/WidgetBP_NormalCursor.WidgetBP_NormalCursor'"));

	if (WIDGET_NORMAL_CORSUR.Succeeded())
	{
		NormalCursur = WIDGET_NORMAL_CORSUR.Object;
	}

	static ConstructorHelpers::FObjectFinder<UUserWidget> WIDGET_HOVER_CORSUR(
		TEXT("/Script/Engine.Texture2D'/Game/Assets/Image/Widget/Common/IMG_Hover_Cursor.IMG_Hover_Cursor'"));

	if (WIDGET_HOVER_CORSUR.Succeeded())
	{
		HoverCursur = WIDGET_HOVER_CORSUR.Object;
	}	

	// ���콺 Ŀ�� ����
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CurrentMouseCursor = EMouseCursor::Default;
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	bShowMouseCursor = true;

	// ���� ���콺 ��ư �Է�ó��
	InputComponent->BindAction(TEXT("LeftMouseBtn"), IE_Pressed, this, &ThisClass::CALLBACK_OnLeftMouseBtnPressed);
	InputComponent->BindAction(TEXT("LeftMouseBtn"), IE_Released, this, &ThisClass::CALLBACK_OnLeftMouseBtnReleased);
	InputComponent->BindAction(TEXT("RightMouseBtn"), IE_Pressed, this, &ThisClass::CALLBACK_OnRightMouseBtnPressed);
	InputComponent->BindAction(TEXT("AttackBtn"), IE_Pressed, this, &ThisClass::CALLBACK_OnAttackBtnPressed);
	InputComponent->BindAction(TEXT("PetrollBtn"), IE_Pressed, this, &ThisClass::CALLBACK_OnPetrollBtnPressed);
	InputComponent->BindAction(TEXT("HoldBtn"), IE_Pressed, this, &ThisClass::CALLBACK_OnHoldBtnPressed);
	InputComponent->BindAction(TEXT("Sell"), IE_Pressed, this, &ThisClass::CALLBACK_OnSellPressed);
}

void AGamePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// �ػ� ����
	UGameUserSettings* userSetting = GEngine->GetGameUserSettings();
	FIntPoint screenSize = FIntPoint(1920, 1080);
	userSetting->SetScreenResolution(screenSize);

	// ���� ��, ���� ����� ���� ����� ����
	GamePlayer = Cast<AGamePlayerCharacter>(aPawn);

	MainContainerWidget = CreateWidget<UMain_Container_Widget>(this, WidgetBP_MainContainer);
	MainContainerWidget->AddToViewport();
	MainContainerWidget->InitializeWidget();
}

void AGamePlayerController::Tick(float deltaTime)
{
	// ���콺 ��ġ�� ���� ȭ���� ������ ó��
	CheckMousePosition();
}

// ���콺 ��ġ�� ���� ȭ���� ������ ó��
void AGamePlayerController::CheckMousePosition()
{
	// ����ó��
	if (!IsValid(GamePlayer)) return;
		
	// ���콺 ��ġ�� ������ ����
	float mouseX, mouseY = 0.0f;
	GetterMousePosition(mouseX, mouseY);

	// ���콺 X �� ����
	if (mouseX < 0.05 || mouseX > 0.95) {
		GamePlayer->OnInputMouseX(mouseX < 0.05 ? -1 : 1);
	}
	else GamePlayer->OnInputMouseX(0); // ���� ������ �ƴ� ��� 0���� ��ȯ

	// ���콺 Y �� ����
	if (mouseY < 0.05 || mouseY > 0.95) {
		GamePlayer->OnInputMouseY(mouseY < 0.05 ? -1 : 1);
	}
	else GamePlayer->OnInputMouseY(0); // ���� ������ �ƴ� ��� 0���� ��ȯ
}

// ���콺 ��ġ�� �������� �Լ�
void AGamePlayerController::GetterMousePosition(float& NormalizeMousePositionX, float& NormalizeMousePositionY)
{
	// ���� �ʱ�ȭ
	float mouseX, mouseY = 0.0f;	// ���콺�� ��ġ
	int32 screenX, screenY = 0; // ���÷��� ũ��

	// ���콺 ��ġ
	if (GetMousePosition(mouseX, mouseY)) {
		GetViewportSize(screenX, screenY); // ���÷��� ũ��

		// ���� ȭ�鿡 ����� ���콺�� ��ġ�� ��ֶ�����ȭ
		NormalizeMousePositionX = mouseX / screenX;
		NormalizeMousePositionY = mouseY / screenY;
	}
}

// ���콺�� ������ ������ Ŀ���� �ٽ� �׸����� ��
void AGamePlayerController::UpdateMousePosition()
{
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);
	SetMouseLocation(MousePosition.X + 1, MousePosition.Y + 1);
}

// ���콺 Ŭ���� ��ġ���� ���ʹ� ���͸� ã�Ƽ� ��ȯ�ϴ� �Լ�
AActor* AGamePlayerController::SelectEnemyActorFromMousePoint()
{
	// ���콺 ��ġ�� ��������
	float mouseX, mouseY = 0;
	GetMousePosition(mouseX, mouseY);

	// ���콺�� ��ũ�� ��ǥ�� ���� ��ǥ�� ��ȯ
	FVector WorldLocation;	// ��ġ
	FVector WorldDirection; // ����
	DeprojectScreenPositionToWorld(mouseX, mouseY, WorldLocation, WorldDirection);

	// ����ĳ��Ʈ ����
	FHitResult hitResult;
	FVector start = WorldLocation; // ������
	FVector end = start + (WorldDirection * 10000); // ���� (ī�޶� ���� �������� ������ ���������� �������� 1�� �Ÿ�)

	// ����ĳ��Ʈ ���ܸ��
	AGamePlayerCharacter* playerCharacter = Cast<AGamePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	TArray<AActor*> actorToIgnore;
	actorToIgnore.Add(this);
	actorToIgnore.Add(playerCharacter);

	// �˻��� ������Ʈ ä�� ����
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this, start, end, 100.0f,
		objectTypes, false,
		actorToIgnore, EDrawDebugTrace::None,
		hitResult, true);

	if (bHit)
	{
		AEnemyCharacterBase* SelectEnemy = Cast<AEnemyCharacterBase>(hitResult.GetActor());
		if (SelectEnemy)
		{
			return SelectEnemy;
		}
	}
	return nullptr;
}

// ���콺 Ŭ���� ��ġ�� ���ͷ� ��ȯ�ϴ� �Լ�
FVector AGamePlayerController::GetMouseLeftBtnClickPointVector()
{
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		// Ŭ���� ��ġ���� ����
		FVector HisLocation = HitResult.ImpactPoint;
		return HisLocation;
	}
	return FVector();
}

// ���콺 Ŀ�� �ʱ�ȭ
void AGamePlayerController::InitializeMouseCursor()
{
	if (CurrentMouseCursor == EMouseCursor::Crosshairs)
		CurrentMouseCursor = EMouseCursor::Default;

	// ���� Ŭ�� Ÿ�� �ʱ�ȭ
	MouseLeftBtnActionType = EMouseLeftBtnActionType::None;
	// ���콺 ���� ������Ʈ
	UpdateMousePosition();
}

// CALLBACK_OnLeftMouseBtnPressed() �� ���� Ÿ�̸ӷ� ����� �Լ�
void AGamePlayerController::CALLBACK_OnLeftMouseBtn_ClickHoldPressed()
{
	// ���� Ŭ���� Ȧ���ϰ� ���� ��� ����
	if (IsValid(hudComponent)) {
		hudComponent->MarqueePressed();
	}
}

// ���콺 ���� ��ư ���� �� �Է� ó��
void AGamePlayerController::CALLBACK_OnLeftMouseBtnPressed()
{
	// ���� ��ư�� �Ҵ�� ��ų �׼��� ���� ���
	if (MouseLeftBtnActionType == EMouseLeftBtnActionType::None)
	{	
		// ���� Ŭ��, �巡�� �Լ�
		FTimerManager& timerManager = GetWorld()->GetTimerManager();

		// Ÿ�̸� ���� �ʱ�ȭ
		if (timerManager.IsTimerActive(DisableClickBtnTimer))
			timerManager.ClearTimer(DisableClickBtnTimer);

		// Ÿ�̸� ����
		timerManager.SetTimer(
			DisableClickBtnTimer,	// Ÿ�̸Ӹ� ������ Ÿ�̸� �ڵ鷯
			FTimerDelegate::CreateUObject(this, &ThisClass::CALLBACK_OnLeftMouseBtn_ClickHoldPressed),	// ������ �Լ� ���ε�
			0.07f, false);	// �� �Ŀ� ������ ���ΰ� , �ݺ�����

		/*
			Ŭ���� �����ϰ� ������ 0.5�� �Ŀ� CALLBACK_OnLeftMouseBtn_ClickHoldPressed() �� ����Ǿ�
			Ŭ�� ������ �׸��� �۾��� �����ϰ�,
			�� ���� ��ư�� ���� Release �Լ��ʿ��� Ÿ�̸Ӱ� ���� ������ �۾��� �ֳ� Ȯ��(IsTimerActive�Լ��� ���)�ϴ� ������
			ª�� Ŭ������ ��� �����ϴ� ������ ������ �� �ִ�.
			�� Ŭ���� ��� Ÿ�̸ӿ� ������ �Լ��� �̹� ����Ǿ� IsTimerActive�� ����� flase�� ���� ���̰�,
			ª�� Ŭ���� ��� ���� Ÿ�̸ӿ� ����� �۾��� ����Ǳ� ���̱� ������ true�� ���� ���̴�.

			�̸� �̿��ؼ� Release �Լ��ʿ��� �� Ŭ���� �������ϴ� �Լ��� �����ϰų�, ª�� Ŭ���� ���� �Լ��� �������� �����Ѵ�.
		*/
	}
	// ���� ��ư�� �Ҵ�� ��ų �׼��� �ִ� ���
	else {
		switch (MouseLeftBtnActionType)
		{		
			case EMouseLeftBtnActionType::Petroll:				
				// ���� ������ ����
				TargetLocation = GetMouseLeftBtnClickPointVector();
				CALLBACK_OnPetrollBtnPressed();
				break;
			case EMouseLeftBtnActionType::Attack:
				// ���� ������ ����
				TargetActor = SelectEnemyActorFromMousePoint();
				CALLBACK_OnAttackBtnPressed();
				break;		
		}
	}
}

// ���콺 ���� ��ư �� �� �Է� ó��
void AGamePlayerController::CALLBACK_OnLeftMouseBtnReleased()
{
	// ���� ��ư�� �Ҵ�� ��ų �׼��� ���� ���
	if (MouseLeftBtnActionType == EMouseLeftBtnActionType::None)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();

		if (timerManager.IsTimerActive(DisableClickBtnTimer)) {
			// Ÿ�̸� ����
			timerManager.ClearTimer(DisableClickBtnTimer);

			// �۾��� �����ִ� = ª�� Ŭ���̹Ƿ� Ŭ���� �ش��ϴ� �Լ� ���� 
			MainContainerWidget->BP_BottomContainerWidget->FindSelectActorToMouseClick();
		}
		else {
			// ���� �۾��� ���� = �� Ŭ���̸�, �̹� �ش��ϴ� �Լ��� �����߱� ������
			// Release������ ������ ����� Press �Լ��� �ݴ�Ǵ� Relased �Լ��� �����ϴ� ������ �������ϰ� �ȴ�.
			if (IsValid(hudComponent)) {
				hudComponent->MarqueeReleased();
			}
		}
	}
}

// ���콺 ������ ��ư �Է�
void AGamePlayerController::CALLBACK_OnRightMouseBtnPressed()
{
	// ��Ʈ ��ȯ ���� ����
	FHitResult HitResult;

	// ���콺 Ŀ�� �ʱ�ȭ
	if (CurrentMouseCursor == EMouseCursor::Crosshairs) InitializeMouseCursor();

	if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		FVector HisLocation = HitResult.ImpactPoint;

		TArray<AFriendlyAbleCharacterBase*> SelectUnitList = hudComponent->GetGrabSelectedUnit();
		for (AFriendlyAbleCharacterBase* Unit : SelectUnitList)
		{
			Unit->UnitMoveCommand(HisLocation);
		}
	}
}

// ���� ��ư
void AGamePlayerController::CALLBACK_OnAttackBtnPressed()
{
	// ����ó��
	if (!IsValid(MainContainerWidget)) return;

	// ���� ���ȴ��� Ȯ�� �� ���콺 Ŀ�� ����
	if (MouseLeftBtnActionType != EMouseLeftBtnActionType::Attack)
	{
		// ���� ���� ���� �ʱ�ȭ
		TargetActor = nullptr;

		// ���콺 Ŀ�� ����
		CurrentMouseCursor = EMouseCursor::Crosshairs;
		UpdateMousePosition();

		// ���콺 ���� ��ư �׼� Ÿ�� ����
		MouseLeftBtnActionType = EMouseLeftBtnActionType::Attack;
	}
	// �� �� ������ �Էµ� ��, �ٽ� �ѹ� ���ݽ�ų�� ���� ��츦 ����
	else if(MouseLeftBtnActionType == EMouseLeftBtnActionType::Attack && TargetActor != nullptr)
	{
		// ���콺 Ŀ�� ����
		CurrentMouseCursor = EMouseCursor::Default;
		UpdateMousePosition();

		// ��Ʈ�� �Լ� ����
		MainContainerWidget->BP_BottomContainerWidget->TargetActor = TargetActor;
		MainContainerWidget->BP_BottomContainerWidget->BP_BottomButtonWidget_Attack->CALLBACK_OnClickCommonSKillFunc();

		// ���콺 ���� ��ư �׼� Ÿ�� �ʱ�ȭ
		MouseLeftBtnActionType = EMouseLeftBtnActionType::None;
	}
	
}

// ���� ��ư
void AGamePlayerController::CALLBACK_OnPetrollBtnPressed()
{
	// ����ó��
	if (!IsValid(MainContainerWidget)) return;

	// ���� ���ȴ��� Ȯ�� �� ���콺 Ŀ�� ����
	if (MouseLeftBtnActionType != EMouseLeftBtnActionType::Petroll)
	{
		// ���� ���� ���� �ʱ�ȭ
		TargetLocation = FVector::ZeroVector;

		// ���콺 Ŀ�� ����
		CurrentMouseCursor = EMouseCursor::Crosshairs;
		UpdateMousePosition();
	
		// ���콺 ���� ��ư �׼� Ÿ�� ����
		MouseLeftBtnActionType = EMouseLeftBtnActionType::Petroll;
	}
	// �� �� ������ư�� ���� ��, Ŭ������ �ʰ� �ٽ� ��ų�� ���� ����� ����ó��
	else if(MouseLeftBtnActionType == EMouseLeftBtnActionType::Petroll && TargetLocation != FVector::ZeroVector){
		// ���콺 Ŀ�� ����
		CurrentMouseCursor = EMouseCursor::Default;
		UpdateMousePosition();

		// ��Ʈ�� �Լ� ����
		MainContainerWidget->BP_BottomContainerWidget->TargetLocation = TargetLocation;
		MainContainerWidget->BP_BottomContainerWidget->BP_BottomButtonWidget_Petroll->CALLBACK_OnClickCommonSKillFunc();

		// ���콺 ���� ��ư �׼� Ÿ�� �ʱ�ȭ
		MouseLeftBtnActionType = EMouseLeftBtnActionType::None;
	}
}

// Ȧ�� ��ư
void AGamePlayerController::CALLBACK_OnHoldBtnPressed()
{
	// ����ó��
	if (!IsValid(MainContainerWidget)) return;
	// ���콺 Ŀ�� �ʱ�ȭ
	if (CurrentMouseCursor == EMouseCursor::Crosshairs) InitializeMouseCursor();

	MainContainerWidget->BP_BottomContainerWidget->BP_BottomButtonWidget_Hold->CALLBACK_OnClickCommonSKillFunc();
}

// �Ǹ� ��ư
void AGamePlayerController::CALLBACK_OnSellPressed()
{
	// ����ó��
	if (!IsValid(MainContainerWidget)) return;	
	// ���콺 Ŀ�� �ʱ�ȭ
	if (CurrentMouseCursor == EMouseCursor::Crosshairs) InitializeMouseCursor();

	MainContainerWidget->BP_BottomContainerWidget->BP_BottomButtonWidget_Sell->CALLBACK_OnClickCommonSKillFunc();
}

void AGamePlayerController::SetHudComponent(ARtsMarqueeHUD* HudComponent)
{
	hudComponent = HudComponent;
}
