// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyPickup.h"

// Sets default values
AKeyPickup::AKeyPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//C++ setup with basemesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AKeyPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeyPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	
}

//Interact Implementation, broadcasts Give Key event to Vertical Door to allow player to open
void AKeyPickup::Interact_Implementation()
{
	KeyGiveKey.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Picked Up Key"));
	this->Destroy();
}

//Displays information when interacted on
void AKeyPickup::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Key"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Walk over to be able to unlock door!"));
}
