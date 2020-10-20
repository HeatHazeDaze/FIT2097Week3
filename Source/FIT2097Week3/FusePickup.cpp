// Fill out your copyright notice in the Description page of Project Settings.


#include "FusePickup.h"

// Sets default values
AFusePickup::AFusePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//C++ setup with basemesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AFusePickup::BeginPlay()
{
	Super::BeginPlay();

	//Dynamic material setup, using BaseMesh because GetMesh() doesn't exist without skeletons
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);

	//Sets the material to blue color to signify fuse, sets to pulse less frequently than a healthpickup for differentiation
	if (matInstance)
	{
		matInstance->SetVectorParameterValue("Color", FLinearColor(0, 0, 1));
		matInstance->SetScalarParameterValue("Intensity", 50.f);
		matInstance->SetScalarParameterValue("Frequency", 0.5f);
	}
}

// Called every frame
void AFusePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Interact Implementation, broadcasts Give Key event to Vertical Door to allow player to open
void AFusePickup::Interact_Implementation()
{
	GiveFuse.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Picked Up Fuse"));
	this->Destroy();
}

//Displays information when interacted on
void AFusePickup::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Fuse"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Walk over to collect a fuse!"));
}
