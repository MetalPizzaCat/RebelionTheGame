// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita KurguzovAll rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "PickupableItem.h"
#include "PropHunt/Managment/ManagmentInterface.h"
#include "Engine.h"

//#define DEBUG_PICKUP_ITEM_METAL
//#define DEBUG_PICKUP_ALL

// Sets default values
APickupableItem::APickupableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupableItem::Interact_Implementation(AActor* interactor, UPrimitiveComponent* interactedComponent)
{
	if (interactor != nullptr)
	{
		if (interactor->Implements<UManagmentInterface>() || (Cast<IManagmentInterface>(interactor) != nullptr))
		{
			if (IManagmentInterface::Execute_RequestDisplayInteractionWidget(interactor, this))
			{
				//on success, right now nothing is supposed to happen
			}
		}
	}
#ifdef DEBUG_PICKUP_ITEM_METAL
	int left = 0;
	bool GaveAllItems = true;
	bool ResultOfGiving = PickupSpecificItem(interactor, "Metal", left);
	if (ResultOfGiving)
	{
		int id = -1;
		GetItemAndIdByName("Metal", id);
		if (id != -1)
		{
			StoredItems[id].Amount = left;
		}
		
	}
	OnGaveItem("Metal");
	
#elif defined DEBUG_PICKUP_ALL
	if (interactor->Implements<UManagmentInterface>() || (Cast<IManagmentInterface>(interactor) != nullptr))
	{
		if (StoredItems.Num() > 0)
		{
			bool GaveAllItems = true;
			TArray<FBuidingItemInfo>NotGivenItems;
			for (int i = 0; i < StoredItems.Num(); i++)
			{
				int left = 0;
				bool ResultOfGiving = IManagmentInterface::Execute_GiveItem(interactor, StoredItems[i], left);
				if (!ResultOfGiving)
				{
					NotGivenItems.Add(StoredItems[i]);
					GaveAllItems = false;
					break;
				}
				else if (ResultOfGiving && left > 0)
				{
					NotGivenItems.Add(FBuidingItemInfo(StoredItems[i].Name, StoredItems[i].Amount - left));
					GaveAllItems = false;
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(left));
					break;
				}

			}
			OnFinishedGivingItems(GaveAllItems, NotGivenItems);
		}
	}
#endif // ITEMTEST	
}

void APickupableItem::RemoveItem(FBuidingItemInfo item)
{
	if (StoredItems.Num() > 0)
	{
		for (int i = 0; i < StoredItems.Num(); i++)
		{
			if (StoredItems[i].Name == item.Name)
			{
				StoredItems[i].Amount -= item.Amount;
			}
		}
	}
}

void APickupableItem::RemoveSomeItems(TArray<FBuidingItemInfo> Items)
{
	if (Items.Num() > 0)
	{
		for (int i = 0; i < Items.Num(); i++) 
		{
			RemoveItem(Items[i]);
		}
	}
}

void APickupableItem::CheckWhatsLeft(AActor* interactor)
{

	if (StoredItems.Num() > 0)
	{
		bool IsSomethingLeft = false;
		for (int i = 0; i < StoredItems.Num(); i++)
		{
			if (StoredItems[i].Amount > 0) { IsSomethingLeft = true; break; }
		}
		if(!IsSomethingLeft){ OnNothingLeft(interactor); }
	}
	else
	{
		OnNothingLeft(interactor);
	}
}

void APickupableItem::ProccessGivingItem(FString Name,AActor*pickuper)
{
	int left = 0;
	bool GaveAllItems = true;
	bool ResultOfGiving = PickupSpecificItem(pickuper, Name, left);
	if (ResultOfGiving)
	{
		int id = -1;
		GetItemAndIdByName(Name, id);
		if (id != -1)
		{
			StoredItems[id].Amount = left;
		}

	}
	OnGaveItem(Name, pickuper);
}

FBuidingItemInfo APickupableItem::GetItemAndIdByName(FString name, int& id)
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
	return FBuidingItemInfo();
}

FBuidingItemInfo APickupableItem::GetItemByName(FString name)
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
	return FBuidingItemInfo();
}

bool APickupableItem::PickupSpecificItem(AActor* interactor, FString ItemName, int& Left)
{
	if (interactor->Implements<UManagmentInterface>() || (Cast<IManagmentInterface>(interactor) != nullptr))
	{
		if (StoredItems.Num() > 0)
		{
			for (int i = 0; i < StoredItems.Num(); i++)
			{
				if (StoredItems[i].Name == ItemName)
				{
					return IManagmentInterface::Execute_GiveItem(interactor, StoredItems[i], Left);
				}
			}
		}
	}
	return false;
}

void APickupableItem::OnFinishedGivingItems_Implementation(bool AllItemsWereGiven, const TArray<FBuidingItemInfo>& NotGivenItems)
{
	if (!AllItemsWereGiven)
	{
		RemoveSomeItems(NotGivenItems);
	}
	else
	{
		Destroy();
	}
}

void APickupableItem::OnNothingLeft_Implementation(AActor* interactor)
{
	if (interactor != nullptr)
	{
		if (interactor->Implements<UManagmentInterface>() || (Cast<IManagmentInterface>(interactor) != nullptr))
		{
			IManagmentInterface::Execute_RequestHideContainerDisplay(interactor);
		}
	}
	Destroy();
}

