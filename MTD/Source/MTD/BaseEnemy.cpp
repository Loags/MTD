// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{

}

void ABaseEnemy::UpdateEnemyColor_Implementation(const FLinearColor& Color)
{
	if (EnemyHead)
	{
		UMaterialInterface* NewMaterial = nullptr;

		// Choose the material based on the input color
		if (Color == FLinearColor::Green)
		{
			NewMaterial = GreenMaterial;
		}
		else if (Color == FLinearColor::White)
		{
			NewMaterial = WhiteMaterial;
		}

		// Apply the chosen material if it's valid
		if (NewMaterial)
		{
			EnemyHead->SetMaterial(0, NewMaterial);
			UE_LOG(LogTemp, Warning, TEXT("EnemyHead material changed based on color input."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No matching material found for the specified color!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No EnemyHead found to change material."));
	}
}


