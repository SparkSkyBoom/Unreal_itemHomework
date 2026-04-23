// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item.h"
#include "Components/WrapBox.h"
#include "Inventory.generated.h"


/**
 * 
 */
UCLASS()
class ITEM_API UInventory : public UUserWidget
{
	GENERATED_BODY()
private:
	TArray<FSItem> items;

public:
	/**
	 *
	 * BindWidget은 이미 Design에 이미 배치된 'instance'를 가져오는 것.
	 * TSubclasOf는 어떤 종류의 위젯을 찍어낼지(Spawn), 그 Class를 지정한다.
     *
     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class UUserWidget> ItemWidgetClass;


	UPROPERTY(BlueprintreadOnly, meta = (BindWidget))
	class UWrapBox* ItemContainer;

	// 위젯이 생성되어 화면에 처음 추가되거나 초기화 될 때 호출되는 가상함수
	virtual void NativeConstruct() override;


	void AddItem(FSItem& item);
	void UpdateWidget();


};
