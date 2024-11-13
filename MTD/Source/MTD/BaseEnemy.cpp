// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "BaseEnemy.h"
#include <Net/UnrealNetwork.h>
#include "CustomGameState.h"
#include "MTD.h"
#include "HealthBarWidget.h"
#include <Components/WidgetComponent.h>


ABaseEnemy::ABaseEnemy()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bIsDead = false;
	EntityManager = nullptr;
	PathSpline = nullptr;
	aAIController = nullptr;
	AcceptanceRadius = 100.0f;
	CurrentSplineIndex = 0;
	bEndOfPathReached = false;
	HealthBar = nullptr;
	GameState = nullptr;

	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (InitializeHealthBar())
	{
		OnRep_CurrentHealth();
	}

	if (HasAuthority())
	{
		if (InitializeGameState())
		{
			InitializeEntityManager();
			InitializePathSpline();
		}
		else
		{
			return;
		}

		if (aAIController && !bEndOfPathReached)
		{
			MoveAlongPath();
		}
	}
}

void ABaseEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	aAIController = Cast<AAIController>(NewController);

	if (aAIController)
	{
		// Bind the OnMoveCompleted delegate to our function using BindUObject
		aAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABaseEnemy::OnMoveCompleted);
	}
	else
	{
		UE_LOG(MTDLog, Error, TEXT("ABaseEnemy::PossessedBy - AIController cast failed."));
	}
}

void ABaseEnemy::MoveAlongPath()
{
	if (CurrentSplineIndex >= PathSpline->GetNumberOfSplinePoints())
	{
		bEndOfPathReached = true;
		UE_LOG(MTDLog, Log, TEXT("MoveAlongPath: Reached the end of the path for ABaseEnemy."));
		return;
	}

	FVector TargetLocation = PathSpline->GetLocationAtSplinePoint(CurrentSplineIndex, ESplineCoordinateSpace::World);
	if (aAIController->MoveToLocation(TargetLocation, AcceptanceRadius) == EPathFollowingRequestResult::Failed)
	{
		UE_LOG(MTDLog, Error, TEXT("MoveToLocation failed."));
	}
}

void ABaseEnemy::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		// Successfully reached the target, proceed to the next spline point
		CurrentSplineIndex++;
		MoveAlongPath();
	}
}

bool ABaseEnemy::InitializeHealthBar()
{
	UWidgetComponent* WidgetComponent = FindComponentByClass<UWidgetComponent>();
	if (WidgetComponent)
	{
		HealthBar = Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject());
		if (!HealthBar)
		{
			UE_LOG(MTDLog, Error, TEXT("Failed to cast HealthBar widget in %s"), *GetName());
			return false;
		}
	}
	else
	{
		UE_LOG(MTDLog, Error, TEXT("WidgetComponent not found in %s"), *GetName());
		return false;
	}

	return true;
}

bool ABaseEnemy::InitializeGameState()
{
	GameState = GetWorld()->GetGameState<ACustomGameState>();
	if (GameState)
	{
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("GameState cast failed!"));
	return false;
}

bool ABaseEnemy::InitializeEntityManager()
{
	EntityManager = GameState->GetEntityManager();
	if (EntityManager)
	{
		EntityManager->RegisterEnemy(this);
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("No EntityManager found in the level for entity registration."));
	return false;
}

bool ABaseEnemy::InitializePathSpline()
{
	PathSpline = GameState->GetPathSpline();
	if (PathSpline)
	{
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("PathSpline is NULL in ABaseEnemy."));
	return false;
}

void ABaseEnemy::OnRep_CurrentHealth()
{
	if (HealthBar && !bIsDead)
	{
		float HealthPercentage = CurrentHealth / MaxHealth;
		HealthBar->SetHealthPercentage(HealthPercentage);
		UE_LOG(MTDLogCombat, Log, TEXT("OnRep_CurrentHealth - Updated HealthBar: %f on %s"), CurrentHealth, HasAuthority() ? TEXT("Server") : TEXT("Client"));
	}
}

void ABaseEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseEnemy, CurrentHealth);
	DOREPLIFETIME(ABaseEnemy, MaxHealth);
	DOREPLIFETIME(ABaseEnemy, bIsDead);
	DOREPLIFETIME(ABaseEnemy, EntityManager);
}

void ABaseEnemy::Attack_Server_Reliable_Implementation(ABaseTower* Tower)
{
	if (HasAuthority())
	{
		UE_LOG(MTDLogCombat, Warning, TEXT("Enemy is attacking!"));
	}
}

void ABaseEnemy::TakeDamage_Server_Reliable_Implementation(float DamageAmount)
{
	if (HasAuthority())
	{
		CurrentHealth -= DamageAmount;

		OnRep_CurrentHealth();

		if (CurrentHealth <= 0.0f)
		{
			CurrentHealth = 0.0f;
			OnDeath_Multicast_Reliable();
		}
	}
}

void ABaseEnemy::Heal_Server_Reliable_Implementation(float HealAmount)
{
	if (HasAuthority())
	{
		CurrentHealth += HealAmount;
		if (CurrentHealth > MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
	}
}

void ABaseEnemy::OnDeath_Multicast_Reliable_Implementation()
{
	bIsDead = true;
	UE_LOG(MTDLog, Warning, TEXT("Enemy has died!"));
}