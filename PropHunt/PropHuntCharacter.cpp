// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PropHuntCharacter.h"
#include "PropHuntProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// APropHuntCharacter

APropHuntCharacter::APropHuntCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	PlayerMesh->SetRelativeRotation(FRotator(0.f,0.f,-90.f));
	PlayerMesh->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	PlayerMesh->SetOnlyOwnerSee(true);
	PlayerMesh->HideBoneByName(HeadBoneName, EPhysBodyOp::PBO_None);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->bCastDynamicShadow = true;
	FP_Gun->CastShadow = true;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	
	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
}

void APropHuntCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SprintUpdateTimer,this,&APropHuntCharacter::SprintUpdate, 0.01f, true);

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(PlayerMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void APropHuntCharacter::OnConstruction(const FTransform& Transform)
{
}

void APropHuntCharacter::StartSprinting()
{
	if (CanSprint())
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;

		//for everything but character reaction
		OnStartSprinting.Broadcast();

		//for in-character reaction
		OnStartedSprinting();

		if (!SprintUpdateTimer.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(SprintUpdateTimer, 0.01f, true);
		}
	}
}

void APropHuntCharacter::StopSprinting()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;

	//for everything but character reaction
	OnStopSprinting.Broadcast();

	//for in-character reaction
	OnStopedSprinting();
}

void APropHuntCharacter::SprintUpdate()
{
	if (bIsSprinting)
	{
		CurrentSprintingTime += 0.01f;
		if (CurrentSprintingTime >= MaxSprintTime)
		{
			StopSprinting();
		}
	}
	else if (CurrentSprintingTime > 0)
	{
		CurrentSprintingTime -= 0.01f;
		if (CurrentSprintingTime < 0)
		{
			CurrentSprintingTime = 0;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void APropHuntCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APropHuntCharacter::StartCrouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APropHuntCharacter::StopCrouching);

	if (bAllowedToSprint)
	{
		PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APropHuntCharacter::StartSprinting);
		PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APropHuntCharacter::StopSprinting);
	}

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APropHuntCharacter::OnFire);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APropHuntCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APropHuntCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APropHuntCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APropHuntCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APropHuntCharacter::LookUpAtRate);
}

bool APropHuntCharacter::CanSprint_Implementation()
{
	return (bAllowedToSprint && !bIsCrouched && CurrentSprintingTime <= (MaxSprintTime - MaxSprintTime * 0.2));
}

void APropHuntCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<APropHuntProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void APropHuntCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APropHuntCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APropHuntCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APropHuntCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APropHuntCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
