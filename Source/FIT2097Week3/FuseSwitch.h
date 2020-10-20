// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "FuseBox.h"
#include "GameFramework/Actor.h"
#include "FuseSwitch.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuseOpenEvent);

UCLASS()
class FIT2097WEEK3_API AFuseSwitch : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFuseSwitch();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Variable to check if fuse has been placed in fuse box, will be UPROPERTY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool fuseActive;

	// A reference to a FuseBox object, can be set in the editor to signify which fusebox and switch will work together.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		AFuseBox* FuseBox;
	
	//Interact Implementation here, broadcasts our event dispatcher
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact();
	virtual void Interact_Implementation() override;
	//Called when interacted on, displays information
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void DisplayInformation();
	virtual void DisplayInformation_Implementation() override;

	//UFUNCTION to be called when place fuse is broadcasted
	UFUNCTION()
		void PlaceFuse();

	//Event Dispatcher to call on the remote open function in listeners.
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FFuseOpenEvent FuseOpen;

	//Dynamic Material setup
	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;
};
