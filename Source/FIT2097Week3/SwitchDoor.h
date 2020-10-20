// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "SwitchDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRemoteOpenEvent);

UCLASS()
class FIT2097WEEK3_API ASwitchDoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitchDoor();

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

	//Event Dispatcher to call on the remote open function in listeners.
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FRemoteOpenEvent RemoteOpen;

	//Dynamic Material setup
	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;
};
