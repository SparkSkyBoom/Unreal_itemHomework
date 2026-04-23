#include "ItemFactory.h"

void FItemFactory::Initialize(UDataTable* InDataTable)
{
    if (!InDataTable) return;

    ItemMap.Empty();

    static const FString ContextString(TEXT("ItemFactoryInitialize"));

    TArray<FName> RowNames = InDataTable->GetRowNames();

    for (const FName& RowName : RowNames)
    {
        FSItem* Row = InDataTable->FindRow<FSItem>(RowName, ContextString);

        if (Row)
        {
            ItemMap.Add(Row->ItemNumber, *Row);
        }
    }
}

bool FItemFactory::GetItemData(int32 ItemNumber, FSItem& OutData)
{
    // Find는 해당 키가 있으면 포인터를, 없으면 nullptr을 반환합니다.
    if (const FSItem* FoundItem = ItemMap.Find(ItemNumber))
    {
        OutData = *FoundItem;
        return true;
    }
    return false;
}