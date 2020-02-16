// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "PropHunt/InteractionInterface.h"
#include "PropCharacter.generated.h"

UCLASS()
class PROPHUNT_API APropCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APropCharacter();

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* PlayerMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsJumpButtonPressed = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Use();

	void Use_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool ChangeProp(UStaticMesh* propMesh,float PropHealth,float CollisionCapluseWidth,float CollisionCapluseHalfHeight,FVector  CollisionCapluseScale);

	bool ChangeProp_Implementation(UStaticMesh* propMesh, float PropHealth, float CollisionCapluseWidth, float CollisionCapluseHalfHeight, FVector  CollisionCapluseScale);
};
