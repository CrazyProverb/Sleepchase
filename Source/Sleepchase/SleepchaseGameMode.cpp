// Copyright Epic Games, Inc. All Rights Reserved.

#include "SleepchaseGameMode.h"
#include "SleepchaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASleepchaseGameMode::ASleepchaseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
