// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadePickup.h"

// Sets default values
AGrenadePickup::AGrenadePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//C++ setup with basemesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;

}

// Called when the game starts or when spawned
void AGrenadePickup::BeginPlay()
{
	Super::BeginPlay();

	//Dynamic material setup, using BaseMesh because GetMesh() doesn't exist without skeletons
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);

	//Sets the material to green color to signify key, sets to pulse less frequently than a healthpickup for differentiation
	if (matInstance)
	{
		matInstance->SetVectorParameterValue("Color", FLinearColor(1, 0, 1));
		matInstance->SetScalarParameterValue("Intensity", 50.f);
		matInstance->SetScalarParameterValue("Frequency", 1.0f);
	}
}

// Called every frame
void AGrenadePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenadePickup::Interact_Implementation()
{
	GiveGrenade.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Picked Up Grenade"));
	this->Destroy();
}

void AGrenadePickup::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grenade"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Walk over to collect a grenade"));
}

