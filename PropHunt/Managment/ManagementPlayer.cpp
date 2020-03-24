// CopyrightNotice=BSD 2-Clause LicenseCopyright (c) 2019, MetalCat aka Nikita KurguzovAll rights reserved.Redistribution and use in source and binary forms, with or withoutmodification, are permitted provided that the following conditions are met:1. Redistributions of source code must retain the above copyright notice, this   list of conditions and the following disclaimer.2. Redistributions in binary form must reproduce the above copyright notice,   this list of conditions and the following disclaimer in the documentation   and/or other materials provided with the distribution.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THEIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AREDISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALDAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS ORSERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVERCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USEOF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "ManagementPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "ManagmentInterface.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"

// Sets default values
AManagementPlayer::AManagementPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = Camera;

	MouseDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("MouseDecal"));
}

// Called when the game starts or when spawned
void AManagementPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	SetupUI();

	if (GetController() != nullptr)
	{
		if (Cast<APlayerController>(GetController()) != nullptr)
		{
			PController = Cast<APlayerController>(GetController());
		}
	}
}

void AManagementPlayer::Interact_Implementation()
{
	if (PController != nullptr)
	{
		FHitResult hit;
		if (PController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, hit))
		{
			if (hit.GetActor() != nullptr && hit.GetComponent() != nullptr)
			{
				if(hit.GetActor()->Implements<UManagmentInterface>() || (Cast<IManagmentInterface>(hit.GetActor()) != nullptr))
				{
					IManagmentInterface::Execute_Interact(hit.GetActor(), this, hit.GetComponent());
				}
			}
			if (bBuilding)
			{
				FinishBuilding();
			}
		}
	}
}

void AManagementPlayer::FinishBuilding()
{
	if (bBuilding)
	{
		if (CurrentBuilding->CanBeBuilt())
		{
			bBuilding = false;
			CurrentBuilding->OnBuildFinished();
			CurrentBuilding->bBuilt = true;
			if (Info != nullptr)
			{
				Info->Buildings.Add(CurrentBuilding);
			}
			CurrentBuilding = nullptr;

			PController->SetInputMode(FInputModeUIOnly());
			PController->bShowMouseCursor = true;
		}
	}
}

void AManagementPlayer::CancelBuilding()
{
	if (bBuilding)
	{
		bBuilding = false;

		CurrentBuilding->Destroy();
		CurrentBuilding = nullptr;

		
		PController->SetInputMode(FInputModeUIOnly());
		PController->bShowMouseCursor = true;
	}

}

void AManagementPlayer::RotateBuilding()
{
	if (bBuilding && CurrentBuilding != nullptr)
	{
		CurrentBuilding->AddActorLocalRotation(FRotator(0.f, 30.f, 0.f));
	}
}

void AManagementPlayer::StartBuilding_Implementation(TSubclassOf<ABaseBuildingBase>BuildingClass)
{
	if (CurrentBuilding == nullptr)
	{
		if (GetWorld() != nullptr)
		{
			FActorSpawnParameters params = FActorSpawnParameters();
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (PController != nullptr)
			{
				FHitResult hit;
				if (PController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, hit))
				{
					CurrentBuilding = GetWorld()->SpawnActor<ABaseBuildingBase>(BuildingClass.Get(), hit.Location, FRotator::ZeroRotator, params);

				}
				else
				{
					CurrentBuilding = GetWorld()->SpawnActor<ABaseBuildingBase>(BuildingClass.Get(), FVector::ZeroVector, FRotator::ZeroRotator, params);
				}
			}
			else
			{
				CurrentBuilding = GetWorld()->SpawnActor<ABaseBuildingBase>(BuildingClass.Get(), FVector::ZeroVector, FRotator::ZeroRotator, params);
			}

			CurrentBuilding->OnBuildStarted();

			PController->SetInputMode(FInputModeGameOnly());
			PController->bShowMouseCursor = false;
			bBuilding = true;
		}
	}
}

bool AManagementPlayer::CanBeBuilt_Implementation(TSubclassOf<ABaseBuildingBase> BuildingClass)
{
	if (!bBuilding) { return true; }
	return false;
}

// Called every frame
void AManagementPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (PController != nullptr)
	{
		FHitResult hit;
		if (PController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, hit))
		{
			MouseDecal->SetWorldLocationAndRotation(hit.Location, UKismetMathLibrary::MakeRotationFromAxes(hit.ImpactNormal, FVector::ZeroVector, FVector::ZeroVector));
			if (bBuilding && CurrentBuilding != nullptr) 
			{
				CurrentBuilding->SetActorLocation(hit.Location);
			}
		}
	}

}

// Called to bind functionality to input
void AManagementPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ManagmentInteract", EInputEvent::IE_Pressed, this, &AManagementPlayer::Interact);

	PlayerInputComponent->BindAction("ManagmentRightMouseInteract", EInputEvent::IE_Pressed, this, &AManagementPlayer::CancelBuilding);

	PlayerInputComponent->BindAction("ManagmentRotateObject", EInputEvent::IE_Pressed, this, &AManagementPlayer::RotateBuilding);

}

