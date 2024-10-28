// Fill out your copyright notice in the Description page of Project Settings.


#include "JBS/J_ObjectiveUIComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/SlateWrapperTypes.h"
#include "JBS/J_MissionPlayerController.h"
#include "JBS/J_ObjectiveUI.h"
#include "JBS/J_Utility.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UJ_ObjectiveUIComponent::UJ_ObjectiveUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJ_ObjectiveUIComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto* myPC = GetOwner<AJ_MissionPlayerController>();
	
	// 클라에서 및 호스트 클라에서 처리
	if(!(myPC->HasAuthority()) || myPC->HasAuthority() && myPC->IsLocalController())
	{
		// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("목표 UI 생성2"));
		// 목표 UI 생성
		InitObjUI();
	}
}


// Called every frame
void UJ_ObjectiveUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJ_ObjectiveUIComponent::InitObjUI()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("목표 UI 생성"));
	// ui 생성
	objUI = CreateWidget<UJ_ObjectiveUI>(this->GetOwner<AJ_MissionPlayerController>(), objUIPrefab);
	check(objUI);
	// 비활성화
	objUI->SetVisibility(ESlateVisibility::Hidden);
	// 뷰포트에 붙이기
	objUI->AddToViewport();
}

void UJ_ObjectiveUIComponent::CRPC_StartObjUI_Implementation(ETacticalOrder orderType, FNeutralizeTargetUIData data)
{
	// 비활성 상태시 활성화
	if(objUI->GetVisibility() == ESlateVisibility::Hidden)
		objUI->SetVisibility(ESlateVisibility::Visible);

	// 시작 처리
	objUI->StartObjUI();
	// 값 설정
	CRPC_UpdateObjUI(orderType, data);
}

void UJ_ObjectiveUIComponent::CRPC_UpdateObjUI_Implementation(ETacticalOrder orderType, FNeutralizeTargetUIData data)
{
	//@@ 나중엔 오더 타입에 따라 다른 데이터 구조체 처리
	FObjUIData uiData;

	switch (orderType) {
        case ETacticalOrder::NONE:
        case ETacticalOrder::MOVE_THIS_POINT:
			break;
        case ETacticalOrder::FORMATION_FLIGHT:
			break;
        case ETacticalOrder::NEUTRALIZE_TARGET:
		{
			auto ntData = static_cast<FNeutralizeTargetUIData>(data);
			
			uiData.headerText = TEXT("지상 목표 무력화");
			uiData.bodyTextAry.Add(FString::Printf(TEXT("남은 지상 목표 %d/%d"), ntData.curTargetAmt, ntData.allTargetAmt));
		}
            break;
	}

	// ui에 값 전달
	objUI->SetObjUI(uiData);
}

void UJ_ObjectiveUIComponent::CRPC_EndSubObjUI_Implementation(int idx, bool isSuccess)
{
	objUI->EndSubObjUI(idx, isSuccess);
}

void UJ_ObjectiveUIComponent::CRPC_EndObjUI_Implementation(bool isSuccess)
{
	objUI->EndObjUI(isSuccess);
}