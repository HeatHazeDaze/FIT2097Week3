// Fill out your copyright notice in the Description page of Project Settings.


#include "BinaryDoor.h"

// Sets default values
ABinaryDoor::ABinaryDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	
}

// Called when the game starts or when spawned
void ABinaryDoor::BeginPlay()
{
	Super::BeginPlay();
	
	//Dynamic material setup, using BaseMesh because GetMesh() doesn't exist without skeletons
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void ABinaryDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//binds event if Binary switch pointers are not null, calls corresponding binary code function
	if (BinarySwitch1 != nullptr)
	{
		BinarySwitch1->BinaryCode1.AddDynamic(this, &ABinaryDoor::BinaryCode1);
	}

	if (BinarySwitch2 != nullptr)
	{
		BinarySwitch2->BinaryCode2.AddDynamic(this, &ABinaryDoor::BinaryCode2);
	}
	
	if (BinarySwitch1 != nullptr)
	{
		BinarySwitch1->BinaryCode1.AddDynamic(this, &ABinaryDoor::BinaryCode1);
	}

	
}

//Displays information when interacted using debug message
void ABinaryDoor::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Binary Door"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Input the right binary code to open the door"));
}

//BinaryCode functions, sets tempCode to whatever is passed through the event dispatch, then checks the current combination
void ABinaryDoor::BinaryCode1(bool code)
{
	tempCode1 = code;
	CheckCombination();
}

void ABinaryDoor::BinaryCode2(bool code)
{
	tempCode2 = code;
	CheckCombination();
}

void ABinaryDoor::BinaryCode3(bool code)
{
	tempCode3 = code;
	CheckCombination();
}

//Code that checks combination

void ABinaryDoor::CheckCombination()
{
	if(tempCode1 == targetCode1 && tempCode2 == targetCode2 && tempCode3 == targetCode3)
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
}
