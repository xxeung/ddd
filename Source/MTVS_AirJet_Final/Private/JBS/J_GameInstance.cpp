// Fill out your copyright notice in the Description page of Project Settings.


#include "JBS/J_GameInstance.h"
#include "Engine/Engine.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "JBS/J_JsonUtility.h"
#include "JBS/J_Utility.h"
#include "JsonObjectConverter.h"
#include "Math/UnrealMathUtility.h"
#include <Engine/World.h>


void UJ_GameInstance::ReqData(FResponseDelegate resDel, const FString& url, bool useDefaultURL, ERequestType type, const FString& jsonData)
{
	// 모듈 생성
	FHttpModule* http = &FHttpModule::Get();

	// 요청 정보 설정
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpReq = http->CreateRequest();
	
	// 응답 함수 연결
	httpReq->OnProcessRequestComplete().BindLambda([resDel, this](FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess)
	{
		if(isSuccess)
		{
			FString resStr = res->GetContentAsString();
			// @@ 로그인 시에 인증 값 저장해두기 인스턴스
			authorValue = res->GetHeader(TEXT("Authorization"));

			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("인증 값 : %s"), *authorValue));

			resDel.ExecuteIfBound(resStr, isSuccess)	;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("요청 실패"));
		}
	});
	// url 설정
	FString resultURL = useDefaultURL ? defaultURL + url : url;
	httpReq->SetURL(resultURL);
	// 보낼 jsondata가 있다면 넣기
	if(type == ERequestType::POST && !jsonData.IsEmpty())
	{
		httpReq->SetContentAsString(jsonData);
	}
	// 요청 타입 설정
	httpReq->SetVerb(reqTypeMap[type]);
	// 헤더 설정
	httpReq->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	httpReq->SetHeader(TEXT("Authorization"), authorValue);

	// 서버 요청
	httpReq->ProcessRequest();
}

//XXX 웹 통신 요청 예시
void UJ_GameInstance::ReqTempCallback()
{
	// json으로 보낼 구조체 준비
	FTempJsonAry jsonAry;
	for(int i = 0; i < 3; i++)
	{
		FTempJson temp;
		temp.id = FMath::RandRange(0, 999);
		temp.userId = FMath::RandRange(1000, 9999);
		temp.title = TEXT("doremi");
		temp.body = TEXT("farondo");

		jsonAry.tempJsons.Add(temp);
	}

	// 내부에서 jsonManager 찾은 후 EJsonType에 해당하는 Req 함수 실행
	UJ_JsonUtility::RequestExecute<FTempJsonAry>(GetWorld(), EJsonType::TEMP01_CALLBACK, jsonAry);
}

void UJ_GameInstance::ResTempCallback(const FString& jsonData, bool isSuccess)
{
	FTempJsonAry jsonAry;
	FJsonObjectConverter::JsonObjectStringToUStruct<FTempJsonAry>(jsonData, &jsonAry,0,0);

	// 변환된 구조체가지고 뭔가 하기
	auto rresult = jsonAry;
	// auto rresult = UJ_JsonUtility::JsonParseTempAry(resStr);

	FString result;
	for(auto r : rresult.tempJsons)
	{
		result.Append(r.ToString());
	}
	

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("결과 : %s"), *result));
	UE_LOG(LogTemp, Warning, TEXT("결과 : %s"), *result);
}

void UJ_GameInstance::ResSignup(const FString &jsonData, bool isSuccess)
{
	FResSimple resData;
	FJsonObjectConverter::JsonObjectStringToUStruct(jsonData, &resData,0,0);

	GEngine->AddOnScreenDebugMessage(-1, 31.f, FColor::Green, FString::Printf(TEXT("%s"), *resData.ToString()));
}

void UJ_GameInstance::ResLogin(const FString &jsonData, bool isSuccess)
{
	FResSimple resData;
	FJsonObjectConverter::JsonObjectStringToUStruct(jsonData, &resData,0,0);

	GEngine->AddOnScreenDebugMessage(-1, 31.f, FColor::Green, FString::Printf(TEXT("%s"), *resData.ToString()));
}
