// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuHUD.generated.h"

class UButton;
class UInventory;

/**
 * 
 */
UCLASS()
class ITEM_API UMenuHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	//UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	//class UButton* ToggleInventoryButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* ToggleTittleButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UInventory* Inventory;


	// 위젯이 생성되어 화면에 처음 추가되거나 초기화 될 때 호출되는 가상함수
	virtual void NativeConstruct() override;

	/*UFUNCTION()
	void ToggleInventory();*/

	UFUNCTION()
	void ToggleTittle();
};
