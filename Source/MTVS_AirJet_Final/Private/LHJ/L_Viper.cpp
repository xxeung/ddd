#include "LHJ/L_Viper.h"

#include <cmath>

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MTVS_AirJet_Final.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"


#pragma region Construct
AL_Viper::AL_Viper()
{
	PrimaryActorTick.bCanEverTick = true;

	JetRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("JetRoot"));
	RootComponent = JetRoot;
	JetRoot->SetSimulatePhysics(true);
	JetRoot->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// 공기저항
	JetRoot->SetLinearDamping(1.f);
	JetRoot->SetAngularDamping(1.f);

	JetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("JetMesh"));
	JetMesh->SetupAttachment(JetRoot);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tmpMesh(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/VigilanteContent/Vehicles/West_Fighter_F18C/SK_West_Fighter_F18C.SK_West_Fighter_F18C'"));
	if (tmpMesh.Succeeded())
	{
		JetMesh->SetSkeletalMesh(tmpMesh.Object);
	}

	JetSprintArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("JetSprintArm"));
	JetSprintArm->SetupAttachment(JetMesh);
	JetSprintArm->SetRelativeLocationAndRotation(FVector(-160 , 0 , 400) , FRotator(-10 , 0 , 0));
	JetSprintArm->TargetArmLength = 2000.f;
	JetSprintArm->bEnableCameraRotationLag = true;
	JetSprintArm->CameraRotationLagSpeed = 3.5f;

	JetCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("JetCamera"));
	JetCamera->SetupAttachment(JetSprintArm);

	JetArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("JetArrow"));
	JetArrow->SetupAttachment(JetMesh);
	JetArrow->SetRelativeLocation(FVector(-1000 , 0 , 0));
	JetArrow->SetHiddenInGame(false); // For Test
}
#pragma endregion

#pragma region Input
// Called to bind functionality to input
void AL_Viper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
	{
		input->BindAction(IA_ViperEngine , ETriggerEvent::Started , this , &AL_Viper::F_ViperEngine);

		input->BindAction(IA_ViperLook , ETriggerEvent::Triggered , this , &AL_Viper::F_ViperLook);

		input->BindAction(IA_ViperUp , ETriggerEvent::Triggered , this , &AL_Viper::F_ViperUpTrigger);
		input->BindAction(IA_ViperUp , ETriggerEvent::Completed , this , &AL_Viper::F_ViperUpCompleted);

		input->BindAction(IA_ViperDown , ETriggerEvent::Triggered , this , &AL_Viper::F_ViperDownTrigger);
		input->BindAction(IA_ViperDown , ETriggerEvent::Completed , this , &AL_Viper::F_ViperDownCompleted);

		input->BindAction(IA_ViperRight , ETriggerEvent::Triggered , this , &AL_Viper::F_ViperRightTrigger);
		input->BindAction(IA_ViperRight , ETriggerEvent::Completed , this , &AL_Viper::F_ViperRightCompleted);

		input->BindAction(IA_ViperLeft , ETriggerEvent::Triggered , this , &AL_Viper::F_ViperLeftTrigger);
		input->BindAction(IA_ViperLeft , ETriggerEvent::Completed , this , &AL_Viper::F_ViperLeftCompleted);

		input->BindAction(IA_ViperTurnRight , ETriggerEvent::Triggered , this , &AL_Viper::F_ViperTurnRightTrigger);
		input->BindAction(IA_ViperTurnRight , ETriggerEvent::Completed , this , &AL_Viper::F_ViperTurnRightCompleted);

		input->BindAction(IA_ViperTurnLeft , ETriggerEvent::Triggered , this , &AL_Viper::F_ViperTurnLeftTrigger);
		input->BindAction(IA_ViperTurnLeft , ETriggerEvent::Completed , this , &AL_Viper::F_ViperTurnLeftCompleted);

		input->BindAction(IA_ViperAccel , ETriggerEvent::Started , this , &AL_Viper::F_ViperAccelStarted);
		input->BindAction(IA_ViperAccel , ETriggerEvent::Completed , this , &AL_Viper::F_ViperAccelCompleted);

		input->BindAction(IA_ViperBreak , ETriggerEvent::Started , this , &AL_Viper::F_ViperBreakStarted);
		input->BindAction(IA_ViperBreak , ETriggerEvent::Completed , this , &AL_Viper::F_ViperBreakCompleted);

		input->BindAction(IA_ViperResetRotation , ETriggerEvent::Started , this , &AL_Viper::F_ViperResetRotation);
	}
}

void AL_Viper::F_ViperEngine(const FInputActionValue& value)
{
	bool b = value.Get<bool>();
	IsEngineOn = !IsEngineOn;
	LOG_SCREEN("%s" , IsEngineOn?TEXT("True"):TEXT("false"));
}

void AL_Viper::F_ViperLook(const FInputActionValue& value)
{
}

void AL_Viper::F_ViperUpTrigger(const FInputActionValue& value)
{
	//FVector inputVec = value.Get<FVector>();
	IsKeyUpPress = true;

	if (CurrentTime < ChangeTime)
		return;

	CurrentTime = 0.f;
	ForceUnitRot = CombineRotate(ChangeMoveVector);
}

