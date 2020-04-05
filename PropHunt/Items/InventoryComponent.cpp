// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita KurguzovAll rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FBuildingItemInfo UInventoryComponent::GetItemAndIdByName(FString name, int& id)
{
	if (StoredItems.Num() > 0)
	{
		for (int i = 0; i < StoredItems.Num(); i++)
		{
			if (StoredItems[i].Name == name)
			{
				id = i;
				return StoredItems[i];
			}
		}
	}
	return FBuildingItemInfo();
}

FBuildingItemInfo UInventoryComponent::GetItemByName(FString name)
{
	if (StoredItems.Num() > 0)
	{
		for (int i = 0; i < StoredItems.Num(); i++)
		{
			if (StoredItems[i].Name == name)
			{
				return StoredItems[i];
			}
		}
	}
	return FBuildingItemInfo();
}

bool UInventoryComponent::HasKey(int KeyId)
{
	if (StoredItems.Num() > 0)
	{
		for (int i = 0; i < StoredItems.Num(); i++)
		{
			if (StoredItems[i].bIsKey)
			{
				if (StoredItems[i].KeyId == KeyId) { return true; }
			}
		}
	}
	return false;
}

void UInventoryComponent::AddItem(FBuildingItemInfo item)
{
	if (StoredItems.Num() > 0)
	{
		for (int i = 0; i < StoredItems.Num(); i++)
		{
			if (StoredItems[i].Name == item.Name)
			{
				StoredItems[i].Amount += item.Amount;
				OnItemAdded.Broadcast(item, item.Amount);
				return;
			}
		}
	}
	StoredItems.Add(item);
	OnItemAdded.Broadcast(item, item.Amount);
}


void UInventoryComponent::RemoveItem(FBuildingItemInfo item)
{
	if (StoredItems.Num() > 0)
	{
		for (int i = 0; i < StoredItems.Num(); i++)
		{
			if (StoredItems[i].Name == item.Name)
			{
				StoredItems[i].Amount -= item.Amount;
				if (StoredItems[i].Amount < 0) { StoredItems[i].Amount = 0; }
				OnItemRemoved.Broadcast(item, item.Amount);
			}
		}
	}
}

void UInventoryComponent::RemoveSomeItems(TArray<FBuildingItemInfo> Items)
{
	if (Items.Num() > 0)
	{
		for (int i = 0; i < Items.Num(); i++)
		{
			RemoveItem(Items[i]);
		}
	}
}
