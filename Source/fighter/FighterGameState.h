// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "FighterGameState.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_API AFighterGameState : public AGameState
{
	GENERATED_BODY()

public:
	AFighterGameState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FighterGameState")
	int32 RemainingTime;
};
