// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "ItemFactory.h"
#include "PlayerData.h"

void UMyGameInstance::Init()
{
    Super::Init();

    // 여기서 데이터 테이블을 넘겨주며 초기화!
    if (ItemDataTable)
    {
        FItemFactory::Get().Initialize(ItemDataTable);
    }
    PlayerData::Get().ClearPlayerData();
}
