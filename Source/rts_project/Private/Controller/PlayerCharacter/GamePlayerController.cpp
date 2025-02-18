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

	// 마우스 커서 설정
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CurrentMouseCursor = EMouseCursor::Default;
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	bShowMouseCursor = true;

	// 왼쪽 마우스 버튼 입력처리
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

	// 해상도 고정
	UGameUserSettings* userSetting = GEngine->GetGameUserSettings();
	FIntPoint screenSize = FIntPoint(1920, 1080);
	userSetting->SetScreenResolution(screenSize);

	// 빙의 시, 빙의 대상을 내부 멤버에 저장
	GamePlayer = Cast<AGamePlayerCharacter>(aPawn);

	MainContainerWidget = CreateWidget<UMain_Container_Widget>(this, WidgetBP_MainContainer);
	MainContainerWidget->AddToViewport();
	MainContainerWidget->InitializeWidget();
}

void AGamePlayerController::Tick(float deltaTime)
{
	// 마우스 위치에 따른 화면의 움직임 처리
	CheckMousePosition();
}

// 마우스 위치에 따른 화면의 움직임 처리
void AGamePlayerController::CheckMousePosition()
{
	// 예외처리
	if (!IsValid(GamePlayer)) return;
		
	// 마우스 위치를 저장할 변수
	float mouseX, mouseY = 0.0f;
	GetterMousePosition(mouseX, mouseY);

	// 마우스 X 값 감지
	if (mouseX < 0.05 || mouseX > 0.95) {
		GamePlayer->OnInputMouseX(mouseX < 0.05 ? -1 : 1);
	}
	else GamePlayer->OnInputMouseX(0); // 감지 영역이 아닌 경우 0으로 반환

	// 마우스 Y 값 감지
	if (mouseY < 0.05 || mouseY > 0.95) {
		GamePlayer->OnInputMouseY(mouseY < 0.05 ? -1 : 1);
	}
	else GamePlayer->OnInputMouseY(0); // 감지 영역이 아닌 경우 0으로 반환
}

// 마우스 위치를 가져오는 함수
void AGamePlayerController::GetterMousePosition(float& NormalizeMousePositionX, float& NormalizeMousePositionY)
{
	// 변수 초기화
	float mouseX, mouseY = 0.0f;	// 마우스의 위치
	int32 screenX, screenY = 0; // 디스플레이 크기

	// 마우스 위치
	if (GetMousePosition(mouseX, mouseY)) {
		GetViewportSize(screenX, screenY); // 디스플레이 크기

		// 실제 화면에 비례한 마우스의 위치를 노멀라이즈화
		NormalizeMousePositionX = mouseX / screenX;
		NormalizeMousePositionY = mouseY / screenY;
	}
}

// 마우스를 강제로 움직여 커서를 다시 그리도록 함
void AGamePlayerController::UpdateMousePosition()
{
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);
	SetMouseLocation(MousePosition.X + 1, MousePosition.Y + 1);
}

// 마우스 클릭된 위치에서 에너미 액터를 찾아서 반환하는 함수
AActor* AGamePlayerController::SelectEnemyActorFromMousePoint()
{
	// 마우스 위치값 가져오기
	float mouseX, mouseY = 0;
	GetMousePosition(mouseX, mouseY);

	// 마우스의 스크린 좌표를 월드 좌표로 변환
	FVector WorldLocation;	// 위치
	FVector WorldDirection; // 방향
	DeprojectScreenPositionToWorld(mouseX, mouseY, WorldLocation, WorldDirection);

	// 레이캐스트 설정
	FHitResult hitResult;
	FVector start = WorldLocation; // 시작점
	FVector end = start + (WorldDirection * 10000); // 끝점 (카메라가 보는 방향으로 나가니 시작점부터 수직으로 1만 거리)

	// 레이캐스트 제외목록
	AGamePlayerCharacter* playerCharacter = Cast<AGamePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	TArray<AActor*> actorToIgnore;
	actorToIgnore.Add(this);
	actorToIgnore.Add(playerCharacter);

	// 검사할 오브젝트 채널 설정
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

// 마우스 클릭된 위치를 벡터로 반환하는 함수
FVector AGamePlayerController::GetMouseLeftBtnClickPointVector()
{
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		// 클릭한 위치정보 저장
		FVector HisLocation = HitResult.ImpactPoint;
		return HisLocation;
	}
	return FVector();
}

