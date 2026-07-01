#include "DungeonEscapeGameState.h"

#include "Net/UnrealNetwork.h"

ADungeonEscapeGameState::ADungeonEscapeGameState()
{
    ActivatedLeverCount = 0;
    RequiredLeverCount = 3;
    bGameWon = false;
    bGameLost = false;

    bReplicates = true;
}

void ADungeonEscapeGameState::AddActivatedLever()
{
    if (!HasAuthority() || bGameWon || bGameLost)
    {
        return;
    }

    ActivatedLeverCount++;
}

void ADungeonEscapeGameState::SetGameWon()
{
    if (!HasAuthority() || bGameLost)
    {
        return;
    }

    bGameWon = true;
}

void ADungeonEscapeGameState::SetGameLost()
{
    if (!HasAuthority() || bGameWon)
    {
        return;
    }

    bGameLost = true;
}

int32 ADungeonEscapeGameState::GetActivatedLeverCount() const
{
    return ActivatedLeverCount;
}

int32 ADungeonEscapeGameState::GetRequiredLeverCount() const
{
    return RequiredLeverCount;
}

bool ADungeonEscapeGameState::AreAllLeversActivated() const
{
    return ActivatedLeverCount >= RequiredLeverCount;
}

bool ADungeonEscapeGameState::IsGameWon() const
{
    return bGameWon;
}

bool ADungeonEscapeGameState::IsGameLost() const
{
    return bGameLost;
}

void ADungeonEscapeGameState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ADungeonEscapeGameState, ActivatedLeverCount);
    DOREPLIFETIME(ADungeonEscapeGameState, bGameWon);
    DOREPLIFETIME(ADungeonEscapeGameState, bGameLost);
}