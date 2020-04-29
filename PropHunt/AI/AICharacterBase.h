// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita KurguzovAll rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacterBase.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAIS_Neutral		UMETA(DisplayName = "Neutral"),
	EAIS_Angry		UMETA(DisplayName = "Angry"),
	EAIS_Panick			UMETA(DisplayName = "Panick"),

	EAIS_Max			UMETA(Hidden)
};

UCLASS()
class PROPHUNT_API AAICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector ImpluseToApply;

	FVector LocationToApplyImpluseTo;

	void ApplyImpulseToBody();


	FTimerHandle impulseTimerHandle;

	FTimerHandle StateUpdateTimerHandle;

public:	

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = AiState, META=(ExposeOnSpawn = "true"))
		EAIState AiState = EAIState::EAIS_Neutral;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AiState)
		bool bShouldAiStateBeUpdated = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AiState)
		float TimeOfStateUpdate = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float DefaultMovementSpeed = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float DefaultRunningSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* HurtSound;

	/*for the animation*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/*for the animation*/
		bool bHurtAnimPlaying = false;


	UFUNCTION(BlueprintCallable)
	void PlayDamageSound();

	UFUNCTION(BlueprintCallable)
	void PlayDeathSound();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateAiState();

	void UpdateAiState_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void CalculateAiState();

	void CalculateAiState_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Die();

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

	void Die_Implementation();

	



	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
