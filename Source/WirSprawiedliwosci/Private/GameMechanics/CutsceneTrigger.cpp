// Po kim ty taki chitry na prawa autorskie?!


#include "GameMechanics/CutsceneTrigger.h"
#include "Characters/MainCharacter.h"
#include "UI/DefaultHUD.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "MediaSoundComponent.h"

ACutsceneTrigger::ACutsceneTrigger()
{
	MediaSound = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("Media Sound"));
	MediaSound->SetupAttachment(GetRootComponent());
}

void ACutsceneTrigger::HandleOverlapped(AActor* OtherActor)
{
	if (OtherActor == nullptr) return;
	if (!bAlreadySeen && OtherActor->IsA(AMainCharacter::StaticClass()))
	{
		PlayCutscene();
	}
}

void ACutsceneTrigger::PlayCutscene()
{
	if (MediaPlayer && Cutscene)
	{
		MediaPlayer->Pause();
		MediaPlayer->Play();
		MediaPlayer->Close();
		MediaPlayer->OpenSource(Cutscene);
	}
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (CurrentHUD == nullptr) return;
	CurrentHUD->ShowCutscenePlayer();
	if (Material)
	{
		CurrentHUD->SetCutscenePlayerBrush(Material);
	}
	CurrentHUD->HidePartyOverlay();
}

void ACutsceneTrigger::HandleCutsceneEnded()
{
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (CurrentHUD == nullptr) return;
	CurrentHUD->HideCutscenePlayer();
	CurrentHUD->ShowPartyOverlay();
	bAlreadySeen = true;
	OnCutsceneEnded.Broadcast();
	if (MediaPlayer)
	{
		MediaPlayer->Pause();
		MediaPlayer->Close();
	}
}
