// Fill out your copyright notice in the Description page of Project Settings.


#include "FuseDoor.h"

// Sets default values
AFuseDoor::AFuseDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AFuseDoor::BeginPlay()
{
	Super::BeginPlay();

	//Dynamic material setup, using BaseMesh because GetMesh() doesn't exist without skeletons
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void AFuseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//binds event if DoorSwitch pointer is not null, calls RemoteOpen function
	if (FuseSwitch != nullptr)
	{
		FuseSwitch->FuseOpen.AddDynamic(this, &AFuseDoor::FuseOpen);
	}
}

//Displays information when interacted using debug message
void AFuseDoor::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Fuse Door"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Interact with fuse switch to open"));
}

//Remote Open function that is called when DoorSwitch broadcasts
void AFuseDoor::FuseOpen()
{
	//Code that moves mesh upwards, simulates a door opening
	FVector target = BaseMesh->GetComponentLocation();
	target.Z = baseZ + 330.0f;
	BaseMesh->SetRelativeLocation(FMath::Lerp(BaseMesh->GetComponentLocation(), target, 0.05f));


	//Sets material to green and glow to signify opened
	if (matInstance)
	{
		matInstance->SetVectorParameterValue("Color", FLinearColor(0, 1, 0));
		matInstance->SetScalarParameterValue("Emission", 50.0f);
	}
}