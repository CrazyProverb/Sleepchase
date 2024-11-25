// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SleepChaseGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SLEEPCHASE_API ASleepChaseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
};
