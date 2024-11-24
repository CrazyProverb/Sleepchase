// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/Action.h"

#include "Actions/ActionComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(Action)


void UAction::StartAction_Implementation(AActor* Instigator)
{
	UActionComponent* Comp = GetOwningComponent();	
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	ActionData.bIsRunning = true;
	ActionData.Instigator = Instigator;

	
	TimeStarted = GetWorld()->TimeSeconds;
	

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void UAction::StopAction_Implementation(AActor* Instigator)
{
	UActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	ActionData.bIsRunning = false;
	ActionData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

bool UAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UActionComponent* Comp = GetOwningComponent();
	
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

UActionComponent* UAction::GetOwningComponent() const
{
	return ActionComp;
}

bool UAction::IsRunning() const
{
	return ActionData.bIsRunning;
}

void UAction::Initialize(UActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

UWorld* UAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}
