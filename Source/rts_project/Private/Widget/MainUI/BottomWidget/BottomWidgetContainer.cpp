#include "Widget/MainUI/BottomWidget/BottomWidgetContainer.h"
#include "Widget/MainUI/BottomWidget/BottomButtonWidget.h"
#include "Widget/MainUI/Main_Container_Widget.h"
#include "SlateCore.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Controller/FriendlyUnit/FriendlyUnitController.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/HorizontalBoxSlot.h"

#include "Components/Image.h"
#include "HUD/RtsMarqueeHUD.h"
#include "Actor/PlayerCharacter/GamePlayerCharacter.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"
#include "DataTableRow/FriendlyUnitDataTableRow.h"
#include "Enum/SkillBtnType.h"
#include "rts_project/rts_project.h"


#define SELECT_UNIT_MAXCOUNT 12
#define WISP_UNITCODE TEXT("0003")

UBottomWidgetContainer::UBottomWidgetContainer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_UNITINFO(
		TEXT("/Script/Engine.DataTable'/Game/DataTableRow/DT_UnitDataRow.DT_UnitDataRow'"));

	if (DT_UNITINFO.Succeeded())
	{
		unitInfo = DT_UNITINFO.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> IMG_BLACKBACKGROUND(
		TEXT("/Script/Engine.Texture2D'/Game/Assets/Image/Widget/MainUi/BG_black.BG_black'"));

	if (IMG_BLACKBACKGROUND.Succeeded())
	{
		SkillBlackBackground = IMG_BLACKBACKGROUND.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> IMG_ATTACK(
		TEXT("/Script/Engine.Texture2D'/Game/Assets/Image/Icons/Attack.Attack'"));

	if (IMG_ATTACK.Succeeded())
	{
		AttackBtnImgTexture = IMG_ATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> IMG_HOLD(
		TEXT("/Script/Engine.Texture2D'/Game/Assets/Image/Icons/Hold.Hold'"));

	if (IMG_HOLD.Succeeded())
	{
		HoldBtnImgTexture = IMG_HOLD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> IMG_PETROLL(
		TEXT("/Script/Engine.Texture2D'/Game/Assets/Image/Icons/Petrol.Petrol'"));

	if (IMG_PETROLL.Succeeded())
	{
		PetrollBtnImgTexture = IMG_PETROLL.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> IMG_SELL(
		TEXT("/Script/Engine.Texture2D'/Game/Assets/Image/Icons/Sell.Sell'"));

	if (IMG_SELL.Succeeded())
	{
		SellBtnImgTexture = IMG_SELL.Object;
	}
}


void UBottomWidgetContainer::SetHudComponent(ARtsMarqueeHUD* hudComponent)
{
	HudComponent = hudComponent;
}

// 초기화
void UBottomWidgetContainer::InitializeWidget()
{
	// 플레이어 컨트롤러 가져오기
	playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());

	// 초기화
	MainUnitHp->SetText(FText::GetEmpty());
	MainUnitMp->SetText(FText::GetEmpty());
	AttackPoint->SetText(FText::GetEmpty());
	Defence_Info_Box->SetVisibility(ESlateVisibility::Collapsed);
	UnitInfo_Border->SetVisibility(ESlateVisibility::Collapsed);
	MultiSelect_Border->SetVisibility(ESlateVisibility::Collapsed);
	MainUnitImg->SetBrushFromTexture(SkillBlackBackground);

	// 버튼 이미지 초기화
	InitializeUISkillImg();

	for (int32 i = 0; i < MultiSelect_Box->GetChildrenCount(); i++)
	{	// 다중 선택 뷰의 자식들을 비활성화
		MultiSelect_Box->GetChildAt(i)->SetVisibility(ESlateVisibility::Hidden);
	}
	
	// 스킬 버튼 초기화
	BP_BottomButtonWidget01->InitializeWidget();
	BP_BottomButtonWidget02->InitializeWidget();
	BP_BottomButtonWidget03->InitializeWidget();
	BP_BottomButtonWidget04->InitializeWidget();
	BP_BottomButtonWidget05->InitializeWidget();
	BP_BottomButtonWidget06->InitializeWidget();
	BP_BottomButtonWidget07->InitializeWidget();
	BP_BottomButtonWidget08->InitializeWidget();
	
	// 공통 스킬 버튼 바인딩
	BP_BottomButtonWidget_Attack->InitializeWidget_CommonSkill();
	BP_BottomButtonWidget_Hold->InitializeWidget_CommonSkill();
	BP_BottomButtonWidget_Petroll->InitializeWidget_CommonSkill();
	BP_BottomButtonWidget_Sell->InitializeWidget_CommonSkill();

	// 멀티 유닛 박스 버튼 바인딩
	MultiUnitBtn01->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn01);
	MultiUnitBtn02->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn02);
	MultiUnitBtn03->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn03);
	MultiUnitBtn04->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn04);
	MultiUnitBtn05->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn05);
	MultiUnitBtn06->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn06);
	MultiUnitBtn07->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn07);
	MultiUnitBtn08->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn08);
	MultiUnitBtn09->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn09);
	MultiUnitBtn10->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn10);
	MultiUnitBtn11->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn11);
	MultiUnitBtn12->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_MultiUnitBtn12);
}

