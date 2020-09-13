// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "KeyPickup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGiveKeyEvent);

UCLASS()
class FIT2097WEEK3_API AKeyPickup : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyPickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Interact Implementation here, broadcasts our event dispatcher
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(); //Prototype declaration
	virtual void Interact_Implementation() override; // The implementation of our interact method

	//Displays information when interacted upon
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void DisplayInformation();
	virtual void DisplayInformation_Implementation() override;
	
	//Event Dispatcher to call on the give key function in listeners.
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FGiveKeyEvent KeyGiveKey;

	
};
