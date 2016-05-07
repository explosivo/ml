// Fill out your copyright notice in the Description page of Project Settings.

#include "fighter.h"
#include "FighterPlayer.h"


// Sets default values
AFighterPlayer::AFighterPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
}

// Called when the game starts or when spawned
void AFighterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFighterPlayer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (PlayerState != State::Idle) // && there is no velocity )
		PlayerState = State::Idle;
}

// Called to bind functionality to input
void AFighterPlayer::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Punch", IE_Pressed, this, &AFighterPlayer::Punch);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFighterPlayer::Jump);

	InputComponent->BindAxis("MoveRight", this, &AFighterPlayer::MoveRight);
}

void AFighterPlayer::MoveRight(float Value)
{
	if (Value > 0)
		PlayerState = State::Forward;
	else if (Value < 0)
		PlayerState = State::Backward;

	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void AFighterPlayer::Punch()
{
	PlayerState = State::Punching;
}

void AFighterPlayer::Jump()
{
	PlayerState = State::Jumping;
	Super::Jump();
}