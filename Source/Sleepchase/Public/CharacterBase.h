// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UAttributeComponent;
class UInteractionComponent;
class UActionComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SLEEPCHASE_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:

	FTimerHandle PrimaryAttackTimerHandle;

	float Intime;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	TObjectPtr<UActionComponent> ActionComp;

	UPROPERTY(VisibleAnywhere,Category = "Components")
	TObjectPtr<UInteractionComponent> InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	TObjectPtr<UAttributeComponent> AttributeComp;
	
	
	//Input
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Move;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_LookMouse;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Jump;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryInteract;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Sprint;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_SwitchGravity;

	virtual void Action_Move(const FInputActionValue& InputValue);

	virtual void Action_LookMouse(const FInputActionValue& InputValue);

	virtual void Action_PrimaryAttack();

	virtual void Action_PrimaryInteract();

	virtual void Action_SprintStart();

	virtual void Action_SprintStop();

	virtual void Action_SwitchGravityStart();

	virtual void Action_SwitchGravityStop();

	
	
public:	
	ACharacterBase();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION(Exec)
	void HealSelf(float amount = 100);

	UCameraComponent* GetCameraComponent();
};
