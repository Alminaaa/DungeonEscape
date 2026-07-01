#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DEMainMenuWidget.generated.h"

UCLASS()
class DUNGEONESCAPE_API UDEMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Main Menu")
    void HostGame();

    UFUNCTION(BlueprintCallable, Category = "Main Menu")
    void JoinGameByIP(const FString& IPAddress);

    UFUNCTION(BlueprintCallable, Category = "Main Menu")
    void QuitGame();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main Menu")
    FString GameplayMapName = TEXT("Lvl_ThirdPerson");
};