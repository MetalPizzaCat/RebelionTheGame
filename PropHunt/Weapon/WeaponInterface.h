// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PropHunt/PropHuntProjectile.h"
#include "WeaponBase.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROPHUNT_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void RequestSpawnProjectile(TSubclassOf<APropHuntProjectile>ProjectileClass, FVector location, FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool PickupWeapon(TSubclassOf<AWeaponBase>weaponClass, int ammoInTheClip);
};
