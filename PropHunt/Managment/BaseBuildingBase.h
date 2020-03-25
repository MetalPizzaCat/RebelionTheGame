// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita KurguzovAll rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PropHunt/Managment/ItemInfo.h"
#include "BaseBuildingBase.generated.h"

UCLASS()
class PROPHUNT_API ABaseBuildingBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBuildingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bBuilt = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FString,int>NeededItems;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnBuildFinished();

	void OnBuildFinished_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnBuildStarted();

	void OnBuildStarted_Implementation() {}

	/*USE ONLY FOR COLLISION CHECKS, EVERYTHING ELSE IS LOCATED IN MANAGEMENT PLAYER*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		/*USE ONLY FOR COLLISION CHECKS, EVERYTHING ELSE IS LOCATED IN MANAGEMENT PLAYER*/
		bool CanBeBuilt();
	

	bool CanBeBuilt_Implementation() { return false; }
};
