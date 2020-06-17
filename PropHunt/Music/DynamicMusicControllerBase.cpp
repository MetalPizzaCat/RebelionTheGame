// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita Kurguzov All rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "DynamicMusicControllerBase.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"

// Sets default values
ADynamicMusicControllerBase::ADynamicMusicControllerBase()
{
	// no need to tick. Update is done using update function which is called less frequently
	PrimaryActorTick.bCanEverTick = false;

	Percussion_Loop = CreateDefaultSubobject<UAudioComponent>(TEXT("Percussion_Loop"));
	Percussion_Loop->SetupAttachment(RootComponent);
	Percussion_Loop->VolumeMultiplier = 0.f;

	Action_Loop = CreateDefaultSubobject<UAudioComponent>(TEXT("Action_Loop"));
	Action_Loop->SetupAttachment(RootComponent);
	Action_Loop->VolumeMultiplier = 0.f;

	Music_Loop = CreateDefaultSubobject<UAudioComponent>(TEXT("Music_Loop"));
	Music_Loop->SetupAttachment(RootComponent);
	Music_Loop->VolumeMultiplier = 0.f;
}

// Called when the game starts or when spawned
void ADynamicMusicControllerBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, this, &ADynamicMusicControllerBase::Update, 0.5f);
	Action_Loop->Stop();
	Percussion_Loop->Stop();
	Music_Loop->Stop();
}

void ADynamicMusicControllerBase::DecreaseVolume()
{
	if (bUseSoundParametrs)
	{
		if (Music_Loop->VolumeMultiplier >= 0.f)
		{
			Music_Loop->VolumeMultiplier -= 0.1f;
		}
		else
		{
			Music_Loop->Stop();
			DescreasePercussionVolumeTimerHandle.Invalidate();
		}
	}
	else 
	{
		if (Percussion_Loop->VolumeMultiplier >= 0.f)
		{
			Percussion_Loop->VolumeMultiplier -= 0.1f;
		}
		else
		{
			Percussion_Loop->Stop();
			DescreasePercussionVolumeTimerHandle.Invalidate();
		}
	}
}

// Called every frame
void ADynamicMusicControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADynamicMusicControllerBase::Update()
{
	if (SoldersToCareAbout.Num() > 0)
	{
		for (int i = 0; i < SoldersToCareAbout.Num(); i++)
		{
			if (CanThisSolderSeePlayer(SoldersToCareAbout[i]))
			{
				if (State == EMusicState::EMS_Idle)
				{
					if (bUseSoundParametrs)
					{
						Music_Loop->Play();
					}
					else
					{//we start music again, because it was stoped when switched to idle
					//they have to be played at the same time to avoid desync.
						Percussion_Loop->Play();
						Action_Loop->Play();
					}
				}
				State = EMusicState::EMS_Action;
				bSawPlayer = true;
				break;
			}
			else if (bSawPlayer && CalmingDownTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().SetTimer(CalmingDownTimerHandle, this, &ADynamicMusicControllerBase::SwitchToPercussion, TimeToSwitchToPercussion);
			}
		}
		if (State == EMusicState::EMS_Action)
		{
			if (bUseSoundParametrs)
			{
				Music_Loop->SetIntParameter("State", 1);
			}
			else
			{
				Percussion_Loop->VolumeMultiplier = 0.f;
				Action_Loop->VolumeMultiplier = 1.f;
			}

			SwitchingToIdleTimerHadnle.Invalidate();
		}
	}
}

void ADynamicMusicControllerBase::SwitchToPercussion()
{
	if (bUseSoundParametrs)
	{
		Music_Loop->SetIntParameter("State", 0);
	}
	else
	{
		Percussion_Loop->VolumeMultiplier = 1.f;
		Action_Loop->VolumeMultiplier = 0.f;
	}
	CalmingDownTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(SwitchingToIdleTimerHadnle, this, &ADynamicMusicControllerBase::SwitchToIdle, TimeToSwitchToIdle);
}

void ADynamicMusicControllerBase::SwitchToIdle()
{
	bSawPlayer = false;
	State = EMusicState::EMS_Idle;
	SwitchingToIdleTimerHadnle.Invalidate();

	GetWorld()->GetTimerManager().SetTimer(DescreasePercussionVolumeTimerHandle, this, &ADynamicMusicControllerBase::DecreaseVolume, 0.1f,true);

	if (!bUseSoundParametrs)
	{
		Action_Loop->Stop();
	}
}

