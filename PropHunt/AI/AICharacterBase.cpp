// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita KurguzovAll rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "AICharacterBase.h"
#include "GameFramework/Controller.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAICharacterBase::AAICharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (bShouldAiStateBeUpdated)
	{
		if (GetWorld() != nullptr)
		{
			GetWorld()->GetTimerManager().SetTimer(StateUpdateTimerHandle,this,&AAICharacterBase::UpdateAiState ,TimeOfStateUpdate, true);
		}
	}
}

void AAICharacterBase::PlayDamageSound()
{
	if (HurtSound != nullptr&& GetWorld()!=nullptr&&!bDead)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HurtSound, GetActorLocation(), GetActorRotation());
	}
}

void AAICharacterBase::PlayDeathSound()
{
	if (DeathSound != nullptr && GetWorld() != nullptr && !bDead)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation(), GetActorRotation());
	}
}

void AAICharacterBase::UpdateAiState_Implementation()
{
	CalculateAiState();
	//here you set current speed, voice lines etc. based on current state

	//everything here is place holdewr
	if (AiState == EAIState::EAIS_Neutral) 
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;
	}
	else if (AiState == EAIState::EAIS_Panick)
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultRunningSpeed;
	}
	else if (AiState == EAIState::EAIS_Angry)
	{

	}
	else 
	{
		//Index out of range expection handling
	}
}

void AAICharacterBase::CalculateAiState_Implementation()
{
	//here is where you write code that defines current state
}

float AAICharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float resultDamage = DamageAmount;
	
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		auto pointEvent = static_cast<const FPointDamageEvent&>(DamageEvent);
		
		if (GetMesh()->FindClosestBone(pointEvent.HitInfo.Location) == "head")
		{
			resultDamage = Health * 1.5f;
		}

		Health -= resultDamage;

		if (Health <= 0.f)
		{
			Die();
		}
		else
		{
			PlayDamageSound();
		}
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) 
	{
		auto radialEvent = static_cast<const FPointDamageEvent&>(DamageEvent);
		
		Health -= resultDamage;

		if (Health <= 0.f)
		{						
			ImpluseToApply = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(radialEvent.ShotDirection, radialEvent.HitInfo.Location)) * 10000;

			LocationToApplyImpluseTo = radialEvent.HitInfo.Location;

			if (GetMesh() != nullptr)
			{
				GetMesh()->SetSimulatePhysics(true);
				GetMesh()->AddImpulseAtLocation(ImpluseToApply, LocationToApplyImpluseTo);
			}

			GetWorld()->GetTimerManager().SetTimer(impulseTimerHandle, this, &AAICharacterBase::ApplyImpulseToBody, 0.01f, false);
			Die();
		}
		else
		{
			PlayDamageSound();
		}
	}
	else
	{
		
	}

	
	return resultDamage;
}

void AAICharacterBase::Die_Implementation()
{
	 bDead = false; 

	 GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	 if (GetController() != nullptr)
	 {
		 GetController()->UnPossess();
	 }
	 PlayDeathSound();
}

void AAICharacterBase::ApplyImpulseToBody()
{
	if (GetMesh() != nullptr)
	{
		GetMesh()->AddImpulseAtLocation(ImpluseToApply, LocationToApplyImpluseTo);
	}
}

// Called every frame
void AAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

