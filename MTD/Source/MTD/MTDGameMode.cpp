// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTDGameMode.h"
#include "MTDCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMTDGameMode::AMTDGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