// 선택된 유닛을 UI에 표기하도록 설정
void UBottomWidgetContainer::SetUnitInfo()
{
	// Hud 컴포넌트가 유효하지 않다면 함수 호출 종료
	if (!IsValid(HudComponent))	return;

	// 리스트가 업데이트된게 아니라면 초기화 진행
	if(!IsUpdateSelectUnitsArray)
		// 변수 초기화 및 할당
		UpdateSelectUnit();
	
	// 선택된 유닛이 없다면 함수 호출 종료
	if (SelectUnits.Num() <= 0)
	{
		UnitInfo_Border->SetVisibility(ESlateVisibility::Collapsed);
		MultiSelect_Border->SetVisibility(ESlateVisibility::Collapsed);
		MainUnitImg->SetBrushFromTexture(SkillBlackBackground);
		InitializeUISkillImg();
		MainUnitHp->SetText(FText::GetEmpty());
		MainUnitMp->SetText(FText::GetEmpty());
		AttackPoint->SetText(FText::GetEmpty());
		Defence_Info_Box->SetVisibility(ESlateVisibility::Collapsed);
	}
	// 선택된 유닛이 하나일 때
	else if (SelectUnits.Num() == 1) {
		UnitInfo_Border->SetVisibility(ESlateVisibility::Visible);
		MultiSelect_Border->SetVisibility(ESlateVisibility::Collapsed);
		Defence_Info_Box->SetVisibility(ESlateVisibility::Visible);

		// 위습이 아닐 경우, 체크
		if (SelectUnits[0]->UnitCode != WISP_UNITCODE)
		{
			FFriendlyUnitDataTableRow* SelectUnitInfo = GetUnitDataTableRow(SelectUnits[0]->UnitCode);
			
			// Bottom UI SkilArea 버튼 셋팅
			SetUnitSkillBtnLink(SelectUnitInfo);

			// 캐릭터 이미지 셋팅
			MainUnitImg->SetBrushFromTexture(SelectUnitInfo->CharacterImage);

			// 유닛 정보 셋팅
			if (SelectUnitInfo->Hp != 0) {
				FText unitHp = FText::FromString(FString::Printf(TEXT("%.f / %.f"), SelectUnits[0]->CurrentHp, SelectUnitInfo->Hp));
				MainUnitHp->SetText(unitHp);
			}

			if (SelectUnitInfo->Mp != 0) {
				FText unitMp = FText::FromString(FString::Printf(TEXT("%.f / %.f"), SelectUnits[0]->CurrentMp, SelectUnitInfo->Mp));
				MainUnitMp->SetText(unitMp);
			}

			if (SelectUnitInfo->MaxAttackPoint != 0 || SelectUnitInfo->MinAttackPoint != 0) {
				FText unitAttacPoint = FText::FromString(FString::Printf(TEXT("%.f ~ %.f"), SelectUnitInfo->MinAttackPoint, SelectUnitInfo->MaxAttackPoint));
				AttackPoint->SetText(unitAttacPoint);
			}
		}
	}
	// 다수의 유닛이 선택될 때
	else
	{	
		// 반복문 카운트 변수
		int Count = 0;

		// 그리드 박스 이미지 셋팅
		for (AFriendlyAbleCharacterBase* UnitActor : SelectUnits)
		{			
			FFriendlyUnitDataTableRow* SelectUnitInfo = GetUnitDataTableRow(UnitActor->UnitCode);

			MultiSelect_Box->GetChildAt(Count)->SetVisibility(ESlateVisibility::Visible);
			//UImage* childImage = Cast<UImage>(Cast<UBorder>(MultiSelect_Box->GetChildAt(Count))->GetChildAt(0));
			UImage* childImage = Cast<UImage>(Cast<UBorder>(Cast<UButton>(MultiSelect_Box->GetChildAt(Count))->GetChildAt(0))->GetChildAt(0));
						
			// 유닛 이미지 변경
			if (SelectUnitInfo != NULL && IsValid(SelectUnitInfo->CharacterImage))	
				childImage->SetBrushFromTexture(SelectUnitInfo->CharacterImage);
			
			Count++;
		}

		// 위습이 아닌 첫 번째 유닛을 찾기
		for(AFriendlyAbleCharacterBase* selectUnitInfo : SelectUnits){			
			if (selectUnitInfo->UnitCode != WISP_UNITCODE) {
				FFriendlyUnitDataTableRow* UnitInfo = GetUnitDataTableRow(selectUnitInfo->UnitCode);				
				SetUnitSkillBtnLink(UnitInfo); // Bottom UI SkilArea 버튼 셋팅

				// 캐릭터 이미지 셋팅
				MainUnitImg->SetBrushFromTexture(UnitInfo->CharacterImage);

				// 유닛 정보 셋팅
				if (UnitInfo->Hp != 0) {
					FText unitHp = FText::FromString(FString::Printf(TEXT("%.f / %.f"), SelectUnits[0]->CurrentHp, UnitInfo->Hp));
					MainUnitHp->SetText(unitHp);
				}

				if (UnitInfo->Mp != 0) {
					FText unitMp = FText::FromString(FString::Printf(TEXT("%.f / %.f"), SelectUnits[0]->CurrentMp, UnitInfo->Mp));
					MainUnitMp->SetText(unitMp);
				}
				break;
			}
		}

		UnitInfo_Border->SetVisibility(ESlateVisibility::Collapsed);
		MultiSelect_Border->SetVisibility(ESlateVisibility::Visible);
	}
	
	// 사용 후 변수 초기화
	IsUpdateSelectUnitsArray = false;
}

