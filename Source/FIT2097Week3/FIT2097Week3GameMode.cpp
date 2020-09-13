// Copyright Epic Games, Inc. All Rights Reserved.

#include "FIT2097Week3GameMode.h"
#include "FIT2097Week3HUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFIT2097Week3GameMode::AFIT2097Week3GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	
	// use our custom HUD class
	HUDClass = AFIT2097Week3HUD::StaticClass();
}

//Initializes several pointers and binds several functions to event listeners
void AFIT2097Week3GameMode::BeginPlay()
{
	Super::BeginPlay();

	//Gets the player controller from the world and stores it as a reference
	Controller = GetWorld()->GetFirstPlayerController();

	//Code to set Controller to Game only when we introduce UI
	FInputModeGameOnly InputMode;
	Controller->SetInputMode(InputMode);
	Controller->bShowMouseCursor = false;
	
	//Set Our FIT2097Week3 Character into our Player pointer
	Player = Cast<AFIT2097Week3Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//Binding functions to event listeners.
	Player->PlayerDeath.AddDynamic(this, &AFIT2097Week3GameMode::LoseGame);
	Player->PlayerPause.AddDynamic(this, &AFIT2097Week3GameMode::PauseGame);
	Player->PlayerWin.AddDynamic(this, &AFIT2097Week3GameMode::WinGame);
	
	// Set a timer for every two seconds and call the decrease health function
	GetWorldTimerManager().SetTimer(PlayerDamageTimer, Player, &AFIT2097Week3Character::DecreaseHealth, 2.0f, true, 2.0f);

	

}

//is called when the game is paused, bool value changes to go from paused to unpaused./
void AFIT2097Week3GameMode::PauseGame(bool isPaused)
{
	//UGameplayStatics::SetGamePaused()

	Controller->SetPause(isPaused);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game Paused"));
	
	if(isPaused)
	{
		//Checks if we have set a default level pause widget
		if (DefaultLevelPauseWidget)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pause Widget Built"));

			//If yes, add to viewport.
			LevelPauseWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultLevelPauseWidget);
			LevelPauseWidget->AddToViewport();
		}
		else
		{
			//Warning message
			UE_LOG(LogTemp, Warning, TEXT("No Default Level Pause Widget Selected!!!"));
		}
	}
	else
	{
		//If no longer paused, remove widget from viewport.
		LevelPauseWidget->RemoveFromViewport();
	}
	
}

void AFIT2097Week3GameMode::WinGame()
{
	//Checks if we have set a default level win widget
	if(DefaultLevelWinWidget)
	{
		//If yes, add to viewport.
		LevelWinWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultLevelWinWidget);
		LevelWinWidget->AddToViewport();
		//Allow mouse to be used to select the button on HUD
		Controller->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		Controller->SetInputMode(InputMode);
	}
	else
	{
		//Warning message
		UE_LOG(LogTemp, Warning, TEXT("No Default Level Win Widget Selected!!!"));
	}
}

void AFIT2097Week3GameMode::LoseGame()
{
	//Checks if we have set a default level lose widget
	if (DefaultLevelLossWidget)
	{
		//If yes, add to viewport.
		LevelLossWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultLevelLossWidget);
		LevelLossWidget->AddToViewport();
		//Allow mouse to be used to select the button on HUD
		Controller->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		Controller->SetInputMode(InputMode);
	}
	else
	{
		//Warning message
		UE_LOG(LogTemp, Warning, TEXT("No Default Level Loss Widget Selected!!!"));
	}
}


