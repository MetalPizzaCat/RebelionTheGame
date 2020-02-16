// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropHunt/Weapon/WeaponBase.h"
#include "WeaponShotgun.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API AWeaponShotgun : public AWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int BulletsPerShot = 8;

		bool PrimaryFire_Implementation(FVector location, FRotator rotation)override;
	
};
