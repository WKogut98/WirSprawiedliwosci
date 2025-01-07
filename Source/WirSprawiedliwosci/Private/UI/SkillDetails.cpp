#include "UI/SkillDetails.h"
#include "Components/TextBlock.h"
#include "GameMechanics/Skill.h"

void USkillDetails::SetNewSkill(USkill* NewSkill)
{
	if (NewSkill == nullptr)
	{
		SkillName->SetText(FText::FromString(TEXT("")));
		Level->SetText(FText::FromString(TEXT("")));
		Description->SetText(FText::FromString(TEXT("")));
		Quote->SetText(FText::FromString(TEXT("")));
	}
	if(!NewSkill->SkillName.IsEmpty())
	{
		SkillName->SetText(FText::FromString(NewSkill->SkillName));
	}
	if (NewSkill->Level>0)
	{
		Level->SetText(FText::FromString(TEXT("poz. ") + FString::FromInt(NewSkill->Level)));
	}
	if(!NewSkill->Description.IsEmpty())
	{
		Description->SetText(FText::FromString(NewSkill->Description));
	}
	if(!NewSkill->Quote.IsEmpty())
	{
		Quote->SetText(FText::FromString(NewSkill->Quote));
	}
}
