

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/Array.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseEnemy.h"
#include "EnemyBlock.generated.h"

USTRUCT(BlueprintType)
struct FEnemyColumn
{
  GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<ABaseEnemy*> Enemies;
};

UENUM(BlueprintType)
enum MovementDirection
{
  Left = 0 UMETA(DisplayName = "Left"),
  Right = 1 UMETA(DUsplaceName = "Right"),
};

UCLASS()
class SPAVEINVADERS_API AEnemyBlock : public ACharacter
{
  GENERATED_BODY()

private:
  bool IsAlive = false;
  UWorld* MyWorld;
  TEnumAsByte<MovementDirection> CurrentMovementDirection;
  UCharacterMovementComponent* CharacterMovementComponent;

public:
  AEnemyBlock();

  UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Behaviour")
    float LeftBound = 0;

  UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Behaviour")
    float RightBound = 100;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
    float MovementSpeedIncrease = 50;

  UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Behaviour")
    TEnumAsByte<MovementDirection> InitialMovementDirection;

  UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Behaviour")
    float RowHeightMovement = 100;

  UPROPERTY(EditAnyWhere, BlueprintReadOnly)
    TArray<FEnemyColumn> EnemyColumns = {};

private:
  UFUNCTION(Server, Unreliable)
    void MoveBlock();

  UFUNCTION()
    void OnEnemyDestroyed(AActor* Act);

  ABaseEnemy* SpawnEnemyChild(TSubclassOf<ABaseEnemy> EnemyClass);
  void CheckIfIsAlive();

protected:
  virtual void BeginPlay() override;

public:
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable)
    TArray<FEnemyColumn> SpawnEnemies(int Columns, int Rows, float SpawnOffset, TSubclassOf<ABaseEnemy> EnemyClass);

  UFUNCTION(BlueprintCallable)
    ABaseEnemy* GetAnyBottomEnemy();
};
