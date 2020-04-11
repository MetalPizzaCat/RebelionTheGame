// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita KurguzovAll rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "PropHunt/Weapon/WeaponBase.h"
#include "PropHunt/Items/ItemActionObjectBase.h"
#include "ItemInfo.generated.h"



USTRUCT(BlueprintType)
struct FBuildingItemInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int Amount = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Keys)
		bool bIsKey = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Keys)
		int KeyId = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Action)
		TSubclassOf<UItemActionObjectBase> ItemAction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo)
		TMap<EAmmoType, int>Ammo;

	FBuildingItemInfo() {}

	FBuildingItemInfo(FString _Name, int _Amount, FText _Description) :Name(_Name), Amount(_Amount), Description(_Description) {}

	FBuildingItemInfo(FString _Name, int _Amount) :Name(_Name), Amount(_Amount){}

	FBuildingItemInfo(FString _Name, int _Amount, FText _Description, bool _bIsKey, int _KeyId) :Name(_Name), Amount(_Amount), Description(_Description), bIsKey(_bIsKey), KeyId(_KeyId){}

	FBuildingItemInfo(FString _Name, int _Amount, bool _bIsKey, int _KeyId) :Name(_Name), Amount(_Amount), bIsKey(_bIsKey), KeyId(_KeyId) {}


	FBuildingItemInfo(FString _Name, int _Amount, FText _Description, TMap<EAmmoType, int>_Ammo) :Name(_Name), Amount(_Amount), Description(_Description),Ammo(_Ammo) {}

	FBuildingItemInfo(FString _Name, int _Amount, TMap<EAmmoType, int>_Ammo) :Name(_Name), Amount(_Amount), Ammo(_Ammo) {}

	FBuildingItemInfo(FString _Name, int _Amount, FText _Description, bool _bIsKey, int _KeyId, TMap<EAmmoType, int>_Ammo) :Name(_Name), Amount(_Amount), Description(_Description), bIsKey(_bIsKey), KeyId(_KeyId), Ammo(_Ammo) {}

	FBuildingItemInfo(FString _Name, int _Amount, bool _bIsKey, int _KeyId, TMap<EAmmoType, int>_Ammo) :Name(_Name), Amount(_Amount), bIsKey(_bIsKey), KeyId(_KeyId), Ammo(_Ammo) {}
};