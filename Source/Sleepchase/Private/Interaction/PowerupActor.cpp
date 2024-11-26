// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/PowerupActor.h"

#include "Components/SphereComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PowerupActor)


// Sets default values
APowerupActor::APowerupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
	RespawnTime = 10.0f;

	bIsActive = true;
}

void APowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void APowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnPowerup, this, &APowerupActor::ShowPowerup, RespawnTime);
}

void APowerupActor::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void APowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	// 设置可见性
	RootComponent->SetVisibility(bIsActive, true);
}

void APowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	
}


