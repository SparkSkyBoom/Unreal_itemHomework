// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Item.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	int32 itemNumber;

public:
	void SetupItemWidget(const FSItem& Data);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TooltipClass;

	UPROPERTY()
	UUserWidget* CurrentTooltip;

	UPROPERTY(BlueprintreadOnly, meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(BlueprintreadOnly, meta = (BindWidget))
	class UImage* Image;

protected:
    virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

};

