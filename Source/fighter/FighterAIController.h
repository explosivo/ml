// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "FighterAIController.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_API AFighterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AFighterAIController();	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
};
