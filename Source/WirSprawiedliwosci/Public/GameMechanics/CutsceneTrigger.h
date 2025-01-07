// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CutsceneTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCutsceneEndedDelegate);
class UMediaSource;
class UMediaPlayer;
class UMediaSoundComponent;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API ACutsceneTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	ACutsceneTrigger();

	UFUNCTION(BlueprintCallable)
	void HandleOverlapped(AActor* OtherActor);
	
	void PlayCutscene();
	
	UFUNCTION(BlueprintCallable)
	void HandleCutsceneEnded();

	UPROPERTY(VisibleAnywhere)
	UMediaSoundComponent* MediaSound;

	UPROPERTY(EditAnywhere)
	UMediaSource* Cutscene;

	UPROPERTY(EditAnywhere);
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMediaPlayer* MediaPlayer;

	UPROPERTY()
	bool bAlreadySeen = false;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCutsceneEndedDelegate OnCutsceneEnded;
};
