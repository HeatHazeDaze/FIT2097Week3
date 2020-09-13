// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FIT2097Week3HUD.generated.h"

UCLASS()
class AFIT2097Week3HUD : public AHUD
{
	GENERATED_BODY()

public:
	AFIT2097Week3HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

