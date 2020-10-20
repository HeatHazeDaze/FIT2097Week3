// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "BinarySwitch.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBinaryCode1Event, bool, code);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBinaryCode2Event, bool, code);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBinaryCode3Event, bool, code);

UCLASS()
class FIT2097WEEK3_API ABinarySwitch : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABinarySwitch();

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
		void Interact();
	virtual void Interact_Implementation() override;
	//Called when interacted on, displays information
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void DisplayInformation();
	virtual void DisplayInformation_Implementation() override;

	// Editable variable to be passed to door, to be used as check against preset code combination
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		bool currentCode = true;

	//Variable to identify which binary switch this would be, used to check which event dispatch to use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		int binaryID;
	
	//Event Dispatcher to call on the corresponding binary code function in listeners.
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FBinaryCode1Event BinaryCode1;

	//Event Dispatcher to call on the corresponding binary code function in listeners.
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FBinaryCode2Event BinaryCode2;

	//Event Dispatcher to call on the corresponding binary code function in listeners.
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FBinaryCode3Event BinaryCode3;

	//Dynamic Material setup
	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;
};