// 버튼위젯에서 사용되는 버튼을 리스트에 등록하는 함수
void UBottomWidgetContainer::AddBtnArray(UBottomButtonWidget* myBtnWidget)
{
	SkilBtns.Add(myBtnWidget);
}

// 유닛에 대한 정보를 넘겨줌
FFriendlyUnitDataTableRow* UBottomWidgetContainer::GetUnitDataTableRow(FName unitCode)
{
	FString contextString;
	FFriendlyUnitDataTableRow* SelectUnitInfo = unitInfo->FindRow<FFriendlyUnitDataTableRow>(unitCode, contextString);
	return SelectUnitInfo;
}

// 사용 가능한 유닛만 리스트에 추가
void UBottomWidgetContainer::CanActionUnitCheck()
{
	// 명령 가능한 유닛목록을 초기화
	CanActionUnits.Empty();
	
	for (AFriendlyAbleCharacterBase* actor : SelectUnits)
		if (actor->UnitCode != WISP_UNITCODE) CanActionUnits.Add(actor);
}

// 공통 사용 함수가 클릭될 때 실행되는 함수
void UBottomWidgetContainer::OnClickCommonSkillButtonAction(SkillBtnType btnType)
{
	// 예외 처리
	CanActionUnitCheck();
	if (CanActionUnits.Num() == 0) return;

	choiceSkillButton(btnType);
}

