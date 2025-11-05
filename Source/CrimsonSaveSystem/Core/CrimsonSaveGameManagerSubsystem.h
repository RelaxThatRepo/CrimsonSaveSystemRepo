// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fragments/CrimsonSaveGameFragmentBase.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CrimsonSaveGameManagerSubsystem.generated.h"

class ICrimsonSaveableSystem;
class UCrimsonSaveSystemSettings;

// Delegate to broadcast when the list of save games has changed (e.g., after a save or delete).
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveSlotListChanged);

// Delegate to forcefully clear save data from systems when creating/loading a new save game.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCrimsonSaveGameManager_ClearActiveSaveData);

/**
 * @brief Manages all save and load operations using a granular, fragment-based approach.
 *
 * --- OVERVIEW ---
 * This decoupled system saves data in 'fragments'. Any object can become saveable by
 * implementing the ICrimsonSaveableSystem interface and registering itself with this manager.
 * Each save slot is a directory of fragment files, one per registered system.
 *
 * @see ICrimsonSaveableSystem
 * @see UCrimsonSaveGameFragment
 *
 * --- HOW TO ADD A NEW SAVEABLE SYSTEM ---
 * 1. CREATE FRAGMENT: Make a new class inheriting from UCrimsonSaveGameFragmentBase to hold your data.
 * 2. IMPLEMENT INTERFACE: In your live data class (e.g., a Component or Subsystem), inherit from ICrimsonSaveableSystem.
 * 3. IMPLEMENT FUNCTIONS: Implement GetFragmentName(), GatherSaveData(), and RestoreFromSaveData().
 * 4. REGISTER: Call RegisterSaveableSystem() on this manager when your object is created (e.g., in BeginPlay).
 * NOTE: You must make sure UCrimsonSaveGameManagerSubsystem is already loaded in-game, for most cases this should be created
 *		 and initialized before anything else, but in the case of another UGameInstanceSubsystem needing saving, you must check
 *		 for validity.
 *
 * --- HOW TO USE FROM GAMEPLAY ---
 * Get a reference to this subsystem from the GameInstance to call these functions:
 *
 * - To Create a New Character: Call RequestSaveGame(SlotIndex, CharacterName) ONCE.
 * - To Overwrite the Full Save: Call RequestOverwriteSave() for checkpoints or manual saves.
 * - To Save Only One System: Call RequestSaveSpecificFragment(SystemToSave) for frequent updates (e.g., inventory).
 * - To Load a Game: Call RequestLoadGame(SlotIndex).
 * - To List All Saves: Call GetAllSaveSlotHeaders().
 *
 * Note: All operations are asynchronous and will not block the game thread.
 */
UCLASS(Config = Game)
class CRIMSONSAVESYSTEM_API UCrimsonSaveGameManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/**
	 * Gets the CrimsonSaveGameManagerSubsystem from a given World Context.
	 * This is the standard, easy way to access the subsystem from C++ or Blueprints.
	 */
	UFUNCTION(BlueprintPure, Category = "Crimson|SaveSystem", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static UCrimsonSaveGameManagerSubsystem* Get(const UObject* WorldContextObject);
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Any saveable system calls this to be included in save/load operations. */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void RegisterSaveableSystem(TScriptInterface<ICrimsonSaveableSystem> System);

	/** Any saveable system should call this when it's being destroyed. */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void UnregisterSaveableSystem(TScriptInterface<ICrimsonSaveableSystem> System);
	
	/** Requests to save a BRAND NEW game to the specified slot. Should only be called once per character. */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool RequestNewGameSave(int32 SlotIndex, const FString& CharacterName, FText& OutErrorMessage);

	/**
	 * C++ overload for RequestNewGameSave for backward compatibility.
	 * This version does not provide an error message and will not be visible to Blueprints.
	 */
	bool RequestNewGameSave(int32 SlotIndex, const FString& CharacterName);
	
	/** Requests to save the progress for the currently active save game. */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void RequestSaveProgress();
	
	/** Requests to load the game state from the specified slot. */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void RequestLoadFromSlot(int32 SlotIndex);

	/** Deletes all files associated with a given slot index. */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void RequestDeleteSlot(int32 SlotIndex);

	/** Returns the Header data for the currently active save slot. Returns nullptr if no game is loaded. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save System")
	UCrimsonSaveGameHeader* GetActiveSaveHeader();
	
	/** Returns the Header data for all currently existing save slots. */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	TArray<UCrimsonSaveGameHeader*> GetAllSaveSlotHeaders();

	/**
	 * Gets the user-facing slot name (e.g., "John_0") for a specific slot index.
	 * Returns an empty string if the slot doesn't exist.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save System")
	FString GetSlotNameByIndex(int32 SlotIndex) const;

	/**
	 * Requests to save only the data for a single, specific system.
	 * Ideal for high-frequency updates like inventory changes.
	 * @param SystemToSave A reference to the system (e.g., an Actor Component) that needs to be saved.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool RequestSaveSpecificFragment(TScriptInterface<ICrimsonSaveableSystem> SystemToSave);

	/** Checks if a fragment with the given name was found and loaded into the cache. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save System")
	bool HasLoadedFragment(const FString& FragmentName) const;
	
	/** Returns a loaded fragment from the cache. Returns nullptr if not found. FOR INTERNAL GAME SYSTEMS ONLY. */
	const UCrimsonSaveGameFragmentBase* GetLoadedFragment(const FString& FragmentName) const;

	/** Gets the player's last highlighted slot index from their user profile. */
	UFUNCTION(BlueprintCallable, Category = "Crimson|SaveSystem|User Settings")
	int32 GetLastSelectedSaveSlot() const;

	/** Saves the player's last highlighted slot index to their user profile. */
	UFUNCTION(BlueprintCallable, Category = "Crimson|SaveSystem|User Settings")
	void SetLastSelectedSaveSlot(int32 SlotIndex);

	/** Checks if the game should automatically load the last used save slot on startup. */
	UFUNCTION(BlueprintPure, Category = "Crimson|SaveSystem|User Settings")
	bool ShouldAutoLoadLastSave() const;

	/** Sets whether the game should automatically load the last used save slot on startup. */
	UFUNCTION(BlueprintCallable, Category = "Crimson|SaveSystem|User Settings")
	void SetShouldAutoLoadLastSave(bool bEnabled);

	/** Sets the currently active save slot index for this game session. */
	UFUNCTION(BlueprintCallable, Category = "Crimson|SaveSystem")
	void SetActiveSaveSlot(int32 NewSaveSlot);

	/** Gets the currently active save slot index. Returns -1 if no slot is active. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crimson|SaveSystem")
	int32 GetActiveSaveSlot() const;

	/** Call this when starting a brand new game to reset the playtime clock. */
	UFUNCTION(BlueprintCallable, Category = "Crimson|SaveSystem")
	void StartNewPlaytimeSession();

	/** Call this after loading a game to set the clock from the save file. */
	UFUNCTION(BlueprintCallable, Category = "Crimson|SaveSystem")
	void LoadPlaytimeFromHeader(FTimespan PreviousPlayTime);

	/** Calculates the total playtime up to this moment for the current session. */
	UFUNCTION(BlueprintCallable, Category = "Crimson|SaveSystem")
	FTimespan GetCurrentTotalPlayTime() const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnSaveSlotListChanged OnSaveSlotListChanged;
	
	// Delegate to broadcast when an async load operation has finished.
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadComplete, bool, bSuccess);
	UPROPERTY(BlueprintAssignable)
	FOnLoadComplete OnLoadComplete;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCrimsonSaveGameManager_ClearActiveSaveData);
	UPROPERTY(BlueprintAssignable)
	FCrimsonSaveGameManager_ClearActiveSaveData ClearActiveSaveData;
	
