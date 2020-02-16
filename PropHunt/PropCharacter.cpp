// Fill out your copyright notice in the Description page of Project Settings.


#include "PropCharacter.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"

// Sets default values
APropCharacter::APropCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	PlayerMesh->SetupAttachment(RootComponent);
	//SetRootComponent(PlayerMesh);
}

// Called when the game starts or when spawned
void APropCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APropCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APropCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APropCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APropCharacter::MoveRight);

	PlayerInputComponent->BindAction("Interact",EInputEvent::IE_Pressed, this, &APropCharacter::Use);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APropCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APropCharacter::LookUpAtRate);
}

void APropCharacter::Use_Implementation()
{

}

bool APropCharacter::ChangeProp_Implementation(UStaticMesh* propMesh, float PropHealth, float CollisionCapluseWidth, float CollisionCapluseHalfHeight, FVector  CollisionCapluseScale)
{
	if (propMesh != nullptr && bDead != true && this->Health > 0.f)
	{
		if (PlayerMesh != nullptr)
		{
			PlayerMesh->SetStaticMesh(propMesh);
			this->Health = PropHealth;
			GetCapsuleComponent()->SetWorldScale3D(CollisionCapluseScale);
			GetCapsuleComponent()->SetCapsuleSize(CollisionCapluseWidth, CollisionCapluseHalfHeight);
			PlayerMesh->SetRelativeScale3D(FVector(1 / CollisionCapluseScale.X, 1 / CollisionCapluseScale.Y, 1 / CollisionCapluseScale.Z));
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void APropCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APropCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APropCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APropCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}



