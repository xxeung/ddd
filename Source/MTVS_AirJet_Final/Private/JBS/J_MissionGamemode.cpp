// Fill out your copyright notice in the Description page of Project Settings.


#include "JBS/J_MissionGamemode.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "Engine/LatentActionManager.h"
#include "GameFramework/PlayerController.h"
#include "JBS/J_GameInstance.h"
#include "JBS/J_JsonUtility.h"
#include "JBS/J_MissionGameState.h"
#include "JBS/J_MissionPlayerController.h"
#include "JBS/J_MissionStartPointActor.h"
#include "JBS/J_Utility.h"
#include "KHS/K_CesiumManager.h"
#include "KHS/K_CesiumTeleportBox.h"
#include "KHS/K_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "LHJ/L_Viper.h"
#include "Math/MathFwd.h"
#include "JBS/J_ObjectiveManagerComponent.h"
#include "Templates/Casts.h"
#include "TimerManager.h"
#include <JBS/J_MissionSpawnPointActor.h>
#include <Engine/World.h>
#include "JBS/J_ObjectiveUIComp.h"
#include "JBS/J_ObjectiveUI.h"
#include "JBS/J_ChapterPopUPUI.h"

#pragma region 안쓰는 무덤
// void AJ_MissionGamemode::LoadMissionMap()
// {
//     // 미션 지역 로드
//     UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), missionMapPrefab, true, true, FLatentActionInfo());
// }

void AJ_MissionGamemode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

#pragma endregion

#pragma region getset
AJ_MissionStartPointActor *AJ_MissionGamemode::GetStartPointActor()
{
    // 없으면 다시 생성
    if(!startPointActor)
        InitMissionStartPoint(curMissionData.startPoint);

    return startPointActor;
}

#pragma endregion

#pragma region 레벨 시작 단
/*
레벨 시작 시 목표 매니저 컴포넌트 캐시 | BeginPlay
-> 미션 시작 | StartMission
-> 미션 데이터 호스트 게임 인스턴스 에서 가져오기 | LoadMissionData
-> 미션 시작 지점 포인트 액터 생성 | InitMissionStartPoint
-> 전술명령 생성 및 초기화 | InitDefaultObj, InitObjectiveList
-> 플레이어 시작 위치 설정 | SetSpawnPoints // 지휘관 있었을때의 영향으로 역할에 따라 분류되서 가져옴
-> 다른 액터들이 준비되도록 .1초 타이머 이후 세슘 액터 캐시 | CacheCesiumActors
*/
void AJ_MissionGamemode::BeginPlay()
{
    Super::BeginPlay();

    // 목표 매니저 컴포넌트 가져오기 | 생성자에서 생성하는게 안되서 블프에 넣음
    objectiveManagerComp = this->GetComponentByClass<UJ_ObjectiveManagerComponent>();
    
    // 미션 시작
    StartMission();
}

void AJ_MissionGamemode::StartMission()
{
    // 디버그용 더미 미션 데이터 사용 여부
    if(enableUsingDummyMissionData)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("현재 디버그용 더미 미션 데이터 사용 중"));
        curMissionData = dummyMissionData;
    }
    // GI 미션 데이터 사용
    else 
        curMissionData = LoadMissionData();

    // 첫 미션 데이터 가져오기
    const auto& missions = curMissionData.mission;
    const FMissionObject& firstMission = missions.Num() > 0 ? missions[0] : FMissionObject();

    // 시작 지점 액터 생성 | 첫 미션을 바라보도록 생성
    InitMissionStartPoint(curMissionData.startPoint, firstMission);

    // 목표 매니저 컴포넌트 존재 확인
    check(objectiveManagerComp);

    // 시동 및 이륙 목표 추가
    objectiveManagerComp->InitDefaultObj();
    // 전술 명령 목표 추가
    objectiveManagerComp->InitObjectiveList(curMissionData.mission);

    // 플레이어 스폰포인트 가져와서 설정
    SetSpawnPoints(spawnPoints);

    // 모든 액터가 준비된 시점에 호출
    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager()
        .SetTimer(timerHandle, [this]() mutable
    {
        //타이머에서 할 거
        // 세슘 관련 액터 캐시
        CacheCesiumActors();
    }, .1f, false);
}

