// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSaveWorldData.h"
#include "Subsystems/WorldSubsystem.h"
#include "CrimsonSaveWorldManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONSAVESYSTEM_API UCrimsonSaveWorldManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	// Called from game instance or player controller to save/load the world state
	UFUNCTION(BlueprintCallable, Category="Crimson|SaveSystem")
	void SaveWorldState();
	UFUNCTION(BlueprintCallable, Category="Crimson|SaveSystem")
	void LoadWorldState();

	// These functions are called by actors in the world to notify the subsystem of a change.
	UFUNCTION(BlueprintCallable, Category="Crimson|SaveSystem", meta = (DefaultToSelf = "InActor"))
	void OnActorStateChanged(const AActor* InActor, UObject* NewActorStateObject);
	UFUNCTION(BlueprintCallable, Category="Crimson|SaveSystem", meta = (DefaultToSelf = "InActor"))
	void OnActorDestroyed(const AActor* InActor, UObject* NewActorStateObject);
	
private:
	
	/**
	* Gets the full save path for this world's data fragment,
	* based on the currently active save slot in the GameInstance.
	* Returns empty string if no save slot is active.
	*/
	FString GetSaveSlotPath() const;

	// This map holds the current, live state of all interacted-with actors in the world.
	// This is what gets written to the save file.
	TMap<FGuid, FCrimsonSaveWorldActorData> CurrentWorldState;
};