// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 5, 0.f);
	//Slows down max walk speed to allow player to run from enemy, set to 200.f to test if working as it would be drastic and apparent
	GetCharacterMovement()->MaxWalkSpeed = 540.f;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//Interact Implementation, broadcasts take damage event to player character
void AEnemyCharacter::Interact_Implementation()
{
	DamageEvent.Broadcast();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Taking Damage!"));
}

//Displays information when interacted on
void AEnemyCharacter::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Character"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Watch out, they hurt you if they get too close"));
}