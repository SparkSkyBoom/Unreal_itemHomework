// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // FTableRowBase를 위해 추가함
#include "Engine/Texture2D.h" // UTexture2D를 위해 추가함
#include "Item.generated.h" // 무조건 마지막

USTRUCT(BlueprintType)
struct FSItem : public FTableRowBase {
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 ItemNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Image;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText RequireTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText Description;
};