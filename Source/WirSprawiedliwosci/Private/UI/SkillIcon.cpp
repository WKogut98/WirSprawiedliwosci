#include "UI/SkillIcon.h"
#include "UI/SkillDetails.h"
#include "Components/Image.h"
#include "GameMechanics/Skill.h"

void USkillIcon::SetNewSkill(USkill* NewSkill)
{
	Skill = NewSkill;
	SetIconToDefault();
}

void USkillIcon::SetIconToDefault()
{
	if (!Skill->IsValidLowLevel())
	{
		Icon->SetBrushFromTexture(nullptr, false);
		return;
	}
	if (Skill->SkillIcon->IsValidLowLevel())
	{
		Icon->SetBrushFromTexture(Skill->SkillIcon, false);
	}
	if ((!Skill->bUnlocked || !Skill->bUsable) && Skill->SkillIconLocked->IsValidLowLevel())
	{
		Icon->SetBrushFromTexture(Skill->SkillIconLocked, false);
	}
}

void USkillIcon::SetIconToHovered()
{
	if (Skill && Skill->SkillIconHovered->IsValidLowLevel())
	{
		Icon->SetBrushFromTexture(Skill->SkillIconHovered, false);
	}
}

void USkillIcon::SetIconToClicked()
{
	if (Skill && Skill->SkillIconClicked->IsValidLowLevel())
	{
		Icon->SetBrushFromTexture(Skill->SkillIconClicked, false);
	}
}

void USkillIcon::HandleOnHover()
{
	if (Skill == nullptr) return;
	if(Skill->bUsable) 
	{
		SetIconToHovered();
	}
	if(SkillDetails)
	{
		ShowSkillDetails();
	}
}

void USkillIcon::HandleOnEndHover()
{
	if (Skill == nullptr) return;
	SetIconToDefault();
	if(SkillDetails)
	{
		HideSkillDetails();
	}
}

void USkillIcon::HandleOnClicked()
{
	if (Skill == nullptr)
	{
		OnSkillIconClicked.Broadcast(nullptr);
		return;
	}
	if ((Skill->SkillType == ESkillType::EST_UltSupport || Skill->SkillType == ESkillType::EST_UltAttack) && !Skill->bUsable) return;
	SetIconToClicked();
	if (SkillDetails)
	{
		HideSkillDetails();
	}
	OnSkillIconClicked.Broadcast(Skill);
}

void USkillIcon::HandleOnReleased()
{
	if (Skill == nullptr) return;
	SetIconToDefault();
}

void USkillIcon::CreateSkillDetails()
{
	if (Skill == nullptr || SkillDetailsClass == nullptr) return;
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	SkillDetails = CreateWidget<USkillDetails>(Controller, SkillDetailsClass);
	SkillDetails->SetNewSkill(Skill);
}

void USkillIcon::ShowSkillDetails()
{
	if (Skill == nullptr) return;
	if (SkillDetails == nullptr)
	{
		CreateSkillDetails();
	}
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller == nullptr) return;
	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	double MouseX;
	double MouseY;
	Controller->GetMousePosition(MouseX, MouseY);
	FMath::Clamp(MouseX, 0.f, ViewportSize.X);
	FMath::Clamp(MouseY, 0.f, ViewportSize.Y);
	SkillDetails->AddToViewport();
	SkillDetails->SetPositionInViewport(FVector2D(MouseX, MouseY)); //MouseY-SkillDetails->GetDesiredSize().Y
}

void USkillIcon::HideSkillDetails()
{
	if (!SkillDetails->IsValidLowLevel() || !SkillDetails->IsInViewport()) return;
	SkillDetails->RemoveFromViewport();
}
