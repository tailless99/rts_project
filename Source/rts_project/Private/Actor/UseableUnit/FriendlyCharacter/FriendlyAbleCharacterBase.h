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

	// 사용할 무기 코드를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "무기")
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

	// 웨이브 컴포넌트
	UPROPERTY()
	class UWaveControllerComponent* waveComponent;

	// 적 탐지 범위
	UPROPERTY()
	float OverlapRadius;

	// 탐지된 적 액터
	UPROPERTY()
	AActor* TargetOverlapActor;
	
	UAnimMontage* attackMontage;

	// 다른 행동을 사용중인지 체크합니다.
	bool IsUseAction;


protected:
	// 조합할 유닛을 담을 리스트
	TArray<AActor*> MixtureUnits;

	// 마지막으로 공격한 시간
	float LastAttackTime;
	
	// 탐지 범위를 넘어 액터를 찾기 위한 bool형 변수.
	// UnitAttack함수로 활성화 후 타이머로 초기화시킨다.
	bool IsSelectAttack;

public:
	AFriendlyAbleCharacterBase();

	void InitializeUnitBase(
		const FFriendlyUnitDataTableRow* unitInfo,
		class AFriendlyUnitController* playerController,
		FName unitCode);

	// 공격 요청 함수
	virtual bool OnAttackRequested(FName weaponCode);

	// 공격이 끝날 경우 호출되는 함수
	virtual void OnAttackFinished();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 적 탐지 로직
	void AutoSurchEnemy();

	// 감지된 적 자동 공격
	void AutoAttack();

	// 타겟을 바라보게 하는 함수
	void LockAtTarget(float dt, float roattionSpeed);

public:
	// 선택 되었을때 처리
	void SelectUnit();
	
	// 선택이 끝났을 때 처리
	void DeSelectUnit();

	// 유닛 움직임 처리
	void UnitMoveCommand(FVector targetPoint);

	// 적 유닛 탐지
	void CheckEnemyOverlap();

	// 타이머로 호출되는 함수
	// 일시적으로 넓어진 탐지범위를 다시 되돌린다.
	void RollBackOverlapRange();

	// 공격이 끝났을 경우 호출되는 함수
	void CALLBACK_OnAttackFinished();

public:
	// 공격
	UFUNCTION()
	void UnitAttack(AActor* targetActor);

	// 위치 고정
	UFUNCTION()
	void UnitHold();

	// 정찰
	UFUNCTION()
	void UnitPetroll(FVector targetLocation);

	// 판매
	UFUNCTION()
	void UnitSell();

	// 각 유닛 별 스킬버튼 기능 버츄얼 화
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
