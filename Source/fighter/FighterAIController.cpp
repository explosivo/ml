// Fill out your copyright notice in the Description page of Project Settings.

#include "fighter.h"
#include "FighterAIController.h"
#include "FighterPlayer.h"
#include "FighterGameState.h"
#include "fighterGameMode.h"
#include "NNManager.h"


AFighterAIController::AFighterAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	bCanPunchAgain = true;
	NNM = new NNManager();
}

AFighterAIController::~AFighterAIController()
{
	delete NNM;
}

void AFighterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AFighterPlayer *FighterPlayer = Cast<AFighterPlayer>(GetPawn());
	AfighterGameMode *FighterGameMode = Cast<AfighterGameMode>(GetWorld()->GetAuthGameMode());
	AFighterGameState *FighterGameState = Cast<AFighterGameState>(GetWorld()->GameState);
	AFighterPlayer *OtherPlayer = FighterGameMode->Player2;
	FColor c = FColor::Green;
	int32 PlayerNumber = 1;
	if (!FighterPlayer->bIsPlayer1)
	{
		AFighterPlayer *OtherPlayer = FighterGameMode->Player1;
	}

	if (FighterPlayer)
	{
		float x1 = FighterGameMode->GetDistanceBetweenPlayers();
		float x2 = OtherPlayer->FighterPlayerState;

		int32 Choice = (int32)NNM->Predict(x1, x2);
		switch (Choice)
		{
		case 0:
			break;
		case 1:
			FighterPlayer->MoveRight(-1.0f);
			break;
		case 2:
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
	GetWorldTimerManager().SetTimer(ChooserDelegate, this, &AFighterAIController::Choose, 0.1, true);
}

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

		float x1 = (FighterGameMode->GetDistanceBetweenPlayers() - 84.000229) / (540 - 84.000229);
		float x2 = (FighterPlayer->Health - 0) / (100 - 0);
		float x3 = (OtherPlayer->FighterPlayerState - 0) / 4;
		float x4 = (OtherPlayer->Health - 0) / (100 - 0);
		float x5 = (FighterGameState->RemainingTime - 13) / (30 - 13);

		result = 0.423623 + x1 * -0.120971 + x2 * -0.525907 + x3 * 14.9564 + x4 * 1.06035;
	}
	return result;
}

void AFighterAIController::Choose()
{
	bCanPunchAgain = true;
}