void UBottomWidgetContainer::OnSkillBtnHover(int32 btnCode)
{
	if (SelectUnits.Num() <= 0 && SelectUnits[0]->UnitCode == WISP_UNITCODE) return;

	// 사용되는 변수
	UMain_Container_Widget* MainWidget = playerController->MainContainerWidget;
	FFriendlyUnitDataTableRow* unitDataInfo = GetUnitDataTableRow(SelectUnits[0]->UnitCode);
	
	// 문자열 변수 초기화
	FName Skillname = FName();
	FString SkillTooltip = FString();

	switch (btnCode)
	{
		case 1:
			Skillname = unitDataInfo->SkillBtn01.SkillName;
			SkillTooltip = unitDataInfo->SkillBtn01.Skill_ToolTip;
			break;
		case 2:
			Skillname = unitDataInfo->SkillBtn01.SkillName;
			SkillTooltip = unitDataInfo->SkillBtn01.Skill_ToolTip;
			break;
		case 3:
			Skillname = unitDataInfo->SkillBtn01.SkillName;
			SkillTooltip = unitDataInfo->SkillBtn01.Skill_ToolTip;
			break;
		case 4:
			Skillname = unitDataInfo->SkillBtn04.SkillName;
			SkillTooltip = unitDataInfo->SkillBtn04.Skill_ToolTip;
			break;
		case 5:
			Skillname = unitDataInfo->SkillBtn05.SkillName;
			SkillTooltip = unitDataInfo->SkillBtn05.Skill_ToolTip;
			break;
		case 6:
			Skillname = unitDataInfo->SkillBtn06.SkillName;
			SkillTooltip = unitDataInfo->SkillBtn06.Skill_ToolTip;
			break;
		case 7:
			Skillname = unitDataInfo->SkillBtn07.SkillName;
			SkillTooltip = unitDataInfo->SkillBtn07.Skill_ToolTip;
			break;
		case 8:
			Skillname = unitDataInfo->SkillBtn08.SkillName;
			SkillTooltip = unitDataInfo->SkillBtn08.Skill_ToolTip;
			break;
		case 9:
			Skillname = FName(TEXT("공격 [ A ]"));
			SkillTooltip = FString(TEXT("대상을 지정하여 공격 명령을 내린다."));
			break;
		case 10:
			Skillname = FName(TEXT("홀드 [ H ]"));
			SkillTooltip = FString(TEXT("대상을 현재 위치에 고정하여 움직이지 않게 한다."));
			break;
		case 11:
			Skillname = FName(TEXT("정찰 [ P ]"));
			SkillTooltip = FString(TEXT("정찰 지점을 클릭하여 현재 위치와 정찰 지점을 순회하게 한다."));
			break;
		case 12:
			Skillname = FName(TEXT("판매 [ S ]"));
			SkillTooltip = FString(TEXT("유닛을 판매하여 골드를 얻는다."));
			break;
	}

	MainWidget->ShowSkillInfoTextBox(Skillname, SkillTooltip);
}

void UBottomWidgetContainer::OnSkillBtnUnHover()
{
	UMain_Container_Widget* MainWidget = playerController->MainContainerWidget;
	MainWidget->UnShowSkillInfoTextBox();
}

// 특정 요소 배열에서 제거
void UBottomWidgetContainer::RemoveArrayAt(AFriendlyAbleCharacterBase* removeActor)
{
	SelectUnits.Remove(removeActor);	// 요소 제거
	HudComponent->UpdateSelectAreaUnit(removeActor);

	// 리스트가 업데이트 되었음을 나타냄 초기화를 진행하지 않게끔 변수값을 바꿈
	// SetUnitInfo 에서 초기화를 진행하지 않게함
	IsUpdateSelectUnitsArray = true;	

	SetUnitInfo();	// UI 변경 정보 업데이트
}

