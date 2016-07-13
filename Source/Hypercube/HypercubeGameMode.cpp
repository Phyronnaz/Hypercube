// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Hypercube.h"
#include "HypercubeGameMode.h"
#include "HypercubeCharacter.h"

AHypercubeGameMode::AHypercubeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
