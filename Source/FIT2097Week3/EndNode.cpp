// Fill out your copyright notice in the Description page of Project Settings.


#include "EndNode.h"

// Sets default values
AEndNode::AEndNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//C++ setup with basemesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;
	
}

// Called when the game starts or when spawned
void AEndNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEndNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Displays information when interacted
void AEndNode::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("End Node"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Walk over me to win the game"));
}