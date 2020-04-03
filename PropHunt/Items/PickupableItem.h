// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita KurguzovAll rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PropHunt/Managment/BaseBuildingBase.h"
#include "PropHunt/InteractionInterface.h"
#include "PickupableItem.generated.h"

UCLASS()
class PROPHUNT_API APickupableItem : public AActor,public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FBuildingItemInfo> StoredItems;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Interact(AActor* interactor, UPrimitiveComponent* interactedComponent);

	void Interact_Implementation(AActor* interactor, UPrimitiveComponent* interactedComponent)override;

	UFUNCTION(BlueprintCallable)
		void RemoveItem(FBuildingItemInfo item);

	UFUNCTION(BlueprintCallable)
		void RemoveSomeItems(TArray<FBuildingItemInfo> Items);

	UFUNCTION(BlueprintCallable)
		void CheckWhatsLeft(AActor* interactor);

	UFUNCTION(BlueprintCallable)
		void ProccessGivingItem(FString Name,AActor*pickuper);

	UFUNCTION(BlueprintPure)
		FBuildingItemInfo GetItemAndIdByName(FString name,int&id);

	UFUNCTION(BlueprintPure)
		FBuildingItemInfo GetItemByName(FString name);

	/*Usual pick up function but only thinks about items with that name*/
	UFUNCTION(BlueprintCallable)
		/*Usual pick up function but only thinks about items with that name*/
		bool PickupSpecificItem(AActor* interactor,FString ItemName,int &Left);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnFinishedGivingItems(bool AllItemsWereGiven, const TArray<FBuildingItemInfo>& NotGivenItems);

	void OnFinishedGivingItems_Implementation(bool AllItemsWereGiven, const TArray<FBuildingItemInfo>& NotGivenItems);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnGaveItem(const FString& ItemName, AActor* interactor);

	void OnGaveItem_Implementation(const FString& ItemName, AActor* interactor) { CheckWhatsLeft(interactor); }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnNothingLeft(AActor* interactor);

	void OnNothingLeft_Implementation(AActor* interactor);

};
