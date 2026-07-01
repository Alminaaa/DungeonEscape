#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "DEGameInstance.generated.h"

UCLASS()
class DUNGEONESCAPE_API UDEGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UDEGameInstance();

    virtual void Init() override;

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void HostSession();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void FindAndJoinSession();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void DestroyCurrentSession();

protected:
    IOnlineSessionPtr SessionInterface;

    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    FName SessionName;

    FString GameplayMapName;

    void OnCreateSessionComplete(
        FName CreatedSessionName,
        bool bWasSuccessful);

    void OnFindSessionsComplete(
        bool bWasSuccessful);

    void OnJoinSessionComplete(
        FName JoinedSessionName,
        EOnJoinSessionCompleteResult::Type Result);

    void OnDestroySessionComplete(
        FName DestroyedSessionName,
        bool bWasSuccessful);

    FDelegateHandle CreateSessionCompleteDelegateHandle;
    FDelegateHandle FindSessionsCompleteDelegateHandle;
    FDelegateHandle JoinSessionCompleteDelegateHandle;
    FDelegateHandle DestroySessionCompleteDelegateHandle;
};