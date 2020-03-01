// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "PropHunt/PropHuntProjectile.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimSequence.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)		//What ammo weapon needs
enum class EAmmoType : uint8
{
	EAT_Rifle 	UMETA(DisplayName = "Rifle"),
	EAT_Shotgun 	UMETA(DisplayName = "Shotgun"),
	EAT_Pistol 	UMETA(DisplayName = "Pistol")
};

UENUM(BlueprintType)		//What ammo weapon needs
enum class EWeaponAnimationType : uint8
{
	EAT_Rifle 	UMETA(DisplayName = "RifleAnimation"),
	EAT_RifleFast 	UMETA(DisplayName = "FastRifleAnimation"),
	EAT_Shotgun 	UMETA(DisplayName = "ShotgunAnimation"),
	EAT_Pistol 	UMETA(DisplayName = "PistolAnimation")
};

UENUM(BlueprintType)		//Shooting style
enum class EShootingType : uint8
{
	//Player needs to press "fire" button for each shot
	EAT_Single 	UMETA(DisplayName = "Single"),
	//Player Can just hold fire button
	EAT_Auto 	UMETA(DisplayName = "Auto")
};

UCLASS()
class PROPHUNT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnBulletNotServer(FVector location, FRotator rotation);
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		FTimerHandle CooldownTimerHadle;

	/**CapsuleCollisionComponent is used for picking up weapons */
	UPROPERTY(Category = PickUp, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleCollisionComponent;

	/*Mesh used during reload animations*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Clip)
		UStaticMesh*EmptyClipMesh;

	/*Mesh used during reload animations*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Clip)
		UStaticMesh* AmmoShellMesh;

	/*Mesh used during reload animations*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Clip)
		 UStaticMesh* ClipMesh;


	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		TSubclassOf<APropHuntProjectile>PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		TSubclassOf<APropHuntProjectile>SecondaryProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* PrimaryFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* PrimaryOutOfAmmoSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* SecondaryOutOfAmmoSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* SecondaryFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		AActor* WeaponOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		int AmmoInTheClip = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		int DefaultAmmoInTheClip = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float WeaponCooldown = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
		bool bSecondaryFireIsAim = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool bCanShoot = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float PrimaryDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float CurrentFiringSpread = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float DefaultFiringSpread = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float SpreadIncrement = 0.45f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Ammo)
		EAmmoType AmmoType = EAmmoType::EAT_Rifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool bCanBePickedUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AIDamageModifier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
		TSubclassOf<UCameraShake> CameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animation)
		EWeaponAnimationType AnimType = EWeaponAnimationType::EAT_Rifle;

	/*used with character animation. IT'S NOT OBLIGATORY TO HAVE THIS ANIMATION*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animation)
		UAnimSequence* ShootingAnimation;
	/*used with character animation. IT'S NOT OBLIGATORY TO HAVE THIS ANIMATION*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animation)
		UAnimSequence*ReloadingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = ShootingStyle)
		EShootingType ShootingType = EShootingType::EAT_Single;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool PrimaryFire(FVector location, FRotator rotation);

	virtual bool PrimaryFire_Implementation(FVector location, FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool CanShoot();

	virtual bool CanShoot_Implementation();

	UFUNCTION(reliable,server,WithValidation)
		void ServerPrimaryFire(FVector location, FRotator rotation);

	UFUNCTION(reliable, server, WithValidation)
		void ServerSecondaryFire(FVector location, FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool  SecondaryFire(FVector location, FRotator rotation);

	bool  SecondaryFire_Implementation(FVector location, FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void PrimaryFireEffects(FVector location, FRotator rotation);

	void PrimaryFireEffects_Implementation(FVector location, FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EndCooldown();

	void EndCooldown_Implementation();

	UFUNCTION( BlueprintCallable)
		void SetOnwerPlayer(AActor* player) { this->SetOwner(player); WeaponOwner = player; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void  SecondaryFireEffects(FVector location, FRotator rotation);

	void  SecondaryFireEffects_Implementation(FVector location, FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);


	void BeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
