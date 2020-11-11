// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 5, 0.f);
	//Changed to now be blueprint settable.
	GetCharacterMovement()->MaxWalkSpeed = 540.0f;

	isStunned = false;
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
	//If enemy character is not stunned, broadcast damage event.
	if(!isStunned)
	{
		DamageEvent.Broadcast();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Taking Damage!"));
	}
}

//Displays information when interacted on
void AEnemyCharacter::DisplayInformation_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Character"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Watch out, they hurt you if they get too close"));
}

void AEnemyCharacter::EnemyFootstepEvent_Implementation()
{

	// What Actors do we want our trace to Ignore?
	//TArray<AActor*> ActorsToIgnore;


	//ActorsToIgnore.Add(this);
	const FVector Offset(0.0f, 0.0f, 100.0f);
	FHitResult HitOut;
	ECollisionChannel CollisionChannel = ECC_Pawn;
	bool ReturnPhysMat = true;

	// Set up our TraceParams object
	FCollisionQueryParams TraceParams;

	// Should we simple or complex collision?
	TraceParams.bTraceComplex = true;

	// We need Physics materials 
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

	//Ignore the player character - so you don't hit yourself!
	TraceParams.AddIgnoredActor(this);



	// When we're debugging it is really useful to see where our trace is in the world
	// We can use World->DebugDrawTraceTag to tell Unreal to draw debug lines for our trace
	// (remove these lines to remove the debug - or better create a debug switch!)
	const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	TraceParams.TraceTag = TraceTag;


	// Force clear the HitData which contains our results
	HitOut = FHitResult(ForceInit);

	// Perform our trace
	bool bIsHit = GetWorld()->LineTraceSingleByChannel
	(
		HitOut,		//result
		GetActorLocation(),	//start
		GetActorLocation() - Offset, //end
		CollisionChannel, //collision channel
		TraceParams
	);

	//Checks if a hit is detected
	if (bIsHit)
	{
		//Stores a reference to object's material
		UPhysicalMaterial* PhysicsMtl = HitOut.PhysMaterial.Get();

		//Checks if surface is default, if yes, play concrete sound cue
		if (PhysicsMtl->SurfaceType == SurfaceType_Default)
		{
			// try and play the sound if specified
			if (EnemyFootstepConcreteSoundCue != NULL)
			{
				//EnemyFootstepConcreteSoundCue->VolumeMultiplier = 0.75f;
				UGameplayStatics::PlaySoundAtLocation(this, EnemyFootstepConcreteSoundCue, GetActorLocation());
			}
		}
		//Checks if surface is default, if yes, play concrete sound cue
		if (PhysicsMtl->SurfaceType == SurfaceType1)
		{
			// try and play the sound if specified
			if (EnemyFootstepConcreteSoundCue != NULL)
			{
				//EnemyFootstepConcreteSoundCue->VolumeMultiplier = 0.75f;
				UGameplayStatics::PlaySoundAtLocation(this, EnemyFootstepConcreteSoundCue, GetActorLocation());
			}
		}
		//Checks if surface is default, if yes, play carpet sound cue
		if (PhysicsMtl->SurfaceType == SurfaceType2)
		{
			// try and play the sound if specified
			if (EnemyFootstepCarpetSoundCue != NULL)
			{
				//EnemyFootstepCarpetSoundCue->VolumeMultiplier = 0.75f;
				UGameplayStatics::PlaySoundAtLocation(this, EnemyFootstepCarpetSoundCue, GetActorLocation());
			}
		}

	}
}

void AEnemyCharacter::EnemyStun()
{
	//GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	GetCharacterMovement()->Deactivate();
	//isStunned = true;
	GetWorldTimerManager().SetTimer(StunTimer, this, &AEnemyCharacter::ResetStun, 5.f, false);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EnemyStunned"));

	isStunned = true;
}

void AEnemyCharacter::ResetStun()
{
	//GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	GetCharacterMovement()->Activate();

	isStunned = false;
}
