// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Perception/AISightTargetInterface.h"
#include "FootstepInterface.h"
#include "PropHuntCharacter.generated.h"

class UInputComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartSprinting);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopSprinting);

UCLASS(config=Game)
class APropHuntCharacter : public ACharacter, public IAISightTargetInterface,public IFootstepInterface
{
	GENERATED_BODY()

	

public:
	APropHuntCharacter();
	APropHuntCharacter(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void BeginPlay();

	virtual void OnConstruction(const FTransform& Transform) override;

	void StartCrouching();

	void StopCrouching() { if (bHoldCrouch) { UnCrouch(); UpdateMeshPositionOnCrouch(false); } }

	void StartSprinting();

	void StopSprinting();

	void SprintUpdate();

	void OnSprintButtonDown() { if (!bSprintKeyDown && !bIsSprinting) { bSprintKeyDown = true; StartSprinting(); } }

	void OnSprintButtonUp() { if (bSprintKeyDown) { bSprintKeyDown = false; StopSprinting(); } }

public:

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category=Components)
		UFootstepComponent* FootstepComponent;

	UPROPERTY(BlueprintAssignable)
		FOnStartSprinting OnStartSprinting;

	UPROPERTY(BlueprintAssignable)
		FOnStopSprinting OnStopSprinting;

	UFUNCTION(BlueprintPure, Category = Water)
		bool IsInWater();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Sprinting)
		void OnStartedSprinting();

	void OnStartedSprinting_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Sprinting)
		void OnStopedSprinting();

	void OnStopedSprinting_Implementation() {}

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Crouching, SaveGame)
		bool bHoldCrouch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crouching, SaveGame)
		bool bIsCurrentlyCrouched = false;


	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool bAllowedToSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ladder, SaveGame)
		bool bIsOnTheLadder = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ladder, SaveGame)
		float LadderSpeedMultiplier = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=PickingUp, SaveGame)
		bool bAllowedToPickUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Sprinting, SaveGame)
		bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprinting, SaveGame)
		float SprintingSpeed = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprinting, SaveGame)
		bool bSprintKeyDown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultMovementSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprinting)
		float MaxSprintTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Falling, SaveGame)
		float TimeInAir = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Falling, SaveGame)
		float MinTimeInAirToDealDamage = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Falling, SaveGame)
		float FallDamageMultiplier = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FallDamage, SaveGame)
		float WaterFallDamageMultiplier = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Falling, SaveGame)
		bool bFallDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Falling, SaveGame)
		FTimerHandle FallingInfoUpdateTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Falling, SaveGame)
		USoundBase* FallingDamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Falling, SaveGame)
		USoundBase* FallInWaterSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprinting, SaveGame)
		FTimerHandle SprintUpdateTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprinting, SaveGame)
		float CurrentSprintingTime = 0.f;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	/**Player Mesh that only player itself can see */
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, Category = PlayerMesh)
		class USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = PlayerMesh)
		FName HeadBoneName = "head";

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class APropHuntProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
		TSubclassOf<UCameraShake> CameraShakeLandingClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
		TSubclassOf<UCameraShake> CameraShakeLandingWeakClass;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StartClimbingLadder();

	void StartClimbingLadder_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StopClimbingLadder();

	void StopClimbingLadder_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateMeshPositionOnCrouch(bool isCrouching);

	void UpdateMeshPositionOnCrouch_Implementation(bool isCrouching){}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool CanSprint();

	bool CanSprint_Implementation();

	/*Returns true if visibility was changed and false otherwise*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool RequestUIVisibility(bool Visibility);

	bool RequestUIVisibility_Implementation(bool Visibility);

	/*Mesh that is used to check if player is visible*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable,Category=AIVisibility)
		/*Mesh that is used to check if player is visible*/
		USkeletalMeshComponent* GetBodyMesh()const;

	USkeletalMeshComponent* GetBodyMesh_Implementation()const { return GetMesh(); }


	UFUNCTION(BlueprintCallable)
		void UpdateAirInfo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool Die();

	bool Die_Implementation() { return false;/*this base class can not die(for now) and as such always reutyrns false*/ }

	virtual void Landed(const FHitResult& Hit)override;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	virtual bool CanBeSeenFrom(
		const FVector& ObserverLocation,
		FVector& OutSeenLocation,
		int32& NumberOfLoSChecksPerformed,
		float& OutSightStrength,
		const AActor* IgnoreActor = NULL
	) const;

	virtual UFootstepComponent* GetFootstepComponent_Implementation() const override 
	{
		return FootstepComponent;
	}

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

