

#include "MyPlayer.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

void AMyPlayer::PrintString(FString Text)
{
	if (!GEngine)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, Text);
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent *> Components;
	Components = GetComponentsByClass(UShapeComponent::StaticClass());

	float ActorXLocation = GetActorLocation().X;

	for (auto *Component : Components)
	{
		auto ShapeComponent = Cast<UShapeComponent>(Component);
		auto ComponentTransform = ShapeComponent->GetComponentTransform();
		auto bounds = ShapeComponent->CalcBounds(ComponentTransform);
		float ShapeLeftBound = bounds.Origin.X - bounds.SphereRadius - ActorXLocation;
		float ShapeRightBound = bounds.Origin.X + bounds.SphereRadius - ActorXLocation;

		if (ShapeRightBound > RightBound)
			RightBound = ShapeRightBound;

		if (ShapeLeftBound < LeftBound)
			LeftBound = ShapeLeftBound;
	}
}

void AMyPlayer::MoveByAxis(float AxisValue)
{
	InputAxisValue = AxisValue;
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Location = GetActorLocation();
	float NewLocation = Location.X + (InputAxisValue * MovementSpeed * DeltaTime);
	if (NewLocation + RightBound > RightAreaBound)
		NewLocation = RightAreaBound + LeftBound;
	else if (NewLocation + LeftBound < LeftAreaBound)
		NewLocation = LeftAreaBound + RightBound; 

	Location.X = NewLocation;
	SetActorLocation(Location);
}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
