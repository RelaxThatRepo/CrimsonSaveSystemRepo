// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CrimsonSaveSystemSettings.generated.h"

/**
 * A simple SaveGame object used to store user preferences
 * for the save system, like the last used slot.
 */
UCLASS()
class CRIMSONSAVESYSTEM_API UCrimsonSaveSystemSettings : public USaveGame
{
	GENERATED_BODY()

public:
	/** The last slot index the player had highlighted in the save/load menu. */
	UPROPERTY()
	int32 LastSelectedSaveSlot = 0;

	/** If true, the game will attempt to load the 'LastSelectedSaveSlot' upon startup. */
	UPROPERTY()
	bool bAutoLoadLastSave = true;
};