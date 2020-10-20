// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "FIT2097Week3Character.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "FuseBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlaceFuseEvent);

UCLASS()
class FIT2097WEEK3_API AFuseBox : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFuseBox();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;
	//Player reference, exposed to blueprints so we direct link between the two objects,
	//used to check if player has fuse to trigger broadcast
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		AFIT2097Week3Character* PlayerRef;
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
	
	//Event Dispatcher to call on the placed fuse function in listeners.
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FPlaceFuseEvent PlaceFuse;

	//Dynamic Material setup
	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;
};
