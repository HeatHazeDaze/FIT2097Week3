// Copyright Epic Games, Inc. All Rights Reserved.

#include "FIT2097Week3Projectile.h"
#include "EnemyCharacter.h"
#include "SpecialEnemyCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"



AFIT2097Week3Projectile::AFIT2097Week3Projectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFIT2097Week3Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 5.0f;

	GrenadeAOEComp = CreateDefaultSubobject<USphereComponent>(TEXT("GrenadeAOEComp"));
	GrenadeAOEComp->InitSphereRadius(20.0f);
	GrenadeAOEComp->BodyInstance.SetCollisionProfileName("Projectile");
	GrenadeAOEComp->OnComponentBeginOverlap.AddDynamic(this, &AFIT2097Week3Projectile::OnOverlapBegin);

	//GrenadeAOEComp->SetActive(false);
}

void AFIT2097Week3Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		
		//Destroy();
	}

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{


		//If OtherActor is an enemy, call the stun function
		AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OtherActor);
		if (enemy)
		{

			GrenadeExplode();
			
			enemy->EnemyStun();

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Calling stun function"));
			//GetWorldTimerManager().SetTimer(ExplosionDelay, this, &AFIT2097Week3Projectile::Destroy, 2.f, false);
			Destroy();
		}


		//If OtherActor is a Health Pickup, binds the HealPlayer function to the heal event dispatch
		ASpecialEnemyCharacter* special = Cast<ASpecialEnemyCharacter>(OtherActor);
		if (special)
		{

			GrenadeExplode();

			special->EnemyStun();

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Calling stun function"));
			//GetWorldTimerManager().SetTimer(ExplosionDelay, this, &AFIT2097Week3Projectile::Destroy, 2.f, false);
			Destroy();
		}

		else
		{
			GrenadeExplode();
			//GetWorldTimerManager().SetTimer(ExplosionDelay, this, &AFIT2097Week3Projectile::Destroy, 2.f, false);
			Destroy();
		}
		
	}

	
	//GrenadeAOEComp->SetActive(true);
	//CollisionComp->SetActive(false);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Stun AOE sphere active"));
}

void AFIT2097Week3Projectile::GrenadeExplode()
{
	if (MyParticleSystem)
	{
		MyParticleSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/NiagaraFX/StunExplosionFX.StunExplosionFX'"), nullptr, LOAD_None, nullptr);

		UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			MyParticleSystem,
			GetActorLocation(), //Position
			FRotator(1), //Rotation
			FVector(1, 1, 1), //Scale
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true);
	}
	
}

void AFIT2097Week3Projectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		//If OtherActor is a Health Pickup, binds the HealPlayer function to the heal event dispatch
		AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OtherActor);
		if (enemy)
		{
			enemy->EnemyStun();

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Calling stun function"));
		}


		Destroy();
	}
}
