// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CutscenePlayer.generated.h"

class UImage;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UCutscenePlayer : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ImgVideo;
};
