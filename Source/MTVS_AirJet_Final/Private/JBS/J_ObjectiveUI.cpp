// Fill out your copyright notice in the Description page of Project Settings.


#include "JBS/J_ObjectiveUI.h"
#include "Components/RichTextBlock.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/Engine.h"
#include "JBS/J_DetailUI.h"
#include "JBS/J_ObjectiveSubElementUI.h"
#include "JBS/J_ObjectiveTextUI.h"
#include "Math/UnrealMathUtility.h"
#include "Styling/SlateColor.h"
#include "JBS/J_MissionCompleteUI.h"
#include "TimerManager.h"



void UJ_ObjectiveUI::SetObjUI(FTextUIData data, bool isInit)
{
    // ui 설정
    objectiveTextUI->SetTextUI(data, isInit);
}

void UJ_ObjectiveUI::SetObjUI(TArray<FTextUIData>& data, bool isInit)
{
    // ui 설정
    objectiveTextUI->SetTextUI(data[0], isInit);
    // 상세 ui 설정
    DETAIL_TEXT_UI->SetTextUI(data[1], isInit);
}



void UJ_ObjectiveUI::StartObjUI()
{
    // 시작 UMG 애니메이션 바인드 | settextui 이후 실행
    objectiveTextUI->startAnimDel.BindUObject(this, &UJ_ObjectiveUI::PlayObjStartAnim);
}

void UJ_ObjectiveUI::EndObjUI(bool isSuccess)
{
    // 완료 UMG
    PlayObjEndAnim(this->GetObjTextUI());

    // umg 종료후 비활성화 | 지금은 하드코딩으로 맞추고 있음
}

void UJ_ObjectiveUI::EndSubObjUI(int idx, bool isSuccess)
{
    // objtextui 의 서브 목표 vbox 가져오기
    auto* vbox = GetObjTextUI()->OBJ_BODY_VBOX;
    if(idx < 0 || idx >= vbox->GetChildrenCount()) return;
    // 종료된 서브 목표 ui
    auto* subUI = vbox->GetChildAt(idx);
    // 서브 완료 UMG
    PlaySubObjEndAnim(subUI,idx);

    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager()
        .SetTimer(timerHandle, [this, subUI]() mutable
    {
        FTimerHandle timerHandle2;
        subObjTimerHandleMap.Add(subUI, timerHandle2);

        GetWorld()->GetTimerManager()
            .SetTimer(timerHandle2, [this, subUI]() mutable
        {
            if(!subUI) return ClearSubObjTimer(subUI);

            auto* slot = Cast<UVerticalBoxSlot>(subUI->Slot);
            if(!slot) return ClearSubObjTimer(subUI);
            
            //타이머에서 할 거
            // 사이즈 줄이기
            auto size = slot->GetSize();
            size.Value = FMath::Clamp(size.Value - 0.025f, 0, 1);
            
            slot->SetSize(size);
            // 사이즈 0 이됨
            if(size.Value <= 0)
            {
                subUI->SetVisibility(ESlateVisibility::Hidden);
                // 타이머 종료
                ClearSubObjTimer(subUI);
                return;
            }
        }, 0.025, true);
    }, 1.5f, false);
}

void UJ_ObjectiveUI::ClearSubObjTimer(class UWidget *subObj)
{
    if(!subObjTimerHandleMap.Contains(subObj)) return;

    auto th = subObjTimerHandleMap[subObj];

    GetWorld()->GetTimerManager().ClearTimer(th);
}

float UJ_ObjectiveUI::PlaySubObjEndAnimLerp(UVerticalBoxSlot *subSlot, float alpha)
{
    // 스케일 러프
    float value = FMath::Lerp(1.0f, 0.f, alpha);

    FSlateChildSize newSize(ESlateSizeRule::Fill);
    newSize.Value = value;
    subSlot->SetSize(newSize);

    // GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("%.2f"), newSize.Value));

    return value;
}

void UJ_ObjectiveUI::ActiveResultUI(const TArray<FObjectiveData>& resultObjData)
{
    MissionUI_Switcher->SetActiveWidget(missionCompleteUI);
    
    // FString asd = FString::Printf(TEXT("결과 obj 개수 : %d"), resultObjData.Num());
    // GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, FString::Printf(TEXT("%s"), *asd));
    // UE_LOG(LogTemp, Warning, TEXT("%s"), *asd);

    missionCompleteUI->SetResultListValue(resultObjData);
}










// XXX
// void UJ_ObjectiveUI::SetObjHeaderText(const FString& value)
// {
//     // 텍스트 설정
//     objHeaderText->SetText(FText::FromString(value));
// }

// XXX
// void UJ_ObjectiveUI::SetObjBodyText(TArray<FString> valueAry)
// {
//     if (valueAry.Num() > objBodyElementVBox->GetChildrenCount()) {
//         GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("서브 조건 리스트 : 너무 많은 값 요청"));
        
//         return;
//     }
//     // solved valueAry의 개수 만큼 ui visible  | 나중엔 요소 생성하는 방향으로 수정
//     auto objBodies = objBodyElementVBox->GetAllChildren();
//     // 전체 비활성화
//     for(auto* subElement : objBodies)
//     {
//         subElement->SetVisibility(ESlateVisibility::Hidden);
//     }

//     // value 들어간 인덱스만큼 설정
//     for(int i = 0; i < valueAry.Num(); i++)
//     {
//         // solved 나중엔 객체지향식으로 변경

//         auto valueStr = valueAry[i];
//         UJ_ObjectiveSubElementUI* subUI = Cast<UJ_ObjectiveSubElementUI>(objBodies[i]);
//         objBodies[i]->SetVisibility(ESlateVisibility::Visible);

//         subUI->objSubText->SetText(FText::FromString(valueStr));
//     }
//     // objBodyText->SetText(FText::FromString(valueAry[0]));


//     // solved ary보다 child가 적을시 추가 생성
// }
// XXX
// void UJ_ObjectiveUI::ResetTextColor()
// {
// // objHeaderText->SetColorAndOpacity(defaultTextColor);

//     // FIXME
//     // auto allSub = objBodyElementVBox->GetAllChildren();
//     // TArray<UJ_ObjectiveSubElementUI*> subUIs;
//     // Algo::Transform(allSub, subUIs, [](UWidget* temp){
//     //     return Cast<UJ_ObjectiveSubElementUI>(temp);
//     // });
//     // //캐스트 후
//     // for(auto* subUI : subUIs)
//     // {
//     //     // FIXME 추가해야함
//     //     // subUI->objSubText->SetColorAndOpacity(defaultTextColor);
//     // }
//     // objBodyText->SetColorAndOpacity(defaultTextColor);
// }