// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SleepyProjectile.h"

#include "GameplayFunctionLibrary.h"
#include "Actions/ActionComponent.h"
#include "Components/SphereComponent.h"


ASleepyProjectile::ASleepyProjectile()
{
	SphereComp->SetSphereRadius(20.0f);

	InitialLifeSpan = 5.0f;
}

void ASleepyProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASleepyProjectile::OnActorOverlap);
}


void ASleepyProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{

		UActionComponent* ActionComp = Cast<UActionComponent>(OtherActor->GetComponentByClass(UActionComponent::StaticClass()));
		// Apply Damage & Impulse
		if (UGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, GetDamageAmount(), SweepResult))
		{
			Explode();
		}
	}
}

#if WITH_EDITOR
// Only required to convert existing properties already stored in Blueprints into the 'new' system
void ASleepyProjectile::MoveDataToSparseClassDataStruct() const
{
	// make sure we don't overwrite the sparse data if it has been saved already
	const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
	if (BPClass == nullptr || BPClass->bIsSparseClassDataSerializable == true)
	{
		return;
	}
	
	Super::MoveDataToSparseClassDataStruct();

#if WITH_EDITORONLY_DATA
	// Unreal Header Tool (UHT) will create GetMySparseClassData automatically.
	FSleeprojectileSparseData* SparseClassData = GetSleeprojectileSparseData();

	// Modify these lines to include all Sparse Class Data properties.
	SparseClassData->DamageAmount = DamageAmount_DEPRECATED;

#endif // WITH_EDITORONLY_DATA
}
#endif