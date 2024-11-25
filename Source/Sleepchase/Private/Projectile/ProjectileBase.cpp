// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ProjectileBase)


// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionProfileName("Projectile");
	
	//SphereComp 组件不会对导航系统产生影响
	SphereComp->SetCanEverAffectNavigation(false);
	RootComponent = SphereComp;

	NiagaraLoopComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EffectComp"));
	NiagaraLoopComp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
	
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
	MoveComp->ProjectileGravityScale = 0.0f;
	MoveComp->InitialSpeed = 8000;
	
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnActorHit);
}

void AProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if((OtherActor!=nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if( OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity()*120.0f, GetActorLocation());
		}
		Explode();
	}
}

void AProjectileBase::Explode_Implementation()
{
	if(ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation(), true, EPSCPoolMethod::AutoRelease);
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation());
		UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(),GetImpactShakeInnerRadius(),GetImpactShakeOuterRadius());
		Destroy();
	}
}


// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


#if WITH_EDITOR
// 只需将蓝图中已存储的现有属性转换为“新”系统
void AProjectileBase::MoveDataToSparseClassDataStruct() const
{
	// 确保我们不会覆盖已保存的稀疏数据
	const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
	if (BPClass == nullptr || BPClass->bIsSparseClassDataSerializable == true)
	{
		return;
	}
	
	Super::MoveDataToSparseClassDataStruct();
#if WITH_EDITORONLY_DATA
	// Unreal Header Tool （UHT） 将自动创建 GetMySparseClassData。
	FProjectileSparseData* SparseClassData = GetProjectileSparseData();
	// 修改这些行以包括所有 Sparse Class Data 属性。
	SparseClassData->ImpactShakeInnerRadius = ImpactShakeInnerRadius_DEPRECATED;
	SparseClassData->ImpactShakeOuterRadius = ImpactShakeOuterRadius_DEPRECATED;
#endif // WITH_EDITORONLY_DATA
}
#endif

