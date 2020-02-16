// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShotgun.h"
#include "Math/RandomStream.h"
#include "Engine.h"

bool AWeaponShotgun::PrimaryFire_Implementation(FVector location, FRotator rotation)
{
	if (CanShoot() && GetWorld() != nullptr && PrimaryProjectileClass != nullptr)
	{
		CurrentFiringSpread = DefaultFiringSpread;
		FRandomStream randomStream;
		for (int i = 0; i < BulletsPerShot - 1; i++)
		{
			randomStream = FRandomStream(FMath::Rand());

			FVector ShootDir = randomStream.VRandCone(rotation.Vector(), FMath::DegreesToRadians(CurrentFiringSpread) * 0.5f, FMath::DegreesToRadians(CurrentFiringSpread) * 0.5f);

			ServerPrimaryFire(location, ShootDir.Rotation());

			CurrentFiringSpread += SpreadIncrement;
		}
		PrimaryFireEffects(location, rotation);
		AmmoInTheClip -= 1;
		if (AmmoInTheClip < 0) { AmmoInTheClip = 0; }
		bCanShoot = false;
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHadle, this, &AWeaponBase::EndCooldown, WeaponCooldown);
		return true;
	}
	else
	{
		return false;
	}

}
