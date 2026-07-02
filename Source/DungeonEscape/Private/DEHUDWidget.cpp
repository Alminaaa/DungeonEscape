#include "DEHUDWidget.h"

#include "DEPlayerCharacter.h"

float UDEHUDWidget::GetHealthPercent() const
{
    const ADEPlayerCharacter* Player =
        Cast<ADEPlayerCharacter>(GetOwningPlayerPawn());

    if (!Player || Player->GetMaxHealth() <= 0.f)
    {
        return 0.f;
    }

    return Player->GetCurrentHealth() / Player->GetMaxHealth();
}

FText UDEHUDWidget::GetHealthText() const
{
    const ADEPlayerCharacter* Player =
        Cast<ADEPlayerCharacter>(GetOwningPlayerPawn());

    if (!Player)
    {
        return FText::FromString(TEXT("HP: 0 / 0"));
    }

    const FString Text =
        FString::Printf(
            TEXT("HP: %.0f / %.0f"),
            Player->GetCurrentHealth(),
            Player->GetMaxHealth());

    return FText::FromString(Text);
}

FText UDEHUDWidget::GetSpeedBoostText() const
{
    const ADEPlayerCharacter* Player =
        Cast<ADEPlayerCharacter>(GetOwningPlayerPawn());

    if (!Player || !Player->IsSpeedBoostActive())
    {
        return FText::GetEmpty();
    }

    const FString Text =
        FString::Printf(
            TEXT("+%.0f Speed"),
            Player->GetCurrentSpeedBonus());

    return FText::FromString(Text);
}

bool UDEHUDWidget::IsSpeedBoostActive() const
{
    const ADEPlayerCharacter* Player =
        Cast<ADEPlayerCharacter>(GetOwningPlayerPawn());

    return Player && Player->IsSpeedBoostActive();
}