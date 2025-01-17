// Fill out your copyright notice in the Description page of Project Settings.


#include "JBS/J_Utility.h"
#include "Algo/Accumulate.h"
#include "Components/AudioComponent.h"
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "JBS/J_BaseMissionPawn.h"
#include "JBS/J_MissionGameState.h"
#include "KHS/K_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathFwd.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/CoreMiscDefines.h"
#include "UObject/Class.h"
#include "UObject/Object.h"
#include <JBS/J_GameInstance.h>
#include <KHS/K_GameInstance.h>
#include <JBS/J_MissionGamemode.h>
#include "JBS/J_MissionPlayerController.h"
#include <Engine/World.h>

#pragma region 전역 함수
// 전역 함수
UJ_GameInstance *UJ_Utility::GetJGameInstance(const UWorld* world)
{
    auto* gi =  Cast<UJ_GameInstance>(UGameplayStatics::GetGameInstance(world));
    check(gi);

    return gi;
}

UK_GameInstance *UJ_Utility::GetKGameInstance(const UWorld* world)
{
    auto* gi =  Cast<UK_GameInstance>(UGameplayStatics::GetGameInstance(world));
    check(gi);

    return gi;
}

AJ_MissionGamemode* UJ_Utility::GetMissionGamemode(const UWorld* world)
{
    auto* gm = world->GetAuthGameMode<AJ_MissionGamemode>();
    check(gm);

    return gm;
}

AK_GameState *UJ_Utility::GetKGameState(const UWorld *world)
{
    auto* gs = world->GetGameState<AK_GameState>();
    check(gs);

    return gs;
}

AJ_MissionGameState* UJ_Utility::GetMissionGameState(const UWorld* world)
{
    auto* gs = world->GetGameState<AJ_MissionGameState>();
    check(gs);

    return gs;
}

AJ_BaseMissionPawn *UJ_Utility::GetBaseMissionPawn(const UWorld *world, int32 playerIdx)
{
    auto* pc = UGameplayStatics::GetPlayerController(world, playerIdx);
    if(!pc) return nullptr;

    auto* player = pc->GetPawn<AJ_BaseMissionPawn>();
    if(!player) return nullptr;

    return player;
}

TArray<AJ_MissionPlayerController *> UJ_Utility::GetAllMissionPC(const UWorld *world)
{
    auto* gs = world->GetGameState<AJ_MissionGameState>();
    check(gs);
    // 모든 pc 가져오기
    auto allPC = gs->GetAllPlayerController();

    return allPC;
}

TArray<APawn*> UJ_Utility::GetAllMissionPawn(const UWorld *world)
{
    auto* gs = world->GetGameState<AJ_MissionGameState>();
    check(gs);
    // 모든 플레이어의 폰 가져오기
    auto allPawns = gs->GetAllPlayerPawn();

    // GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("폰 개수 : %d"), allPawns.Num()));

    return allPawns;
}

bool UJ_Utility::CheckValueRange(float value, float min, float max, bool inClusive)
{
    bool result = inClusive ?
        min <= value && value <= max
        : min < value && value < max;

    return result;
}

FString UJ_Utility::PilotRoleToString(EPilotRole role)
{
    FString result;

    switch (role) {
    case EPilotRole::WING_COMMANDER:
        result = TEXT("편대장");
        break;
    case EPilotRole::LEFT_WINGER:
        result = TEXT("Left Winger");
        break;
    case EPilotRole::RIGHT_WINGER:
        result = TEXT("Right Winger");
        break;
    }

    return result;
}

FString UJ_Utility::TacticalOrderToString(ETacticalOrder type)
{
    FString result;

    switch(type)
    {

    case ETacticalOrder::NONE:
        result = TEXT("");
        break;
    case ETacticalOrder::MOVE_THIS_POINT:
        result = TEXT("");
        break;
    case ETacticalOrder::FORMATION_FLIGHT:
        result = TEXT("편대 비행 유지하기");
        break;
    case ETacticalOrder::NEUTRALIZE_TARGET:
        result = TEXT("지상 목표 제거하기");
        break;
    }

    return result;
}

