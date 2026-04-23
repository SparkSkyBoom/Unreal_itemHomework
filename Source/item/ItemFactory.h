#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Item.h"

class ITEM_API FItemFactory
{
public:
    // 싱글톤 인스턴스 접근자
    static FItemFactory& Get()
    {
        static FItemFactory Instance;
        return Instance;
    }


    void Initialize(UDataTable* InDataTable);
    bool GetItemData(int32 ItemNumber, FSItem& OutData);
    ~FItemFactory() {}

private:
    FItemFactory() {}    

    TMap<int32, FSItem> ItemMap;
};