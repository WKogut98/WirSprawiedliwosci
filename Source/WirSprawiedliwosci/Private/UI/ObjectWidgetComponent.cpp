#include "UI/ObjectWidgetComponent.h"
#include "UI/InteractPrompt.h"

void UObjectWidgetComponent::SetPromptText(FString Text)
{
	if(InteractPrompt)
	{
		InteractPrompt->SetPromptText(Text);
	}
}
