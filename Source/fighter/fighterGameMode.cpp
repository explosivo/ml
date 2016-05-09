// Fill out your copyright notice in the Description page of Project Settings.

#include "fighter.h"
#include "fighterGameMode.h"
#include "FighterGameState.h"
#include "FighterPlayer.h"
#include "FighterAIController.h"
#include "Blueprint/UserWidget.h"
#include "GameUIWidget.h"
#include "Logger.h"

AfighterGameMode::AfighterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player"));
	static ConstructorHelpers::FClassFinder<UUserWidget> GameUIClass(TEXT("/Game/GameUI"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	GameStateClass = AFighterGameState::StaticClass();

	wGameUI = GameUIClass.Class;

	bUsingAI = false;

	logger = new Logger();

	RoundTime = 30;
	TimeBetweenMatches = 5;
}
AfighterGameMode::~AfighterGameMode()
{
	delete logger;
}

AActor* AfighterGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	APlayerController *PC = Cast<APlayerController>(Player);
	int32 controllerId = 0;
	if (PC != NULL)
	{
		controllerId = PC->PlayerState->PlayerId - GetWorld()->GetFirstPlayerController()->PlayerState->PlayerId;
	}
	APlayerStart* ChosenStart = NULL;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* TestSpawn = *It;
		if (controllerId == 0)
		{
			if (TestSpawn->PlayerStartTag == FName(TEXT("Player1")))
			{
				ChosenStart = TestSpawn;
			}
		}
		else
		{
			if (TestSpawn->PlayerStartTag == FName(TEXT("Player2")))
			{
				ChosenStart = TestSpawn;
			}
		}
	}

	return ChosenStart ? ChosenStart : Super::ChoosePlayerStart(Player);
}

void AfighterGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (!GameUI)
	{
		if (wGameUI)
		{
			GameUI = Cast<UGameUIWidget>(CreateWidget<UUserWidget>(GetWorld(),  wGameUI));
			GameUI->AddToViewport();
		}
	}

	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AfighterGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void AfighterGameMode::CreateBot()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Name = FName(TEXT("Bot"));
	SpawnInfo.Instigator = nullptr;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = nullptr;

	APlayerStart* ChosenStart = NULL;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if ((*It)->PlayerStartTag == FName(TEXT("Player2")))
		{
				ChosenStart = *It;
				break;
		}
	}

	Bot = GetWorld()->SpawnActor<AFighterPlayer>(DefaultPawnClass, ChosenStart->GetActorLocation(), ChosenStart->GetActorRotation(), SpawnInfo);
	Bot->SpawnDefaultController();

	Player2 = Bot;
	//AFighterAIController *FighterAIC = GetWorld()->SpawnActor<AFighterAIController>(SpawnInfo);
	//FighterAIC->Possess(Bot);
	//Bot->SpawnDefaultController();
}

/** update remaining time */
void AfighterGameMode::DefaultTimer()
{
	AFighterGameState* const MyGameState = Cast<AFighterGameState>(GameState);
	if (MyGameState && MyGameState->RemainingTime)
	{
		MyGameState->RemainingTime--;
		if (MyGameState->RemainingTime <= 0)
		{
			if (GetMatchState() == MatchState::WaitingPostMatch)
			{
				RestartGame();
			}
			else if (GetMatchState() == MatchState::InProgress)
			{
				FinishMatch();
			}
			else if (GetMatchState() == MatchState::WaitingToStart)
			{
				StartMatch();
			}
		}
	}
}

/** called before startmatch */
void AfighterGameMode::HandleMatchIsWaitingToStart()
{
	if (!bUsingAI)
	{
		FString Error;
		ULocalPlayer *LP = GetWorld()->GetGameInstance()->CreateLocalPlayer(1, Error, true);
		Player2 = Cast<AFighterPlayer>(*(GetWorld()->GetPawnIterator() + 1));
	}
	else
		CreateBot();
}

/** starts new match */
void AfighterGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	Player1 = Cast<AFighterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	GameUI->MatchStart();
	AFighterGameState* const MyGameState = Cast<AFighterGameState>(GameState);
	MyGameState->RemainingTime = RoundTime;
}

void AfighterGameMode::FinishMatch()
{
	AFighterGameState* const MyGameState = Cast<AFighterGameState>(GameState);
	if (IsMatchInProgress())
	{
		EndMatch();
		DetermineWinner();
		if (Winner == "Player1")
		{
			logger->Flush(1);
		}
		else if (Winner == "Player2")
		{
			logger->Flush(2);
		}
		// if tie flush both?


		// lock all pawns
		// pawns are not marked as keep for seamless travel, so we will create new pawns on the next match rather than
		// turning these back on.
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}

		// set up to restart the match
		GameUI->MatchEnd();
		MyGameState->RemainingTime = TimeBetweenMatches;
	}
}

/** hides the onscreen hud and restarts the map */
void AfighterGameMode::RestartGame()
{
	Super::RestartGame();
}

void AfighterGameMode::DetermineWinner()
{
	int32 PlayerNumber = 1;
	int32 WinningNumber = PlayerNumber;
	int32 HighestHealth = -1;
	if (!bUsingAI)
	{
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			AFighterPlayer *FighterPlayer = Cast<AFighterPlayer>(*It);
			if (FighterPlayer->Health == HighestHealth)
			{
				Winner = FString::Printf(TEXT("Nobody"));
				break;
			}
			else if (FighterPlayer->Health > HighestHealth)
			{
				WinningNumber = PlayerNumber;
				HighestHealth = FighterPlayer->Health;
			}
			PlayerNumber++;
		}
		Winner = FString::Printf(TEXT("Player%d"), WinningNumber);
	}
	else
	{
		AFighterPlayer *FighterPlayer = Cast<AFighterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (FighterPlayer->Health == Bot->Health)
			Winner = FString::Printf(TEXT("Nobody"));
		else if (FighterPlayer->Health > Bot->Health)
			Winner = FString::Printf(TEXT("Player1"));
		else
			Winner = FString::Printf(TEXT("Player2"));
	}
}

float AfighterGameMode::GetDistanceBetweenPlayers()
{
	float Distance = 0.f;
	if (Player1 && Player2)
	{
		float y1 = Player1->GetActorLocation().Y;
		float z1 = Player1->GetActorLocation().Z;
		float y2 = Player2->GetActorLocation().Y;
		float z2 = Player2->GetActorLocation().Z;
		Distance = FGenericPlatformMath::Sqrt(FGenericPlatformMath::Pow(y1 - y2, 2) + FGenericPlatformMath::Pow(z1 - z2, 2));
	}
	return Distance;
}

void AfighterGameMode::LogData(int32 PlayerNumber, FString Data)
{
	logger->AddData(PlayerNumber, TCHAR_TO_ANSI(*Data));
}