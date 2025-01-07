#include "UI/InteractPrompt.h"
#include "Components/TextBlock.h"

void UInteractPrompt::SetPromptText(FString NewText)
{
	PromptText->SetText(FText::FromString(NewText));
}
