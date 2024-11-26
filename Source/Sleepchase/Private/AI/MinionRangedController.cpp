// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MinionRangedController.h"

void AMinionRangedController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
}
