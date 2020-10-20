// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalDoor.h"

// Sets default values
AVerticalDoor::AVerticalDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AVerticalDoor::BeginPlay()
{
	Super::BeginPlay();

	if(KeyPickup != nullptr)
	{
		KeyPickup->KeyGiveKey.AddDynamic(this, &AVerticalDoor::GiveKey);
	}

	//Dynamic material setup, using BaseMesh because GetMesh() doesn't exist without skeletons
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);
	
}

// Called every frame
void AVerticalDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//If both conditions are met, the mesh moves upwards, simulating a door opening
	if(open && hasKey)
	{
		FVector target = BaseMesh->GetComponentLocation();
		target.Z = baseZ + 330.0f;
		BaseMesh->SetRelativeLocation(FMath::Lerp(BaseMesh->GetComponentLocation(), target, 0.05f));

		//Sets the material to green to signify opened
		if(matInstance)
		{
			matInstance->SetVectorParameterValue("Color", FLinearColor(0, 1, 0));
			//matInstance->SetScalarParameterValue("Emission", 50.f);
		}
	}
	//Else, simualtes a closing door by moving downwards
	else
	{
		FVector target = BaseMesh->GetComponentLocation();
		target.Z = baseZ;
		BaseMesh->SetRelativeLocation(FMath::Lerp(BaseMesh->GetComponentLocation(), target, 0.05f));

		//Sets material to red to signify closed
		if (matInstance)
		{
			matInstance->SetVectorParameterValue("Color", FLinearColor(1, 0, 0));
			//matInstance->SetScalarParameterValue("Emission", 1.0f);
		}
	}
}

//Interaction implementation, called when interacted upon, sets open bool to the inverse value
void AVerticalDoor::Interact_Implementation()
{
	open = !open;

}

//Displays information when interacted upon
void AVerticalDoor::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Vertical Door"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Press mouse 1 to open"));
}

//Give Key function, tied to event listener for GiveKey event broadcast from key pickup
void AVerticalDoor::GiveKey()
{
	hasKey = !hasKey;

	//Sets material to glow to signify interactable now
	if (matInstance)
	{
		matInstance->SetScalarParameterValue("Emission", 50.0f);
	}
}

