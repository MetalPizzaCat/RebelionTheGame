// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PropHuntGameMode.h"
#include "PropHuntHUD.h"
#include "PropHuntCharacter.h"
#include "UObject/ConstructorHelpers.h"

APropHuntGameMode::APropHuntGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APropHuntHUD::StaticClass();
}
