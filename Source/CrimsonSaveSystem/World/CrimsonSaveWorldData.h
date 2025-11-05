// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "CrimsonSaveWorldData.generated.h"


USTRUCT(BlueprintType, Category="Crimson|SaveSystem")
struct FCrimsonSaveWorldActorData
{
	GENERATED_BODY()
	
	// Was this actor destroyed and should be removed on load?
	UPROPERTY()
	bool bWasDestroyed = false;

	// The real-world time this actor was last interacted with.
	UPROPERTY()
	FDateTime LastInteractionTime;

	UPROPERTY(Instanced)
	TObjectPtr<UObject> PayloadData;
};

/**
 * 
 */
UCLASS()
class UCrimsonSaveWorldData : public USaveGame
{
	GENERATED_BODY()

public:
	// A map of all changed actor states in the world, keyed by their persistent GUID.
	UPROPERTY()
	TMap<FGuid, FCrimsonSaveWorldActorData> SavedActorStates;
};
