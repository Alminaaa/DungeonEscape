#pragma once

#include "CoreMinimal.h"
#include "DECharacterBase.h"
#include "InputActionValue.h"
#include "DEPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class DUNGEONESCAPE_API ADEPlayerCharacter : public ADECharacterBase
{
    GENERATED_BODY()

public:
    ADEPlayerCharacter();

protected:
    virtual void BeginPlay() override;

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* Camera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* SprintAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* DashAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* AttackAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* InteractAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float WalkSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float SprintSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float DashStrength;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    float InteractionDistance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float AttackRadius;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float AttackCooldown;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bCanAttack;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsSprinting;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bSpeedBoostActive;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float CurrentSpeedBonus;

    FTimerHandle AttackCooldownTimerHandle;
    FTimerHandle SpeedBoostTimerHandle;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartSprint();
    void StopSprint();

    void Dash();

    void Attack();
    void ServerAttackLogic();
    void ResetAttack();

    void Interact();
    void ServerInteractLogic();

    AActor* FindFocusedInteractable() const;

    void ClearSpeedBoost();
    void UpdateMovementSpeed();

    UFUNCTION(Server, Reliable)
    void Server_Attack();

    UFUNCTION(Server, Reliable)
    void Server_Interact();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnPlayerAttack();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnPlayerHitEnemy(AActor* HitEnemy);

    UFUNCTION(BlueprintImplementableEvent, Category = "Player")
    void OnPlayerDeath();

public:
    virtual void Die() override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void AddSpeedBoost(float BoostAmount, float Duration);

    UFUNCTION(BlueprintPure, Category = "Movement")
    bool IsSpeedBoostActive() const;

    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetCurrentSpeedBonus() const;
};