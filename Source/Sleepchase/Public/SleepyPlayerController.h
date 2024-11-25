// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SleepyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SLEEPCHASE_API ASleepyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void UpdateRotation(float DeltaTime) override;
 
	//将旋转从世界空间转化为重力相对空间。
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection);
 
	//将旋转从重力相对空间转化为世界空间。
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection);
 
private:
	FVector LastFrameGravity = FVector::ZeroVector;
};
