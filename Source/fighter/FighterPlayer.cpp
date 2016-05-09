// Fill out your copyright notice in the Description page of Project Settings.

#include "fighter.h"
#include "FighterPlayer.h"
#include "fighterGameMode.h"
#include "FighterGameState.h"
#include "FighterAIController.h"

// Sets default values
AFighterPlayer::AFighterPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	TotalHealth = 100;
	Health = TotalHealth;
	PunchDamage = 10;
	bIsPunching = false;
	FighterPlayerState = State::Idle;

	AutoPossessAI =  EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AFighterAIController::StaticClass();

	bMovementPressed = false;
	
	//ULocalPlayer *LP = Cast<ULocalPlayer>(Controller->CastToPlayerController()->Player);
	//LP->GetControllerId;
}

// Called when the game starts or when spawned
void AFighterPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (GetActorRotation().Yaw > 0)
		bIsPlayer1 = false;
	else
		bIsPlayer1 = true;
}

// Called every frame
void AFighterPlayer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (!bIsPunching && FighterPlayerState != State::Idle  && GetCharacterMovement()->Velocity.SizeSquared() <= 0)
	{
		FighterPlayerState = State::Idle;
		LogData();
	}
}

// Called to bind functionality to input
void AFighterPlayer::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Punch", IE_Pressed, this, &AFighterPlayer::Punch);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFighterPlayer::Jump);

	InputComponent->BindAxis("MoveRight", this, &AFighterPlayer::MoveRight);
}

void AFighterPlayer::LogData()
{
	AfighterGameMode *FighterGameMode = Cast<AfighterGameMode>(GetWorld()->GetAuthGameMode());
	AFighterGameState *FighterGameState = Cast<AFighterGameState>(GetWorld()->GameState);
	AFighterPlayer *OtherPlayer = FighterGameMode->Player2;
	FColor c = FColor::Green;
	int32 PlayerNumber = 1;
	if (!bIsPlayer1)
	{
		AFighterPlayer *OtherPlayer = FighterGameMode->Player1;
		c = FColor::Red;
		PlayerNumber = 2;
	}
	if (!OtherPlayer)
		return;
	//PlayerState Distance PlayerHealth AgentState AgentHealth RemainingTime
	FString Data = FString::Printf(TEXT("%d %f %d %d %d %d"), FighterPlayerState, FighterGameMode->GetDistanceBetweenPlayers(), Health, OtherPlayer->FighterPlayerState, OtherPlayer->Health, FighterGameState->RemainingTime);
	FighterGameMode->LogData(PlayerNumber, Data);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, c, Data);
}

void AFighterPlayer::MoveRight(float Value)
{
	if (!bIsPunching && Value != 0)
	{
		if (!bMovementPressed)
		{
			bMovementPressed = true;
			if (bIsPlayer1)
			{
				if (Value > 0 && FighterPlayerState != State::Forward)
				{
					FighterPlayerState = State::Forward;
					LogData();
				}
				else if (Value < 0 && FighterPlayerState != State::Backward)
				{
					FighterPlayerState = State::Backward;
					LogData();
				}
			}
			else
			{
				if (Value < 0 && FighterPlayerState != State::Forward)
				{
					FighterPlayerState = State::Forward;
					LogData();
				}
				else if (Value > 0 && FighterPlayerState != State::Backward)
				{
					FighterPlayerState = State::Backward;
					LogData();
				}
			}
		}
		AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
	}
	else
	{
		bMovementPressed = false;
	}
}

void AFighterPlayer::Punch()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Punch Happens")));
	FighterPlayerState = State::Punching;
	bIsPunching = true;
	LogData();
	PunchEvent();
}

void AFighterPlayer::TakeDamage(int32 Damage)
{
	Health -= Damage;
	if (IsDead())
	{
		AfighterGameMode *MyGameMode = Cast<AfighterGameMode>(GetWorld()->GetAuthGameMode());
		MyGameMode->FinishMatch();
	}
}

bool AFighterPlayer::IsDead()
{
	if (Health <= 0)
		return true;
	return false;
}

void AFighterPlayer::Jump()
{
	FighterPlayerState = State::Jumping;
	LogData();
	Super::Jump();
}