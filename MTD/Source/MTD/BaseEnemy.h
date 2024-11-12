// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "AIController.h"                  
#include "Navigation/PathFollowingComponent.h"
#include "Components/SplineComponent.h"
#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class AEntityManager;
class ACustomGameState;
class UHealthBarWidget;

UCLASS()
class MTD_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	const float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	const float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(Server, Reliable)
	virtual void TakeDamage_Server_Reliable(float DamageAmount);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	virtual void Attack_Server_Reliable(ABaseTower* Target);

	UFUNCTION(Server, Reliable)
	virtual void Heal_Server_Reliable(float HealAmount);

	UFUNCTION(NetMulticast, Reliable)
	virtual void OnDeath_Multicast_Reliable();

	UFUNCTION(NetMulticast, Reliable)
	virtual void UpdateHealthBar_Multicast_Reliable();

private:

	virtual void PossessedBy(AController* NewController) override;
	void MoveAlongPath();
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	bool InitializeHealthBar();
	bool InitializeGameState();
	bool InitializeEntityManager();
	bool InitializePathSpline();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Combat")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Combat")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Combat")
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Management")
	AEntityManager* EntityManager;

	AAIController* aAIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Management")
	USplineComponent* PathSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UHealthBarWidget* HealthBar;

private:
	float AcceptanceRadius;
	bool bEndOfPathReached;
	int32 CurrentSplineIndex;
	ACustomGameState* GameState;
};