FString UJ_Utility::ToStringBool(bool value)
{
    return value ? TEXT("TRUE") : TEXT("FALSE");
}

float UJ_Utility::CalcAverage(TArray<float> valueAry)
{
    if(valueAry.Num() == 0)
        return 0.0f;

    float sum = Algo::Accumulate(valueAry, 0.0f);

    return sum / valueAry.Num();
}

FString UJ_Utility::SubStrEnumString(const FString &typeStr)
{
    // 자를 인덱스
    int idx = typeStr.Find("::");
    if(idx == INDEX_NONE) return "";

    // :: 다음 부분 추출
    return typeStr.RightChop(idx + 2);
}

FString UJ_Utility::ToStringPercent(float percent)
{
    return FString::Printf(TEXT("%d%%"), static_cast<int>(percent * 100));
}

void UJ_Utility::PrintFullLog(const FString &str, const float &time, const FColor &color)
{
    GEngine->AddOnScreenDebugMessage(-1, time, color, FString::Printf(TEXT("%s"), *str));
    UE_LOG(LogTemp, Warning, TEXT("%s"), *str);
}

int UJ_Utility::ConvertEngineProgressToMissionProcessIdx(const EEngineProgress& value)
{
    int result = static_cast<int>(value) + 6;
    if(value >= EEngineProgress::STANDBY_OTHER_PLAYER)
    {
        result++;
    }

    return result;
}

UDataTable *UJ_Utility::GetMissionProgressDT(const UWorld *world)
{
    // gi 가져오기
    auto* gi = GetJGameInstance(world);

    return gi->missionProgressDT;
}

void UJ_Utility::PlaySoundWithPause(class UAudioComponent *audio, class USoundBase *sound)
{
    if(!sound || !audio) return;
    // 정지
    audio->Stop();
    // 소리 설정
    audio->SetSound(sound);
    // 재생
    audio->Play();
}

#pragma endregion

#pragma region 구조체 함수 

ETacticalOrder FMissionObject::GetOrderType() const
{
    ETacticalOrder type;

    switch(this->commandNo)
    {
        case 0: type = ETacticalOrder::MOVE_THIS_POINT; break;
        case 1: type = ETacticalOrder::FORMATION_FLIGHT; break;
        case 2: type = ETacticalOrder::NEUTRALIZE_TARGET; break;

        default: type = ETacticalOrder::NONE; break;
    }

    return type;
}

FTransform FJVector2D::GetTransform() const
{
    // x,y 값 이랑 정해진 고도 값을 가지고 변환
    // 범위 == 5킬로 기준 -25만 ~ 25만
    float minValue = UJ_Utility::defaultMissionMapSize * -.5f;
    float maxValue = UJ_Utility::defaultMissionMapSize * .5f;
    // x,y,z 계산
    float mapX = FMath::Lerp(minValue, maxValue, this->x / 100.f);
    float mapY = FMath::Lerp(minValue, maxValue, this->y / 100.f);
    float mapZ = UJ_Utility::defaultMissionObjectHeight;
    // 위치 값
    FVector vec(mapX, mapY, mapZ);
    // @@ 회전 값 | 임시로 0,0,mapZ 를 바라보도록 | 받아와야 할지도?
    FVector targetLoc(0,0,mapZ);
    FRotator rot = (targetLoc - vec).GetSafeNormal().Rotation();

    
    FTransform tr = FTransform(rot,vec,  FVector::OneVector);

    return tr;
}

FTransform FMissionStartPos::GetTransform(const FVector &targetLoc) const
{
    auto newTR = this->GetTransform();
    const auto& location = newTR.GetLocation();
    
    // 방향 계산 | Z를 동기화해서 2d로 계산되도록
    FVector targetLocation = FVector(targetLoc.X, targetLoc.Y, location.Z);
    FVector dir = (targetLocation - location).GetSafeNormal();

    // 방향을 바라보도록 회전
    newTR.SetRotation(dir.ToOrientationQuat());

    return newTR;
}

