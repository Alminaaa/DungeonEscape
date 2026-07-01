#include "DEMeleeEnemy.h"

ADEMeleeEnemy::ADEMeleeEnemy()
{
    MaxHealth = 80.f;
    CurrentHealth = MaxHealth;
    BaseDamage = 20.f;

    DetectionRange = 1000.f;
    AttackRange = 150.f;
    AttackCooldown = 1.2f;
    EnemyMoveSpeed = 300.f;
}