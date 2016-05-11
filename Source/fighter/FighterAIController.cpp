// Fill out your copyright notice in the Description page of Project Settings.

#include "fighter.h"
#include "FighterAIController.h"
#include "FighterPlayer.h"
#include "FighterGameState.h"
#include "fighterGameMode.h"
#include <cmath>


AFighterAIController::AFighterAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	bCanPunchAgain = true;
}


/*
	Tick() happens every tick, so 60ish times per second. (tick is tied to framerate in unreal engine 4)
	This is where the AIController gets the predicted state and makes the controlled character do the action is should do
*/
void AFighterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AFighterPlayer *FighterPlayer = Cast<AFighterPlayer>(GetPawn());
	if (FighterPlayer)
	{
		int32 Choice = ceil(Predict());
		//Choice = Choice % 3;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), Choice));
		switch (Choice)
		{
		case 0:
			break;
		case 1:
			FighterPlayer->MoveRight(-1.0f);
			break;
		case 2:
			if (bCanPunchAgain)
			{
				FighterPlayer->Punch();
				bCanPunchAgain = false;
			}
			FighterPlayer->MoveRight(1.0f);
			break;
		case 3:
			if (bCanPunchAgain)
			{
				FighterPlayer->Punch();
				bCanPunchAgain = false;
			}
			break;
		case 4:
			FighterPlayer->Jump();
		}
	}
}

void AFighterAIController::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(ChooserDelegate, this, &AFighterAIController::Choose, 0.3, true);
}

/*
	Predict() is used to predict what the state should be given what is happening in the game at the current tick
*/
float AFighterAIController::Predict()
{

	float result = 0;
	AFighterPlayer *FighterPlayer = Cast<AFighterPlayer>(GetPawn());
	if (FighterPlayer)
	{
		AfighterGameMode *FighterGameMode = Cast<AfighterGameMode>(GetWorld()->GetAuthGameMode());
		AFighterGameState *FighterGameState = Cast<AFighterGameState>(GetWorld()->GameState);
		AFighterPlayer *OtherPlayer = FighterGameMode->Player2;
		FColor c = FColor::Green;
		int32 PlayerNumber = 1;
		if (!FighterPlayer->bIsPlayer1)
		{
			AFighterPlayer *OtherPlayer = FighterGameMode->Player1;
		}

		float x1 = (FighterGameMode->GetDistanceBetweenPlayers() - 84) / (673 - 84.000229);
		float x2 = (FighterPlayer->Health / 100);
		float x3 = (OtherPlayer->FighterPlayerState / 4);
		float x4 = (OtherPlayer->Health / 100);

		result = 1.0634 + x1 * -0.809182 + x2 * -0.0954728 + x3 * -0.023783 + x4 * -0.074074; // dodging master
		//result = 0.961519 + x1 * -0.783637 + x2 * -0.174309 + x3 * -0.034926 + x4 * 0.0276498;
	}
	return result;
}

void AFighterAIController::Choose()
{
	bCanPunchAgain = true;
}