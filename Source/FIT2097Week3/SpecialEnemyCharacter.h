// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "FIT2097Week3Character.h"
#include "Components/BoxComponent.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/Character.h"
#include "SpecialEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKillPlayerEvent);

UCLASS()
class FIT2097WEEK3_API ASpecialEnemyCharacter : public ACharacter, public IInteractable 
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpecialEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Interact Implementation here, broadcasts our event dispatcher
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(); //Prototype declaration
	virtual void Interact_Implementation() override; // The implementation of our interact method

	//Displays information when interacted upon
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void DisplayInformation();
	virtual void DisplayInformation_Implementation() override;

	//Post processing components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UBoxComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UPostProcessComponent* PostProcessComp;
	
	//blueprint exposed variable to set the default speed of enemies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MovementSpeed;

	/** Footstep concrete sounds*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundCue* SpecialEnemyFootstepConcreteSoundCue;

	/** Footstep carpet sounds*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundCue* SpecialEnemyFootstepCarpetSoundCue;


	//Function bound to animBP footstep event, is called when anim notify event occurs
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Gameplay)
		void SpecialEnemyFootstepEvent();

	//bool to check if corresponding enemy character is stunned
	bool isStunned;


	//stun function, sets movement speed to 0
	void EnemyStun();

	//reset stun function
	void ResetStun();

	FTimerHandle StunTimer;

	//Event Dispatcher to call on the take damage function in listeners.
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FKillPlayerEvent KillPlayerEvent;

	//Dynamic Material setup
	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;

};
