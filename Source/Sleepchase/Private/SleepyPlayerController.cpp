// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepyPlayerController.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SleepyPlayerController)


void ASleepyPlayerController::UpdateRotation(float DeltaTime)
{
	FVector GravityDirection = FVector::DownVector;
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		if (UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement())
		{
			GravityDirection = MoveComp->GetGravityDirection();
		}
	}
 
	//获取世界空间中的当前控件旋转
	FRotator ViewRotation = GetControlRotation();
 
	// 添加重力变化的任何旋转（如果发生）。
	// 如果您不希望摄像机自动补偿重力旋转，请删除此代码块。
	if (!LastFrameGravity.Equals(FVector::ZeroVector))
	{
		const FQuat DeltaGravityRotation = FQuat::FindBetweenNormals(LastFrameGravity, GravityDirection);
		const FQuat WarpedCameraRotation = DeltaGravityRotation * FQuat(ViewRotation);
 
		ViewRotation = WarpedCameraRotation.Rotator();	
	}
	LastFrameGravity = GravityDirection;
 
	// 将视图旋转从世界空间转换为重力相对空间。
	// 现在我们可以处理旋转，就好像没有自定义重力影响它一样。
	ViewRotation = GetGravityRelativeRotation(ViewRotation, GravityDirection);
 
	// Calculate Delta to be applied on ViewRotation
	FRotator DeltaRot(RotationInput);
 
	if (PlayerCameraManager)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn());
 
		PlayerCameraManager->ProcessViewRotation(DeltaTime, ViewRotation, DeltaRot);
 
		// 将摄像机的滚动归零，因为我们始终希望它相对于重力保持水平。
		ViewRotation.Roll = 0;
 
		// 将旋转转换回世界空间，并将其设置为当前控件旋转。
		SetControlRotation(GetGravityWorldRotation(ViewRotation, GravityDirection));
	}
 
	APawn* const P = GetPawnOrSpectator();
	if (P)
	{
		P->FaceRotation(ViewRotation, DeltaTime);
	}
}

FRotator ASleepyPlayerController::GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		FQuat GravityRotation = FQuat::FindBetweenNormals(GravityDirection, FVector::DownVector);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}
 
	return Rotation;
}

FRotator ASleepyPlayerController::GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		FQuat GravityRotation = FQuat::FindBetweenNormals(FVector::DownVector, GravityDirection);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}
 
	return Rotation;
}

