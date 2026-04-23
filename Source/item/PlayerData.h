// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ITEM_API PlayerData
{
private:
	PlayerData();
    TSet<FString> titles;

public:
    // 싱글톤 인스턴스 접근자
    static PlayerData& Get()
    {
        static PlayerData Instance;
        return Instance;
    }

	~PlayerData();

    bool HasTitle(const FString& InTitle) const;

    void AddTitle(const FString& InTitle);

    void ClearPlayerData();
};
