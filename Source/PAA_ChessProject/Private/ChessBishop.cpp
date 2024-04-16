// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessBishop.h"
#include "GameField.h"

// Sets default values
AChessBishop::AChessBishop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

}

UStaticMeshComponent* AChessBishop::GetStatMeshComp()
{
	return StaticMeshComponent;
}

void AChessBishop::PossibleMoves(AGameField* Field)
{
	double StartX = this->PieceGridPosition[0];
	double StartY = this->PieceGridPosition[1];
	EStatus EnemyStatus;
	TArray<FVector2D> Directions = { FVector2D(1,1), FVector2D(-1,1), FVector2D(1,-1), FVector2D(-1,-1) };
	if (this->BitColor == 0)
	{
		EnemyStatus = EStatus::BLACKOCCUPIED;
	}
	else
	{
		EnemyStatus = EStatus::WHITEOCCUPIED;
	}
	for (int32 i = 0; i < Directions.Num(); i++)
	{
		FVector2D temp(StartX, StartY);
		temp = temp + Directions[i];
		while (Field->TileMap.Find(temp) != nullptr)
		{
			ATile* PossibleNext = Field->TileMap[temp];
			if (PossibleNext->GetTileStatus() == EStatus::EMPTY)
			{
				this->Moves.Add(temp);
				temp = temp + Directions[i];
			}
			else if (PossibleNext->GetTileStatus() == EnemyStatus)
			{
				this->Moves.Add(temp);
				temp.X = StartX;
				temp.Y = StartY;
				break;
			}
			else
			{
				temp.X = StartX;
				temp.Y = StartY;
				break;
			}

		}
	}
}



// Called when the game starts or when spawned
void AChessBishop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void AChessBishop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