#if WITH_EDITOR
	/** Performs a synchronous load of a save slot. FOR EDITOR USE ONLY. */
	void RequestLoadGameSynchronous_EditorOnly(int32 SlotIndex);
#endif

	// -- Helper Functions --
	FString GetSlotDirectoryPath(int32 SlotIndex) const;
	FString GetHeaderSavePath(int32 SlotIndex) const;
	FString GetFragmentSavePath(int32 SlotIndex, const FString& FragmentName) const;

private:
	// -- Internal Save/Load Process --
	void StartSaveProcess(int32 InSlotIndex);
	void SaveNextFragment();
	void OnFragmentSaved(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	void FinishSaveProcess();

	void StartLoadProcess(int32 InSlotIndex);
	void LoadNextFragment();
	void OnFragmentLoaded(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedDataObject);
	void FinishLoadProcess();

	// Helper function to update the header file with the latest metadata.
	void UpdateSaveHeader();

	/** Loads the plugin's user settings from the root save folder. */
	void LoadUserPluginSettings();

	/** Saves the plugin's user settings to the root save folder. */
	void SaveUserPluginSettings();

	/** Checks user settings and attempts to auto-load the last played save slot. */
	void TryAutoLoadLastSave();

	/** Called when the initial auto-load completes. */
	UFUNCTION()
	void OnAutoLoadPreloadComplete(bool bSuccess);

private:
	
	/** The name of the save file for our user settings. */
	const FString SettingsSaveSlotName = TEXT("CrimsonSaveGamePluginSettings");

	/** The cached instance of our plugin's user settings. */
	UPROPERTY()
	TObjectPtr<UCrimsonSaveSystemSettings> UserPluginSettings;

	/** The index of the save slot that is currently loaded or being played. -1 means none. */
	int32 CurrentSlotIndex = -1;

	/** The total time played for this save slot *before* the current session began. */
	UPROPERTY()
	FTimespan TotalTimePlayedPriorToSession;

	/** The real-world time when the current game session started (either from a new game or a load). */
	UPROPERTY()
	FDateTime SessionStartTime;
	
	/** A list of all subsystems that implement the saveable interface. */
	UPROPERTY()
	TArray<TScriptInterface<ICrimsonSaveableSystem>> SaveableSystems;

	// -- State for the current save/load operation --
	bool bIsOperationPending = false;

	// Queue of fragments to be saved for the current operation.
	UPROPERTY()
	TArray<FCrimsonSaveGameDataPayload> SaveDataQueue;

	// List of fragment names to be loaded for the current operation.
	TArray<FString> LoadQueueFragmentNames;

	// A cache to hold loaded fragment data until the target system is ready for it.
	UPROPERTY()
	TMap<FString, TObjectPtr<UCrimsonSaveGameFragmentBase>> LoadedDataCache;

};