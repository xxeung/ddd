// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <JBS/J_Utility.h>
#include <JsonObjectConverter.h>
#include <JBS/J_JsonManager.h>
#include "J_GameInstance.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_TwoParams(FResponseDelegate, const FString&, bool);

DECLARE_DELEGATE_OneParam(FResSimpleDelegate, const FResSimple&);

DECLARE_DELEGATE_OneParam(FLoginResDelegate, const FLoginRes&);

// _One~ _NineParams 까지 가능

UCLASS()
class MTVS_AIRJET_FINAL_API UJ_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	// 기본 ip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	FString defaultURL = TEXT("http://125.132.216.190:7757/api/");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	FString authorValue = TEXT("");

	// 전송 타입 맵
	TMap<ERequestType, FString> reqTypeMap = {
		{ERequestType::GET, TEXT("GET")}
		,{ERequestType::POST, TEXT("POST")}
	};	

#pragma region 반응 딜리게이트 단
	// json 데이터 변환 시킬 res 함수 연결용 딜리게이트
	// 통신 api 하나당 한 개 씩 추가
	FResponseDelegate tempDel;

	FResponseDelegate signupDel;

	FResponseDelegate loginDel;

	FResponseDelegate tempLoginAuthDel;

#pragma endregion

	// 미션
	// 자신 플레이어 역할
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values", BlueprintGetter=GetPlayerRole, BlueprintSetter=SetPlayerRole)
	EPlayerRole playerRole;
		public:
	__declspec(property(get = GetPlayerRole, put = SetPlayerRole)) EPlayerRole PLAYER_ROLE;
	UFUNCTION(BlueprintGetter)
	EPlayerRole GetPlayerRole()
	{
		return playerRole;
	}
	UFUNCTION(BlueprintSetter)
	void SetPlayerRole(EPlayerRole value)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("현재 플레이어 역할 : %s"), *UEnum::GetValueAsString(value)));
		playerRole = value;
	}
		protected:

	// 미션 플레이어 역할, 캐릭터 프리팹 맵
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Classes")
	TMap<EPlayerRole, TSubclassOf<class APawn>> playerPrefabMap;

public:
#pragma region 사용 함수 연결용 딜리게이트 단
	// res 구조체 데이터 사용할 함수 연결용 딜리게이트
	FResSimpleDelegate tempLoginAuthUseDel;

	FLoginResDelegate tempLoginResUseDel;
#pragma endregion

protected:
	// 3-1. 서버에 요청 : 반응 딜리게이트, 구조체 데이터, url, 기본 url 사용 여부, 전송 타입(POST or GET)
	template<typename InStructType>
	void RequestData(FResponseDelegate resDel, const InStructType& structData, const FString& url = "", bool useDefaultURL = true, ERequestType type = ERequestType::POST)
	{
		// 구조체 -> json 화
		FString jsonData;
		FJsonObjectConverter::UStructToJsonObjectString(structData, jsonData, 0, 0);

		RequestData(resDel, jsonData, url, useDefaultURL, type);
	}

	// 3-2. 서버에 요청 : 반응 딜리게이트, json 데이터, url, 기본 url 사용 여부, 전송 타입(POST or GET)
	void RequestData(FResponseDelegate resDel, const FString& jsonData = TEXT(""),  const FString& url = "", bool useDefaultURL = true, ERequestType type = ERequestType::POST);

#pragma region 반응 함수 단

	// solved 테스트용 데이터 주고 받기
	UFUNCTION(BlueprintCallable)
	virtual void ReqTempCallback();

	// 4. 요청 후 반응 함수 예시
	UFUNCTION()
	virtual void ResTempCallback(const FString& jsonData, bool isSuccess);

	UFUNCTION()
	virtual void ResSignup(const FString &jsonData, bool isSuccess);

	UFUNCTION()
	virtual void ResLogin(const FString &jsonData, bool isSuccess);

	UFUNCTION()
	virtual void ResLoginAuth(const FString &jsonData, bool isSuccess);

#pragma endregion

public:
	// 2-1. 통신 타입, 데이터 받아서 서버에 요청 시작
	template<typename InStructType>
	void RequestToServerByType(EJsonType type, const InStructType& structData)
	{
		// 구조체 데이터 json 변환
		FString jsonData;
		FJsonObjectConverter::UStructToJsonObjectString(structData, jsonData, 0, 0);

		// 서버에 요청 시작
		RequestToServerByType(type, jsonData);
	}

	// 2-2. 요청 타입에 따라 다른 요청 실행 : 요청 타입, json string 데이터(생략 가능)
	virtual void RequestToServerByType(EJsonType type, const FString &sendJsonData = TEXT(""));

	// 자신 플레이어 역할에 맞는 프리팹 주기
	virtual TSubclassOf<APawn> GetMissionPlayerPrefab();
};