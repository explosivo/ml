// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_API UGameUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "FighterGameUI")
	void MatchStart();

	UFUNCTION(BlueprintImplementableEvent, Category = "FighterGameUI")
	void MatchEnd();
	
};
