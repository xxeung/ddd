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
    ,MISSION_DATA_RECEIVE
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

USTRUCT(BlueprintType)
struct FMissionDataReq
{
    GENERATED_BODY()
    
    // 맵 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString mapName;

    FMissionDataReq() : mapName(TEXT("미설정")) {}
    FMissionDataReq(const FString& mapName) : mapName(mapName) {}
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

    FJVector2D() : x(-1.f), y(-1.f) {}

    FJVector2D(float x, float y) : x(x), y(y) {}

    virtual FString ToString() const
    {
        FString str = FString::Printf(TEXT("x : %.2f, y : %.2f"), x,y);

        return str;
    }

    // x,y 위치 transform으로 변환
    virtual FTransform GetTransform() const;
};

USTRUCT(BlueprintType)
struct FMissionStartPos : public FJVector2D
{
    GENERATED_BODY()

    // fjvector2d 상속 받아서 x, y 값 있음
};

USTRUCT(BlueprintType)
struct FMissionObject : public FJVector2D
{
    GENERATED_BODY()
    
    // fjvector2d 상속 받아서 x, y 값 있음
    // 핀 순서
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    int pinNo;
    // 명령 ENUM
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    int commandNo;

    FMissionObject() : FJVector2D(), pinNo(-1), commandNo(-1) {}

    FMissionObject(float x, float y, int pinNo, int commandNo)
        : FJVector2D(x, y), pinNo(pinNo), commandNo(commandNo) {}

    FMissionObject(const FJVector2D& vec2D)
        : FJVector2D(vec2D) {}

    virtual FString ToString() const override
    {
        FString str = FString::Printf(TEXT("핀 No : %d\n명령 No : %d\nx : %.2f, y : %.2f")
        , pinNo, commandNo, x,y);

        return str;
    }
    // 명령 enum 변환
    virtual ETacticalOrder GetOrderType() const;

    // 변환 연산자
    operator FJVector2D() const
    {
        return FJVector2D(x,y);
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
    FString mapImage;
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
        , !mapImage.IsEmpty() ? TEXT("TRUE") : TEXT("FALSE")
        // ,*mapImage
        , *startPoint.ToString() 
        , *allMissionStr);

        return str;
    }

    // @@ 이미지 변환하는거 내장할까?
    
};

// 전체 미션 데이터 
USTRUCT(BlueprintType)
struct FAllMissionDataRes
{
    GENERATED_BODY()
    
    // @@ 이름 나중에 확인 필요
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    TArray<FMissionDataRes> allMissionData;

    // @@ 모든 미션 썸네일 이미지 받아오는 기능 추가
};



#pragma endregion

#pragma endregion

#pragma region 미션 관련

// XXX 플레이어 역할 : 현재 파일럿만 있음
UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
    COMMANDER = 0
    ,PILOT = 1
};

// 전술명령 | 번호 하나 밀려잇는건 목표 매니저 쪽에서 보정해줌
UENUM(BlueprintType)
enum class ETacticalOrder : uint8
{
    NONE
    ,MOVE_THIS_POINT = 1
    ,FORMATION_FLIGHT
    ,NEUTRALIZE_TARGET

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

USTRUCT(BlueprintType)
struct FObjectiveData
{
    GENERATED_BODY()
public:
    // 목표 액터
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Default|Objects")
    class AJ_BaseMissionObjective* objectiveActor = nullptr;
    // 목표 수행도 | 목표 완료시 갱신됨
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Default|Values")
    float successPercent = 0.f;
};

// 편대 역할
UENUM(BlueprintType)
enum class EPilotRole : uint8
{
    WING_COMMANDER = 0
    ,LEFT_WINGER = 1
    ,RIGHT_WINGER = 2
    ,None = 3
};

// 편대 비행 델타 용 값 | 삼각형 조건
USTRUCT(BlueprintType)
struct FFormationValue
{
    GENERATED_BODY()
    
    // 시작 지점 + forv 할 리더 위치
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    float height = 20000.f;
    // 밑변 길이
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    float baseLength = 122000.f;
    // 빗변 길이
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    float sideLength = 68100.f;
};

// 편대 비행 조건 비트마스크 enum
UENUM(BlueprintType)
enum class EFormationChecklist : uint8
{
    NONE = 0
    // 고도
    ,FLIGHT_HEIGHT = 1
    // 진형 유지
    ,ALIGN_FORMATION = 2
};

// // XXXUI 텍스트, 완료 유무, 성공 유무
// USTRUCT(BlueprintType)
// struct FObjUITextData
// {
//     GENERATED_BODY()
// public:
//     // 텍스트
//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
//     FString text;
//     // 완료 유무
//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
//     bool isEnd = false;
//     // 성공 유무
//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
//     bool isSuccess = true;

//     FObjUITextData() {}

//     FObjUITextData(const FString& text, bool isEnd = false, bool isSuccess = true) 
//         : text(text), isEnd(isEnd), isSuccess(isSuccess) {}
// };

// 목표 UI 표시용 
USTRUCT(BlueprintType)
struct FObjUIData
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    FString headerText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    TArray<FString> bodyTextAry;
    
};



// 1. 편대비행
USTRUCT(BlueprintType)
struct FFormationFlightUIData
{
    GENERATED_BODY()
public:
    // 고도 조건 체크
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    float checkHeight;
    // 현재 파일럿 고도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    float curHeight;
    // 해당 파일럿 역할
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    EPilotRole pilotRole = EPilotRole::None;
    // 진형 조건 체크 | 전체 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    bool checkFormation;
    // 진형 위치 충족 여부 | 개인
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    bool isCorrectPosition;

    FFormationFlightUIData() {}

    FFormationFlightUIData(
        float checkHeight, float curHeight, EPilotRole pilotRole, bool checkFormation, bool isCorrectPosition) :
        checkHeight(checkHeight), curHeight(curHeight), pilotRole(pilotRole), checkFormation(checkFormation), isCorrectPosition(isCorrectPosition) {}
};

// 2. 목표 무력화
USTRUCT(BlueprintType)
struct FNeutralizeTargetUIData
{
    GENERATED_BODY()
public:
    // 전체 목표 갯수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    int allTargetAmt;
    // 현재 남은 목표 갯수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
    int curTargetAmt;

    FNeutralizeTargetUIData() {}

    FNeutralizeTargetUIData(int allAmt, int curAmt) : allTargetAmt(allAmt), curTargetAmt(curAmt) {}
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
    // 미션 게임 스테이트 가져오기
    static class AJ_MissionGameState* GetMissionGameState(const UWorld* world);
    // 미션맵 로컬 플레이어 가져오기
    static class AJ_BaseMissionPawn *GetBaseMissionPawn(const UWorld *world, int32 playerIdx = 0);
    // 미션 모든 플레이어 컨트롤러 가져오기
    static class TArray<class AJ_MissionPlayerController *> GetAllMissionPC(const UWorld *world);
    // 미션 모든 플레이어 폰 가져오기
    static class TArray<APawn*> GetAllMissionPawn(const UWorld *world);
    // 범위 조건 체크
    static bool CheckValueRange(float value, float min, float max, bool inClusive = true);
    // 파일럿 역할 enum -> string 변환
    static FString PilotRoleToString(EPilotRole role);

    // 기본 미션 맵 사이즈 | 50만 cm == 5킬로
    constexpr static const float defaultMissionMapSize = 500000.f;
    // 기본 목표 지점 고도
    constexpr static const float defaultMissionObjectHeight = 400000.f;
};
