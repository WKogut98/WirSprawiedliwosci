#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ObjectWidgetComponent.generated.h"

class UInteractPrompt;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UObjectWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetPromptText(FString Text);

	UPROPERTY()
	UInteractPrompt* InteractPrompt;
};
