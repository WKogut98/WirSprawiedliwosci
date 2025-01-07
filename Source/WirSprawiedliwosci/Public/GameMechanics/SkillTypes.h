#pragma once
UENUM(BlueprintType)
enum class ESkillType : uint8
{
	EST_Attack UMETA(DisplayName = "Atak"),
	EST_Heal UMETA(DisplayName = "Leczenie"),
	EST_Support UMETA(DisplayName = "Wsparcie"),
	EST_AttackAlly UMETA(DisplayName = "Atak na Sojusznika"),
	EST_UltAttack UMETA(DisplayName = "Ult Atak"),
	EST_UltSupport UMETA(DisplayName = "Ult Wsparcie"),
	EST_Summon UMETA(DisplayName = "Przyzwanie")
};

UENUM(BlueprintType)
enum class ESkillSpecialProperty : uint8
{
	EST_None UMETA(DisplayName = "Brak"),
	EST_UsesRandomFood UMETA(DisplayName = "Zuzywa Losowe Jedzenie"),
	EST_MakesTargetInvisible UMETA(DisplayName = "Powoduje Znikniecie Celu")
};


UENUM(BlueprintType)
enum class EEffectTurnMode : uint8
{
	EETM_EndOfTurn UMETA(DisplayName = "Na Koniec Tury"),
	EETM_BuffDebuff UMETA(DisplayName = "Buff/Debuff"),
	EETM_Passive UMETA(DisplayName = "Pasywny"),
	EETM_Other UMETA(DisplayName = "Inny")
};

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	EET_Oppresion UMETA(DisplayName = "Ciemiezenie"),
	EET_Blessing UMETA(DisplayName = "Blogoslawienstwo"),
	EET_Justice UMETA(DisplayName = "Sprawiedliwosc"),
	EET_Drunk UMETA(DisplayName = "Najebanie"),
	EET_Defender UMETA(DisplayName = "Obronca"),
	EET_DefensiveWall UMETA(DisplayName = "Mur"),
	EET_Counter UMETA(DisplayName = "Kontra"),
	EET_Stink UMETA(DisplayName = "Smrud"),
	EET_Disgrace UMETA(DisplayName = "Kompromitacja Cwela"),
	EET_Deathwish UMETA(DisplayName = "Zyczenie Smierci"),
	EET_Tired UMETA(DisplayName = "Zamulenie"),
	EET_Boost UMETA(DisplayName = "Spid")
};