// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHUD.h"
#include "Components/Button.h"
#include "Engine/Engine.h"        // GEngine 사용을 위해 필요
#include "Item.h"
#include "ItemFactory.h"
#include "Inventory.h"
#include "PlayerData.h"

static void CheckingLogForText(const FString& DebugMessage) {
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, DebugMessage);
	}
}

void UMenuHUD::NativeConstruct()
{
	// Binding Button
	/*if (ToggleInventoryButton) {
		ToggleInventoryButton->OnClicked.AddDynamic(this, &UMenuHUD::ToggleInventory);
	}*/
	
	if (ToggleTittleButton) {
		ToggleTittleButton->OnClicked.AddDynamic(this, &UMenuHUD::ToggleTittle);
	}
	
	
	if (Inventory)
	{
		FSItem ItemOne;
		FItemFactory::Get().GetItemData(1, ItemOne);
		Inventory->AddItem(ItemOne);

		FSItem ItemTwo;
		FItemFactory::Get().GetItemData(2, ItemTwo);
		Inventory->AddItem(ItemTwo);

		Inventory->UpdateWidget();
	}
}


/*void UMenuHUD::ToggleInventory()
{
	CheckingLogForText(TEXT("Toggle Inventroy 버튼 테스트"));
}*/

void UMenuHUD::ToggleTittle()
{
	PlayerData::Get().AddTitle(TEXT("기사"));
	CheckingLogForText(TEXT("기사 칭호 획득"));

}
