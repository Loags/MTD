// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#include "CustomGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MTD.h"
#include "CustomGameState.h"
#include <Components/SplineComponent.h>
#include "SplinePath.h"


void ACustomGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Retrieve the custom game state and verify the cast
	ACustomGameState* CustomGameState = GetGameState<ACustomGameState>();
	if (!CustomGameState)
	{
		UE_LOG(MTDLog, Error, TEXT("CustomGameState could not be found or casted correctly in GameMode."));
		return;
	}

	// Attempt to retrieve BP_EntityManager (derived from AEntityManager) from the level
	AEntityManager* EntityManager = Cast<AEntityManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEntityManager::StaticClass()));
	if (!EntityManager)
	{
		UE_LOG(MTDLog, Error, TEXT("BP_EntityManager (AEntityManager) not found in the level! Ensure it's added."));
		return;
	}

	// Set the EntityManager in CustomGameState
	CustomGameState->SetEntityManager(EntityManager);
	UE_LOG(MTDLog, Log, TEXT("EntityManager successfully set in CustomGameState."));


	AActor* PathActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASplinePath::StaticClass());
	if (PathActor)
	{
		USplineComponent* PathSpline = PathActor->FindComponentByClass<USplineComponent>();
		if (PathSpline)
		{
			CustomGameState->SetPathActor(PathActor);
			CustomGameState->SetPathSpline(PathSpline);
			UE_LOG(MTDLog, Log, TEXT("PathActor successfully set in CustomGameState."));
			UE_LOG(MTDLog, Log, TEXT("PathSpline successfully set in CustomGameState."));
		}
		else
		{
			UE_LOG(MTDLog, Error, TEXT("No SplineComponent found on BP_PathActor."));
		}
	}
	else
	{
		UE_LOG(MTDLog, Error, TEXT("BP_PathActor not found in the level! Ensure it's added."));
	}

}
