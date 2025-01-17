﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <queue>

#include "CoreMinimal.h"
#include "JBS/J_BaseMissionPawn.h"
#include "JBS/J_Utility.h"
#include "L_Viper.generated.h"

UENUM(BlueprintType)
enum class EWeapon : uint8
{
	Missile = 0 ,
	Flare ,
	// 항상 마지막에 추가하여 열거형의 크기를 구할 수 있게 함
	Max
};

DECLARE_MULTICAST_DELEGATE(FStartMissionViper);

UCLASS()
class MTVS_AIRJET_FINAL_API AL_Viper : public AJ_BaseMissionPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AL_Viper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void PrintNetLog();

public:
#pragma region One Depth Component
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetRoot;
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	class USkeletalMeshComponent* JetMesh;
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UArrowComponent* JetArrow;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UArrowComponent* JetFlareArrow3;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UArrowComponent* JetFlareArrow2;
#pragma endregion

#pragma region Camera
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class USpringArmComponent* JetSprintArm;
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadWrite)
	class UCameraComponent* JetCamera;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class USpringArmComponent* JetSprintArmFPS;
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadWrite)
	class UCameraComponent* JetCameraFPS;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class USpringArmComponent* JetSpringArmMissileCam;
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadWrite)
	class USceneCaptureComponent2D* JetCameraMissileCam;
#pragma endregion

