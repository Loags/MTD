// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "MTDHealthComponent.h"
#include "MTD.h"
#include <Net/UnrealNetwork.h>
#include <Components/WidgetComponent.h>
#include "HealthBarWidget.h"

UMTDHealthComponent::UMTDHealthComponent()
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bIsDead = false;
	OwnerActor = nullptr;
	HealthBar = nullptr;
}

void UMTDHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMTDHealthComponent, CurrentHealth);
	DOREPLIFETIME(UMTDHealthComponent, MaxHealth);
	DOREPLIFETIME(UMTDHealthComponent, bIsDead);
}

void UMTDHealthComponent::TakeDamage_Server_Reliable_Implementation(float DamageAmount)
{
	if (!OwnerActor->HasAuthority()) return;

	CurrentHealth -= DamageAmount;
	OnRep_CurrentHealth();

	if (CurrentHealth <= 0.0f)
	{
		CurrentHealth = 0.0f;
		OnRep_bIsDead();
	}
}

void UMTDHealthComponent::Heal_Server_Reliable_Implementation(float HealAmount)
{
	if (!OwnerActor || !OwnerActor->HasAuthority()) return;

	CurrentHealth += HealAmount;
	OnRep_CurrentHealth();

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

void UMTDHealthComponent::Death_Server_Reliable_Implementation()
{
	if (!OwnerActor || !OwnerActor->HasAuthority()) return;

	bIsDead = true;
	OnRep_bIsDead();
}

bool UMTDHealthComponent::InitializeHealthBar()
{
	UWidgetComponent* WidgetComponent = OwnerActor->FindComponentByClass<UWidgetComponent>();
	if (WidgetComponent)
	{
		HealthBar = Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject());
		if (!HealthBar)
		{
			UE_LOG(MTDLog, Error, TEXT("[%s] - UMTDHealthComponent::InitializeHealthBar - Failed to cast HealthBarWidget: on %s"),
				*FString(OwnerActor->HasAuthority() ? TEXT("Server") : TEXT("Client")),
				*OwnerActor->GetName());
			return false;
		}
	}
	else
	{
		UE_LOG(MTDLog, Error, TEXT("[%s] - UMTDHealthComponent::InitializeHealthBar - WidgetComponent not found: on %s"),
			*FString(OwnerActor->HasAuthority() ? TEXT("Server") : TEXT("Client")),
			*OwnerActor->GetName());
		return false;
	}

	OnRep_CurrentHealth();
	return true;
}

void UMTDHealthComponent::BeginPlay()
{
	OwnerActor = GetOwner();
}

void UMTDHealthComponent::OnRep_MaxHealth()
{
	if (HealthBar && !bIsDead)
	{
		float HealthPercentage = CurrentHealth / MaxHealth;
		HealthBar->SetHealthPercentage(HealthPercentage);
		UE_LOG(MTDLog, Log, TEXT("[%s] - OnRep_MaxHealth - Updated HealthBar: %f on %s"),
			*FString(OwnerActor->HasAuthority() ? TEXT("Server") : TEXT("Client")),
			MaxHealth,
			*OwnerActor->GetName());

	}
}

void UMTDHealthComponent::OnRep_CurrentHealth()
{
	if (HealthBar && !bIsDead)
	{
		float HealthPercentage = CurrentHealth / MaxHealth;
		HealthBar->SetHealthPercentage(HealthPercentage);
		UE_LOG(MTDLog, Log, TEXT("[%s] - OnRep_CurrentHealth - Updated HealthBar: %f on %s"),
			*FString(OwnerActor->HasAuthority() ? TEXT("Server") : TEXT("Client")),
			CurrentHealth,
			*OwnerActor->GetName());
	}
}

void UMTDHealthComponent::OnRep_bIsDead()
{
	UE_LOG(MTDLog, Log, TEXT("[%s] - OnRep_bIsDead - Updated bIsDead: on %s on %s"),
		*FString(OwnerActor->HasAuthority() ? TEXT("Server") : TEXT("Client")),
		bIsDead ? TEXT("true") : TEXT("false"),
		*OwnerActor->GetName());
}


