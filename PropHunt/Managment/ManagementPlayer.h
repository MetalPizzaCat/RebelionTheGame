// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita KurguzovAll rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "PropHunt/Weapon/WeaponBase.h"
#include "BaseBuildingBase.h"
#include "Components/DecalComponent.h"
#include "BaseInfo.h"
#include "ManagementPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedBuilding);

UCLASS()
class PROPHUNT_API AManagementPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AManagementPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APlayerController* PController = nullptr;
public:	

	UPROPERTY(BlueprintAssignable)
		FOnFinishedBuilding OnFinishedBuilding;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UDecalComponent* MouseDecal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<EAmmoType, int> TotalAmmoInBase;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Building)
		bool bBuilding = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Building)
		ABaseBuildingBase* CurrentBuilding = nullptr;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Interact();

	void Interact_Implementation();

	UFUNCTION(BlueprintCallable)
		void FinishBuilding();


	UFUNCTION(BlueprintCallable)
		void CancelBuilding();

	UFUNCTION(BlueprintCallable)
		void RotateBuilding();

	UFUNCTION(BlueprintCallable)
		void StartDestroyingBuildings();

	/*UFUNCTION(BlueprintCallable)
		void RemoveItemsFromInventory(TArray<FBuildingItemInfo>items);

	UFUNCTION(BlueprintCallable)
		void AddItemToInventory(FBuildingItemInfo item);*/

	UFUNCTION(BlueprintCallable)
		void FinishDestroyingBuildings(ABaseBuildingBase*building);


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ABaseInfo* Info;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StartBuilding(TSubclassOf<ABaseBuildingBase>BuildingClass);

	void StartBuilding_Implementation(TSubclassOf<ABaseBuildingBase>BuildingClass);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool CanBeBuilt(TSubclassOf<ABaseBuildingBase>BuildingClass);

	bool CanBeBuilt_Implementation(TSubclassOf<ABaseBuildingBase>BuildingClass);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetupUI();

	void  SetupUI_Implementation() {}

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
