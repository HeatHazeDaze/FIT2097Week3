// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

// Sets default values
AHealthPickup::AHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//C++ setup with basemesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();

	//Dynamic material setup, using BaseMesh because GetMesh() doesn't exist without skeletons
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);


	//Sets the material to red to signify health pickup and pulse faster, almost blinking to better distinguish between pickups
	if (matInstance)
	{
		matInstance->SetVectorParameterValue("Color", FLinearColor(1, 0, 0));
		matInstance->SetScalarParameterValue("Intensity", 50.f);
		matInstance->SetScalarParameterValue("Frequency", 1.0f);
	}
}

// Called every frame
void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	
}
//Called when colliding with another actor (set to player)
void AHealthPickup::Interact_Implementation()
{
	//Broadcasts the heal event to all listeners
	HealthPickupHeal.Broadcast(health);
	
	//Debug message
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Picked Up Health"));
	//BaseMesh->SetWorldLocation(GetActorLocation() + FVector(0, 0, 20.f));
	//Destroys itself so player cannot get more heals from one pickup object.
	this->Destroy();
}

//Displays Information when interacted upon
void AHealthPickup::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Health Pickup"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Walk over to pickup health"));
}