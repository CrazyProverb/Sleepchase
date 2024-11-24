// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "Action.generated.h"

/**
 * 
 */

class UActionComponent;

USTRUCT()
struct FActionData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	bool bIsRunning;
	
	UPROPERTY()
	AActor* Instigator;
	
	FActionData()
	{
		bIsRunning = false;
	}
};


UCLASS(Blueprintable)
class SLEEPCHASE_API UAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY()
	TObjectPtr<UActionComponent> ActionComp;
	
	UPROPERTY()
	FActionData ActionData;

	/*激活时添加到拥有角色的标签，操作停止时删除*/
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/*仅当 OwningActor 未应用这些 Tags 时，操作才能启动*/
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	UActionComponent* GetOwningComponent() const;

	UPROPERTY()
	float TimeStarted;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;
	
	UFUNCTION(BlueprintNativeEvent,Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable,Category="Action")
	bool IsRunning() const;
	

	void Initialize(UActionComponent* NewActionComp);
	

	UFUNCTION(BlueprintNativeEvent,Category="Action")
	bool CanStart(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly,Category="Action")
	FGameplayTag ActivationTag;
	
	UWorld* GetWorld() const override;
};
