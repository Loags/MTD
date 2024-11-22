// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"                  
#include "Components/ActorComponent.h"
#include "Navigation/PathFollowingComponent.h" 
#include "MTDPathFollowingComponent.generated.h"

class USplineComponent;
class ABaseEnemy;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MTD_API UMTDPathFollowingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMTDPathFollowingComponent();

	void StartFollowingPath();
	bool InitializePath(USplineComponent* PathSplineRef);
	bool InitializeAiController(AAIController* AiControllerRef);
	bool InitializeBaseEnemy(ABaseEnemy* BaseEnemyRef);
	void BindOnMoveCompleted();
	float GetDistanceToGoal() const;

private:
	void MoveToNextPoint();
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	USplineComponent* PathSpline;
	AAIController* AiController;
	ABaseEnemy* BaseEnemy;

	bool bEndOfPathReached;
	int32 CurrentSplineIndex;
	float AcceptanceRadius;

};
