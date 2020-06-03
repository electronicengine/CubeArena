// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoxArenaGameMode.h"
#include "BoxArenaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABoxArenaGameMode::ABoxArenaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
