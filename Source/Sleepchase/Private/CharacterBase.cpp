// Fill out your copyright notice in the Description page of Project Settings.


#include "Sleepchase/Public/CharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actions/ActionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CharacterBase)

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	// We control the rotation of spring arm with pawn control rotation already, this disables a subtle side effect
	// where rotating our CapsuleComponent (eg. caused by bOrientRotationToMovement in Character Movement) will rotate our spring arm until it self corrects later in the update
	// This may cause unwanted effects when using CameraLocation during Tick as it may be slightly offset from our final camera position.
	SpringArmComp->SetUsingAbsoluteRotation(true);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	ActionComp=CreateDefaultSubobject<UActionComponent>(TEXT("ActionComp"));
}


// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			//将增强输入子系统与我们创建的映射文件绑定
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultInputMapping,0);
			
		}
	}

	//映射文件添加好后，我们还要与我们的Action进行绑定
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(Input_LookMouse,ETriggerEvent::Triggered,this,&ACharacterBase::Action_LookMouse);
		EnhancedInputComponent->BindAction(Input_Move,ETriggerEvent::Triggered,this,&ACharacterBase::Action_Move);
		EnhancedInputComponent->BindAction(Input_Jump,ETriggerEvent::Triggered,this,&ACharacter::Jump);
		EnhancedInputComponent->BindAction(Input_PrimaryAttack,ETriggerEvent::Triggered,this,&ACharacterBase::Action_PrimaryAttack);
		EnhancedInputComponent->BindAction(Input_PrimaryInteract,ETriggerEvent::Triggered,this,&ACharacterBase::Action_PrimaryInteract);
		
		EnhancedInputComponent->BindAction(Input_Sprint, ETriggerEvent::Started, this, &ACharacterBase::Action_SprintStart);
		EnhancedInputComponent->BindAction(Input_Sprint, ETriggerEvent::Completed, this, &ACharacterBase::Action_SprintStop);

	}
}

void ACharacterBase::Action_Move(const FInputActionValue& InputValue)
{
	FRotator ControlRot = GetControlRotation();
	//只与偏航角Yaw有关，其余设置为0
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//从输入得到二维向量
	const FVector2D AxisValue = InputValue.Get<FVector2D>();

	//前后移动
	AddMovementInput(ControlRot.Vector(),AxisValue.Y);

	//左右移动
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector,AxisValue.X);
}

void ACharacterBase::Action_LookMouse(const FInputActionValue& InputValue)
{
	const FVector2d Value = InputValue.Get<FVector2d>();
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ACharacterBase::Action_PrimaryAttack()
{
	static FGameplayTag ActivationTag = FGameplayTag::RequestGameplayTag("Action.PrimaryAttack");

	ActionComp->StartActionByName(this,ActivationTag);
}

void ACharacterBase::Action_PrimaryInteract()
{
}

void ACharacterBase::Action_SprintStart()
{
}

void ACharacterBase::Action_SprintStop()
{
}

