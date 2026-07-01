#include "DEBossEnemy.h"

ADEBossEnemy::ADEBossEnemy()
{
    MaxHealth = 350.f;
    CurrentHealth = MaxHealth;
    BaseDamage = 40.f;

    DetectionRange = 1800.f;
    AttackRange = 200.f;
    AttackCooldown = 1.5f;
    EnemyMoveSpeed = 220.f;

    BossScale = 1.7f;
}

void ADEBossEnemy::BeginPlay()
{
    Super::BeginPlay();

    SetActorScale3D(
        FVector(
            BossScale,
            BossScale,
            BossScale));

    OnBossSpawned();
}