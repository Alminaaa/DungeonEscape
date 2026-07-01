#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DungeonEscapeGameState.generated.h"

UCLASS()
class DUNGEONESCAPE_API ADungeonEscapeGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ADungeonEscapeGameState();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Game")
    int32 ActivatedLeverCount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
    int32 RequiredLeverCount;

    UPROPERTY(BlueprintReadOnly, Replicated, Category = "Game")
    bool bGameWon;

    UPROPERTY(BlueprintReadOnly, Replicated, Category = "Game")
    bool bGameLost;

public:
    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable)
    void AddActivatedLever();

    UFUNCTION(BlueprintCallable)
    void SetGameWon();

    UFUNCTION(BlueprintCallable)
    void SetGameLost();

    UFUNCTION(BlueprintPure)
    int32 GetActivatedLeverCount() const;

    UFUNCTION(BlueprintPure)
    int32 GetRequiredLeverCount() const;

    UFUNCTION(BlueprintPure)
    bool AreAllLeversActivated() const;

    UFUNCTION(BlueprintPure)
    bool IsGameWon() const;

    UFUNCTION(BlueprintPure)
    bool IsGameLost() const;
};