FMissionDataRes AJ_MissionGamemode::LoadMissionData()
{
    // 호스트 gi의 미션 데이터 가져오기 | 설정은 세션 단에서 함
    const auto* gi = UJ_Utility::GetKGameInstance(GetWorld());
    const FMissionDataRes& resultData = gi->MissionData;

    // 잘못된 미션 데이터 일 경우 더미 데이터 사용
    bool isValid = !resultData.mapName.IsEmpty();
    if(isValid)
    {
        FString debugStr = TEXT("잘못된 미션 데이터 | 더미 데이터 사용");
        UJ_Utility::PrintFullLog(debugStr, 5.f, FColor::Red);
    }
    
    return isValid ? resultData : dummyMissionData;
}

void AJ_MissionGamemode::InitMissionStartPoint(const FMissionStartPos &startPointData, const FMissionObject& firstMission)
{
    // 첫 미션이 존재하면 그걸 바라보게, 아니면 맵 중앙 바라보도록 위치 가져오기
    const FTransform& spawnTR = firstMission.GetOrderType() != ETacticalOrder::NONE 
        ? startPointData.GetTransform(firstMission.GetTransform().GetLocation())
        : startPointData.GetTransform();

    // 액터 생성
    auto* spActor = GetWorld()->
        SpawnActor<AJ_MissionStartPointActor>(startPointActorPrefab, spawnTR);
    if(!spActor) return;

    // 캐시
    START_POINT_ACTOR = spActor;
}

void AJ_MissionGamemode::SetSpawnPoints(FMissionPlayerSpawnPoints& spawnPointsStruct)
{
    // 레벨에 이미 배치된 스폰 포인트 찾기
    TArray<AActor*> outActors;
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AJ_MissionSpawnPointActor::StaticClass(), spawnPointsStruct.spawnPointTag, outActors);
    // 클래스 변환
    TArray<AJ_MissionSpawnPointActor*> spPointActors;
    Algo::Transform(outActors, spPointActors, [](AActor* temp){
        return Cast<AJ_MissionSpawnPointActor>(temp);
    });
    //캐스트 후
    for(auto* spPoint : spPointActors)
    {
        // 맵에 추가
        spawnPointsStruct.spawnPointMap[spPoint->spawnType] = spPoint;
    }
}

void AJ_MissionGamemode::CacheCesiumActors()
{
    // 텔포 박스 가져오기
    cesiumTPBox = Cast<AK_CesiumTeleportBox>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AK_CesiumTeleportBox::StaticClass()));

    // 정점 액터 가져오기
    cesiumZeroPos = Cast<AK_CesiumManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(),
         AK_CesiumManager::StaticClass()));

    // GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("무사히 세슘 관련 액터 캐시 성공"));
}
#pragma endregion

#pragma region 플레이어 접속 단
/*
플레이어 접속 | PostLogin
-> 호스트 pc 시작 버튼에 미션 시작 함수 바인드 | PostLogin
-> 전투기 생성 요청 | pc->SpawnMyPlayer
-> 게임 인스턴스에서 자기 역할에 맞는 플레이어 프리팹 가져오기 | pc->gi->GetMissionPlayerPrefab
-> 자기 역할에 맞는 스폰위치 가져오기 | GetPlayerSpawnTransfrom // 무조건 파일럿
-> 스폰 포인트 구하기->없으면 월드에서 다시 찾기-> 없으면 생성 | GetSpawnPoint, AddSpawnPoint
-> 플레이어 생성 후 포제스 | SpawnActor, Possess
-> 역할에 따라 마우스 커서 및 입력 처리

미션 시작 버튼 누르면 미션 시작 설정 | StartMissionLevel
-> 시작 보이스 및 자막 재생 | CRPC_PlayCommanderVoice3 | EMissionProcess::MISSION_START
-> 팝업 UI 시작 | CRPC_ActivePopupUI
-> 팝업 UI 종료시 시동 목표 시작 | StartDefaultObjective
-> 미션 시작시 시동 절차 목표 활성화 | StartDefualtObj
*/
void AJ_MissionGamemode::PostLogin(APlayerController *newPlayer)
{
    Super::PostLogin(newPlayer);

    // 역할 설정후 플레이어 스폰
    AJ_MissionPlayerController* pc = CastChecked<AJ_MissionPlayerController>(newPlayer);
    if(!pc) return;

    // 현재 주요 스폰 시스템
    if(enableUseOldSpawnSystem)
        pc->SpawnMyPlayer();

    // 미션 보이스 로드 하라고 요청
    pc->CRPC_ReqMissionVoiceData();
    
    // 호스트 시작 버튼에 미션 시작 함수 딜리게이트
    if(HasAuthority() && newPlayer->IsLocalController())
    {
        pc->StartGameDel_Mission.AddUObject(this, &AJ_MissionGamemode::StartMissionLevel);

        // 호스트 일때 비로그인시 임시 로그인 처리
        auto* gi = UJ_Utility::GetJGameInstance(GetWorld());
        if(gi->authorValue.IsEmpty())
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("디버그용 임시 로그인 처리"));

            FLoginReq tempLogin(TEXT("user"), TEXT("1234"));

            UJ_JsonUtility::RequestExecute(GetWorld(), EJsonType::LOGIN, tempLogin, gi);
        }
    }
}

