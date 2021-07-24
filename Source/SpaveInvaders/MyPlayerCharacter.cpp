

#include "MyPlayerCharacter.h"

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	CharacterMovementComponent->GravityScale = 0;
	CharacterMovementComponent->MovementMode = EMovementMode::MOVE_Flying;
	auto ActorXLocation = GetActorLocation().X;
	auto MyCapsuleComponent = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
	auto CapsuleRadius = MyCapsuleComponent->GetScaledCapsuleRadius();
	LeftBound = ActorXLocation - CapsuleRadius;
	RightBound = ActorXLocation + CapsuleRadius;
}

void AMyPlayerCharacter::PrintString(FString Text)
{
	if (!GEngine)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, Text);
}

// Called when the game starts or when spawned
void AMyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPlayerCharacter::MoveByAxis(float AxisValue)
{
	InputAxisValue = AxisValue;
	AddMovementInput(FVector(AxisValue * MovementSpeed, 0, 0));
}

void AMyPlayerCharacter::CheckLocation()
{
	auto CurrentLocation = GetActorLocation();

	if (CurrentLocation.X + RightBound > RightAreaBound)
	{
		FVector NewLocation = FVector(RightAreaBound - RightBound, CurrentLocation.Y, CurrentLocation.Z);
		SetActorLocation(NewLocation);
	}
	else if (CurrentLocation.X - LeftBound < LeftAreaBound)
	{
		FVector NewLocation = FVector(LeftAreaBound + LeftBound, CurrentLocation.Y, CurrentLocation.Z);
		SetActorLocation(NewLocation);
	}
}

// Called every frame
void AMyPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckLocation();
}

// Called to bind functionality to input
void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
