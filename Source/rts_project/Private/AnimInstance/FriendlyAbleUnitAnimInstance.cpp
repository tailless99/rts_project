#include "AnimInstance/FriendlyAbleUnitAnimInstance.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "rts_project/rts_project.h"


UFriendlyAbleUnitAnimInstance::UFriendlyAbleUnitAnimInstance()
{
	// �ʱ�ȭ
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
	// �ִϸ��̼� ����
	PlayerUnit->StopAnimMontage(CurrentMontage);
	
	// Ȥ�� �ʱ�ȭ �ȉ��� ���� ������ ��ó��
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
	// ���� �� �̺�Ʈ �߻�
	OnAttackFinished.Broadcast();
}

void UFriendlyAbleUnitAnimInstance::AnimNotify_StartAttackAreaCheck()
{
	// ���� ���� �˻� ���� �̺�Ʈ �߻�
	OnAttackAreaCheckStarted.Broadcast();
}

void UFriendlyAbleUnitAnimInstance::AnimNotify_FinishAttackAreaCheck()
{
	// ���� ���� �˻� �� �̺�Ʈ �߻�
	OnAttackAreaCheckFinished.Broadcast();
}
