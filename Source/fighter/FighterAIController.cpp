// Fill out your copyright notice in the Description page of Project Settings.

#include "fighter.h"
#include "FighterAIController.h"
#include "FighterPlayer.h"
#include "FighterGameState.h"
#include "fighterGameMode.h"


AFighterAIController::AFighterAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	bCanPunchAgain = true;
}


void AFighterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AFighterPlayer *FighterPlayer = Cast<AFighterPlayer>(GetPawn());
	if (FighterPlayer)
	{
		int32 Choice = FMath::Round(Predict());
		//Choice = Choice % 3;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), Choice));
		switch (Choice)
		{
		case 0:
			if (bCanPunchAgain)
			{
				FighterPlayer->Punch();
				bCanPunchAgain = false;
			}
			break;
		case 1:
			FighterPlayer->MoveRight(-1.0f);
			break;
		case 2:
			FighterPlayer->MoveRight(1.0f);
			break;
		case 3:
			//FighterPlayer->Punch();
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