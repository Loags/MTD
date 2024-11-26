// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "CoreMinimal.h"
#include "BaseTower.h"
#include "MTDRapidFireTower.generated.h"

/**
 * 
 */
UCLASS()
class MTD_API AMTDRapidFireTower : public ABaseTower
{
	GENERATED_BODY()

public:
	AMTDRapidFireTower();


protected:
	virtual void BeginPlay() override;
	
};
