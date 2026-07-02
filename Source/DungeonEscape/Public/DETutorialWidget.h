#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DETutorialWidget.generated.h"

UCLASS()
class DUNGEONESCAPE_API UDETutorialWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void CloseTutorial();
};