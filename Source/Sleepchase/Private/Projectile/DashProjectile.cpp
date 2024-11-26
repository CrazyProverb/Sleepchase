// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/DashProjectile.h"
#include "Sound/SoundCue.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DashProjectile)


ADashProjectile::ADashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MoveComp->InitialSpeed = 6000.f;
}

void ADashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ADashProjectile::Explode, DetonateDelay);
}

void ADashProjectile::Explode_Implementation()
{
	//如果是OnActorHit产生的爆炸，则不执行
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	
	MoveComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ADashProjectile::TeleportInstigator, TeleportDelay);
}

void ADashProjectile::TeleportInstigator()
{
	TObjectPtr<AActor> ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		// 保持玩家的旋转角度否则可能卡住
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);

		// 对我们传送的玩家进行Shake
		APawn* InstigatorPawn = Cast<APawn>(ActorToTeleport);
		APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
		PC->ClientStartCameraShake(ImpactShake);
		
		
	}
	Destroy();
}