// 산개 로직 적용해서 스폰 위치 계산
FTransform AJ_MissionGamemode::GetPlayerSpawnTransfrom(EPlayerRole role, AJ_MissionPlayerController* pc)
{
    // 해당 역할의 스폰 위치 계산
    auto spawnTR = this->GetSpawnPoint(role)->GetActorTransform();
    // 몇 번째 pc 인지 받아서 시작 위치에서 뒤로 - * i 만큼 계산

    // 시작 지점의 뒤 벡터 구하기
    const FVector& backVector = -1 * spawnTR.GetUnitAxis(EAxis::X)
                        * spawnInterval * spawnCnt;

    // 위치 결정
    spawnTR.SetLocation(spawnTR.GetLocation() + backVector);

    // 스폰 카운트
    spawnCnt++;

    return spawnTR;
}

// 스폰포인트 있으면 그거 주고 없으면 찾기 + 그래도 없으면 생성해서 주기
AJ_MissionSpawnPointActor* AJ_MissionGamemode::GetSpawnPoint(EPlayerRole role)
{
    // 절대 획득
    auto* spPos = spawnPoints.spawnPointMap[role];
    if(!spPos)
    {
        // 없으면 월드에서 다시 가져와서 설정
        SetSpawnPoints(spawnPoints);
        spPos = spawnPoints.spawnPointMap[role];
        // 그래도 없으면 생성하기
        if(!spPos)
            spPos = AddSpawnPoint(spawnPoints, role);
    }

    return spPos;
}

AJ_MissionSpawnPointActor* AJ_MissionGamemode::AddSpawnPoint(FMissionPlayerSpawnPoints& spawnPointsStruct, const EPlayerRole& addRole)
{
    // 해당 역할을 가진 스폰 포인트 생성
    auto* spPoint = GetWorld()->SpawnActor<AJ_MissionSpawnPointActor>(spawnPointsStruct.spawnPointPrefab, spawnPointsStruct.spawnPointDefaultPos[addRole]);
    // 역할 설정
    spPoint->spawnType = addRole;

    // 맵에 추가
    spawnPointsStruct.spawnPointMap[addRole] = spPoint;

    return spPoint;
}

void AJ_MissionGamemode::StartMissionLevel()
{
    auto allPC = UJ_Utility::GetAllMissionPC(GetWorld());
    for(auto* pc : allPC)
    {
        if(!pc) continue;
        // 미션 시작 보이스 재생
        pc->CRPC_PlayCommanderVoice3(static_cast<int>(EMissionProcess::MISSION_START));

        // 호스트면 팝업 비활성화시 시동 시작 바인드
        if(pc->IsLocalPlayerController())
            pc->objUIComp->popupEndDel.AddDynamic( this, &AJ_MissionGamemode::StartDefaultObjective);

        // 팝업 활성화
        pc->objUIComp->CRPC_ActivePopupUI(EMissionProcess::MIC_SWITCH_ON);
    }
}

void AJ_MissionGamemode::StartDefaultObjective()
{
    // 호스트 팝업의 바인드 제거

    // 호스트 가져오기
    AJ_MissionPlayerController* outHost;
    UJ_Utility::GetLocalPlayerController(GetWorld(), outHost);
    if(!outHost) return;

    // 바인드 제거
    outHost->objUIComp->popupEndDel.RemoveDynamic( this, &AJ_MissionGamemode::StartDefaultObjective);

    // 시동 목표 시작
    objectiveManagerComp->StartDefualtObj();
}

#pragma endregion

