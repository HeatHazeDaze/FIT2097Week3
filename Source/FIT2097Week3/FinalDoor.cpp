// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalDoor.h"

// Sets default values
AFinalDoor::AFinalDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	isActivated = false;

	isPowered = false;
}

// Called when the game starts or when spawned
void AFinalDoor::BeginPlay()
{
	Super::BeginPlay();

	//binds event if fusebox is not null, calls corresponding setPowered function
	if(FuseBox != nullptr)
	{
		FuseBox->PlaceFuse.AddDynamic(this, &AFinalDoor::SetPowered);
	}
	//binds event if Binary switch pointers are not null, calls corresponding binary code function
	if (BinarySwitch1 != nullptr)
	{
		BinarySwitch1->BinaryCode1.AddDynamic(this, &AFinalDoor::BinaryCode1);
	}

	if (BinarySwitch2 != nullptr)
	{
		BinarySwitch2->BinaryCode2.AddDynamic(this, &AFinalDoor::BinaryCode2);
	}

	if (BinarySwitch3 != nullptr)
	{
		BinarySwitch3->BinaryCode3.AddDynamic(this, &AFinalDoor::BinaryCode3);
	}

	if (BinarySwitch4 != nullptr)
	{
		BinarySwitch4->BinaryCode4.AddDynamic(this, &AFinalDoor::BinaryCode4);
	}

	//Dynamic material setup, using BaseMesh because GetMesh() doesn't exist without skeletons
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void AFinalDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//opens door
	if (isActivated)
	{
		//Code that moves mesh upwards, simulates a door opening
		FVector target = BaseMesh->GetComponentLocation();
		target.Z = baseZ + 330.0f;
		BaseMesh->SetRelativeLocation(FMath::Lerp(BaseMesh->GetComponentLocation(), target, 0.05f));
	}
}


//Displays information when interacted using debug message
void AFinalDoor::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Final Door"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Place the fuse and turn on all the switches to open!"));
}


//Sets isPowered to true once the event is broadcasted
void AFinalDoor::SetPowered()
{
	isPowered = true;

	//Sets material to green and glow to signify opened
	if (matInstance)
	{
		matInstance->SetVectorParameterValue("Color", FLinearColor(0, 0, 1));
		matInstance->SetScalarParameterValue("Emission", 50.0f);
	}

	CheckCombination();
}


//BinaryCode functions, sets tempCode to whatever is passed through the event dispatch, then checks the current combination
void AFinalDoor::BinaryCode1(bool code)
{
	tempCode1 = code;
	CheckCombination();
}

void AFinalDoor::BinaryCode2(bool code)
{
	tempCode2 = code;
	CheckCombination();
}

void AFinalDoor::BinaryCode3(bool code)
{
	tempCode3 = code;
	CheckCombination();
}

void AFinalDoor::BinaryCode4(bool code)
{
	tempCode4 = code;
	CheckCombination();
}

//Code that checks combination

void AFinalDoor::CheckCombination()
{
	if (tempCode1 == targetCode1 && tempCode2 == targetCode2 && tempCode3 == targetCode3 && tempCode4 == targetCode4 && isPowered)
	{

		isActivated = true;

		//Sets material to green and glow to signify opened
		if (matInstance)
		{
			matInstance->SetVectorParameterValue("Color", FLinearColor(0, 1, 0));
			matInstance->SetScalarParameterValue("Emission", 50.0f);
		}
	}
}
