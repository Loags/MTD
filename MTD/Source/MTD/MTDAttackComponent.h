// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MTDAttackComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MTD_API UMTDAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMTDAttackComponent();

	inline const float GetAttackDamage() const { return AttackDamage; }
	inline const float GetAttackRange() const { return AttackRange; }
	inline const float GetAttackSpeed() const { return AttackSpeed; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Attack_Server_Reliable();

	UFUNCTION()
	virtual void OnRep_AttackDamage();

	UFUNCTION()
	virtual void OnRep_AttackRange();

	UFUNCTION()
	virtual void OnRep_AttackSpeed();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_AttackDamage)
	float AttackDamage;

	UPROPERTY(ReplicatedUsing = OnRep_AttackRange)
	float AttackRange;

	UPROPERTY(ReplicatedUsing = OnRep_AttackSpeed)
	float AttackSpeed;

	AActor* OwnerActor;
};
