// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita Kurguzov All rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "PropBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartedSimulatingPhysicsDelegate);

UCLASS()
/*DO NOT USE THIS CLASS. USE ONLY IT'S CHILDREN*/
class PROPHUNT_API APropBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APropBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintAssignable)
		FStartedSimulatingPhysicsDelegate OnStartedSimulatingPhysicsDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), SaveGame)
		UStaticMeshComponent* Mesh=nullptr;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), SaveGame)
		UBoxComponent* Box = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), SaveGame)
		UAudioComponent* ScrapeSound = nullptr;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Update();

	virtual void Update_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool IsTouchingAnything();

	virtual bool IsTouchingAnything_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Water)
		bool IsInTheWater();

	virtual bool IsInTheWater_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Water)
		void WaterImpact();

	virtual void WaterImpact_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable,Category=Water)
		void OnLeftWater();

	virtual void OnLeftWater_Implementation();


	UFUNCTION(BlueprintCallable)
		void OnPropHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
		void StartSimulatingPhysics();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool bIsInTheWater = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Impulse, meta = (ExposeOnSpawn = "true"), SaveGame)
		float MinImpulse = 8000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Impulse, meta = (ExposeOnSpawn = "true"), SaveGame)
		float MinImpulseToScrape = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Water, meta = (ExposeOnSpawn = "true"), SaveGame)
		float Bouyancy = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Water, meta = (ExposeOnSpawn = "true"), SaveGame)
		float WaterImpactSize = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Water, meta = (ExposeOnSpawn = "true"), SaveGame)
		float DefaultWaterImpactPower = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (ExposeOnSpawn = "true"), SaveGame)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Sound, meta = (ExposeOnSpawn = "true"), SaveGame)
		USoundBase* SplashSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (ExposeOnSpawn = "true"), SaveGame)
		USoundAttenuation* SoundAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn, meta = (ExposeOnSpawn = "true"), SaveGame)
		bool bSimulatePhysics = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (ExposeOnSpawn = "true"), SaveGame)
		USoundBase* StartSimulatePhysicsSound;



	/*Smaller this value better the performance, but worse sound system*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		/*Smaller this value better the performance, but worse sound system*/
		float UpdateTime = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FTimerHandle UpdateTimerHandle;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

};
