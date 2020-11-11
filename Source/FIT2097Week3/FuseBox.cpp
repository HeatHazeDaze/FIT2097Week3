// Fill out your copyright notice in the Description page of Project Settings.


#include "FuseBox.h"

// Sets default values
AFuseBox::AFuseBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AFuseBox::BeginPlay()
{
	Super::BeginPlay();


	//Dynamic material setup, using BaseMesh because GetMesh() doesn't exist without skeletons
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);

	//sets material to red to signify inactive
	if (matInstance)
	{
		matInstance->SetVectorParameterValue("Color", FLinearColor(1, 0, 0));
	}
}

// Called every frame
void AFuseBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Interaction implementation, called when interacted upon, sets open bool to the inverse value
void AFuseBox::Interact_Implementation()
{
	if (PlayerRef->hasFuse == true)
	{
		PlaceFuse.Broadcast();
		//sets material to blue to signify fuse placed
		if (matInstance)
		{
			matInstance->SetVectorParameterValue("Color", FLinearColor(0, 0, 1));
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Fuse bos is now active"));

		PlayerRef->hasFuse = false;
	}

}

//Displays information when interacted upon
void AFuseBox::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Fuse Box"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Steal the fuse and press mouse 1 to fix"));
}