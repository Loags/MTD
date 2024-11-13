// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MTDHealthComponent.generated.h"


class UHealthBarWidget;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MTD_API UMTDHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMTDHealthComponent();

	UFUNCTION(Server, Reliable)

	virtual void TakeDamage_Server_Reliable(float DamageAmount);

	UFUNCTION(Server, Reliable)
	virtual void Heal_Server_Reliable(float HealAmount);

	UFUNCTION(Server, Reliable)
	virtual void Death_Server_Reliable();

	bool InitializeHealthBar();

	inline const void InitializeOwner(AActor* OwnerRef) { OwnerActor = OwnerRef; }
	inline const float GetCurrentHealth() const { return CurrentHealth; }
	inline const float GetMaxHealth() const { return MaxHealth; }
	inline const bool GetIsDead() const { return bIsDead; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnRep_MaxHealth();

	UFUNCTION()
	virtual void OnRep_CurrentHealth();

	UFUNCTION()
	virtual void OnRep_bIsDead();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	UPROPERTY(ReplicatedUsing = OnRep_bIsDead)
	bool bIsDead;

private:
	AActor* OwnerActor;
	UHealthBarWidget* HealthBar;

};