void AL_Viper::F_ViperUpCompleted(const FInputActionValue& value)
{
	ForceUnitRot = CombineRotate(-1 * ChangeMoveVector);
	IsKeyUpPress = false;
}

void AL_Viper::F_ViperDownTrigger(const FInputActionValue& value)
{
	IsKeyDownPress = true;

	if (CurrentTime < ChangeTime)
		return;

	CurrentTime = 0.f;
	ForceUnitRot = CombineRotate(-1 * ChangeMoveVector);
}

void AL_Viper::F_ViperDownCompleted(const FInputActionValue& value)
{
	ForceUnitRot = CombineRotate(ChangeMoveVector);
	IsKeyDownPress = false;
}

void AL_Viper::F_ViperRightTrigger(const FInputActionValue& value)
{
	IsKeyRightPress = true;
}

void AL_Viper::F_ViperRightCompleted(const FInputActionValue& value)
{
	IsKeyRightPress = false;
}

void AL_Viper::F_ViperLeftTrigger(const FInputActionValue& value)
{
	IsKeyLeftPress = true;
}

void AL_Viper::F_ViperLeftCompleted(const FInputActionValue& value)
{
	IsKeyLeftPress = false;
}

void AL_Viper::F_ViperTurnRightTrigger(const FInputActionValue& value)
{
	IsRightRoll = true;
}

void AL_Viper::F_ViperTurnRightCompleted(const FInputActionValue& value)
{
	IsRightRoll = false;
}

void AL_Viper::F_ViperTurnLeftTrigger(const FInputActionValue& value)
{
	IsLeftRoll = true;
}

void AL_Viper::F_ViperTurnLeftCompleted(const FInputActionValue& value)
{
	IsLeftRoll = false;
}

void AL_Viper::F_ViperResetRotation(const FInputActionValue& value)
{
	SetActorRotation(FRotator(0 , 0 , 0));
	SetActorRelativeRotation(FRotator(0 , 0 , 0));
	JetArrow->SetRelativeRotation(FRotator(0 , 0 , 0));
}

void AL_Viper::F_ViperAccelStarted(const FInputActionValue& value)
{
	IsAccel = true;
}

void AL_Viper::F_ViperAccelCompleted(const FInputActionValue& value)
{
	IsAccel = false;
}

void AL_Viper::F_ViperBreakStarted(const FInputActionValue& value)
{
	IsBreak = true;
}

void AL_Viper::F_ViperBreakCompleted(const FInputActionValue& value)
{
	IsBreak = false;
}

FRotator AL_Viper::CombineRotate(FVector NewVector)
{
	FRotator loc_rot = FRotator(NewVector.Y , NewVector.X , NewVector.Z);
	return FRotator(ForceUnitRot.Pitch + loc_rot.Pitch , ForceUnitRot.Yaw + loc_rot.Yaw ,
	                ForceUnitRot.Roll + loc_rot.Roll);
}
#pragma endregion

void AL_Viper::BeginPlay()
{
	Super::BeginPlay();

	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			pc->GetLocalPlayer());
		if (subsys)
		{
			subsys->AddMappingContext(IMC_Viper , 0);
		}
	}
}

void AL_Viper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;

