// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PropHuntProjectile.generated.h"

UCLASS(config=Game)
class APropHuntProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 10.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShouldApplyImpulse = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ImpluseMultiplier = 1.f;

	/*Needed for calculations of effects*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/*Needed for calculations of effects*/
		FVector StartLocation;

	APropHuntProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

