// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/ProjectileBase.h"
#include "SleepyProjectile.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSleeprojectileSparseData : public FProjectileSparseData
{
	GENERATED_BODY()

	FSleeprojectileSparseData()
	: DamageAmount(20.f)
	{}
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;
	
};


UCLASS(SparseClassDataTypes = SleeprojectileSparseData)
class SLEEPCHASE_API ASleepyProjectile : public AProjectileBase
{
	GENERATED_BODY()


protected:
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void PostInitializeComponents() override;

public:
	ASleepyProjectile();
	

#if WITH_EDITORONLY_DATA
	//~ These properties are moving out to the FMySparseClassData struct:
	
private:
	UPROPERTY()
	float DamageAmount_DEPRECATED;
	
#endif
	
#if WITH_EDITOR
public:
	// ~ This function transfers existing data into FMySparseClassData.
	virtual void MoveDataToSparseClassDataStruct() const override;
#endif
	
};
