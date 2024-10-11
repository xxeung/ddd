// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "J_Utility.generated.h"

/**
 * 
 */
// 전송 타입
UENUM(BlueprintType)
enum class ERequestType : uint8
{
    GET = 0
    ,POST = 1
};

// 통신 타입 
UENUM(BlueprintType)
enum class EJsonType : uint8
{
    TEMP01_CALLBACK = 0
    ,SIGN_UP
    ,LOGIN
    ,TEMP02_AUTH
};

#pragma region 웹 통신 용 구조체

#pragma region 요청단
USTRUCT(BlueprintType)
struct FSignupReq
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString loginId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString password;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString confirmPassword;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString nickname;

    FSignupReq() : loginId(TEXT("미설정")), password(TEXT("미설정")), confirmPassword(TEXT("미설정")), nickname(TEXT("미설정")) {}

    FSignupReq(FString loginId ,FString password ,FString confirmPassword ,FString nickname) 
        : loginId(loginId)
        ,password(password)
        ,confirmPassword(confirmPassword)
        ,nickname(nickname) {}
};

USTRUCT(BlueprintType)
struct FLoginReq
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString loginId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString password;

    FLoginReq() : loginId(TEXT("미설정")), password(TEXT("미설정")) {}

    FLoginReq(FString loginId ,FString password) 
        : loginId(loginId)
        ,password(password) {}
};

#pragma endregion

#pragma region 반환단

USTRUCT(BlueprintType)
struct FResSimple
{
    GENERATED_BODY()
    
    // 성공 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    bool success;
    // 서버단 메시지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString response;

    FString ToString() const
    {
        FString str = FString::Printf(TEXT("성공 여부 : %s\n응답 결과 데이터 : %s")
            , this->success ? TEXT("TRUE") : TEXT("FALSE")
            , *this->response);

        return str;
    }
};

USTRUCT(BlueprintType)
struct FLoginRes : public FResSimple
{
    GENERATED_BODY()

    // 로그인 한 아이디
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString loginId;

    FString ToString() const
    {
        FString str = FString::Printf(TEXT("성공 여부 : %s\n응답 결과 데이터 : %s\n로그인 id : %s")
            , this->success ? TEXT("TRUE") : TEXT("FALSE")
            , *this->response
            , *this->loginId);

        return str;
    }
};

USTRUCT(BlueprintType)
struct FJVector2D
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    float x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    float y;

    virtual FString ToString() const
    {
        FString str = FString::Printf(TEXT("x : %.2f, y : %.2f"), x,y);

        return str;
    }
};

USTRUCT(BlueprintType)
struct FMissionStartPos : public FJVector2D
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FMissionObject : public FJVector2D
{
    GENERATED_BODY()
    
    // 핀 순서
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    int pinNo;
    // 명령 ENUM
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    int commandNo;

    virtual FString ToString() const override
    {
        FString str = FString::Printf(TEXT("핀 No : %d\n명령 No : %d\nx : %.2f, y : %.2f")
        , pinNo, commandNo, x,y);

        return str;
    }
};

USTRUCT(BlueprintType)
struct FMissionDataRes
{
    GENERATED_BODY()
    
    // 맵 제작자
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString producer;
    // 위도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    float latitude;
    // 경도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    float longitude;
    // 맵 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString mapName;
    // 썸네일 이미지 바이트 배열
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values|Image")
    TArray<uint8> mapImage;
    // 시작 지점
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FMissionStartPos startPoint;
    // 목표 데이터 배열
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    TArray<FMissionObject> mission;

    FString ToString() const
    {
        FString allMissionStr = TEXT("[");
        for(auto m : mission)
        {
            FString temp = FString::Printf(TEXT("{\n    %s\n    },"), *m.ToString());
            allMissionStr.Append(temp);

        }
        allMissionStr.Append(TEXT("]"));

        FString str = FString::Printf(
        TEXT("맵 제작자 : %s\n위도 : %.2f , 경도 : %.2f\n맵 이름 : %s\n썸네일 이미지 존재 : %s\n시작 지점 : %s\n목표들 : %s")
        , *producer
        , latitude
        , longitude
        , *mapName
        , mapImage.Num() > 0 ? TEXT("TRUE") : TEXT("FALSE")
        , *startPoint.ToString() 
        , *allMissionStr);

        return str;
    }
};



#pragma endregion

#pragma endregion

#pragma region 미션 관련

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
    COMMANDER = 0
    ,PILOT = 1
};

USTRUCT(BlueprintType)
struct FMissionPlayerSpawnPoints
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FName spawnPointTag;
    // 스폰 포인트 맵
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Objects")
    TMap<EPlayerRole, class AJ_MissionSpawnPointActor*> spawnPointMap;

    // 스폰 포인트 기본 위치 맵
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    TMap<EPlayerRole, FTransform> spawnPointDefaultPos;

    // 스폰 포인트 프리팹
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Classes")
    TSubclassOf<class AJ_MissionSpawnPointActor> spawnPointPrefab;
    // TSubclassOf<AActor> spawnPointPrefab;
};

#pragma endregion

// @@ 테스트 용
USTRUCT(BlueprintType)
struct FTempJson
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    int userId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    int id;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString body;

    FTempJson() : userId(-1), id(-1), title(TEXT("미정")), body(TEXT("없음")) {}

    FString ToString() const
    {
        return FString::Printf(TEXT("userid : %d, id : %d, title : %s, body : %s\n"), userId, id, *title, *body);
    }
};

USTRUCT(BlueprintType)
struct FTempJsonAry
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    TArray<FTempJson> tempJsons;
};

UCLASS()
class MTVS_AIRJET_FINAL_API UJ_Utility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    static class UJ_GameInstance *GetJGameInstance(const UWorld* world);
    // 게임 인스턴스 가져오기
    static class UK_GameInstance* GetKGameInstance(const UWorld* world);
    // 미션 게임모드 가져오기
    static class AJ_MissionGamemode* GetMissionGamemode(const UWorld* world);
    // 게임 스테이트 가져오기
    static class AK_GameState *GetKGameState(const UWorld *world);
};