bool UJ_Utility::GetLocalPlayerController(const UWorld *world, class AJ_MissionPlayerController *&outPC)
{
    // 로컬 플레이어 찾기
    outPC = Cast<AJ_MissionPlayerController>(world->GetFirstPlayerController());

    // 유효성 체크
    return IsValid(outPC);
}

// 엔진 수행 맵 초기화
void FEngineProgressAllData::InitDataMap(const TArray<AJ_MissionPlayerController *> &pcAry)
{
    for(auto* pc : pcAry)
    {
        dataMap.Add(pc, FEngineProgressData());
    }
}

int FEngineProgressData::ConvertProgressEnumToInt(EEngineProgress type) const
{
    // uint8 범위 넘어가서 그대로 사용 불가 변환 해야함 | 2의 type -1 제곱
    int pow = static_cast<int>(type) - 1;
    int value = static_cast<int>(FMath::Pow(2.0, pow));

    return value;
}

void FEngineProgressData::AddSuccessValue(EEngineProgress type)
{
    this->successValue += ConvertProgressEnumToInt(type);
}

void FEngineProgressData::SetNextProgress()
{
    switch(this->curProgress)
    {
    case EEngineProgress::None:
        break;
    case EEngineProgress::MIC_SWITCH_ON:
        this->curProgress = EEngineProgress::ENGINE_GEN_SWITCH_ON;
        break;
    case EEngineProgress::ENGINE_GEN_SWITCH_ON:
        this->curProgress = EEngineProgress::ENGINE_CONTROL_SWITCH_ON;
        break;
    case EEngineProgress::ENGINE_CONTROL_SWITCH_ON:
        this->curProgress = EEngineProgress::ENGINE_MASTER_SWITCH_ON;
        break;
    case EEngineProgress::ENGINE_MASTER_SWITCH_ON:
        this->curProgress = EEngineProgress::JFS_STARTER_SWITCH_ON;
        break;
    case EEngineProgress::JFS_STARTER_SWITCH_ON:
        this->curProgress = EEngineProgress::JFS_HANDLE_PULL;
        break;
    case EEngineProgress::JFS_HANDLE_PULL:
        this->curProgress = EEngineProgress::ENGINE_THROTTLE_IDLE;
        break;
    case EEngineProgress::ENGINE_THROTTLE_IDLE:
        this->curProgress = EEngineProgress::CLOSE_CANOPY;
        break;
    case EEngineProgress::CLOSE_CANOPY:
        this->curProgress = EEngineProgress::STANDBY_OTHER_PLAYER;
        break;
    case EEngineProgress::STANDBY_OTHER_PLAYER:
        this->curProgress = EEngineProgress::RELEASE_SIDE_BREAK;
        break;
    case EEngineProgress::RELEASE_SIDE_BREAK:
        this->curProgress = EEngineProgress::TAKE_OFF;
        break;
    }
}

float FEngineProgressData::CalcSuccessRate(int value)
{
    int cnt = 0;
    int enumCnt = 0;

    for(int i = static_cast<int>(EEngineProgress::MIC_SWITCH_ON); i <= static_cast<int>(EEngineProgress::RELEASE_SIDE_BREAK); i++)
    {
        // @@ 어떤 거 틀렸는지 반환 할 수 있게 고도화 가능
        // 수행 절차 하나하나 성공 확인
        bool isSuccess = value & i;
        if(isSuccess)
            cnt++;

        enumCnt++;
    }
    // 비율 반환
    return (float) cnt / enumCnt;
}

