// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerData.h"
#include "Engine/Engine.h"        // GEngine 사용을 위해 필요

PlayerData::PlayerData()
{
}

PlayerData::~PlayerData()
{
}

bool PlayerData::HasTitle(const FString& InTitle) const
{
    return titles.Contains(InTitle);
}

void PlayerData::AddTitle(const FString& InTitle)
{
    titles.Add(InTitle);
}

void PlayerData::ClearPlayerData()
{
    titles.Empty();
}
