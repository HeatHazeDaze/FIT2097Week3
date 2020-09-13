// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "KeyPickup.h"
#include "GameFramework/Actor.h"
#include "VerticalDoor.generated.h"

UCLASS()
class FIT2097WEEK3_API AVerticalDoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVerticalDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;
	//determines whether door is opened or closed
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		bool open = false;
	//Float value used to move
	float baseZ = 330.0f;
	//Key Pickup reference, exposed to blueprints so we can pair doors and keys, used to bind the Give key function to the event listener
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	AKeyPickup* KeyPickup;
	//Bool value hasKey, determines if a door can be opened or closed
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true") ,Category = "Interact")
		bool hasKey = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Interaction implementation, called when interacted upon, sets open bool to the inverse value
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact();
	virtual void Interact_Implementation() override;
	//Displays information when interacted upon
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void DisplayInformation();
	virtual void DisplayInformation_Implementation() override;
	//Give Key function, tied to event listener for GiveKey event broadcast from key pickup
	UFUNCTION()
		void GiveKey();
};
