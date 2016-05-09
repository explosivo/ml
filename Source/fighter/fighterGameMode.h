// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "fighterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_API AfighterGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AfighterGameMode();
	~AfighterGameMode();
	TSubclassOf<class UUserWidget> wGameUI;
	class UGameUIWidget* GameUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FighterGameMode")
	FString Winner;

	/** Handle for efficient management of DefaultTimer timer */
	FTimerHandle TimerHandle_DefaultTimer;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	int32 RoundTime;
	int32 TimeBetweenMatches;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FighterGameMode")
	bool bUsingAI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FighterGameMode")
	class AFighterPlayer *Bot;

	class AFighterPlayer *Player1;
	class AFighterPlayer *Player2;

	virtual void PreInitializeComponents() override;
	void CreateBot();
	/** update remaining time */
	virtual void DefaultTimer();
	/** called before startmatch */
	virtual void HandleMatchIsWaitingToStart() override;
	/** starts new match */
	virtual void HandleMatchHasStarted() override;
	/** hides the onscreen hud and restarts the map */
	void FinishMatch();
	virtual void RestartGame() override;
	void DetermineWinner();

	class Logger *logger;

	void LogData(int32 PlayerNumber, FString Data);

	float GetDistanceBetweenPlayers();
};
