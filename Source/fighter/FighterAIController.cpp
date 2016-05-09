// Fill out your copyright notice in the Description page of Project Settings.

#include "fighter.h"
#include "FighterAIController.h"
#include "FighterPlayer.h"

AFighterAIController::AFighterAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AFighterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AFighterPlayer *FighterPlayer = Cast<AFighterPlayer>(GetPawn());
	if (FighterPlayer)
	{
		FighterPlayer->MoveRight(-1.0f);
	}
}

void AFighterAIController::BeginPlay()
{
	Super::BeginPlay();
}