#pragma region 이륙 단
/*
텔포 박스에서 충돌시 해당 pc 이륙 체크 | AddFlightedPC
-> 한 명 이륙 딜리게이트 실행 | onePilotTakeOffDel
-> 이륙 목표 액터에서 수행도 갱신 처리 | takeOffObj->SuccessTakeOff
-> 로딩 UI 추가 | CRPC_AddLoadingUI
-> 전체 플레이어 이륙 체크
-> 이륙 종료 딜리게이트 실행 | takeOffEndDel
-> 이륙 목표 종료 처리 | takeOffObj->ObjectiveEnd

전술명령 시작 | StartTacticalOrder
-> 끝내지 않은 기본 목표(시동,이륙) 종료 처리 | startTODel, SkipDefaultObj, ObjectiveEnd
*/

// 이륙 체크 및 미션 시작 단
bool AJ_MissionGamemode::AddFlightedPC(AJ_MissionPlayerController *pc, bool isSuccess)
{
    // 이미 있으면 무시
    if(flightedPCAry.Contains(pc)) return false;

    // 이륙 배열에 추가
    flightedPCAry.Add(pc);

    // 이륙 딜리게이트 실행
    onePilotTakeOffDel.ExecuteIfBound(pc, isSuccess);

    // 해당 pc 로딩 ui 추가
    pc->CRPC_AddLoadingUI();

    const int curAllPCNum = GetGameState<AJ_MissionGameState>()->GetAllPlayerController().Num();
    // 배열 크기가 현재 플레이어 수와 같아지면 시작 지점 텔포 및 미션 시작
    isTPReady = flightedPCAry.Num() >= curAllPCNum;

    if(isTPReady)
    {
        // 이륙 종료 딜리게이트 실행
        takeOffEndDel.Broadcast(isTPReady);
        // 미션 시작
        StartTacticalOrder();
    }

    // XXX 반환 해도 안쓰긴함
    return isTPReady;
}

void AJ_MissionGamemode::StartTacticalOrder()
{
    // 이미 시작 했으면 무시
    if(isStartTO) return;

    // 안 끝난 기본 목표 종료 처리
    startTODel.Broadcast(true);
    isStartTO = true;

    // 약간 늦게 텔포 ( 로딩 스크린 이후 )
    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager()
        .SetTimer(timerHandle, [this]() mutable
    {
        // 미션 영역 변경
        ChangeMissionArea();

        // 모든 플레이어 시작 지점으로 위치 이동
        TeleportAllStartPoint(startPointActor);

        // 딜레이 이후 미션 시작
        DelayStartTacticalOrder(missionStartDelay);
    }, loadingDelay, false);
}

#pragma region 미션 지역 이동 단
/*
약간 딜레이 이후 미션 지역으로 변경 | ChangeMissionArea
-> 미션 지역으로 위경도 변경 | SetDestinationLogitudeLatitude
-> 미션 지역으로 세슘 머티리얼 변경 | SRPC_ChangeMissionArea
-> 텔포 박스 콜리전 제거 | boxComp->DestroyComponent
*/

void AJ_MissionGamemode::ChangeMissionArea()
{
    if(!cesiumTPBox) return;

    // 위경도 미션 지역으로 설정
    cesiumTPBox->SetDestinationLogitudeLatitude(curMissionData.longitude, curMissionData.latitude);
    // 변경 요청
    cesiumTPBox->SRPC_ChangeMissionArea();

    //텔포박스 비활성화
    auto* boxComp = cesiumTPBox->GetComponentByClass<UBoxComponent>();
    if(!boxComp) return;

    // 충돌의 원인을 제거
    boxComp->DestroyComponent();
}
#pragma endregion

#pragma region 전투기 텔포 단
/*
미션 시작 지점으로 전투기 위치 이동 | TeleportAllStartPoint
-> 각 pc 로딩 ui 제거 | pc->CRPC_RemoveLoadingUI
-> 번호순 파일럿 역할 설정
-> 역할에 따른 위치 계산 | CalcTeleportTransform
-> 계산된 위치로 텔레포트 | pc->MRPC_TeleportStartPoint
*/
void AJ_MissionGamemode::TeleportAllStartPoint(AJ_MissionStartPointActor *startPoint)
{
    if(!HasAuthority()) return;

    // 모든 pc 가져오기
    auto allPC = UJ_Utility::GetAllMissionPC(GetWorld());

    // 미션 시작 지점으로 위치 이동
    for(int i = 0; i < allPC.Num(); i++)
    {
        auto* pc = allPC[i];
        const auto* pawn = pc->GetPawn();

        // pc 로딩 ui 잇으면 제거
        pc->CRPC_RemoveLoadingUI();
        
        // idx 0,1,2 를 각각 리더, rt 윙맨, lt 윙맨으로 설정
        pc->pilotRole = static_cast<EPilotRole>(i % 3);

        // 순서에 따른 위치 조정 (산개용)
        FTransform newTR = CalcTeleportTransform(pc->pilotRole);
        // 스케일 조정되지 않도록 변경
        newTR = FTransform(newTR.GetRotation()
                        , newTR.GetLocation()
                        , pawn->GetActorScale());

        pc->MRPC_TeleportStartPoint(newTR);
    }
}

