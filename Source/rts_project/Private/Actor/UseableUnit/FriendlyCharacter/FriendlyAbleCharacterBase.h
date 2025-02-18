// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SkillFuncInterface.h"
#include "DataTableRow/FriendlyUnitDataTableRow.h"
#include "FriendlyAbleCharacterBase.generated.h"



UCLASS()
class AFriendlyAbleCharacterBase : public ACharacter, public ISkillFuncInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* CapsuleCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* SelectionDecal;

	UPROPERTY()
	class AFriendlyUnitController* PlayerController;

	UPROPERTY()
	class UFriendlyAbleUnitAnimInstance* AnimInstance;

	// ����� ���� �ڵ带 ��Ÿ���ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "����")
	FName UseWeaponCode;

	UPROPERTY()
	class UFriendlyUnitAttackComponentBase* AttackComponent;

	UPROPERTY()
	bool CanMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UnitCode;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxMp;

	UPROPERTY()
	float CurrentMp;

	UPROPERTY()
	float MinAttackPoint;

	UPROPERTY()
	float MaxAttackPoint;

	UPROPERTY()
	float AttackRange;

	UPROPERTY()
	float AttackDelay;

	UPROPERTY()
	float SellPrice;

	// ���̺� ������Ʈ
	UPROPERTY()
	class UWaveControllerComponent* waveComponent;

	// �� Ž�� ����
	UPROPERTY()
	float OverlapRadius;

	// Ž���� �� ����
	UPROPERTY()
	AActor* TargetOverlapActor;
	
	UAnimMontage* attackMontage;

	// �ٸ� �ൿ�� ��������� üũ�մϴ�.
	bool IsUseAction;


protected:
	// ������ ������ ���� ����Ʈ
	TArray<AActor*> MixtureUnits;

	// ���������� ������ �ð�
	float LastAttackTime;
	
	// Ž�� ������ �Ѿ� ���͸� ã�� ���� bool�� ����.
	// UnitAttack�Լ��� Ȱ��ȭ �� Ÿ�̸ӷ� �ʱ�ȭ��Ų��.
	bool IsSelectAttack;

public:
	AFriendlyAbleCharacterBase();

	void InitializeUnitBase(
		const FFriendlyUnitDataTableRow* unitInfo,
		class AFriendlyUnitController* playerController,
		FName unitCode);

	// ���� ��û �Լ�
	virtual bool OnAttackRequested(FName weaponCode);

	// ������ ���� ��� ȣ��Ǵ� �Լ�
	virtual void OnAttackFinished();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �� Ž�� ����
	void AutoSurchEnemy();

	// ������ �� �ڵ� ����
	void AutoAttack();

	// Ÿ���� �ٶ󺸰� �ϴ� �Լ�
	void LockAtTarget(float dt, float roattionSpeed);

public:
	// ���� �Ǿ����� ó��
	void SelectUnit();
	
	// ������ ������ �� ó��
	void DeSelectUnit();

	// ���� ������ ó��
	void UnitMoveCommand(FVector targetPoint);

	// �� ���� Ž��
	void CheckEnemyOverlap();

	// Ÿ�̸ӷ� ȣ��Ǵ� �Լ�
	// �Ͻ������� �о��� Ž�������� �ٽ� �ǵ�����.
	void RollBackOverlapRange();

	// ������ ������ ��� ȣ��Ǵ� �Լ�
	void CALLBACK_OnAttackFinished();

public:
	// ����
	UFUNCTION()
	void UnitAttack(AActor* targetActor);

	// ��ġ ����
	UFUNCTION()
	void UnitHold();

	// ����
	UFUNCTION()
	void UnitPetroll(FVector targetLocation);

	// �Ǹ�
	UFUNCTION()
	void UnitSell();

	// �� ���� �� ��ų��ư ��� ����� ȭ
	virtual bool SkillBtn01() override;
	virtual bool SkillBtn02() override;
	virtual bool SkillBtn03() override;
	virtual bool SkillBtn04() override;
	virtual bool SkillBtn05() override;
	virtual bool SkillBtn06() override;
	virtual bool SkillBtn07() override;
	virtual bool SkillBtn08() override;

public:
	FORCEINLINE UFriendlyAbleUnitAnimInstance* GetAnimInstance() { return AnimInstance; }
};
