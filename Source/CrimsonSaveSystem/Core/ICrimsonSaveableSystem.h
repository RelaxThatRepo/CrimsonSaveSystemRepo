// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "ICrimsonSaveableSystem.generated.h"

class UCrimsonSaveGameFragmentBase;

UINTERFACE(MinimalAPI, Blueprintable)
class UCrimsonSaveableSystem : public UInterface
{
	GENERATED_BODY()
};

class ICrimsonSaveableSystem
{
	GENERATED_BODY()

public:
	/** * Returns the unique name for this system's save file fragment. */
	UFUNCTION(BlueprintNativeEvent, Category = "Crimson|SaveSystem")
	FString GetFragmentName() const;

	/** * Called by the Save Manager to request this system's data for saving. 
	 * @return A fragment object containing this system's save data.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Crimson|SaveSystem")
	UCrimsonSaveGameFragmentBase* GatherSaveData();

	/** * Called by the Save Manager to push loaded data back into this system. 
	 * This *must* be implemented by the Blueprint.
	 * @param Fragment The data fragment to restore from.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Crimson|SaveSystem")
	void RestoreFromSaveData(const UCrimsonSaveGameFragmentBase* Fragment);
};