FTransform AJ_MissionGamemode::CalcTeleportTransform(const int idx)
{
    return CalcTeleportTransform(static_cast<EPilotRole>(idx % 3));
}

FTransform AJ_MissionGamemode::CalcTeleportTransform(const EPilotRole& role)
{
    // 시작 지점 트랜스폼
    FTransform baseTR = START_POINT_ACTOR->GetActorTransform();
    // 값이 없으면 반환
    if(baseTR.Equals(FTransform::Identity))
        return baseTR;
    
    // 방향 구하기
    const FVector& forV = START_POINT_ACTOR->GetActorForwardVector();
    const FVector& rtV = START_POINT_ACTOR->GetActorRightVector();
    /*  산개 로직
        def triangle_vertices(h, BC, AB):
        A = (0, h)
        B = (-BC / 2, -h)
        C = (BC / 2, -h)
        return A, B, C

        A, B, C = triangle_vertices(200, 1220, 681)
    */
    // 멀어질 거리 가져오기
    FVector addVec;
    const float& forValue = formationData.height;
    const float& rtValue = formationData.baseLength;

    // 역할에 따라 거리 적용
    switch (role) {
    case EPilotRole::WING_COMMANDER:
        addVec = forV * forValue;
        break;
    case EPilotRole::LEFT_WINGER:
        addVec = rtV * (-1 * rtValue / 2) + forV * -1 * forValue;
        break;
    case EPilotRole::RIGHT_WINGER:
        addVec = rtV * (rtValue / 2) + forV * -1 * forValue;
        break;
    }

    baseTR.SetLocation(baseTR.GetLocation() + addVec);

    return baseTR;
}

#pragma endregion

#pragma region 미션 시작 단
/*
약간 딜레이 이후 미션 시작 | DelayStartTacticalOrder
-> 이륙 보이스 재생 요청 | CRPC_PlayCommanderVoice3, FLIGHT_START
-> 이륙 팝업 UI 활성화 | CRPC_ActivePopupUI
-> 팝업 종료시 첫 번째 전술명령 활성화 | StartFirstTacticalOrder
-> 전투기 고정 해제 | viper->SetEngineOn
-> 첫 목표 시작 | ActiveNextObjective
*/
void AJ_MissionGamemode::DelayStartTacticalOrder(float delayTime)
{
    // 타임 만큼 딜레이
    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager()
    .SetTimer(timerHandle, [this]() mutable
    {
        auto allPC = UJ_Utility::GetAllMissionPC(GetWorld());

        for(auto* pc : allPC)
        {
            // 미션 시작 보이스 재생
            pc->CRPC_PlayVoiceByMP(EMissionProcess::FLIGHT_START);

            // 팝업 비활성화 될때 전술 명령 시작 바인드 | 호스트만
            if(pc->IsLocalPlayerController())
                pc->objUIComp->popupEndDel.AddDynamic( this, &AJ_MissionGamemode::StartFirstTacticalOrder);
            
            // 팝업 UI 활성화
            pc->objUIComp->CRPC_ActivePopupUI(EMissionProcess::TAKE_OFF_END);
        }
    }, delayTime, false);
}

void AJ_MissionGamemode::StartFirstTacticalOrder()
{
    auto allPC = UJ_Utility::GetAllMissionPC(GetWorld());

    for(auto* pc : allPC)
    {
        // 호스트면 팝업에 바인드된 함수 제거
        if(pc->IsLocalPlayerController())
            pc->objUIComp->popupEndDel.RemoveDynamic(this, &AJ_MissionGamemode::StartFirstTacticalOrder);
        
        auto* viper = pc->GetPawn<AL_Viper>();
        if(!viper) continue;
        // 전투기 고정 해제
        viper->SetEngineOn();
    }
    
    // 미션 시작
    this->objectiveManagerComp->ActiveNextObjective();
}
#pragma endregion

#pragma endregion




