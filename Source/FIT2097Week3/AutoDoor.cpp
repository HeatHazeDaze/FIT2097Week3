// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoDoor.h"

// Sets default values
AAutoDoor::AAutoDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AAutoDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAutoDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//binds event if DoorSwitch pointer is not null, calls RemoteOpen function
	if (DoorSwitch != nullptr)
	{
		DoorSwitch->RemoteOpen.AddDynamic(this, &AAutoDoor::RemoteOpen);
	}
}

//Displays information when interacted using debug message
void AAutoDoor::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Auto Door"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Interact with switch to open"));
}

//Remote Open function that is called when DoorSwitch broadcasts
void AAutoDoor::RemoteOpen()
{
	//Code that moves mesh upwards, simulates a door opening
	FVector target = BaseMesh->GetComponentLocation();
	target.Z = baseZ + 330.0f;
	BaseMesh->SetRelativeLocation(FMath::Lerp(BaseMesh->GetComponentLocation(), target, 0.05f));
	
}