// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita Kurguzov All rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicMusicControllerBase.generated.h"

UENUM(BlueprintType)		//What ammo weapon needs
enum class EMusicState : uint8
{
	EMS_Idle 	UMETA(DisplayName = "Idle"),
	EMS_Percussion 	UMETA(DisplayName = "Percussion"),
	EMS_Action 	UMETA(DisplayName = "Action")
};

UCLASS()
class PROPHUNT_API ADynamicMusicControllerBase : public AActor
{
	GENERATED_BODY()
	
public:	

	FTimerHandle UpdateTimerHandle;

	FTimerHandle CalmingDownTimerHandle;

	FTimerHandle SwitchingToIdleTimerHadnle;

	FTimerHandle DescreasePercussionVolumeTimerHandle;

	// Sets default values for this actor's properties
	ADynamicMusicControllerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* EditorBillboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), SaveGame)
		UAudioComponent* Percussion_Loop = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), SaveGame)
		UAudioComponent* Action_Loop = nullptr;

	/*used only if bUseSoundParametrs = true*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), SaveGame)
		/*used only if bUseSoundParametrs = true*/
		UAudioComponent* Music_Loop = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), SaveGame)
		EMusicState State = EMusicState::EMS_Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), SaveGame)
		bool bSawPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), SaveGame)
		bool bOnlyStopWhenEveryOneIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), SaveGame)
		TArray<AActor*>SoldersToCareAbout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), SaveGame)
		float TimeToSwitchToPercussion = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), SaveGame)
		float TimeToSwitchToIdle = 15.f;

	/*if true only "Music_Loop" will be used and instead of enabling/disabling different music components parametrs in sound file will be used*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		/*if true only "Music_Loop" will be used and instead of enabling/disabling different music components parametrs in sound file will be used*/
		bool bUseSoundParametrs = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Update();

	UFUNCTION(BlueprintCallable)
		void SwitchToPercussion();

	UFUNCTION(BlueprintCallable)
		void SwitchToIdle();

	UFUNCTION(BlueprintCallable)
	void DecreaseVolume();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool CanThisSolderSeePlayer(AActor*Solder);

	bool CanThisSolderSeePlayer_Implementation(AActor* Solder) { return false; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool IsEveryoneDead();

	bool IsEveryoneDead_Implementation() { return false; }

};
