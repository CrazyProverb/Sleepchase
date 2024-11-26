// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInterface.h"
#include "GameFramework/Actor.h"
#include "PowerupActor.generated.h"

class USphereComponent;

UCLASS()
class SLEEPCHASE_API APowerupActor : public AActor,public IGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerupActor();

protected:

	UPROPERTY()
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();
	
	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnPowerup;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UFUNCTION()
	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);


public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

};
