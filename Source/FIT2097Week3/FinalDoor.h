// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "BinarySwitch.h"
#include "FuseBox.h"
#include "GameFramework/Actor.h"
#include "FinalDoor.generated.h"

UCLASS()
class FIT2097WEEK3_API AFinalDoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFinalDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	float baseZ = 330.0f;

	//A reference to a FuseBox object, can be set in the editor to signify which fuse box is interactable with this door
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		AFuseBox* FuseBox;
	
	// A reference to a BinarySwitch object, can be set in the editor to signify which switch is 1, 2 or 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		ABinarySwitch* BinarySwitch1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		ABinarySwitch* BinarySwitch2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		ABinarySwitch* BinarySwitch3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		ABinarySwitch* BinarySwitch4;

	//Target combination variable, can be set in the editor as to what our code combination will be
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		bool targetCode1 = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		bool targetCode2 = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		bool targetCode3 = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		bool targetCode4 = true;

	//Containers to store code messages from our switches
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		bool tempCode1 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		bool tempCode2 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		bool tempCode3 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		bool tempCode4 = false;

	//Open boolean condition
	bool isActivated;
	//Checks if fuse has been delivered to new fusebox
	bool isPowered;
	
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

	//UFUNCTION to be called when PlaceFuse is broadcasted
	UFUNCTION()
		void SetPowered();
	//List of UFUNCTION to be called based on which BinaryCode was triggered.
	UFUNCTION()
		void BinaryCode1(bool code);
	UFUNCTION()
		void BinaryCode2(bool code);
	UFUNCTION()
		void BinaryCode3(bool code);
	UFUNCTION()
		void BinaryCode4(bool code);

	//Check code combination function
	void CheckCombination();

	//Dynamic Material setup
	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;
};
