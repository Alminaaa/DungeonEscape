#include "DEEnemyHealthWidget.h"

#include "DEEnemyBase.h"

void UDEEnemyHealthWidget::InitializeEnemy(ADEEnemyBase* Enemy)
{
    OwningEnemy = Enemy;
}

float UDEEnemyHealthWidget::GetHealthPercent() const
{
    if (!OwningEnemy || OwningEnemy->GetMaxHealth() <= 0.f)
    {
        return 0.f;
    }

    return OwningEnemy->GetCurrentHealth() / OwningEnemy->GetMaxHealth();
}

FText UDEEnemyHealthWidget::GetHealthText() const
{
    if (!OwningEnemy)
    {
        return FText::FromString(TEXT("0 / 0"));
    }

    const FString Text =
        FString::Printf(
            TEXT("%.0f / %.0f"),
            OwningEnemy->GetCurrentHealth(),
            OwningEnemy->GetMaxHealth());

    return FText::FromString(Text);
}