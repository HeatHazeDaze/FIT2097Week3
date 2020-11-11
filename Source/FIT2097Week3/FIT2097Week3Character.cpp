// Copyright Epic Games, Inc. All Rights Reserved.

#include "FIT2097Week3Character.h"

#include "EndNode.h"
#include "EnemyCharacter.h"
#include "FIT2097Week3Projectile.h"
#include "FusePickup.h"
#include "Animation/AnimInstance.h"
#include "Engine/PostProcessVolume.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HealthPickup.h"
#include "GrenadePickup.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Interactable.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "SpecialEnemyCharacter.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFIT2097Week3Character

AFIT2097Week3Character::AFIT2097Week3Character()
{
	//Initializes Player Health at 50
	PlayerHealth = 50;

	//Initializes hasFuse to false
	hasFuse = false;

	//Initialize isImmune to false
	isImmune = false;

	//Initialize Grenade value to 5
	GrenadeCount = 5;
	
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.0f, 96.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFIT2097Week3Character::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AFIT2097Week3Character::OnOverlapEnd);
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AFIT2097Week3Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}

	//To allow resuming
	PrimaryActorTick.bTickEvenWhenPaused = true;

	//Sets damage volume to not unbound
	if (DamagePostProcess != nullptr)
	{
		DamagePostProcess->bUnbound = 0;
	}

}

//////////////////////////////////////////////////////////////////////////
// Input

void AFIT2097Week3Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFIT2097Week3Character::OnFire);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFIT2097Week3Character::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFIT2097Week3Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFIT2097Week3Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFIT2097Week3Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFIT2097Week3Character::LookUpAtRate);

	// Bind Pause event
	FInputActionBinding& toggle = InputComponent->BindAction("Pause", IE_Pressed, this, &AFIT2097Week3Character::OnPause);
	toggle.bExecuteWhenPaused = true; //EVEN THOUGH THE GAME IS PAUSED, CATCH THIS EVENT !!!!

	// Bind throw grenade event
	PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &AFIT2097Week3Character::OnGrenade);
}

void AFIT2097Week3Character::OnFire()
{
	CallMyTrace();
	
	/*
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AFIT2097Week3Projectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AFIT2097Week3Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	*/
}

void AFIT2097Week3Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFIT2097Week3Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AFIT2097Week3Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AFIT2097Week3Character::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AFIT2097Week3Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFIT2097Week3Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFIT2097Week3Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFIT2097Week3Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AFIT2097Week3Character::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFIT2097Week3Character::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFIT2097Week3Character::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AFIT2097Week3Character::TouchUpdate);
		return true;
	}
	
	return false;
}

void AFIT2097Week3Character::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{

		//If OtherActor is a Health Pickup, binds the HealPlayer function to the heal event dispatch
		AHealthPickup* pickup = Cast<AHealthPickup>(OtherActor);
		if (pickup)
		{
			pickup->HealthPickupHeal.AddDynamic(this, &AFIT2097Week3Character::HealPlayer);
		}

		//If OtherActor is a Fuse Pickup, binds the GivePlayerFuse function to the give player fuse event dispatch
		AFusePickup* fuse = Cast<AFusePickup>(OtherActor);
		if (fuse)
		{
			fuse->GiveFuse.AddDynamic(this, &AFIT2097Week3Character::GivePlayerFuse);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player now has fuse"));
		}

		//If OtherActor is an enemy, binds the take damage function to the takedamage event dispatch
		AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OtherActor);
		if (enemy)
		{
			enemy->DamageEvent.AddDynamic(this, &AFIT2097Week3Character::TakeDamage);
		}

		//If OtherActor is a Grenade Pickup, binds the GiveGrenade function to the give grenade dispatch
		AGrenadePickup* grenade = Cast<AGrenadePickup>(OtherActor);
		if (grenade)
		{
			grenade->GiveGrenade.AddDynamic(this, &AFIT2097Week3Character::GiveGrenade);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grenade Collected"));
		}

		//If OtherActor is a Special Enemy, binds the kill player function to the kill player event dispatch
		ASpecialEnemyCharacter* special = Cast<ASpecialEnemyCharacter>(OtherActor);
		if (special)
		{
			special->KillPlayerEvent.AddDynamic(this, &AFIT2097Week3Character::KillPlayer);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player was killed by special enemy"));
		}
		//Calls execute interact from any objects implementing the interactable interface
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
		
		// Implements interface
		if (OtherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_Interact(OtherActor);
			IInteractable::Execute_DisplayInformation(OtherActor);
		}

		//If OtherActore is an End Node, broadcasts Player Win Event Dispatch
		AEndNode* endnode = Cast<AEndNode>(OtherActor);
		if (endnode)
		{
			PlayerWin.Broadcast();
		}


	}


}