// 클릭된 액터를 찾는다
void UBottomWidgetContainer::FindSelectActorToMouseClick()
{
	// 마우스 위치값 가져오기
	float mouseX, mouseY = 0;
	playerController->GetMousePosition(mouseX, mouseY);

	// 마우스의 스크린 좌표를 월드 좌표로 변환
	FVector WorldLocation;	// 위치
	FVector WorldDirection; // 방향
	playerController->DeprojectScreenPositionToWorld(mouseX, mouseY, WorldLocation, WorldDirection);

	// 레이캐스트 설정
	FHitResult hitResult;
	FVector start = WorldLocation; // 시작점
	FVector end = start + (WorldDirection * 10000); // 끝점 (카메라가 보는 방향으로 나가니 시작점부터 수직으로 1만 거리)
	
	// 트레이스에 사용될 변수
	TArray<AActor*> actorToIgnore;	// 레이캐스트 제외목록
	AGamePlayerCharacter* playerCharacter = Cast<AGamePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	actorToIgnore.Add(playerCharacter);

	bool bHit = UKismetSystemLibrary::SphereTraceSingleByProfile(
		this, start, end, 100.0f,
		TEXT("Pawn"), false,
		actorToIgnore, EDrawDebugTrace::Type::None,
		hitResult, true);

	if (bHit)
	{
		AFriendlyAbleCharacterBase* SelectActor = Cast<AFriendlyAbleCharacterBase>(hitResult.GetActor());
		if (SelectActor)
		{
			// 리스트 초기화 및 재할당
			SelectUnits.Empty();
			SelectUnits.Add(SelectActor);

			// 리스트가 업데이트 되었다고 변수를 설정 후, 다시 리스트의 정보를 전달
			IsUpdateSelectUnitsArray = true;
			SetUnitInfo();
			HudComponent->SetGrabSelectedUnit(SelectUnits);
		}
	}
	else
	{	
		// 아무것도 잡히지 않았다면 초기화
		SelectUnits.Empty();

		// 리스트가 업데이트 되었다고 변수를 설정 후, 다시 리스트의 정보를 전달
		IsUpdateSelectUnitsArray = true;
		SetUnitInfo();
		HudComponent->SetGrabSelectedUnit(SelectUnits);
	}
}

// 스킬 버튼이 클릭되면 실행되는 함수
void UBottomWidgetContainer::OnClickButtonAction(SkillBtnType btnType)
{
	// 유닛에 대한 정보가 정상적으로 로드가 되고, 해당 유닛이 스킬을 가지고 있을 때
	AFriendlyAbleCharacterBase* PlayerUnit = SelectUnits[0];
	SelectUnits.Empty();
	SelectUnits.Add(PlayerUnit);
	CanActionUnitCheck();

	// 리스트가 업데이트 되었다고 변수를 설정 후, 다시 리스트의 정보를 전달
	IsUpdateSelectUnitsArray = true;
	SetUnitInfo();
	HudComponent->SetGrabSelectedUnit(SelectUnits);

	choiceSkillButton(btnType);
}

