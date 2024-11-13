// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "Components/SplineComponent.h"
#include "EntityManager.h"
#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CustomGameState.generated.h"


UCLASS()
class MTD_API ACustomGameState : public AGameState
{
	GENERATED_BODY()

public:
	ACustomGameState();

	inline void SetEntityManager(AEntityManager* EntityManagerRef) { EntityManager = EntityManagerRef; }
	inline void SetPathSpline(USplineComponent* PathSplineRef) { PathSpline = PathSplineRef; }
	inline void SetPathActor(AActor* PathActorRef) { PathActor = PathActorRef; }

	inline AEntityManager* GetEntityManager() const { return EntityManager; }
	inline USplineComponent* GetPathSpline() const { return PathSpline; }
	inline AActor* GetPathActor() const { return PathActor; }

private:
	AEntityManager* EntityManager;
	AActor* PathActor;
	USplineComponent* PathSpline;
};
