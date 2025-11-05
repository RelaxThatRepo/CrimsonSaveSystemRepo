
#pragma once

#include "UObject/Interface.h"
#include "ICrimsonSaveableActor.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UCrimsonSaveableActor : public UInterface
{
	GENERATED_BODY()
};

class ICrimsonSaveableActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="Crimson|SaveSystem")
	FGuid GetSaveID() const;
	
	UFUNCTION(BlueprintNativeEvent, Category="Crimson|SaveSystem")
	void ResetToDefaultState();

	UFUNCTION(BlueprintNativeEvent, Category="Crimson|SaveSystem")
	void RestoreFromSaveData(UObject* ActorData);
};