// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTD.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, MTD, "MTD");

DEFINE_LOG_CATEGORY(MTDLog);

void DrawLineBetweenActors(AActor* Actor1, AActor* Actor2, UWorld* World, FColor LineColor, float LineThickness, float Duration)
{
	if (Actor1 && Actor2 && World)
	{
		// Get the locations of the two actors
		FVector StartPos = Actor1->GetActorLocation();
		FVector EndPos = Actor2->GetActorLocation();

		// Draw a debug line between the two positions
		DrawDebugLine(World, StartPos, EndPos, LineColor, true, Duration, 0, LineThickness);
	}
}