// 마우스 커서 초기화
void AGamePlayerController::InitializeMouseCursor()
{
	if (CurrentMouseCursor == EMouseCursor::Crosshairs)
		CurrentMouseCursor = EMouseCursor::Default;

	// 왼쪽 클릭 타입 초기화
	MouseLeftBtnActionType = EMouseLeftBtnActionType::None;
	// 마우스 정보 업데이트
	UpdateMousePosition();
}

// CALLBACK_OnLeftMouseBtnPressed() 에 의해 타이머로 실행될 함수
void AGamePlayerController::CALLBACK_OnLeftMouseBtn_ClickHoldPressed()
{
	// 왼쪽 클릭을 홀딩하고 있을 경우 실행
	if (IsValid(hudComponent)) {
		hudComponent->MarqueePressed();
	}
}

// 마우스 왼쪽 버튼 누를 때 입력 처리
void AGamePlayerController::CALLBACK_OnLeftMouseBtnPressed()
{
	// 왼쪽 버튼에 할당된 스킬 액션이 없는 경우
	if (MouseLeftBtnActionType == EMouseLeftBtnActionType::None)
	{	
		// 유닛 클릭, 드래그 함수
		FTimerManager& timerManager = GetWorld()->GetTimerManager();

		// 타이머 변수 초기화
		if (timerManager.IsTimerActive(DisableClickBtnTimer))
			timerManager.ClearTimer(DisableClickBtnTimer);

		// 타이머 설정
		timerManager.SetTimer(
			DisableClickBtnTimer,	// 타이머를 설정할 타이머 핸들러
			FTimerDelegate::CreateUObject(this, &ThisClass::CALLBACK_OnLeftMouseBtn_ClickHoldPressed),	// 실행할 함수 바인딩
			0.07f, false);	// 얼마 후에 실행할 것인가 , 반복여부

		/*
			클릭을 유지하고 있으면 0.5초 후에 CALLBACK_OnLeftMouseBtn_ClickHoldPressed() 가 실행되어
			클릭 섹션을 그리는 작업을 진행하고,
			그 전에 버튼을 때면 Release 함수쪽에서 타이머가 아직 실행할 작업이 있나 확인(IsTimerActive함수를 사용)하는 것으로
			짧은 클릭인지 길게 유지하는 것인지 구분할 수 있다.
			긴 클릭의 경우 타이머에 설정된 함수가 이미 실행되어 IsTimerActive의 결과가 flase가 나올 것이고,
			짧은 클릭의 경우 아직 타이머에 내장된 작업이 실행되기 전이기 때문에 true가 나올 것이다.

			이를 이용해서 Release 함수쪽에서 긴 클릭을 마무리하는 함수를 실행하거나, 짧은 클릭에 사용될 함수를 실행할지 결정한다.
		*/
	}
	// 왼쪽 버튼에 할당된 스킬 액션이 있는 경우
	else {
		switch (MouseLeftBtnActionType)
		{		
			case EMouseLeftBtnActionType::Petroll:				
				// 전역 변수에 저장
				TargetLocation = GetMouseLeftBtnClickPointVector();
				CALLBACK_OnPetrollBtnPressed();
				break;
			case EMouseLeftBtnActionType::Attack:
				// 전역 변수에 저장
				TargetActor = SelectEnemyActorFromMousePoint();
				CALLBACK_OnAttackBtnPressed();
				break;		
		}
	}
}

// 마우스 왼쪽 버튼 땔 때 입력 처리
void AGamePlayerController::CALLBACK_OnLeftMouseBtnReleased()
{
	// 왼쪽 버튼에 할당된 스킬 액션이 없는 경우
	if (MouseLeftBtnActionType == EMouseLeftBtnActionType::None)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();

		if (timerManager.IsTimerActive(DisableClickBtnTimer)) {
			// 타이머 중지
			timerManager.ClearTimer(DisableClickBtnTimer);

			// 작업이 남아있다 = 짧은 클릭이므로 클릭에 해당하는 함수 실행 
			MainContainerWidget->BP_BottomContainerWidget->FindSelectActorToMouseClick();
		}
		else {
			// 남은 작업이 없다 = 긴 클릭이며, 이미 해당하는 함수를 실행했기 때문에
			// Release에서는 이전에 실행된 Press 함수에 반대되는 Relased 함수를 실행하는 것으로 마무리하게 된다.
			if (IsValid(hudComponent)) {
				hudComponent->MarqueeReleased();
			}
		}
	}
}