#pragma region Prop
	UPROPERTY(EditdefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetFirstEngine;
	UPROPERTY(EditdefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetMic;
	UPROPERTY(EditdefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetEngineGen;
	UPROPERTY(EditdefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetEngineGen2;
	UPROPERTY(EditdefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetEngineControl;
	UPROPERTY(EditdefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetEngineControl2;
	UPROPERTY(EditdefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetFuelStarter;
	UPROPERTY(EditdefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetEngineMaster;
	UPROPERTY(EditdefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetEngineMaster2;
	UPROPERTY(EditdefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetJFSHandle;
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetCanopy;
	UPROPERTY(EditdefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetBreakHold;
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UBoxComponent* JetRotationStick;
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	class USceneComponent* JetStickScene;
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UStaticMeshComponent* JetLeftPannel;
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UStaticMeshComponent* JetRightPannel;
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	class UStaticMeshComponent* JetJFSPannel;
#pragma endregion

#pragma region Prop2
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class USceneComponent* JetPropRootScene;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetPropObj4;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetPropObj11;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetPropObj9;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetPropObj8;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetPropObj7;

	// Canopy Prop
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetProp0_C;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetProp0_R;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetProp0_L;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetProp3_L;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetProp3_R;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetProp1;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetProp2;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetProp10;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetProp6_R;
	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UStaticMeshComponent* JetProp6_L;
#pragma endregion

public:
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category="Default|Component")
	class USpringArmComponent* PlayerNameSpringArm;
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category="Default|Component")
	class UJ_CustomWidgetComponent* PlayerNameWidgetComponent;
	UPROPERTY(Replicated)
	bool bOnceUpdatePlayerName;

private:
	UFUNCTION(Server , Unreliable)
	void SRPC_SetMyName(const FString& PlayerName);
	UFUNCTION(NetMulticast , Reliable)
	void MRPC_SetMyName(const FString& PlayerName);

private:
#pragma region Widget & Effect
	UPROPERTY(EditDefaultsOnly , category="Components")
	class UWidgetComponent* JetWidget;
	UPROPERTY(EditDefaultsOnly , category="Components")
	class UWidgetComponent* MissileWidget;

	UPROPERTY(EditDefaultsOnly , category="Components")
	class UNiagaraComponent* BoosterLeftVFX;
	UPROPERTY(EditDefaultsOnly , category="Components")
	class UNiagaraComponent* BoosterRightVFX;
	UPROPERTY(EditDefaultsOnly , category="Components")
	class UNiagaraComponent* JetTailVFXLeft;
	UPROPERTY(EditDefaultsOnly , category="Components")
	class UNiagaraComponent* JetTailVFXRight;

	UPROPERTY(EditDefaultsOnly , Category="Components")
	class UPostProcessComponent* JetPostProcess;

	UPROPERTY(EditDefaultsOnly , Category="Default|Sound")
	class UAudioComponent* JetAudio;
	UPROPERTY(EditDefaultsOnly , Category="Default|Sound")
	class UAudioComponent* JetCanopyAudio;
	UPROPERTY(EditDefaultsOnly , Category="Default|Sound")
	class UAudioComponent* JetMICAudio;
	UPROPERTY(EditDefaultsOnly , Category="Default|Sound")
	class UAudioComponent* JetEngineAudio;
	UPROPERTY(EditDefaultsOnly , Category="Default|Sound")
	class UAudioComponent* JetAirAudio;
	UPROPERTY(EditDefaultsOnly , Category="Default|Sound")
	class UAudioComponent* JetMissileAudio;
	UPROPERTY(EditDefaultsOnly , Category="Default|Sound")
	class USoundBase* SwitchSound;
	UPROPERTY(EditDefaultsOnly , Category="Default|Sound")
	class USoundBase* LockOnSound;
	UPROPERTY(EditDefaultsOnly , Category="Default|Sound")
	class USoundBase* ImpactSound;
	UPROPERTY(EditDefaultsOnly , category="Defalut|VFX")
	class UNiagaraSystem* DistroyVFX;
	UPROPERTY(EditDefaultsOnly , category="Defalut|VFX")
	class UStaticMeshComponent* AirResistanceVFX;
#pragma endregion

public:
#pragma region Clicked Event
	UFUNCTION(BlueprintCallable)
	void OnMyFirstEngineClicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	void OnMyMicClicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	void OnMyEngineGen1Clicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	void OnMyEngineGen2Clicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	void OnMyEngineControlClicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	void OnMyEngineControl2Clicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	void OnMyJetFuelStarterClicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	void OnMyEngineMaster1Clicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	void OnMyEngineMaster2Clicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	void OnMyJFSHandle1Clicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	void OnMyCanopyClicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	void OnMyBreakHoldClicked(UPrimitiveComponent* TouchedComponent , struct FKey ButtonPressed);
#pragma endregion

#pragma region Input Action
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputMappingContext* IMC_Viper;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputMappingContext* IMC_Fun;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperLook;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperZoonIn;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperZoonOut;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperResetRotation;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperAccel;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperBreak;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperShoot;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperFPS;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperTPS;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperChangeWeapon;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperRotateViewTrigger;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperVoice;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperDevelop;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	class UInputAction* IA_ViperMove;

	UFUNCTION()
	void F_ViperLook(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperZoomInStarted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperZoomInCompleted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperZoomOutStarted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperZoomOutCompleted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperResetRotation(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperAccelStarted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperAccelCompleted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperBreakStarted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperBreakCompleted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperShootStarted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperFpsStarted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperTpsStarted(const struct FInputActionValue& value);
	UFUNCTION(BlueprintCallable)
	void F_ViperChangeWeaponStarted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperRotateTriggerStarted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperRotateTriggerCompleted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperVoiceStarted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperDevelopStarted(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperMoveTrigger(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ViperMoveCompleted(const struct FInputActionValue& value);
#pragma endregion

private:
	UPROPERTY(EditDefaultsOnly)
	FRotator TargetArmRotation = FRotator(-10 , 0 , 0);

public:
	UPROPERTY(BlueprintReadOnly)
	bool bFirstEngine;

	// Rotate vector
	UPROPERTY(BlueprintReadOnly)
	bool IsKeyUpPress;
	UPROPERTY(BlueprintReadOnly)
	bool IsKeyDownPress;
	UPROPERTY(BlueprintReadOnly)
	bool IsRightRoll;
	UPROPERTY(BlueprintReadOnly)
	bool IsLeftRoll;
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	FRotator RotateRollValue = FRotator(0 , 0 , 1.0f);
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	FRotator RotatePitchValue = FRotator(1.2f , 0 , 0);

private:
	// For Engine Using
	UPROPERTY(EditDefaultsOnly , Category="Engine" , Replicated)
	bool IsEngineOn;

	// For Change Arrow Rotate
	float CurrentTime;
	UPROPERTY(EditDefaultsOnly , Category="Inputs")
	float ChangeTime = .3f;
	// Zoom
	bool IsZoomIn;
	bool IsZoomOut;

public:
	// For Check Distance
	UPROPERTY(EditDefaultsOnly , Category="Rotate" , BlueprintReadOnly)
	float MaxPitchValue = 85.f;
	UPROPERTY(EditDefaultsOnly , Category="Rotate" , BlueprintReadOnly)
	float MinPitchValue = -85.f;
	UPROPERTY(EditDefaultsOnly , Category="Rotate" , BlueprintReadOnly)
	float MaxYawValue = 80.f;
	UPROPERTY(EditDefaultsOnly , Category="Rotate" , BlueprintReadOnly)
	float MinYawValue = -80.f;

private:
	UPROPERTY(EditDefaultsOnly , Category="Movements")
	float MaxValueOfMoveForce = 244464000.f;
	UPROPERTY(EditDefaultsOnly , Category="Movements")
	float ValueOfMoveForce = 0.0f;
	UPROPERTY(EditDefaultsOnly , Category="Movements")
	float ValueOfHeightForce = 1.f;
	UPROPERTY(EditDefaultsOnly , Category="Movements")
	FVector HeightForceLoc = FVector(1000 , 0 , 0);
	UPROPERTY(EditDefaultsOnly , Category="Movements")
	float ValueOfDivRot = 50.f;
	// UPROPERTY(EditDefaultsOnly, Category="Movements")
	// float ValueOfArrowReset = -.1f;

private:
	UPROPERTY(EditDefaultsOnly , Category="Movements")
	float ChangeAccelTime = 1.f; // 다음 기어가 되기까지 소요되는 시간
	float KeyDownAccel = 0.f; // 액셀 누르고 있는 시간

	bool IsAccel = false;
	bool IsBreak = false;
	int32 AccelGear = 0; // 현재 엑셀 기어 (0=0%,1=50%,2=100%,3=100% + 애프터버너)
	float GetAddTickSpeed();
	// 기어 0
	UPROPERTY(EditDefaultsOnly , category = "ZeroGear")
	float ZeroGearFlight = -3000.f;
	// 수평비행
	UPROPERTY(EditDefaultsOnly , category = "BasicFlight")
	float BasicFlight50 = 2000.f;
	UPROPERTY(EditDefaultsOnly , category = "BasicFlight")
	float BasicFlight100 = 6000.f;
	UPROPERTY(EditDefaultsOnly , category = "BasicFlight")
	float BasicFlightAB = 8000.f;
	UPROPERTY(EditDefaultsOnly , category = "UpFlight")
	float UpFlight50 = 1000.f;
	UPROPERTY(EditDefaultsOnly , category = "UpFlight")
	float UpFlight100 = 4000.f;
	UPROPERTY(EditDefaultsOnly , category = "UpFlight")
	float UpFlightAB = 6000.f;
	UPROPERTY(EditDefaultsOnly , category = "DownFlight")
	float DownFlight50 = 3000.f;
	UPROPERTY(EditDefaultsOnly , category = "DownFlight")
	float DownFlight100 = 7000.f;
	UPROPERTY(EditDefaultsOnly , category = "DownFlight")
	float DownFlightAB = 9000.f;
	UPROPERTY(EditDefaultsOnly , category = "TurnFlight")
	float TurnFlight50 = 1500.f;
	UPROPERTY(EditDefaultsOnly , category = "TurnFlight")
	float TurnFlight100 = 5500.f;
	UPROPERTY(EditDefaultsOnly , category = "TurnFlight")
	float TurnFlightAB = 6500.f;

private:
	UPROPERTY(EditDefaultsOnly , Category="Default|UI")
	float HeightOfSea = -480000; // Cesium 해수면 높이
	UPROPERTY(EditDefaultsOnly , Category="Default|UI")
	TSubclassOf<class UUserWidget> HUD_UI;
	UPROPERTY(EditDefaultsOnly , Category="Default|UI")
	TSubclassOf<class AL_Target> TargetUIActorFac;
	UPROPERTY()
	class AL_Target* TargetActor;

public:
	UFUNCTION(Client , Reliable)
	void ClientRPC_LockOnStart();

private:
	UPROPERTY(EditDefaultsOnly , Category="Default|Attack")
	bool bLockOnStart;

	float Diametr = 30.f;
	UPROPERTY(EditDefaultsOnly , Category="Attack")
	TSubclassOf<class AL_Missile> Missile;

	UPROPERTY(EditDefaultsOnly , Category="Attack")
	TSubclassOf<class AL_Flare> FlareFactory;

public:
	UPROPERTY(EditDefaultsOnly , Category="Default|Attack")
	int32 RangeCnt = 14;

public:
	UPROPERTY(EditDefaultsOnly , Category="Default|Attack" , BlueprintReadWrite)
	AActor* LockOnTarget = nullptr;
	UPROPERTY(EditDefaultsOnly , Category="Default|Attack" , BlueprintReadWrite)
	class USceneComponent* MissileMoveLoc;
	UPROPERTY(EditDefaultsOnly , Category="Default|Attack" , BlueprintReadWrite)
	class USceneComponent* FlareMoveLoc1;
	UPROPERTY(EditDefaultsOnly , Category="Default|Attack" , BlueprintReadWrite)
	class USceneComponent* FlareMoveLoc2;
	UPROPERTY(EditDefaultsOnly , Category="Default|Attack" , BlueprintReadWrite)
	class USceneComponent* FlareMoveLoc3;

private:
	UFUNCTION()
	void ChangeBooster();

public:
	FTimerHandle syncLocTimer;
	UFUNCTION(Server , Reliable)
	void ServerRPC_SyncLocation(const FVector& location);

private:
	UFUNCTION(NetMulticast , Unreliable)
	void MultiRPC_SyncLocation(const FVector& location);
	UFUNCTION(Client , Unreliable)
	void ClientRPCLocation();
	UFUNCTION(Server , Unreliable)
	void ServerRPCRotation(FQuat newQuat);
	UFUNCTION(NetMulticast , Unreliable)
	void MultiRPCRotation(const FQuat& newQuat);
	UFUNCTION(NetMulticast , Unreliable)
	void MultiRPCVisibleAirVFX(bool isOn);

#pragma region Boost VFX(키고 끌때 1번씩만 수행됌)
	UFUNCTION(Server , Reliable)
	void ServerRPCBoost(bool isOn);
	UFUNCTION(NetMulticast , Reliable)
	void MulticastRPCBoost(bool isOn);
	bool bCurrBoostState;
#pragma endregion

	UFUNCTION(Client , Unreliable)
	void ClientRPCLockOn();
	UFUNCTION(Client , Unreliable)
	void CRPC_MissileCapture();

	bool bStartLockOn;

	UFUNCTION(Server , Unreliable)
	void ServerRPCLockOn(AActor* target);
	UFUNCTION(NetMulticast , Unreliable)
	void MulticastRPCLockOn(AActor* target);
	void PlayLockOnSound();

#pragma region Missile & Flare

public:
	UFUNCTION(Server , Reliable , BlueprintCallable)
	void ServerRPCMissile(AActor* newOwner);
	UFUNCTION(Server , Reliable , BlueprintCallable)
	void ServerRPCFlare(AActor* newOwner);
	UPROPERTY(replicated , BlueprintReadOnly)
	EWeapon CurrentWeapon = EWeapon::Missile;

private:
	UPROPERTY(replicated , EditDefaultsOnly)
	float FlareCurCnt = 60;
#pragma endregion

private:
	bool IsRotateTrigger = false;
	bool IsRotateStickTrigger = false;

private:
	UPROPERTY(EditDefaultsOnly)
	float ZoomInValue = 25;
	UPROPERTY(EditDefaultsOnly)
	float ZoomOutValue = 140;

public:
#pragma region Prop Collision
	void CreateDumyComp();

	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyMICMesh;
	UPROPERTY(BlueprintReadOnly)
	bool bMIC;

	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyEngineGenerMesh1;
	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyEngineGenerMesh2;
	UPROPERTY(BlueprintReadOnly)
	bool bEngineGen1;
	UPROPERTY(BlueprintReadOnly)
	bool bEngineGen2;

	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyEngineControlMesh1;
	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyEngineControlMesh2;
	UPROPERTY(BlueprintReadOnly)
	bool bEngineControl1;
	UPROPERTY(BlueprintReadOnly)
	bool bEngineControl2;

	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyJFSMesh;
	UPROPERTY(BlueprintReadOnly)
	bool bJFS;

	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyEngineMasterMesh1;
	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyEngineMasterMesh2;
	UPROPERTY(BlueprintReadOnly)
	bool bEngineMaster1;
	UPROPERTY(BlueprintReadOnly)
	bool bEngineMaster2;

	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyJFSHandleMesh;
	UPROPERTY(BlueprintReadOnly)
	bool bJFSHandle;

	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	class USceneComponent* JetThrottleScene;
	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyThrottleMesh;
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite)
	bool bThrottleAccel;
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite)
	bool bThrottleBreak;
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite)
	FVector ThrottleOffLoc = FVector(515 , -35 , 250);
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite)
	FVector ThrottleMilLoc = FVector(520 , -35 , 250);
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite)
	FVector ThrottleMaxLoc = FVector(530 , -35 , 250);
	UPROPERTY(EditDefaultsOnly , Category="ThrottleSpeed" , BlueprintReadOnly)
	float ThrottleMoveSpeed1 = .1f;
	UPROPERTY(EditDefaultsOnly , Category="ThrottleSpeed" , BlueprintReadOnly)
	float ThrottleMoveSpeed2 = .15f;
	void SetAccelGear();

	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyJFSBreakHold;
	UPROPERTY(BlueprintReadOnly)
	bool bBreakHold;
	bool IsFirstBreakHoldClick;
	bool bStartAudio;

	float CurAudioTime = 0.f;
	UPROPERTY(EditDefaultsOnly , Category="Default|MissionTextUI")
	float PlayAudioTime = 3.f;

	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class USkeletalMeshComponent* DummyStick;
	// UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	// class UPhysicsConstraintComponent* RotationStickConstraint;
#pragma endregion

public:
	UPROPERTY(BlueprintReadOnly)
	FString CurrentScenario = "";
	UPROPERTY(EditDefaultsOnly , Category="Engine" , BlueprintReadOnly)
	bool IsStart;

private:
	//==================================
	// 시동 절차
	std::queue<FString> StartScenario;
	void PushQueue();
	UPROPERTY(EditDefaultsOnly , Category="Engine")
	bool IsFlyStart;

public:
	UPROPERTY(EditDefaultsOnly , Category="Engine" , BlueprintReadOnly)
	int32 intTriggerNum = 0; // 0=출발, 1=80%까지만 가능, 2 = 100%까지 가능
private:
	UFUNCTION()
	void OnMyMeshOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor ,
	                     UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep ,
	                     const FHitResult& SweepResult);
	//=====================================
	// 캐노피
	int iCanopyNum = 1; //0=열기, 1=기본, 2=닫기, 3=잠금
	void PerformLineTrace();

public:
	UFUNCTION(BlueprintCallable)
	void BackMoveCanopyHandle();

private:
	// FVector CanopyOpenLoc = FVector(-35 , 0 , 5);
	// FVector CanopyNormalLoc = FVector(-30 , 0 , 5);
	// FVector CanopyCloseLoc = FVector(-25 , 0 , 5);
	// FVector CanopyHoldLoc = FVector(-20 , 0 , 5);
	FVector CanopyOpenLoc = FVector(485, 42, 270);
	FVector CanopyNormalLoc = FVector(490, 42, 270);
	FVector CanopyCloseLoc = FVector(495, 42, 270);
	FVector CanopyHoldLoc = FVector(500, 42, 270);
	UFUNCTION(Server , Reliable)
	void ServerRPC_Canopy(bool bOpen);
	UPROPERTY(EditDefaultsOnly , Category="Canopy")
	float CanopyRotatePitchValue = .3f;

public:
	UPROPERTY(EditDefaultsOnly , Category="Components" , BlueprintReadOnly)
	class USceneComponent* JetCanopyScene;
	UPROPERTY(EditDefaultsOnly , Category="DumyComponents" , BlueprintReadOnly)
	class UStaticMeshComponent* DummyCanopyMesh;

private:
	//=====================================
	// 바퀴
	UFUNCTION(Server , Reliable)
	void ServerRPC_Wheel();

public:
	UPROPERTY(EditAnywhere , Category="Default|Anim" , Replicated)
	float CanopyPitch = 80.f;
	UPROPERTY(EditAnywhere , Category="Default|Anim" , Replicated)
	float FrontWheel = 0.f;
	UPROPERTY(EditAnywhere , Category="Default|Anim" , Replicated)
	float RearWheel = 0.f;

private:
	UPROPERTY(EditDefaultsOnly , Category="JetTail")
	bool bJetTailVFXOn;
	UPROPERTY(EditDefaultsOnly , Category="Default|JetAirVFX")
	bool bJetAirVFXOn;

	UPROPERTY(EditDefaultsOnly , Category="Sound")
	class USoundBase* JetRunOnRoad;
	UPROPERTY(EditDefaultsOnly , Category="Sound")
	class USoundBase* JetFlySound;
	UPROPERTY(EditDefaultsOnly , Category="Sound")
	class USoundCue* JetSoundCue;

private:
	bool bVoice;
	void StartVoiceChat();
	void StopVoiceChat();

public:
	UPROPERTY(Category="UI" , EditDefaultsOnly)
	class UL_WaitingForStart* WaitingForStartUI;

private:
	bool bReadyTimeEndFlag;
	UPROPERTY(EditDefaultsOnly , Category="Default|Engine")
	float TimeToReady = 30.f;

#pragma region KHS Works

	//KHS 작업부분
public :
	// 플레이어리스트 업데이트 서버 요청
	UFUNCTION(Server , Reliable)
	void ServerRPC_SetConnectedPlayerNames(const FString& newName);
	// 플레이어리스트 Multicast 업데이트
	UFUNCTION(Client , Reliable)
	void ClientRPC_SetConnectedPlayerNames(const TArray<FString>& newNames);


#pragma endregion

private:
	UFUNCTION(Client , Reliable)
	void CRPC_MissileImpact(FVector ImpactLoc);
	UFUNCTION(Client , Reliable)
	void CRPC_AudioControl(bool bStart , int32 idx = 0);
	UFUNCTION(Client , Reliable)
	void CRPC_PlaySwitchSound(FVector SoundLoc);
	UFUNCTION(Client , Reliable)
	void CRPC_EngineSound(bool bStart , int32 idx = 0);
	UFUNCTION(Client , Reliable)
	void CRPC_AirSound(bool bStart);
	UFUNCTION(Client , Reliable)
	void CRPC_MissileSound(bool bStart);
	UFUNCTION(Client , Reliable)
	void CRPC_MissilePitch(const float& Pitch);

	UPROPERTY()
	class AJ_GroundTarget* GroundTarget;

public:
	void Call_CRPC_MissileImpact(FVector ImpactLoc);
	UFUNCTION(Client , Reliable)
	void CRPC_CanopyAudioControl(bool bStart , int32 idx = 0);
	UFUNCTION(Client , Reliable)
	void CRPC_MICAudioControl(bool bStart);

private:
	bool bPlayCanopyEndSound;

	UPROPERTY(EditDefaultsOnly , Category="Default|Camera")
	TSubclassOf<class UCameraShakeBase> LoadCameraShake;
	UFUNCTION(Client , Reliable)
	void CRPC_CameraShake();

private:
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton8;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton15;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton34;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton35;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton36;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton37;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton38;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton39;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton40;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton41;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton42;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton43;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton46;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleButton47;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleAxis4;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Throttle")
	class UInputAction* IA_ThrottleAxis6;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Stick")
	class UInputAction* IA_StickButton1;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Stick")
	class UInputAction* IA_StickButton2;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Stick")
	class UInputAction* IA_StickButton5;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Stick")
	class UInputAction* IA_StickButton11;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Stick")
	class UInputAction* IA_StickButton13;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Stick")
	class UInputAction* IA_StickAxis1;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Stick")
	class UInputAction* IA_StickAxis2;
	UPROPERTY(EditDefaultsOnly , Category="Default|Inputs|Stick")
	class UInputAction* IA_StickAxis3;

	UFUNCTION()
	void F_ThrottleButton8Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton15Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton34Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton35Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton36Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton37Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton38Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton39Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton40Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton41Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton42Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton43Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton46Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleButton47Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleAxis4(const struct FInputActionValue& value);
	UFUNCTION()
	void F_ThrottleAxis6(const struct FInputActionValue& value);
	UFUNCTION()
	void F_StickButton1Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_StickButton2Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_StickButton5Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_StickButton11Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_StickButton11Completed(const struct FInputActionValue& value);
	UFUNCTION()
	void F_StickButton13Started(const struct FInputActionValue& value);
	UFUNCTION()
	void F_StickButton13Completed(const struct FInputActionValue& value);
	UFUNCTION()
	void F_StickAxis1(const struct FInputActionValue& value);
	UFUNCTION()
	void F_StickAxis2(const struct FInputActionValue& value);
	UFUNCTION()
	void F_StickAxis3(const struct FInputActionValue& value);

	float switchLate = 0.4f;
	float DeviceThrottleCurrentValue = 0.f;
#pragma region JBS 추가 부분

public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite , BlueprintAssignable , Category="Default|Values")
	FSuccessProgress engineProgSuccessDel;
#pragma endregion

public:
	// 회전 속도 설정
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Default|Move")
	float RotationSpeed = 15.f;

	// 최대 회전 각도
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Default|Move")
	float MaxRotationAngle = 15.f;

	// 현재 회전 상태를 쿼터니언으로 저장
	UPROPERTY()
	FQuat QuatCurrentRotation = FQuat::Identity;

	// 목표 회전 상태를 쿼터니언으로 저장
	UPROPERTY()
	FQuat QuatTargetRotation = FQuat::Identity;

	float StickRollAngle = 0.f;
	float StickPitchAngle = 0.f;

public:
	float VRStickCurrentRollValue = 0.f;
	float VRStickCurrentPitchValue = 0.f;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Default|Move")
	float VRStickMaxThreshold = 0.6f;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Default|Move")
	float VRStickMinThreshold = -0.6f;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Default|Move")
	float VRStickBankRollDiv = 8.f;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Default|Move")
	float VRStickBankPitchDiv = 13.f;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Default|Move")
	float VRStickkRollDiv = 10.f;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Default|Move")
	float VRStickPitchDiv = 15.f;
	UFUNCTION(BlueprintCallable)
	void VRSticAxis(const FVector2D& value);

private:
	bool bCanopyHoldSound;
	bool bCanopyCloseSound;
	bool bCanopyNormalSound;
	bool bCanopyOpenSound;

public:
	// 미션 시작 시 사용할 델리게이트 | 6번 나레이션 실행에 쓰임
	bool bStartMission;
	FStartMissionViper StartMissionViper_Del;

	// isEngineOn 변수를 켜는 함수 (범서가 사용 중)
	UFUNCTION(BlueprintCallable)
	void SetEngineOn();

	UFUNCTION(BlueprintCallable , Client , Reliable)
	void CRPC_TeleportSetting();

private:
	UFUNCTION(Client , Reliable)
	void CRPC_SetMissileCamRotate();
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* MissileSceneMat;
	UFUNCTION(Server , Reliable)
	void SRPC_VisiblePlayerName();
	UFUNCTION(NetMulticast , Reliable)
	void MRPC_VisiblePlayerName();

private:
	UFUNCTION()
	void StopAllVoice();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Default|StickMove")
	float StickMaxRoll = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Default|StickMove")
	float StickMinRoll = -20;
	UPROPERTY(EditDefaultsOnly, Category = "Default|StickMove")
	float StickMaxPitch = -20;
	UPROPERTY(EditDefaultsOnly, Category = "Default|StickMove")
	float StickMinPitch = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Default|StickMove")
	float StickMaxThreshold = .4f;
	UPROPERTY(EditDefaultsOnly, Category = "Default|StickMove")
	float StickDivBankRoll = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "Default|StickMove")
	float StickDivBankPitch = 10.f;
	UPROPERTY(EditDefaultsOnly, Category = "Default|StickMove")
	float StickDivRoll = 9.f;
	UPROPERTY(EditDefaultsOnly, Category = "Default|StickMove")
	float StickDivPitch = 8.f;
public:
	UFUNCTION(BlueprintCallable)
	void StickRotation(const FRotator& NewRotation);
	//=============================================
	UFUNCTION(BlueprintCallable)
	void SetCanopyGearLevel();
	//=============================================
	UFUNCTION(Blueprintable)
	void SetThrottleLoc(const FVector& NewLocation, bool bIsAccel);
};