void AFIT2097Week3Character::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));

	}



}



//***************************************************************************************************
//** Trace functions - used to detect items we are looking at in the world
//***************************************************************************************************
//***************************************************************************************************

//***************************************************************************************************
//** Trace() - called by our CallMyTrace() function which sets up our parameters and passes them through
//***************************************************************************************************

bool AFIT2097Week3Character::Trace(
	UWorld* World,
	TArray<AActor*>& ActorsToIgnore,
	const FVector& Start,
	const FVector& End,
	FHitResult& HitOut,
	ECollisionChannel CollisionChannel = ECC_Pawn,
	bool ReturnPhysMat = false
) {

	// The World parameter refers to our game world (map/level) 
	// If there is no World, abort
	if (!World)
	{
		return false;
	}

	// Set up our TraceParams object
	FCollisionQueryParams TraceParams(FName(TEXT("My Trace")), true, ActorsToIgnore[0]);

	// Should we simple or complex collision?
	TraceParams.bTraceComplex = true;

	// We don't need Physics materials 
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

	// Add our ActorsToIgnore
	TraceParams.AddIgnoredActors(ActorsToIgnore);

	// When we're debugging it is really useful to see where our trace is in the world
	// We can use World->DebugDrawTraceTag to tell Unreal to draw debug lines for our trace
	// (remove these lines to remove the debug - or better create a debug switch!)
	const FName TraceTag("MyTraceTag");
	World->DebugDrawTraceTag = TraceTag;
	TraceParams.TraceTag = TraceTag;


	// Force clear the HitData which contains our results
	HitOut = FHitResult(ForceInit);

	// Perform our trace
	World->LineTraceSingleByChannel
	(
		HitOut,		//result
		Start,	//start
		End, //end
		CollisionChannel, //collision channel
		TraceParams
	);

	// If we hit an actor, return true
	return (HitOut.GetActor() != NULL);
}

//***************************************************************************************************
//** CallMyTrace() - sets up our parameters and then calls our Trace() function
//***************************************************************************************************

void AFIT2097Week3Character::CallMyTrace()
{
	// Get the location of the camera (where we are looking from) and the direction we are looking in
	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();

	// How for in front of our character do we want our trace to extend?
	// ForwardVector is a unit vector, so we multiply by the desired distance
	const FVector End = Start + ForwardVector * 1256;

	// Force clear the HitData which contains our results
	FHitResult HitData(ForceInit);

	// What Actors do we want our trace to Ignore?
	TArray<AActor*> ActorsToIgnore;

	//Ignore the player character - so you don't hit yourself!
	ActorsToIgnore.Add(this);

	// Call our Trace() function with the paramaters we have set up
	// If it Hits anything
	if (Trace(GetWorld(), ActorsToIgnore, Start, End, HitData, ECC_Visibility, false))
	{
		// Process our HitData
		if (HitData.GetActor())
		{

			UE_LOG(LogClass, Warning, TEXT("This a testing statement. %s"), *HitData.GetActor()->GetName());
			ProcessTraceHit(HitData);

		}
		else
		{
			// The trace did not return an Actor
			// An error has occurred
			// Record a message in the error log
		}
	}
	else
	{
		// We did not hit an Actor
		//ClearPickupInfo();

	}

}

//***************************************************************************************************
//** ProcessTraceHit() - process our Trace Hit result
//***************************************************************************************************