#pragma region Rotate JetArrow
	FRotator jetRot = JetArrow->GetRelativeRotation();
	// Check Distance Into Area
	if (IsKeyUpPress || IsKeyDownPress)
	{
		if (jetRot.Pitch > MaxPitchValue)
		{
			JetArrow->SetRelativeRotation(FRotator(MaxPitchValue - 1.f , jetRot.Yaw , jetRot.Roll));
			ForceUnitRot = FRotator(0 , 0 , 0);
		}
		else if (jetRot.Pitch < MinPitchValue)
		{
			JetArrow->SetRelativeRotation(FRotator(MinPitchValue + 1.f , jetRot.Yaw , jetRot.Roll));
			ForceUnitRot = FRotator(0 , 0 , 0);
		}
		else
		{
			JetArrow->AddRelativeRotation(FRotator(ForceUnitRot.Pitch , 0 , 0));
		}

		if (jetRot.Yaw > MaxYawValue)
		{
			JetArrow->SetRelativeRotation(FRotator(jetRot.Pitch , MaxYawValue - 1.f , jetRot.Roll));
			ForceUnitRot = FRotator(0 , 0 , 0);
		}
		else if (jetRot.Yaw < MinYawValue)
		{
			JetArrow->SetRelativeRotation(FRotator(jetRot.Pitch , MinYawValue + 1.f , jetRot.Roll));
			ForceUnitRot = FRotator(0 , 0 , 0);
		}
		else
		{
			JetArrow->AddRelativeRotation(FRotator(0 , ForceUnitRot.Yaw , 0));
		}
	}

	if (IsKeyLeftPress)
	{
		if (MinYawValue <= JetArrow->GetRelativeRotation().Yaw)
		{
			if (float emptyYaw = (MinYawValue - JetArrow->GetRelativeRotation().Yaw) > -1)
			{
				JetArrow->AddRelativeRotation(FRotator(0 , emptyYaw , 0));
			}
			else
			{
				FRotator lerpRot = FRotator(jetRot.Pitch , MinYawValue , jetRot.Roll);
				FRotator newRot = FMath::Lerp(jetRot , lerpRot , DeltaTime);
				//LOG_SCREEN("%f, %f, %f" , newRot.Roll , newRot.Pitch , newRot.Yaw);
				LOG_S(Warning , TEXT("%f, %f, %f") , newRot.Roll , newRot.Pitch , newRot.Yaw);

				JetArrow->AddRelativeRotation(newRot);
			}
		}
	}
	else if (IsKeyRightPress)
	{
		if (MaxYawValue >= JetArrow->GetRelativeRotation().Yaw)
		{
			if (float emptyYaw = (MaxYawValue - JetArrow->GetRelativeRotation().Yaw) < 1)
			{
				JetArrow->AddRelativeRotation(FRotator(0 , emptyYaw , 0));
			}
			else
			{
				FRotator lerpRot = FRotator(jetRot.Pitch , MaxYawValue , jetRot.Roll);
				FRotator newRot = FMath::Lerp(jetRot , lerpRot , DeltaTime);
				//LOG_SCREEN("%f, %f, %f" , newRot.Roll , newRot.Pitch , newRot.Yaw);
				LOG_S(Warning , TEXT("%f, %f, %f") , newRot.Roll , newRot.Pitch , newRot.Yaw);

				JetArrow->AddRelativeRotation(newRot);
			}
		}
	}

	// 방향전환중이 아니라면 방향을 가운데로 변환
	if (!IsKeyLeftPress && !IsKeyRightPress)
		JetArrow->SetRelativeRotation(FRotator(JetArrow->GetRelativeRotation().Pitch , 0 ,
		                                       JetArrow->GetRelativeRotation().Roll));

	//LOG_SCREEN("현재 각도는 %f 입니다." , JetArrow->GetRelativeRotation().Pitch);
#pragma endregion

#pragma region	Change Accel Value
	if (IsAccel)
	{
		CurrAccelValue += DeltaTime;
		if (CurrAccelValue > MaxAccelValue)
			CurrAccelValue = MaxAccelValue;
	}
	else if (IsBreak)
	{
		CurrAccelValue -= DeltaTime;
		if (CurrAccelValue < 0)
			CurrAccelValue = 0;
	}
	LOG_SCREEN("Now Accel Value : %f" , CurrAccelValue);
#pragma endregion

#pragma region Jet Move
	if (IsEngineOn)
	{
		// Add Force
		// LOG_S(Warning , TEXT("======================="));
		FVector forceVec = JetArrow->GetForwardVector() * ValueOfMoveForce * CurrAccelValue * 100;
		//LOG_S(Warning , TEXT("forceVec x : %f, y : %f, z : %f") , forceVec.X , forceVec.Y , forceVec.Z);
		FVector forceLoc = JetRoot->GetComponentLocation();
		//LOG_S(Warning , TEXT("forceLoc x : %f, y : %f, z : %f") , forceLoc.X , forceLoc.Y , forceLoc.Z);
		if (JetRoot->IsSimulatingPhysics())
			JetRoot->AddForceAtLocation(forceVec , forceLoc);

		// Move Up & Down
		jetRot = JetArrow->GetRelativeRotation();
		float zRot = jetRot.Quaternion().Y * jetRot.Quaternion().W * ValueOfHeightForce * 10.f;
		// LOG_S(Warning , TEXT("zRot %f") , zRot);
		JetRoot->AddForceAtLocation(FVector(0 , 0 , zRot) , HeightForceLoc);

		// Rotate
		jetRot = JetArrow->GetRelativeRotation();
		JetRoot->AddRelativeRotation(FRotator(0 , jetRot.Yaw / ValueOfDivRot , 0));

		if (IsLeftRoll)
			JetRoot->AddRelativeRotation(-1 * RotateValue);
		if (IsRightRoll)
			JetRoot->AddRelativeRotation(RotateValue);
	}
#pragma endregion

	JetArrow->SetRelativeRotation(FRotator(0 , 0 , 0));

#pragma region Reset Jet Arrow (Not Using)
	// if (IsKeyUpPress || IsKeyDownPress)
	// {
	// 	jetRot = JetArrow->GetRelativeRotation();
	// 	float newPitch = jetRot.Pitch * ValueOfArrowReset;
	// 	ForceUnitRot = FRotator(newPitch , ForceUnitRot.Yaw , ForceUnitRot.Roll);
	// 	JetArrow->AddRelativeRotation(FRotator(newPitch , 0 , 0));
	// }

	// if (IsKeyRightPress || IsKeyLeftPress)
	// {
	// 	jetRot = JetArrow->GetRelativeRotation();
	// 	float newYaw = jetRot.Yaw * ValueOfArrowReset;
	// 	ForceUnitRot = FRotator(ForceUnitRot.Pitch , newYaw , ForceUnitRot.Roll);
	// 	JetArrow->AddRelativeRotation(FRotator(0 , newYaw , 0));
	// }
#pragma endregion
}
