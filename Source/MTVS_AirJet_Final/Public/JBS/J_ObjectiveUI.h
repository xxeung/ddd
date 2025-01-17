// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JBS/J_Utility.h"
#include "J_ObjectiveUI.generated.h"

/**
 * 
 */
UCLASS()
class MTVS_AIRJET_FINAL_API UJ_ObjectiveUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	// 위젯 스위쳐 | 목표 UI, 결산 UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Default|UI", meta = (BindWidget))	
	class UWidgetSwitcher* MissionUI_Switcher;

	// 전술 명령 UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Default|UI", meta = (BindWidget))
	class UJ_ObjectiveTextUI* objectiveTextUI;
		public:
	__declspec(property(get = GetObjTextUI, put = SetObjTextUI)) class UJ_ObjectiveTextUI* OBJ_TEXT_UI;
	class UJ_ObjectiveTextUI* GetObjTextUI()
	{
		return objectiveTextUI;
	}
	void SetObjTextUI(class UJ_ObjectiveTextUI* value)
	{
		objectiveTextUI = value;
	}
		protected:

	// 상세 ui
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Default|UI", meta = (BindWidget))
	class UJ_DetailUI* detailTextUI;
		public:
	__declspec(property(get = GetDetailTextUI, put = SetDetailTextUI)) class UJ_DetailUI* DETAIL_TEXT_UI;
	class UJ_DetailUI* GetDetailTextUI()
	{
		return detailTextUI;
	}
	void SetDetailTextUI(class UJ_DetailUI* value)
	{
		detailTextUI = value;
	}
		protected:

	// obj ui 서브 완료 타이머 관리 배열
	TArray<FTimerHandle> subObjTimerAry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	float subObjEndTimerInterval = 0.025f;

	// 팝업 ui
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Default|UI", meta = (BindWidget))
	class UJ_ChapterPopUPUI* popupUI;
		public:
	__declspec(property(get = GetPopupUI, put = SetPopupUI)) class UJ_ChapterPopUPUI* POPUP_UI;
	class UJ_ChapterPopUPUI* GetPopupUI() {return popupUI;}
	void SetPopupUI(class UJ_ChapterPopUPUI* value) {popupUI = value;}
		protected:
	

public:
	// 결산 UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Default|UI", meta = (BindWidget))
	class UJ_MissionCompleteUI* missionCompleteUI;

protected:
	// 전술명령 시작 UMG
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayObjStartAnim(class UJ_ObjectiveTextUI* objTextUI);

	// 전술명령 완료 UMG
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayObjEndAnim(class UJ_ObjectiveTextUI* objTextUI);

	// 서브 목표 완료 UMG
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlaySubObjEndAnim(UWidget* subTextUI, int32 idx);

	// 서브 목표 완료 스케일 lerp
	UFUNCTION(BlueprintCallable)
	float PlaySubObjEndAnimLerp(class UVerticalBoxSlot *subSlot, float alpha);

	// 서브 목표 완료 size 줄이기 애니메이션 실행
	void RunSubObjTimer(UWidget* subObjUI);
	// size 줄이기 단위 실행
	void RunAlphaSubObjTimer(UWidget *subObjUI);

	// 서브 목표 완료 애니메이션 타이머 종료
	void ClearSubObjTimer();

	
	

    public:
	// 전술명령 텍스트 설정
    void SetObjUI(FTextUIData data, bool isInit = false);
	void SetObjUI(TArray<FTextUIData>& data, bool isInit = false);
	// 전술명령 시작 처리
    void StartObjUI();
    // 세부 조건 완료 처리
    void EndSubObjUI(int idx, bool isSuccess);
    // 전술명령 완료 처리
    void EndObjUI(bool isSuccess);
	// 텍스트 색상 초기화
    // void ResetTextColor();

    // 결산 UI 표시
	UFUNCTION(BlueprintCallable)
    void ActiveResultUI(const TArray<FObjectiveData>& resultObjData);

};
