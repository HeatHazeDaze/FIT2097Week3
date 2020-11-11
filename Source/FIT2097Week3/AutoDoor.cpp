// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoDoor.h"

// Sets default values
AAutoDoor::AAutoDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	isActivated = false;
}

// Called when the game starts or when spawned
void AAutoDoor::BeginPlay()
{
	Super::BeginPlay();

	//binds event if DoorSwitch pointer is not null, calls RemoteOpen function
	if (DoorSwitch != nullptr)
	{
		DoorSwitch->RemoteOpen.AddDynamic(this, &AAutoDoor::RemoteOpen);
	}
	
	//Dynamic material setup, using BaseMesh because GetMesh() doesn't exist without skeletons
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void AAutoDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isActivated)
	{
		FVector target = BaseMesh->GetComponentLocation();
		target.Z = baseZ + 330.0f;
		BaseMesh->SetRelativeLocation(FMath::Lerp(BaseMesh->GetComponentLocation(), target, 0.05f));

		//Sets the material to green to signify opened
		if (matInstance)
		{
			matInstance->SetVectorParameterValue("Color", FLinearColor(0, 1, 0));
			//matInstance->SetScalarParameterValue("Emission", 50.f);
		}
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



	//Sets material to green and glow to signify opened
	if (matInstance)
	{
		matInstance->SetVectorParameterValue("Color", FLinearColor(0, 1, 0));
		matInstance->SetScalarParameterValue("Emission", 50.0f);
	}

	//Code that moves mesh upwards, simulates a door opening
	isActivated = true;
}