// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

#include "SleepChaseGameModeBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AttributeComponent)


// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HealthMax = 100;
	Health = HealthMax;

	DamageMultiplier = 1.0f;
	
}

bool UAttributeComponent::ApplyHealthChanged(AActor* InstigatorActor, float Delta)
{
	if(Delta < 0.0f)
	{
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;


	Health = NewHealth;
	
	if (ActualDelta != 0.0f)
	{
		OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
	}
	if(ActualDelta < 0.0f && Health == 0.0f)
	{
		ASleepChaseGameModeBase* GM = GetWorld()->GetAuthGameMode<ASleepChaseGameModeBase>();
		if(GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	

	return ActualDelta != 0;
}

bool UAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UAttributeComponent::IsFullHealth() const
{
	return Health >= HealthMax;
}

float UAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

float UAttributeComponent::GetHealth() const
{
	return Health;
}

bool UAttributeComponent::ApplySleepy(AActor* InstigatorActor, float Delta)
{
	float OldSleepy = Sleepy;

	float NewSleepy = FMath::Clamp(Sleepy + Delta, 0.0f, SleepyMax);

	float ActualDelta = NewSleepy - OldSleepy;
	
	if (ActualDelta != 0.0f)
	{
		OnSleepyChanged.Broadcast(InstigatorActor, this, NewSleepy, Delta);
	}

	return ActualDelta != 0;
}



float UAttributeComponent::GetSleepy() const
{
	return Sleepy;
}

bool UAttributeComponent::IsActorAlive(AActor* Actor)
{
	TObjectPtr<UAttributeComponent> AttributeComp =GetAttributes(Actor);
	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	
	return false;
}

UAttributeComponent* UAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return FromActor->FindComponentByClass<UAttributeComponent>();
	}
	
	return nullptr;
}

bool UAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChanged(InstigatorActor, -HealthMax);
}

