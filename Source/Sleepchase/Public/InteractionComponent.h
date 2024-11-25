// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLEEPCHASE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Interact();
	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	UPROPERTY()
	TObjectPtr<AActor> FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	
		
};
