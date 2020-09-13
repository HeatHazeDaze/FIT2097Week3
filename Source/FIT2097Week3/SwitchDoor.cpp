// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchDoor.h"

// Sets default values
ASwitchDoor::ASwitchDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void ASwitchDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwitchDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Called when interacted on, broadcasts event to all listeners
void ASwitchDoor::Interact_Implementation()
{
	//Broadcasts the Remote Open event to all listeners
	RemoteOpen.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Door has been opened"));
}

//Called when interacted on, displays information
void ASwitchDoor::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Switch"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Press Mouse 1 to unlock the AutoDoor!"));
}
