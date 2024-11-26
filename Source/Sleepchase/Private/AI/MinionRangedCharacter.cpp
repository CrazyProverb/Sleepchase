// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MinionRangedCharacter.h"

#include "AIController.h"
#include "AttributeComponent.h"
#include "BrainComponent.h"
#include "Actions/ActionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "UI/WorldUserWidgetBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MinionRangedCharacter)

// Sets default values
AMinionRangedCharacter::AMinionRangedCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComp"));

	ActionComp = CreateDefaultSubobject<UActionComponent>(TEXT("ActionComp"));

	TimeToHitParamName = "TimeToHit";

	TargetActorKey = "TargetActor";

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMinionRangedCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AMinionRangedCharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AMinionRangedCharacter::OnHealthChanged);
}


void AMinionRangedCharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = GetController<AAIController>();
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}

AActor* AMinionRangedCharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}

void AMinionRangedCharacter::OnPawnSeen(APawn* Pawn)
{
	// 如果已设置目标，则忽略
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);

		UWorldUserWidgetBase* NewWidget = CreateWidget<UWorldUserWidgetBase>(GetWorld(), SpottedWidgetClass);
		if (NewWidget)
		{
			NewWidget->AttachedActor = this;
			//Index 为 10（或高于默认值 0 的任何值）将其置于任何其他 widget 的顶部，否则可能会落在生命条后面。
			NewWidget->AddToViewport(10);
		}
	}
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 0.5f, true);
}

void AMinionRangedCharacter::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
		

		// Create once, and skip creating if its an instant kill
		if (ActiveHealthBar == nullptr && NewHealth > 0.0)
		{
			ActiveHealthBar = CreateWidget<UWorldUserWidgetBase>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f)
		{
			// stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			// ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// set lifespan
			SetLifeSpan(8.0f);
		}
	}
}



// Called to bind functionality to input
void AMinionRangedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

