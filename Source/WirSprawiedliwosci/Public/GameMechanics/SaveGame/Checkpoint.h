// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "Checkpoint.generated.h"

UCLASS()
class WIRSPRAWIEDLIWOSCI_API ACheckpoint : public ATriggerSphere
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void HandlePlayerOverlap(AActor* OtherActor);
};
