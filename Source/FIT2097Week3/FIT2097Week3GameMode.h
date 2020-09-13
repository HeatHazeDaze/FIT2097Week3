// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FIT2097Week3Character.h"
#include "GameFramework/GameModeBase.h"
#include "FIT2097Week3GameMode.generated.h"

UCLASS(minimalapi)
class AFIT2097Week3GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFIT2097Week3GameMode();

	//UFUNCTIONs bounded to event listeners.
	UFUNCTION()
	void PauseGame(bool isPaused);

	UFUNCTION()
	void WinGame();

	UFUNCTION()
	void LoseGame();
	
protected:

	//References to Widgets, exposed to blueprints so we can set them in the editor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
		TSubclassOf<UUserWidget> DefaultLevelWinWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
		UUserWidget* LevelWinWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
		TSubclassOf<UUserWidget> DefaultLevelLossWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
		UUserWidget* LevelLossWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
		TSubclassOf<UUserWidget> DefaultLevelPauseWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
		UUserWidget* LevelPauseWidget;

private:

	void BeginPlay() override;
	//Pointer to our player controller, will be used to pause the game
	APlayerController* Controller;
	//Pointer to our player character, will be used to decrement the player's health
	AFIT2097Week3Character* Player;
	//Timer used to call decrement health function every two seconds
	FTimerHandle PlayerDamageTimer;
	
};



