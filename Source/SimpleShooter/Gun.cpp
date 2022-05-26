// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	LoadClip();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& OutHitResult, FVector& OutShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	FVector ViewpointLocation;
	FRotator ViewpointRotation;

	OwnerController->GetPlayerViewPoint(ViewpointLocation, ViewpointRotation);
	OutShotDirection = -ViewpointRotation.Vector();

	FVector End = ViewpointLocation + (ViewpointRotation.Vector() * MaxRange);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(OutHitResult, ViewpointLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;

	return OwnerPawn->GetController();
}

void AGun::PullTrigger()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--; 
		
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

		FVector ShotDirection;
		FHitResult HitResult;
		if (GunTrace(HitResult, ShotDirection))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, HitResult.ImpactPoint, ShotDirection.Rotation());
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, HitResult.ImpactPoint);
			if (HitResult.GetActor())
			{

				FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);

				AController* OwnerController = GetOwnerController();
				if (OwnerController == nullptr) return;

				HitResult.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);
			}
		}
	}
	else
	{
		UGameplayStatics::SpawnSoundAttached(EmptySound, Mesh, TEXT("MuzzleFlashSocket"));
	}

	

}

void AGun::LoadClip()
{
	CurrentAmmo = MaxAmmo;
}

int32 AGun::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 AGun::GetMaxAmmo() const
{
	return MaxAmmo;
}
