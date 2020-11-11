// Fill out your copyright notice in the Description page of Project Settings.


#include "FuseSwitch.h"

// Sets default values
AFuseSwitch::AFuseSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	fuseActive = false;
}

// Called when the game starts or when spawned
void AFuseSwitch::BeginPlay()
{
	Super::BeginPlay();

	//binds event if fusebox pointer is not null, calls Place fuse function
	if (FuseBox != nullptr)
	{
		FuseBox->PlaceFuse.AddDynamic(this, &AFuseSwitch::PlaceFuse);
	}
	
	//Dynamic material setup, using BaseMesh because GetMesh() doesn't exist without skeletons
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void AFuseSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

//Interaction implementation, called when interacted upon, sets open bool to the inverse value
void AFuseSwitch::Interact_Implementation()
{
	if(fuseActive == true)
	{
		FuseOpen.Broadcast();
		//sets material to green to signify switch is activated
		if (matInstance)
		{
			matInstance->SetVectorParameterValue("Color", FLinearColor(0, 1, 0));
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Door has been opened"));
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Make sure fuse box is active"));
	}

}

//Displays information when interacted upon
void AFuseSwitch::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Fuse connected switch"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Place fuse and press mouse 1 to open door"));
}

//Place Fuse function, tied to event listener for PlaceFuse event broadcast from fusebox
void AFuseSwitch::PlaceFuse()
{
	fuseActive = true;

	//Sets material to blue to signify interactable now
	if (matInstance)
	{
		matInstance->SetVectorParameterValue("Color", FLinearColor(0, 0, 1));
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Switch is now active"));
}


