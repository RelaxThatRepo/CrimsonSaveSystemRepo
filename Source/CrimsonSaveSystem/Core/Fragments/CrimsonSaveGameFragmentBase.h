// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "CrimsonSaveGameFragmentBase.generated.h"


/**
 * An abstract base class for a piece of save data.
 * Each game system that needs to be saved will have its own fragment class derived from this.
 */
UCLASS(Abstract)
class CRIMSONSAVESYSTEM_API UCrimsonSaveGameFragmentBase : public USaveGame
{
	GENERATED_BODY()
};

USTRUCT(Blueprintable)
struct FCrimsonSaveGameDataPayload
{
	GENERATED_BODY()

	UPROPERTY()
	FString FragmentName;

	UPROPERTY()
	TObjectPtr<UCrimsonSaveGameFragmentBase> Fragment;
};

UCLASS()
class CRIMSONSAVESYSTEM_API UCrimsonSaveGameHeader : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save Metadata")
	FString SlotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save Metadata")
	FDateTime SaveDateTime;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save Metadata")
	FTimespan PlayTime;

	// Add other metadata here, like a path to a screenshot texture.
};