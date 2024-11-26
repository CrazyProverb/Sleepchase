// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Action_ProjectileAttack.h"

#include "CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sleepchase/Sleepchase.h"


UAction_ProjectileAttack::UAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;

	SweepRadius = 20.0f;
	SweepDistanceFallback = 5000;
}

void UAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if(TObjectPtr<ACharacter> Character = Cast<ACharacter>(Instigator))
	{
		Character->PlayAnimMontage(AttackAnim);
		
		UGameplayStatics::SpawnEmitterAttached(ImpactVFX, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget, true, EPSCPoolMethod::AutoRelease);
		
		UGameplayStatics::SpawnSoundAttached(CastingSound, Character->GetMesh());
		
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		//GetWorld() 被调用来获取当前 USAction_ProjectileAttack 实例所在的游戏世界（UWorld）的引用。
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}


void UAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if(ensureAlways(ProjectileClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		
		

		/*APlayerCameraManager* CurrentCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector TraceDirection = CurrentCamera->GetActorForwardVector();
		FVector TraceStart = CurrentCamera->GetCameraLocation();
		FVector TraceEnd = TraceStart + (TraceDirection * 5000);
		FVector AdjustedTraceEnd = TraceEnd;*/

		//获得Cameracomponent
		/*TObjectPtr<ACharacterBase> Player = Cast<ACharacterBase>(InstigatorCharacter);
		UCameraComponent * CameraComp = Player->GetCameraComponent();
		const FVector TraceStart = CameraComp->GetComponentLocation();
		// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
		const FVector TraceEnd = CameraComp->GetComponentLocation() + (Player->GetControlRotation().Vector() * 5000);
		FVector AdjustedTraceEnd = TraceEnd;*/

		FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();

		// Add sweep radius onto start to avoid the sphere clipping into floor/walls the camera is directly against.
		const FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (TraceDirection * SweepRadius);
		// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
		const FVector TraceEnd = TraceStart + (TraceDirection * SweepDistanceFallback);
		FVector AdjustedTraceEnd = TraceEnd;

		TArray<FHitResult> Hits;
		// returns true if we got to a blocking hit
		if (GetWorld()->SweepMultiByChannel(Hits, TraceStart, TraceEnd, FQuat::Identity, COLLISION_PROJECTILE, Shape, Params))
		{
			AdjustedTraceEnd = Hits[0].ImpactPoint;
		}
		FRotator ProjRotation = (AdjustedTraceEnd - HandLocation).Rotation();
		
		// 起始点是我们的子弹生成点（手），终点是目标点，获得Rotation
		// 最终获得校正后的生成方位
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
	
		// 在世界中生成
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
		
	}
	StopAction(InstigatorCharacter);
}


