// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita KurguzovAll rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PropHunt/Managment/ItemInfo.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAdded, FBuildingItemInfo, Item,int,Amount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRemoved, FBuildingItemInfo, Item, int, Amount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class PROPHUNT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintAssignable)
		FOnItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable)
		FOnItemRemoved OnItemRemoved;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FBuildingItemInfo> StoredItems;

	UFUNCTION(BlueprintCallable)
		void RemoveItem(FBuildingItemInfo item);

	UFUNCTION(BlueprintCallable)
		void RemoveSomeItems(TArray<FBuildingItemInfo> Items);

	UFUNCTION(BlueprintPure)
		FBuildingItemInfo GetItemAndIdByName(FString name, int& id);

	UFUNCTION(BlueprintPure)
		FBuildingItemInfo GetItemByName(FString name);

	UFUNCTION(BlueprintPure)
		bool HasKey(int KeyId);

	UFUNCTION(BlueprintCallable)
		void AddItem(FBuildingItemInfo item);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
