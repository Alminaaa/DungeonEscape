#include "DECharacterBase.h"

ADECharacterBase::ADECharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    SetReplicateMovement(true);

    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
    BaseDamage = 20.f;
}

void ADECharacterBase::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;
}

void ADECharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

float ADECharacterBase::TakeDamage(
    float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    if (DamageAmount <= 0.f)
    {
        return 0.f;
    }

    const float DamageApplied =
        FMath::Min(CurrentHealth, DamageAmount);

    CurrentHealth -= DamageApplied;

    if (CurrentHealth <= 0.f)
    {
        Die();
    }

    return DamageApplied;
}

void ADECharacterBase::Heal(float HealAmount)
{
    if (HealAmount <= 0.f)
    {
        return;
    }

    CurrentHealth =
        FMath::Clamp(
            CurrentHealth + HealAmount,
            0.f,
            MaxHealth);
}

void ADECharacterBase::Die()
{
    Destroy();
}

float ADECharacterBase::GetCurrentHealth() const
{
    return CurrentHealth;
}

float ADECharacterBase::GetMaxHealth() const
{
    return MaxHealth;
}

void ADECharacterBase::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ADECharacterBase, CurrentHealth);
}