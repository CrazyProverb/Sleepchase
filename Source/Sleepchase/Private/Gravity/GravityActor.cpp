// Fill out your copyright notice in the Description page of Project Settings.


#include "Gravity/GravityActor.h"

#include "Components/SphereComponent.h"

// Sets default values
AGravityActor::AGravityActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

}

// Called when the game starts or when spawned
void AGravityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

