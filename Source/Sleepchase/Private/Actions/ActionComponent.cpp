// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ActionComponent.h"

#include "Actions/Action.h"
#include "Sleepchase/Sleepchase.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(ActionComponent)


// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UAction> ActionClass : DefaultAction)
	{
		AddAction(GetOwner(), ActionClass);
	}
	
}

// Called every frame
void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (UAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),*GetNameSafe(GetOwner()),*GetNameSafe(Action));
			
			
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UActionComponent::AddAction(AActor* Instigator, TSubclassOf<UAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}
	
	UAction* NewAction = NewObject<UAction>(GetOwner(), ActionClass);

	if(ensure(NewAction))
	{
		NewAction->Initialize(this);
		
		Actions.Add(NewAction);

		if((NewAction->bAutoStart) && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UActionComponent::RemoveAction(UAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

UAction* UActionComponent::GetAction(TSubclassOf<UAction> ActionClass) const
{
	for (UAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

bool UActionComponent::StartActionByName(AActor* Instigator, FGameplayTag ActionName)
{
	for(UAction* Action : Actions)
	{
		if(Action && Action->ActivationTag == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			Action->StartAction(Instigator);
			return true;
			
		}
	}
	return false;
	
}

bool UActionComponent::StopActionByName(AActor* Instigator, FGameplayTag ActionName)
{
	for(UAction* Action : Actions)
	{
		if(Action && Action->ActivationTag == ActionName)
		{
			if(Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

