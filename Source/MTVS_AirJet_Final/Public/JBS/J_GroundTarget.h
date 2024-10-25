// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JBS/J_MissionActorInterface.h"
#include "J_GroundTarget.generated.h"

DECLARE_MULTICAST_DELEGATE(FDestroyedTargetDelegate)

UCLASS()
class MTVS_AIRJET_FINAL_API AJ_GroundTarget : public ACharacter, public IJ_MissionActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJ_GroundTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	

	// 디버그용 타이머 자괴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	bool debugDestroySelf = false;

public:
	// 파괴 딜리게이트
	FDestroyedTargetDelegate destroyedDelegate;

protected:
	// 파괴
    void Death();

public:
	// 피격
    virtual void GetDamage(AActor *attacker = nullptr, FVector hitPoint = FVector::ZeroVector,
                           FVector hitNormal = FVector::ZeroVector);

	
};
