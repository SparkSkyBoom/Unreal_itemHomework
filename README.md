
## 과제 목표
1. 가방은 TArray를 통해서 만드세요
2. TMap은 저희 TArray에 담긴 아이템들을 확인하게 되면 그 아이템 정보를 Key값으로 아이템 정보들이 뜨도록 만들어주시세요.
3. TSet은 칭호 획득을 사용하시면됩니다.
4. 해당 칭호가 있어야 아이템을 사용할 수 있도록 만들어주세요.

--- 

### 구조
- BluePrint로 Data Table 작성, 이때 자료형은 FSItem (구조체)
  - Item 리스트를 Data Table로 작성을 한 이유는, 전체 아이템을 보기 편하고, csv로 export & import하여 관리하기 쉽기 떄문.

```C++
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
```

- GameInstance 단계에서, 'ItemDataTable'를 가져와서 FItemFactory를 초기화
  - FItemFactory는 싱글톤 클래스로 내부에 TMap을 사용하여 ItemNumber과 Item을 매칭하는 기능을 가짐. -> ItemNumber을 주면 Item으로 변환 가능
  - PlayerData(싱글톤, 예, 소유한 아이템 등) 도 초기화 해야하므로, ClearPlayerData()를 호출.
  - Blueprint에서 GameInstance를 만들고, C++로 작성한 UMyGameInstance를 상속하도록 함. 이유는, UMyGameInstance에 ItemDataTable을 바인딩 할 때, Blueprint의 Detail 패널에서 바인딩하여 좀더 쉽게 관리학 위함.

```C++
void UMyGameInstance::Init()
{
    Super::Init();

    // 여기서 데이터 테이블을 넘겨주며 초기화!
    if (ItemDataTable)
    {
        FItemFactory::Get().Initialize(ItemDataTable);
    }
    PlayerData::Get().ClearPlayerData();
}

```

- 아이템 지급은 마땅한 곳이 없어, MenuHUD가 초기화 될 때 하도록 함.
  - FItemFactory(TMap으로 만들어짐) 에서 ItemNumber을 조회하여 FSItem를 초기화 시키고, 이를 Inventory AddItem을 통해 전달함.
  - 모든 AddItem이 끝난 후, UpdateWidget() 함수를 호출하여 반영함.
    - 현재 UpdateWidget의 경우 다 지우고 TArray에 따라서 AddItem을 하고 있기 떄문에 매우 부하가 높은 작업임. 따라서 AddItem을 할 떄마다 호출하지 않고 모든 작업이 끝난 후에 처리함
    - UpdateWidget을 처리할 때 다 지우기 보단 비교해서 diff 계산하고 차이만 추가 & 삭제를 진행하는 쪽이 더 올바른 구조라고 생각하긴 함.
```C++
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

```

- 칭호 획득은 버튼을 눌러 칭호를 얻도록 처리함.
- 임의로 기사 칭호를 부여함
  - RequireTitle의 상태가 None이면 칭호 없이 아이템 사용이 가능하고 그 밖에는 해당 칭호가 있어야 사용이 가능한 아이템.
  - 목검의 경우 None이고, 철검의 경우, RequireTitle가 '기사'이기 때문에 해당 조건이 있어야 사용이 가능함.
```C++
	if (ToggleTittleButton) {
		ToggleTittleButton->OnClicked.AddDynamic(this, &UMenuHUD::ToggleTittle);
	}


void UMenuHUD::ToggleTittle()
{
	PlayerData::Get().AddTitle(TEXT("기사"));
	CheckingLogForText(TEXT("기사 칭호 획득"));

}
```

---

### 아이템 관리하는 인벤토리 (가방)
- 아이템을 추가하는 경우, TArray에 사용하여 FSItem(아이템 구조체)들을 저장함.
- ItemWidgetClass 를 Binding하여, UpdateWidget을 하는 경우 보유한 아이템에 맞춰 Widget을 최신화함. -> 인벤토리 UI에 아이템들이 표시됨.

```c++
// UIventory Class

UCLASS()
class ITEM_API UInventory : public UUserWidget
{
	GENERATED_BODY()
private:
	TArray<FSItem> items;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class UUserWidget> ItemWidgetClass;


	UPROPERTY(BlueprintreadOnly, meta = (BindWidget))
	class UWrapBox* ItemContainer;

	// 위젯이 생성되어 화면에 처음 추가되거나 초기화 될 때 호출되는 가상함수
	virtual void NativeConstruct() override;


	void AddItem(FSItem& item);
	void UpdateWidget();
};


void UInventory::AddItem(FSItem& item) {
    items.Add(item);
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
            NewItemWidget->SetupItemWidget(ItemData); // ItemData에 맞춰서 ItemWidget을 적절하게 초기화 하는 함수

            ItemContainer->AddChildToWrapBox(NewItemWidget);           
        }
    }
}
```
--- 

### 아이템 정보 확인
- NativeOnMouseEnter를 상속받아서, ItemWidget위에 마우스를 올릴 때, 해당 아이템의 Description이 표시되게 함.
- TooltipClass의 경우 Blueprint로 작성한 클래스로, Text Block을 하나만 가지고 있음.
- FItemFactory가 TMap으로 되어있기 떄문에 FItemFactory::Get().GetItemData(itemNumber, ItemData); 부분이 TMap을 사용하여 아이템 정보를 Key값으로 아이템 정보들이 뜨도록 만든 부분.
- 마우스를 떼는 경우 Tooltip을 삭제함.

```C++
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
```
--- 

### 칭호 관리
- PlayerData를 싱글톤으로 만듬.
-  TSet으로 소유한 칭호를 관리할 수 있도록 처리함
-  AddTitle를 통해 칭호를 추가하며, HasTitle을 통해 해당 칭호가 보유중인지 확인함.

```C++
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
```

현재 작성된 프로그램에서는 마땅히 칭호를 얻는 컨텐츠가 없기 떄문에,
UMenuHUD에 버튼을 만들어 버튼을 누를 경우, '기사' 칭호를 지급함. 위 내용에 대한 코드는 '##구조' 쪽에 있음.

---

### 칭호가 있어야 아이템 사용
- 아이템 위젯 클릭시 아이템을 사용함. (사용했다고 Text를 출력함)
- RequireTitle가 None인 경우, 따로 조건 확인 없이 아이템을 사용함.
- RequireTitle가 None이 아닌 경우, PlayerData::Get().HasTitle를 통해 칭호를 가진 상황에서만 아이템을 사용할 수 있도록 처리함.
  - 만약 칭호를 보유하지 않은 경우, 칭호가 없어서 아이템을 사용하지 못한다는 문구를 출력함.

```C++
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

        FReply Reply = FReply::Handled();
        Reply.CaptureMouse(TakeWidget());
        return Reply;
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

```
