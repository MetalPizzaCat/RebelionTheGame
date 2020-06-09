// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita Kurguzov All rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "PropHunt/Prop/PropBase.h"
#include "Kismet/GameplayStatics.h"
#include "PropHunt/Water/WaterBody.h"
#include "Engine.h"
#include "TimerManager.h"

// Sets default values
APropBase::APropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//SetRootComponent(WeaponMesh);
	Mesh->SetSimulatePhysics(true);
	RootComponent = Mesh;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Mesh);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Box->OnComponentHit.AddDynamic(this, &APropBase::OnPropHit);

	ScrapeSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ScrapeSound"));
	ScrapeSound->SetupAttachment(Mesh);
	ScrapeSound->bAutoActivate = false;
	ScrapeSound->VolumeMultiplier = 0.5f;
}

// Called when the game starts or when spawned
void APropBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, this, &APropBase::Update, UpdateTime, true);

	Box->IgnoreActorWhenMoving(this, true);
}

void APropBase::Update_Implementation()
{
}

bool APropBase::IsTouchingAnything_Implementation()
{
	TSet<AActor*>Actors;
	Box->GetOverlappingActors(Actors);
	if (Actors.Num() > 0)
	{
		for (int i = 0; i < Actors.Num(); i++)
		{
			if (Cast<AWaterBody>(Actors.Array()[i])==nullptr)
			{
				return true;//we need to check if it overlaps something besides water
			}
		}
		return false;//we check everything but it only overlaped water,we return false
	}
	return false;
}

bool APropBase::IsInTheWater_Implementation()
{
	TSet<AActor*>Actors;
	Box->GetOverlappingActors(Actors);
	if (Actors.Num() > 0)
	{
		for (int i = 0; i < Actors.Num(); i++)
		{
			if (Cast<AWaterBody>(Actors.Array()[i]) != nullptr)
			{
				return true;//found water, return true
			}
		}
		return false;
	}
	return false;
}

void APropBase::OnPropHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if ( HitSound != nullptr && NormalImpulse.Size() >= MinImpulse)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.Location, 1.f, 1.f, 0.f, SoundAttenuation);
	}
}

// Called every frame
void APropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ScrapeSound != nullptr)
	{
		if (IsTouchingAnything() )
		{
			if (IsInTheWater() && !bIsInTheWater)
			{
				//we touch water for the first time
				bIsInTheWater = true;

				if (SplashSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), SplashSound, GetActorLocation(), 1.f, 1.f, 0.f, SoundAttenuation);
				}
			}
			else if (!IsInTheWater() && bIsInTheWater)
			{
				//first moment of leaving water
				bIsInTheWater = false;

				if (SplashSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), SplashSound, GetActorLocation(), 1.f, 1.f, 0.f, SoundAttenuation);
				}
			}

			if (ScrapeSound->GetPlayState() == EAudioComponentPlayState::Stopped)
			{
				ScrapeSound->Play();
			}
			else if (ScrapeSound->GetPlayState() == EAudioComponentPlayState::Paused)
			{
				ScrapeSound->SetPaused(false);
			}
		}
		else if (IsInTheWater() && !bIsInTheWater)
		{
			//we touch water for the first time
			bIsInTheWater = true;

			if (SplashSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), SplashSound, GetActorLocation(), 1.f, 1.f, 0.f, SoundAttenuation);
			}
		}
		else if (!IsInTheWater() && bIsInTheWater)
		{
			//first moment of leaving water
			bIsInTheWater = false;

			if (SplashSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), SplashSound, GetActorLocation(), 1.f, 1.f, 0.f, SoundAttenuation);
			}
		}
		else
		{
			ScrapeSound->SetPaused(true);
		}
	}
}

void APropBase::OnConstruction(const FTransform& Transform)
{

}

