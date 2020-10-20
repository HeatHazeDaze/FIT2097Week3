// Fill out your copyright notice in the Description page of Project Settings.


#include "BinarySwitch.h"

// Sets default values
ABinarySwitch::ABinarySwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	//initialize params
	binaryID = 0;
	currentCode = true;

}

// Called when the game starts or when spawned
void ABinarySwitch::BeginPlay()
{
	Super::BeginPlay();

	//Dynamic material setup, using BaseMesh because GetMesh() doesn't exist without skeletons
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void ABinarySwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(currentCode ==true)
	{
		//Sets material to green to show switch is active
		if (matInstance)
		{
			matInstance->SetVectorParameterValue("Color", FLinearColor(0, 1, 0));
		}
	}
	else
	{
		if (matInstance)
		{
			matInstance->SetVectorParameterValue("Color", FLinearColor(1, 0, 0));
		}
	}
}


//Called when interacted on, broadcasts event to all listeners
void ABinarySwitch::Interact_Implementation()
{
	currentCode = !currentCode;
	
	switch(binaryID)
	{
	case 1:
		BinaryCode1.Broadcast(currentCode);
		break;
	case 2:
		BinaryCode2.Broadcast(currentCode);
		break;
	case 3:
		BinaryCode3.Broadcast(currentCode);
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Please set up this switch"));
		break;
	}
}

//Called when interacted on, displays information
void ABinarySwitch::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Binary Switch"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Press Mouse 1 to enter in the code combination!"));
}