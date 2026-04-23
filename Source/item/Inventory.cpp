// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "ItemWidget.h"

void UInventory::AddItem(FSItem& item) {
    items.Add(item);
}

void UInventory::NativeConstruct()
{
    Super::NativeConstruct();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Item Initialized!"));
    }
}


void UInventory::UpdateWidget()
{
    if (!ItemContainer)
    {
        return;
    }

    ItemContainer->ClearChildren();

    for (const FSItem& ItemData : items)
    {
        UItemWidget* NewItemWidget = CreateWidget<UItemWidget>(this, ItemWidgetClass);

        if (NewItemWidget)
        {
            NewItemWidget->SetupItemWidget(ItemData);

            ItemContainer->AddChildToWrapBox(NewItemWidget);
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Success Added"));
        }
        else
        {
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Failed Added"));
        }
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Update End"));
    }
}