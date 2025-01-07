// Po kim ty taki chitry na prawa autorskie?!


#include "GameMechanics/SaveGame/Checkpoint.h"
#include "GameMechanics/GameModes/DefaultGameInstance.h"
#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

void ACheckpoint::HandlePlayerOverlap(AActor* OtherActor)
{
	if (!(OtherActor->IsA(AMainCharacter::StaticClass()))) return;
	UDefaultGameInstance* CurrentInstance = Cast<UDefaultGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (CurrentInstance == nullptr) return;
	CurrentInstance->SaveGame();
}
