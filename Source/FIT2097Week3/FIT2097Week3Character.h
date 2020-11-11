// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/PostProcessVolume.h"
#include "Components/CapsuleComponent.h"
#include "FIT2097Week3Character.generated.h"

class UInputComponent;
//Event Dispatchers
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerPauseEvent, bool, bSetPause);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerWinEvent);

UCLASS(config=Game)
class AFIT2097Week3Character : public ACharacter
{
	GENERATED_BODY()

	//Create trigger capsule
	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* TriggerCapsule;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

public:
	AFIT2097Week3Character();

protected:
	virtual void BeginPlay();

public:

	//Declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFIT2097Week3Projectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	//My Variables and functions here
	
	// Player Health, to be modified with health pickups, will be a UPROPERTY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int PlayerHealth;

	//Variable to signify if player has fuse, to be modified with fuse pickups, will be a UPROPERTY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool hasFuse;

	//bool to allow player to become immune momentarily, done so the player does not immediately die
	bool isImmune;

	/** Damage Post Process Material */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class APostProcessVolume* DamagePostProcess;

	//FTimerHandle to allow for immunity period
	FTimerHandle ImmuneTimer;

	//Resets isImmune once immunity period ends, also resets damage screen to not unbound stopping the damage screen post processing
	void ImmuneReset();

	//Resets DamageScreen to not unbound, stopping the damage screen post processing
	//void DamageScreenReset();
	
	// UFUNCTION call by the GameMode to decrease health every 2 seconds.
	UFUNCTION()
		void DecreaseHealth();

	// UFUNCTION call when character collides with an enemy.
	UFUNCTION()
		void TakeDamage();

	// UFUNCTION call when character collides with a special enemy.
	UFUNCTION()
		void KillPlayer();
	
	// Function bound to event listener, is called when HealthPickup broadcasts 
	UFUNCTION()
		void HealPlayer(int health);

	//Function bound to event listener, is called when FusePickup broadcasts
	UFUNCTION()
		void GivePlayerFuse();
	
	//Function bound to event listener, is called when GrenadePickup broadcasts
	UFUNCTION()
		void GiveGrenade();

	//Function bound to animBP footstep event, is called when anim notify event occurs
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Gameplay)
		void FootstepEvent();

	//UPROPERTY used to pass to gamemode's pause game function. Player still gives input when paused,
	//every OnPause function call would give a different bool value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool bSetPause = false;

	/** Footstep concrete sounds*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundCue* FootstepConcreteSoundCue;

	/** Footstep carpet sounds*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundCue* FootstepCarpetSoundCue;
	
	//Event Dispatcher to call on the remote open function in listeners. Responsible for death
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FPlayerDeathEvent PlayerDeath;
	//Event Dispatcher to call on the remote open function in listeners. Responsible for pause
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FPlayerPauseEvent PlayerPause;
	//Event Dispatcher to call on the remote open function in listeners. Responsible for win
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FPlayerWinEvent PlayerWin;

	//Fires a grenade
	void OnGrenade();

	//Int variable, stores how much grenades the player has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int GrenadeCount;
	

protected:
	
	/** Conducts linetrace, main interaction method. */
	void OnFire();

	// Pauses the game custom function
	void OnPause();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	//Trace code given in Moodle
	bool Trace(UWorld* World, TArray<AActor*>& ActorsToIgnore, const FVector& Start,
		const FVector& End, FHitResult& Hit, ECollisionChannel CollisionChannel, bool ReturnPhysMat);

	void CallMyTrace();

	void ProcessTraceHit(FHitResult& HitOut);


};