FString FEngineProgressData::ToStringProgressEnum(EEngineProgress type) const
{
    FString str = "";

    switch (type) {
    case EEngineProgress::None:
        str = TEXT("미설정");
        break;
    case EEngineProgress::MIC_SWITCH_ON:
        str = TEXT("통신 시스템 활성화");
        break;
    case EEngineProgress::ENGINE_GEN_SWITCH_ON:
        str = TEXT("엔진 발전기 준비 완료");
        break;
    case EEngineProgress::ENGINE_CONTROL_SWITCH_ON:
        str = TEXT("엔진 제어 장치 가동");
        break;
    case EEngineProgress::ENGINE_MASTER_SWITCH_ON:
        str = TEXT("엔진 마스터 켜기");
        break;
    case EEngineProgress::JFS_STARTER_SWITCH_ON:
        str = TEXT("JFS 스타트 준비");
        break;
    case EEngineProgress::JFS_HANDLE_PULL:
        str = TEXT("JFS 핸들 당기기");
        break;
    case EEngineProgress::ENGINE_THROTTLE_IDLE:
        str = TEXT("엔진 공회전 상태 유지");
        break;
    case EEngineProgress::CLOSE_CANOPY:
        str = TEXT("조종석 밀폐");
        break;
    case EEngineProgress::STANDBY_OTHER_PLAYER:
        str = TEXT("다른 파일럿 대기");
        break;
    case EEngineProgress::RELEASE_SIDE_BREAK:
        str = TEXT("기체 정지 상태 해제");
        break;
    }

    return str;
}

FString FRichString::FormatString(const FString &str, ETextStyle type) const
{
    // 결과 string
    FString result = "";

    // 줄바꿈을 구분자로 문자열 정리
    TArray<FString> stringAry;

    FString temp = str;
    temp.ReplaceInline(TEXT("\r\n"), TEXT("\n"));
    temp.ReplaceInline(TEXT("\r"), TEXT("\n"));
    temp.ParseIntoArray(stringAry, TEXT("\n"), false);

    // 한 문장을 리치 텍스트로 묶어서 합치기
    if(stringAry.Num() > 1)
    {
        for(const auto& sentence : stringAry)
        {
            // if(!sentence.IsEmpty())
                result.Append(FRichString(sentence, type).FormatString(sentence, type));

            result.Append(TEXT("\n"));
        }
    }
    else {
        // 스타일 처리
        result = FString::Printf(TEXT("<%s>%s</>")
        , *UJ_Utility::ToStringEnumPure(type)
        , *temp);
    }

    return result;
}

FString FRichString::GetFormatString() const
{
    return FormatString(value, styleType);
}

bool FEngineProgressData::CheckProgressSuccess(EEngineProgress type) const
{
    int checkType = ConvertProgressEnumToInt(type);

    return successValue & checkType;
}

FString FAIFeedbackRes::ToString() const
{
    return FString::Printf(TEXT("결과 등급 : %d, 코멘트 : %s"), rank,*comment);
}

int FCommanderVoiceReq::ConvertOrderTypeToId(ETacticalOrder type)
{
    // FIXME 정의서 확인 후 수정 필요
    int result = -1;
    switch (type) {
    case ETacticalOrder::NONE:
        break;
    case ETacticalOrder::MOVE_THIS_POINT:
        break;
    case ETacticalOrder::FORMATION_FLIGHT:
        result = 12;
        break;
    case ETacticalOrder::NEUTRALIZE_TARGET:
        result = 15;
        break;
    case ETacticalOrder::ENGINE_START:
        result = 1;
        break;
    case ETacticalOrder::TAKE_OFF:
        result = 10;
        break;
    }

    return result;
}

FString FFormationFlightUIData::ToStringPilotRolt() const
{
    FString result = "";

    switch (pilotRole) {
    case EPilotRole::WING_COMMANDER:
        result = TEXT("편대장");
        break;
    case EPilotRole::RIGHT_WINGER:
        result = TEXT("우측 윙맨");
        break;
    case EPilotRole::LEFT_WINGER:
        result = TEXT("좌측 윙맨");
        break;
    case EPilotRole::None:
        result = TEXT("미설정");
        break;
    }

    return result;
}
#pragma endregion

