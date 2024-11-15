// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "CoreMinimal.h"
#include "MTDHealthComponent.h"
#include "MTDEnemyHealthComponent.generated.h"

class ABaseEnemy;

UCLASS()
class MTD_API UMTDEnemyHealthComponent : public UMTDHealthComponent
{
	GENERATED_BODY()

public:
	UMTDEnemyHealthComponent();

	virtual void Death_Server_Reliable_Implementation() override;

	inline const void SetBaseEnemy(ABaseEnemy* BaseEnemyRef) { BaseEnemy = BaseEnemyRef; }

protected:
	virtual void OnRep_bIsDead() override;

private:
	ABaseEnemy* BaseEnemy;
};
