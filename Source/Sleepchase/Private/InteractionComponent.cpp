// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "GameplayInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractionComponent)


static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
	
	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	
	CollisionChannel = ECC_WorldDynamic;
}


void UInteractionComponent::Interact()
{
	const bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	TObjectPtr<AActor> MyOwner = GetOwner();
	
	//这里的EyeLocation并不是摄像机的位置信息，而是角色的位置加上角色定义的高度的位置
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + EyeRotation.Vector() * TraceDistance;

	TArray<FHitResult> Hits;
	
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	//颜色判断
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;
	
	for(FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 0.0f);
		}
		
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(HitActor->Implements<UGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				IGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
		
	}
	
	if(bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 0.0f);
	}  
}



