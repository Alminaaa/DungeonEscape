#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DEPauseWidget.generated.h"

UCLASS()
class DUNGEONESCAPE_API UDEPauseWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Pause")
    void ResumeGame();

    UFUNCTION(BlueprintCallable, Category = "Pause")
    void ReturnToMainMenu();

    UFUNCTION(BlueprintCallable, Category = "Pause")
    void QuitGame();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pause")
    FName MainMenuMapName = TEXT("MainMenuMap");
};