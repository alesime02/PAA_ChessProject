// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessRook.h"
#include "GameField.h"

// Sets default values
AChessRook::AChessRook()
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


UStaticMeshComponent* AChessRook::GetStatMeshComp()
{
	return StaticMeshComponent;
}

void AChessRook::PossibleMoves(AGameField* Field)
{
	Moves.Empty();
	double StartX = this->PieceGridPosition.X;
	double StartY = this->PieceGridPosition.Y;
	EStatus EnemyStatus;
	TArray<FVector2D> Directions = { FVector2D(1,0), FVector2D(-1,0), FVector2D(0,1), FVector2D(0,-1) };
	if (this->BitColor == 0)
	{
		EnemyStatus = EStatus::BLACKOCCUPIED;
	}
	else
	{
		EnemyStatus = EStatus::WHITEOCCUPIED;
	}
	// iterate on all the directions possible for this piece
	for (int32 i = 0; i < Directions.Num(); i++)
	{
		FVector2D temp(StartX, StartY);
		temp = temp + Directions[i];
		// explore the current direction until it find an obstacle or the end of the field
		while (Field->TileMap.Find(temp) != nullptr)
		{
			ATile* PossibleNext = Field->TileMap[temp];
			if (PossibleNext->GetTileStatus() == EStatus::EMPTY)
			{
				this->Moves.Add(temp);
				temp = temp + Directions[i];
			}
			// if it find an enemy piece
			else if (PossibleNext->GetTileStatus() == EnemyStatus)
			{
				this->Moves.Add(temp);
				// return to the starting position
				temp.X = StartX;
				temp.Y = StartY;
				break;
			}
			// if it arrives at the end of the field or it finds an ally piece
			else
			{
				// return to the starting position
				temp.X = StartX;
				temp.Y = StartY;
				break;
			}

		}
	}
}

// Called when the game starts or when spawned
void AChessRook::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void AChessRook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

