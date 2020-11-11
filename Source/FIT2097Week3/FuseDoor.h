// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "FuseSwitch.h"
#include "GameFramework/Actor.h"
#include "FuseDoor.generated.h"

UCLASS()
class FIT2097WEEK3_API AFuseDoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFuseDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	float baseZ = 330.0f;

	// A reference to a SwitchDoor object, can be set in the editor to signify which door and switch will work together.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		AFuseSwitch* FuseSwitch;

	bool isActivated;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Displays information when object is interacted on
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void DisplayInformation();
	virtual void DisplayInformation_Implementation() override;
	//UFUNCTION to be called when FuseOpen event is broadcasted
	UFUNCTION()
		void FuseOpen();

	//Dynamic Material setup
	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;
};
