// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"
#include "Engine/Engine.h" 
#include "item.h"
#include "ItemFactory.h"
#include "Engine/Engine.h"        // GEngine 사용을 위해 필요
#include "PlayerData.h"

void UItemWidget::SetupItemWidget(const FSItem& Data)
{
    itemNumber = Data.ItemNumber;

    if (ItemName)
    {
        ItemName->SetText(Data.ItemName);
        if (GEngine)
        {
            FString DebugMsg = FString::Printf(TEXT("아이템 이름 : %s"), *Data.ItemName.ToString());
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, DebugMsg);
        }
    }

    if (Image)
    {
        // UImage 위젯의 브러시를 텍스처로 변경
        Image->SetBrushFromTexture(Data.Image);
    }
}

void UItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

    if (CurrentTooltip != nullptr) return;

    FSItem ItemData;
    FItemFactory::Get().GetItemData(itemNumber, ItemData);

    if (TooltipClass)
    {
        CurrentTooltip = CreateWidget<UUserWidget>(GetWorld(), TooltipClass);
        if (CurrentTooltip)
        {

            UTextBlock* DescText = Cast<UTextBlock>(CurrentTooltip->GetWidgetFromName(TEXT("Text")));

            if (DescText)
            {
                // 2. 텍스트 설정
                DescText->SetText(ItemData.Description);
            }

            CurrentTooltip->AddToViewport();            
        }
    }
}

void UItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);
    
    if (CurrentTooltip)
    {
        CurrentTooltip->RemoveFromParent();
        CurrentTooltip = nullptr;
    }
}


FReply UItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

    FSItem ItemData;
    FItemFactory::Get().GetItemData(itemNumber, ItemData);
    
    // 좌클릭인지 확인
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        if (GEngine)
        {
            if (ItemData.RequireTitle.ToString() == TEXT("None")) {
                FString Msg = FString::Printf(TEXT("아이템 사용: %s"), *ItemData.ItemName.ToString());
                GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, Msg);
            }
            else {
                if (PlayerData::Get().HasTitle(ItemData.RequireTitle.ToString())) {
                    FString Msg = FString::Printf(TEXT("아이템 사용: %s"), *ItemData.ItemName.ToString());
                    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, Msg);
                }
                else {
                    FString Msg = FString::Printf(TEXT("%s 칭호가 없어서 아이템 사용 불가"), *ItemData.RequireTitle.ToString());
                    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, Msg);
                }
            }
        }


        //return FReply::Handled().CaptureMouse();
        FReply Reply = FReply::Handled();
        Reply.CaptureMouse(TakeWidget());
        return Reply;
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}