// 마우스 오른쪽 버튼 입력
void AGamePlayerController::CALLBACK_OnRightMouseBtnPressed()
{
	// 히트 반환 변수 선언
	FHitResult HitResult;

	// 마우스 커서 초기화
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

// 공격 버튼
void AGamePlayerController::CALLBACK_OnAttackBtnPressed()
{
	// 예외처리
	if (!IsValid(MainContainerWidget)) return;

	// 먼저 눌렸는지 확인 및 마우스 커서 변경
	if (MouseLeftBtnActionType != EMouseLeftBtnActionType::Attack)
	{
		// 사용될 액터 변수 초기화
		TargetActor = nullptr;

		// 마우스 커서 변경
		CurrentMouseCursor = EMouseCursor::Crosshairs;
		UpdateMousePosition();

		// 마우스 왼쪽 버튼 액션 타입 설정
		MouseLeftBtnActionType = EMouseLeftBtnActionType::Attack;
	}
	// 한 번 공격이 입력된 후, 다시 한번 공격스킬을 누른 경우를 방지
	else if(MouseLeftBtnActionType == EMouseLeftBtnActionType::Attack && TargetActor != nullptr)
	{
		// 마우스 커서 변경
		CurrentMouseCursor = EMouseCursor::Default;
		UpdateMousePosition();

		// 패트롤 함수 실행
		MainContainerWidget->BP_BottomContainerWidget->TargetActor = TargetActor;
		MainContainerWidget->BP_BottomContainerWidget->BP_BottomButtonWidget_Attack->CALLBACK_OnClickCommonSKillFunc();

		// 마우스 왼쪽 버튼 액션 타입 초기화
		MouseLeftBtnActionType = EMouseLeftBtnActionType::None;
	}
	
}

// 정찰 버튼
void AGamePlayerController::CALLBACK_OnPetrollBtnPressed()
{
	// 예외처리
	if (!IsValid(MainContainerWidget)) return;

	// 먼저 눌렸는지 확인 및 마우스 커서 변경
	if (MouseLeftBtnActionType != EMouseLeftBtnActionType::Petroll)
	{
		// 사용될 백터 변수 초기화
		TargetLocation = FVector::ZeroVector;

		// 마우스 커서 변경
		CurrentMouseCursor = EMouseCursor::Crosshairs;
		UpdateMousePosition();
	
		// 마우스 왼쪽 버튼 액션 타입 설정
		MouseLeftBtnActionType = EMouseLeftBtnActionType::Petroll;
	}
	// 한 번 정찰버튼을 누른 후, 클릭하지 않고 다시 스킬을 누른 경우의 예외처리
	else if(MouseLeftBtnActionType == EMouseLeftBtnActionType::Petroll && TargetLocation != FVector::ZeroVector){
		// 마우스 커서 변경
		CurrentMouseCursor = EMouseCursor::Default;
		UpdateMousePosition();

		// 패트롤 함수 실행
		MainContainerWidget->BP_BottomContainerWidget->TargetLocation = TargetLocation;
		MainContainerWidget->BP_BottomContainerWidget->BP_BottomButtonWidget_Petroll->CALLBACK_OnClickCommonSKillFunc();

		// 마우스 왼쪽 버튼 액션 타입 초기화
		MouseLeftBtnActionType = EMouseLeftBtnActionType::None;
	}
}

// 홀드 버튼
void AGamePlayerController::CALLBACK_OnHoldBtnPressed()
{
	// 예외처리
	if (!IsValid(MainContainerWidget)) return;
	// 마우스 커서 초기화
	if (CurrentMouseCursor == EMouseCursor::Crosshairs) InitializeMouseCursor();

	MainContainerWidget->BP_BottomContainerWidget->BP_BottomButtonWidget_Hold->CALLBACK_OnClickCommonSKillFunc();
}

// 판매 버튼
void AGamePlayerController::CALLBACK_OnSellPressed()
{
	// 예외처리
	if (!IsValid(MainContainerWidget)) return;	
	// 마우스 커서 초기화
	if (CurrentMouseCursor == EMouseCursor::Crosshairs) InitializeMouseCursor();

	MainContainerWidget->BP_BottomContainerWidget->BP_BottomButtonWidget_Sell->CALLBACK_OnClickCommonSKillFunc();
}

void AGamePlayerController::SetHudComponent(ARtsMarqueeHUD* HudComponent)
{
	hudComponent = HudComponent;
}