void AFIT2097Week3Character::ProcessTraceHit(FHitResult& HitOut)
{
	if (HitOut.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable::Execute_Interact(HitOut.GetActor());
		IInteractable::Execute_DisplayInformation(HitOut.GetActor());
	}
	else
	{
		//UE_LOG(LogClass, Warning, TEXT("Actor is NOT Interactable!"));
		//ClearPickupInfo();
	}
}

//Heal function, takes in health from health pickups, caps player health at 100.
void AFIT2097Week3Character::HealPlayer(int health)
{
	PlayerHealth += health;

	//Caps health to a maximum of 100.
	if (PlayerHealth >= 100)
	{
		PlayerHealth = 100;
	}
}

//Modifies hasFuse bool to true
void AFIT2097Week3Character::GivePlayerFuse()
{
	hasFuse = !hasFuse;
}

void AFIT2097Week3Character::GiveGrenade()
{
	GrenadeCount++;
}

void AFIT2097Week3Character::FootstepEvent_Implementation()
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
			if (FootstepConcreteSoundCue != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FootstepConcreteSoundCue, GetActorLocation());
			}
		}
		//Checks if surface is default, if yes, play concrete sound cue
		if (PhysicsMtl->SurfaceType == SurfaceType1)
		{
			// try and play the sound if specified
			if (FootstepConcreteSoundCue != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FootstepConcreteSoundCue, GetActorLocation());
			}
		}
		//Checks if surface is default, if yes, play carpet sound cue
		if (PhysicsMtl->SurfaceType == SurfaceType2)
		{
			// try and play the sound if specified
			if (FootstepCarpetSoundCue != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FootstepCarpetSoundCue, GetActorLocation());
			}
		}

	}
}

//Decreases the player's health, is called from gamemode every two seconds.
//Includes check for if the player is dead, broadcasts player death event
void AFIT2097Week3Character::DecreaseHealth()
{
	//PlayerHealth--;

	PlayerHealth -= 5;
	
	if(PlayerHealth <= 0)
	{
		PlayerDeath.Broadcast();
	}
}

//Checks if the player is immune, if immune, create a timer to then reset immunity, else, decrease health
//and make player immune, done to make sure that player does not die immediately
void AFIT2097Week3Character::TakeDamage()
{
	//If isImmune, create a timer to then reset immunity
	if(isImmune)
	{
		GetWorldTimerManager().SetTimer(ImmuneTimer, this, &AFIT2097Week3Character::ImmuneReset, 2.f, false);
		return;
	}
	//else, decrease health and make player immune
	else
	{
		DecreaseHealth();
		isImmune = true;

		DamagePostProcess->bUnbound = 1;
	}
}

void AFIT2097Week3Character::KillPlayer()
{
	PlayerDeath.Broadcast();
}

void AFIT2097Week3Character::ImmuneReset()
{
	isImmune = false;

	DamagePostProcess->bUnbound = 0;
}

/*void AFIT2097Week3Character::DamageScreenReset()
{
	DamagePost
}*/


//Changes SetPause bool value and broadcasts it to GameMode, the changing bool value is how we unpause.
void AFIT2097Week3Character::OnPause()
{
	bSetPause = !bSetPause;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player pressed p"));
	PlayerPause.Broadcast(bSetPause);

}


// Fires a grenade, recycle projectile firing code from OnFire
void AFIT2097Week3Character::OnGrenade()
{
	//If we have grenades, fire it off
	if (GrenadeCount > 0)
	{
		// try and fire a projectile
		if (ProjectileClass != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				if (bUsingMotionControllers)
				{
					const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
					const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
					World->SpawnActor<AFIT2097Week3Projectile>(ProjectileClass, SpawnLocation, SpawnRotation);
				}
				else
				{
					const FRotator SpawnRotation = GetControlRotation();
					// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
					const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// spawn the projectile at the muzzle
					World->SpawnActor<AFIT2097Week3Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}

		// try and play the sound if specified
		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}

		GrenadeCount--;
	}
}