// 유닛의 스킬을 UI에 버튼에 적용
void UBottomWidgetContainer::SetUnitSkillBtnLink(FFriendlyUnitDataTableRow* selectUnitInfo)
{
	if (!selectUnitInfo)	return;

	// 스킬 버튼 초기화
	InitializeUISkillImg();

	// 공통 행동 4개 셋팅
	// 공격, 홀드, 정찰, 판매
	if(selectUnitInfo->UnitName != TEXT("Shop"))
	{
		UImage* AttackBtnImg = Cast<UImage>(BP_BottomButtonWidget_Attack->GetChildImg());
		AttackBtnImg->SetBrushFromTexture(AttackBtnImgTexture);

		UImage* HoldBtnImg = Cast<UImage>(BP_BottomButtonWidget_Hold->GetChildImg());
		HoldBtnImg->SetBrushFromTexture(HoldBtnImgTexture);

		UImage* PetrollBtnImg = Cast<UImage>(BP_BottomButtonWidget_Petroll->GetChildImg());
		PetrollBtnImg->SetBrushFromTexture(PetrollBtnImgTexture);

		UImage* SellBtnImg = Cast<UImage>(BP_BottomButtonWidget_Sell->GetChildImg());
		SellBtnImg->SetBrushFromTexture(SellBtnImgTexture);
	}

	// 유닛별 스킬 8개 셋팅
	if(selectUnitInfo->SkillBtn01.IsUseSkill)
	{
		UImage* childImage = BP_BottomButtonWidget01->GetChildImg();
		childImage->SetBrushFromTexture(selectUnitInfo->SkillBtn01.Skill_Img);
	}
	if (selectUnitInfo->SkillBtn02.IsUseSkill)
	{
		UImage* childImage = BP_BottomButtonWidget02->GetChildImg();
		childImage->SetBrushFromTexture(selectUnitInfo->SkillBtn02.Skill_Img);
	}
	if (selectUnitInfo->SkillBtn03.IsUseSkill)
	{
		UImage* childImage = BP_BottomButtonWidget03->GetChildImg();
		childImage->SetBrushFromTexture(selectUnitInfo->SkillBtn03.Skill_Img);
	}
	if (selectUnitInfo->SkillBtn04.IsUseSkill)
	{
		UImage* childImage = BP_BottomButtonWidget04->GetChildImg();
		childImage->SetBrushFromTexture(selectUnitInfo->SkillBtn04.Skill_Img);
	}
	if (selectUnitInfo->SkillBtn05.IsUseSkill)
	{
		UImage* childImage = BP_BottomButtonWidget05->GetChildImg();
		childImage->SetBrushFromTexture(selectUnitInfo->SkillBtn05.Skill_Img);
	}
	if (selectUnitInfo->SkillBtn06.IsUseSkill)
	{
		UImage* childImage = BP_BottomButtonWidget06->GetChildImg();
		childImage->SetBrushFromTexture(selectUnitInfo->SkillBtn06.Skill_Img);
	}
	if (selectUnitInfo->SkillBtn07.IsUseSkill)
	{
		UImage* childImage = BP_BottomButtonWidget07->GetChildImg();
		childImage->SetBrushFromTexture(selectUnitInfo->SkillBtn07.Skill_Img);
	}
	if (selectUnitInfo->SkillBtn08.IsUseSkill)
	{
		UImage* childImage = BP_BottomButtonWidget08->GetChildImg();
		childImage->SetBrushFromTexture(selectUnitInfo->SkillBtn08.Skill_Img);
	}
}

// UI의 스킬버튼의 이미지를 초기화
void UBottomWidgetContainer::InitializeUISkillImg()
{
	for (UBottomButtonWidget* myWidget : SkilBtns)
	{
		UImage* skillBtn = myWidget->GetChildImg();
		skillBtn->SetBrushFromTexture(SkillBlackBackground);
	}	
}

// 선택된 유닛 목록 업데이트
void UBottomWidgetContainer::UpdateSelectUnit()
{
	SelectUnits.Empty();
	SelectUnits = HudComponent->GetGrabSelectedUnit();

	for (int i = 0; i < SELECT_UNIT_MAXCOUNT; i++)
		MultiSelect_Box->GetChildAt(i)->SetVisibility(ESlateVisibility::Hidden);
}

