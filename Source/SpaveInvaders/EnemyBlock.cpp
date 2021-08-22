

#include "EnemyBlock.h"

AEnemyBlock::AEnemyBlock()
{
  PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBlock::BeginPlay()
{
  Super::BeginPlay();
  MyWorld = GetWorld();
  CurrentMovementDirection = InitialMovementDirection;
  IsAlive = true;
  CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
  CharacterMovementComponent->GravityScale = 0;
  CharacterMovementComponent->MovementMode = EMovementMode::MOVE_Flying;
}

void AEnemyBlock::MoveBlock_Implementation()
{
  FVector Origin, Extend;
  GetActorBounds(true, Origin, Extend, true);
  auto Location = GetActorLocation();
  auto BoundsOffset = FVector(
    Location.X - Origin.X + Extend.X,
    Location.Y - Origin.Y + Extend.Y,
    Location.Z - Origin.Z + Extend.Z);

  auto BoundHit = false;
  FVector NewLocation;

  if (CurrentMovementDirection == Left)
  {
    float LeftPosition = Origin.X - Extend.X;
    if (LeftPosition <= LeftBound)
    {
      CurrentMovementDirection = MovementDirection::Right;
      NewLocation = FVector(LeftPosition - BoundsOffset.X, Location.Y, Location.Z - RowHeightMovement);
      BoundHit = true;
    }
  }
  else if (CurrentMovementDirection == Right)
  {
    float RightPosition = Origin.X + Extend.X;
    if (RightPosition >= RightBound)
    {
      CurrentMovementDirection = MovementDirection::Left;
      NewLocation = FVector(RightPosition - (2 * Extend.X), Location.Y, Location.Z - RowHeightMovement);
      BoundHit = true;
    }
  }

  if (BoundHit)
  {
    CharacterMovementComponent->StopMovementImmediately();
    SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);
    CharacterMovementComponent->MaxFlySpeed += MovementSpeedIncrease;
  }

  float Movement = (CurrentMovementDirection == Left) ? -1 : 1;
  AddMovementInput(FVector(Movement, 0, 0));
}

void AEnemyBlock::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  if (GetNetMode() == ENetMode::NM_Client)
    return;

  if (!IsAlive || MyWorld->IsPaused())
    return;

  if (GetNetMode() != ENetMode::NM_Client)
    MoveBlock_Implementation();
}

ABaseEnemy* AEnemyBlock::SpawnEnemyChild(TSubclassOf<ABaseEnemy> EnemyClass)
{
  auto EnemyActor = GetWorld()->SpawnActor(EnemyClass);
  auto AttRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
  EnemyActor->AttachToActor(this, AttRules);
  return Cast<ABaseEnemy>(EnemyActor);
}

void AEnemyBlock::CheckIfIsAlive()
{
  IsAlive = EnemyColumns.Num() > 0;
}

void AEnemyBlock::OnEnemyDestroyed(AActor* Act)
{
  for (auto X = 0; X < EnemyColumns.Num(); X++)
    for (auto Y = 0; Y < EnemyColumns[X].Enemies.Num(); Y++) {
      if (EnemyColumns[X].Enemies[Y] != Act)
        continue;

      EnemyColumns[X].Enemies.RemoveAt(Y, 1, true);

      if (EnemyColumns[X].Enemies.Num() == 0) {
        EnemyColumns.RemoveAt(X, 1, true);
        CheckIfIsAlive();
      }

      return;
    }
}

TArray<FEnemyColumn> AEnemyBlock::SpawnEnemies(int Columns, int Rows, float SpawnOffset, TSubclassOf<ABaseEnemy> EnemyClass)
{
  for (auto Column : this->EnemyColumns)
    for (auto Enemy : Column.Enemies)
      Enemy->Destroy();

  this->EnemyColumns = {};

  for (int X = 0; X < Columns; X++)
  {
    FEnemyColumn EnemyColumn = FEnemyColumn();

    for (int Z = 0; Z < Rows; Z++)
    {
      auto Enemy = SpawnEnemyChild(EnemyClass);
      FScriptDelegate Delegate;
      Delegate.BindUFunction(this, "OnEnemyDestroyed");
      Enemy->OnDestroyed.AddUnique(Delegate);

      EnemyColumn.Enemies.Add(Enemy);
      FVector ActorOrigin = FVector();
      FVector ActorBoxExtend = FVector();
      Enemy->GetActorBounds(false, ActorOrigin, ActorBoxExtend);
      ActorOrigin.X = SpawnOffset * X;
      ActorOrigin.Y = 0;
      ActorOrigin.Z = SpawnOffset * -1 * Z;
      Enemy->SetActorRelativeLocation(ActorOrigin);
    }

    EnemyColumns.Add(EnemyColumn);
  }

  CheckIfIsAlive();

  return EnemyColumns;
}

ABaseEnemy* AEnemyBlock::GetAnyBottomEnemy()
{
  if (!IsAlive)
    return NULL;

  auto ColumnIndex = FMath::RandRange(0, EnemyColumns.Num() - 1);
  auto RowIndex = FMath::RandRange(0, EnemyColumns.Num() - 1);
  return EnemyColumns[ColumnIndex].Enemies[RowIndex];
}