#include "AnimInstance/FriendlyAbleUnitAnimInstance.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "rts_project/rts_project.h"


UFriendlyAbleUnitAnimInstance::UFriendlyAbleUnitAnimInstance()
{
	// 초기화
	IsAttack = false;
	Speed = 0.0f;
}

void UFriendlyAbleUnitAnimInstance::PlayAnimMontage(UAnimMontage* montageAsset)
{
	OnAttack();
	CurrentMontage = montageAsset;
	float result = PlayerUnit->PlayAnimMontage(montageAsset, 1.0f);
}

void UFriendlyAbleUnitAnimInstance::StopAnimMontage()
{
	// 애니메이션 정지
	PlayerUnit->StopAnimMontage(CurrentMontage);
	
	// 혹여 초기화 안됬을 것을 생각해 후처리
	CurrentMontage = nullptr;
	IsAttack = false;
	AnimNotify_FinishAttackAreaCheck();
	AnimNotify_OnAttackFinished();
}

void UFriendlyAbleUnitAnimInstance::OnInitializeAinmInstance(AFriendlyAbleCharacterBase* playerUnit)
{
	PlayerUnit = playerUnit;
}

void UFriendlyAbleUnitAnimInstance::OnUpdateSpeed(float NewSpeed)
{
	Speed = NewSpeed;
}

void UFriendlyAbleUnitAnimInstance::OnAttack()
{
	IsAttack = true;
}

void UFriendlyAbleUnitAnimInstance::AnimNotify_OnAttackFinished()
{
	// 공격 끝 이벤트 발생
	OnAttackFinished.Broadcast();
}

void UFriendlyAbleUnitAnimInstance::AnimNotify_StartAttackAreaCheck()
{
	// 공격 영역 검사 시작 이벤트 발생
	OnAttackAreaCheckStarted.Broadcast();
}

void UFriendlyAbleUnitAnimInstance::AnimNotify_FinishAttackAreaCheck()
{
	// 공격 영역 검사 끝 이벤트 발생
	OnAttackAreaCheckFinished.Broadcast();
}