// 스킬 버튼 타입에 따라 처리
// * 버튼 클릭 이벤트 핵심 처리 함수 *
void UBottomWidgetContainer::choiceSkillButton(SkillBtnType skillType)
{
	UpdateSelectUnit();
	if (CanActionUnits.Num() <= 0)	return;


	// 스킬 타입에 따라 다르게 행동
	switch (skillType)
	{
		case SkillBtnType::Attack :
			for (AFriendlyAbleCharacterBase* unit : CanActionUnits)
				unit->UnitAttack(TargetActor);
			TargetActor = nullptr;
			break;

		case SkillBtnType::Hold :
			for (AFriendlyAbleCharacterBase* unit : CanActionUnits) {
				unit->UnitHold();	}
			break;

		case SkillBtnType::Petroll :			
			for (AFriendlyAbleCharacterBase* unit : CanActionUnits)
				unit->UnitPetroll(TargetLocation);
			TargetLocation = FVector::ZeroVector;
			break;

		case SkillBtnType::Sell :
			if(CanActionUnits.Num() > 0)
			{
				AFriendlyAbleCharacterBase* unit = CanActionUnits[0];
				unit->UnitSell();
				RemoveArrayAt(unit);
			}
			break;

		case SkillBtnType::skill01:
			SelectUnits[0]->SkillBtn01();
			break;

		case SkillBtnType::skill02:
			SelectUnits[0]->SkillBtn02();
			break;

		case SkillBtnType::skill03:
			SelectUnits[0]->SkillBtn03();
			break;

		case SkillBtnType::skill04:
			SelectUnits[0]->SkillBtn04();
			break;

		case SkillBtnType::skill05:
			SelectUnits[0]->SkillBtn05();
			break;

		case SkillBtnType::skill06:
			SelectUnits[0]->SkillBtn06();
			break;

		case SkillBtnType::skill07:
			SelectUnits[0]->SkillBtn07();
			break;

		case SkillBtnType::skill08:
			SelectUnits[0]->SkillBtn08();
			break;
	}
}

/// <summary>
/// 멀티 유닛 박스의 공통 사용 함수
/// 
/// 사용된 버튼을 특정하기 위해 따로 분류하였고, 
/// SelectUnits Array의 구성을 초기화하고, 선택된 유닛의 대한 정보만 남긴다
/// </summary>
void UBottomWidgetContainer::CALLBACK_MultiUnitBoxSelect(UButton* selectMultiUnitBtn)
{
	// 버튼 유효성 체크
	if (!selectMultiUnitBtn) return;

	// 선택된 유닛을 임시로 저장할 변수 선언
	AFriendlyAbleCharacterBase* temp = nullptr;

	// 버튼이 눌린 타입의 이름을 가져와, 몇 번째 유닛을 선택한건지 분류 후 임시저장 변수에 저장
	if (selectMultiUnitBtn->GetName() == "MultiUnitBtn01")
		temp = SelectUnits[0];
	else if (selectMultiUnitBtn->GetName() == "MultiUnitBtn02")
		temp = SelectUnits[1];
	else if (selectMultiUnitBtn->GetName() == "MultiUnitBtn03")
		temp = SelectUnits[2];
	else if (selectMultiUnitBtn->GetName() == "MultiUnitBtn04")
		temp = SelectUnits[3];
	else if (selectMultiUnitBtn->GetName() == "MultiUnitBtn05")
		temp = SelectUnits[4];
	else if (selectMultiUnitBtn->GetName() == "MultiUnitBtn06")
		temp = SelectUnits[5];		
	else if (selectMultiUnitBtn->GetName() == "MultiUnitBtn07")			
		temp = SelectUnits[6];
	else if (selectMultiUnitBtn->GetName() == "MultiUnitBtn08")
		temp = SelectUnits[7];
	else if (selectMultiUnitBtn->GetName() == "MultiUnitBtn09")
		temp = SelectUnits[8];
	else if (selectMultiUnitBtn->GetName() == "MultiUnitBtn10")
		temp = SelectUnits[9];
	else if (selectMultiUnitBtn->GetName() == "MultiUnitBtn11")
		temp = SelectUnits[10];
	else if (selectMultiUnitBtn->GetName() == "MultiUnitBtn12")
		temp = SelectUnits[11];

	// 리스트 초기화 및 재할당
	SelectUnits.Empty();
	SelectUnits.Add(temp);
	HudComponent->SetGrabSelectedUnit(SelectUnits);

	// 리스트가 업데이트 되었다고 변수를 설정 후, 다시 리스트의 정보를 전달
	IsUpdateSelectUnitsArray = true;
	SetUnitInfo();
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn01()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn01);
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn02()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn02);
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn03()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn03);
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn04()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn04);
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn05()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn05);
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn06()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn06);
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn07()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn07);
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn08()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn08);
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn09()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn09);
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn10()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn10);
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn11()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn11);
}

void UBottomWidgetContainer::CALLBACK_MultiUnitBtn12()
{
	CALLBACK_MultiUnitBoxSelect(MultiUnitBtn12);
}