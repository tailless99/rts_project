// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/SpawnType.h"
#include "SpawnPotal.generated.h"

UCLASS()
class ASpawnPotal : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* BodyMesh;
	
	// 액터의 이름을 블루프린터에서 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActorName;

	UPROPERTY()
	class UWidgetComponent* ActorNameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpawnType SpawnType;

	UPROPERTY()
	class UWaveControllerComponent* waveComponent;

	UPROPERTY()
	class UBottomWidgetContainer* BottomWidgetContainer;

	UPROPERTY()
	class UTopWidgetContainer* TopWidgetContainer;

public:	
	ASpawnPotal();

protected:
	virtual void BeginPlay() override;

	void SpwanFriendlyUnit(ESpawnType spawnType);

public:
	// 충돌시 실행되는 함수
	UFUNCTION()
	void CALLBACK_OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

};
