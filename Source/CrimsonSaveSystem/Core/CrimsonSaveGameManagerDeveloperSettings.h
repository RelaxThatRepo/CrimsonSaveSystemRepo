// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "CrimsonSaveGameManagerDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Crimson Save Game Manager"))
class CRIMSONSAVESYSTEM_API UCrimsonSaveGameManagerDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** The save slot index to automatically activate when Playing In Editor (PIE). */
	UPROPERTY(Config, EditAnywhere, Category = "Crimson|SaveSystem")
	int32 DefaultPIESaveSlot = 0;

	/** The default character name to use when creating a new debug save file in PIE. */
	UPROPERTY(Config, EditAnywhere, Category = "Crimson|SaveSystem")
	FString DefaultPIECharacterName = TEXT("Play In Editor Character");

	/** If true, the default PIE save slot will be deleted and reset every time you start a new PIE session. */
	UPROPERTY(Config, EditAnywhere, Category = "Crimson|SaveSystem")
	bool bResetPIESaveSlotOnStart = false;

	/** If the map being loaded in PIE is this one, the game will start normally without forcing a debug save load. */
	UPROPERTY(Config, EditAnywhere, Category = "Play In Editor", meta = (AllowedClasses = "/Script/Engine.World"))
	TSoftObjectPtr<UWorld> MainMenuMap;

	/** If set to false, you must call Load World State manually whenever you decide it is time to do so.*/
	UPROPERTY(Config, EditAnywhere, Category = "Crimson|SaveSystem")
	bool bAutoLoadWorldState = false;

	/** If set to false, you must call Save World State manually whenever you decide it is time to do so.*/
	UPROPERTY(Config, EditAnywhere, Category = "Crimson|SaveSystem")
	bool bSaveWorldStateAfterEveryChange = false;
};