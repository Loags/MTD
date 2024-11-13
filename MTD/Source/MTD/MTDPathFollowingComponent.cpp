// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#include "MTDPathFollowingComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/SplineComponent.h"
#include "MTD.h"


UMTDPathFollowingComponent::UMTDPathFollowingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	PathSpline = nullptr;
	AiController = nullptr;
	bEndOfPathReached = false;
	CurrentSplineIndex = 0;
	AcceptanceRadius = 50.0f;
}

void UMTDPathFollowingComponent::StartFollowingPath()
{
	if (!PathSpline || !AiController || bEndOfPathReached) return;

	MoveToNextPoint();
}

bool UMTDPathFollowingComponent::InitializePath(USplineComponent* PathSplineRef)
{
	if (PathSplineRef)
	{
		PathSpline = PathSplineRef;
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("[%s] - UMTDPathFollowingComponent::InitializePath - PathSpline is nullptr: on %s"),
		*FString(GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client")),
		*GetOwner()->GetName());
	return false;

}

bool UMTDPathFollowingComponent::InitializeAiController(AAIController* AiControllerRef)
{
	if (AiControllerRef)
	{
		AiController = AiControllerRef;
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("[%s] - UMTDPathFollowingComponent::InitializeAiController - PathSpline is nullptr: on %s"),
		*FString(GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client")),
		*GetOwner()->GetName());
	return false;
}

void UMTDPathFollowingComponent::BindOnMoveCompleted()
{
	if (!AiController)
	{
		UE_LOG(MTDLog, Error, TEXT("[%s] - UMTDPathFollowingComponent::BindOnMoveCompleted - AiController is nullptr: on %s"),
			*FString(GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client")),
			*GetOwner()->GetName());
		return;
	}

	UPathFollowingComponent* PathFollowingComponent = AiController->GetPathFollowingComponent();
	if (!PathFollowingComponent)
	{
		UE_LOG(MTDLog, Error, TEXT("[%s] - UMTDPathFollowingComponent::BindOnMoveCompleted - PathFollowingComponent is nullptr: on %s"),
			*FString(GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client")),
			*GetOwner()->GetName());
		return;
	}

	PathFollowingComponent->OnRequestFinished.AddUObject(this, &UMTDPathFollowingComponent::OnMoveCompleted);
}

void UMTDPathFollowingComponent::MoveToNextPoint()
{
	if (CurrentSplineIndex >= PathSpline->GetNumberOfSplinePoints())
	{
		bEndOfPathReached = true;
		return;
	}

	FVector TargetLocation = PathSpline->GetLocationAtSplinePoint(CurrentSplineIndex, ESplineCoordinateSpace::World);
	if (AiController->MoveToLocation(TargetLocation, AcceptanceRadius) == EPathFollowingRequestResult::Failed)
	{
		UE_LOG(MTDLog, Error, TEXT("[%s] - UMTDPathFollowingComponent::MoveToNextPoint - MoveToLocation failed: on %s"),
			*FString(GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client")),
			*GetOwner()->GetName());
	}
}

void UMTDPathFollowingComponent::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		CurrentSplineIndex++;
		StartFollowingPath();
	}

}
