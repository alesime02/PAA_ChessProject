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

//aggiunto per i test
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

	



	/*
	// Checking the possible moves on X+
	for (double i = StartX; i < 7; i++) 
	{
		temp.X +=  1;
		ATile* Next = Field->TileMap[(temp)];
		if (Next->GetTileStatus() == EStatus::EMPTY)
		{
			this->Moves.Add(temp);
		}
		else if (Next->GetTileStatus() == EnemyStatus) 
		{
			this->Moves.Add(temp);
			i = 7;
		}
		else 
		{
			i = 7;
		}
	}
	temp.X = StartX;


	// Checking the possible moves on X-
	for (double i = StartX; i > 0; i--)
	{
		temp.X -= 1;
		ATile* Next = Field->TileMap[(temp)];
		if (Next->GetTileStatus() == EStatus::EMPTY)
		{
			this->Moves.Add(temp);
		}
		else if (Next->GetTileStatus() == EnemyStatus)
		{
			this->Moves.Add(temp);
			i = 0;
		}
		else
		{
			i = 0;
		}
	}
	temp.X = StartX;

	// Checking the possible moves on Y+
	for (double i = StartY; i < 7; i++)
	{
		temp.Y += 1;
		ATile* Next = Field->TileMap[(temp)];
		if (Next->GetTileStatus() == EStatus::EMPTY)
		{
			this->Moves.Add(temp);
		}
		else if (Next->GetTileStatus() == EnemyStatus)
		{
			this->Moves.Add(temp);
			i = 7;
		}
		else
		{
			i = 7;
		}
	}
	temp.Y = StartY;

	// Checking the possible moves on Y-
	for (double i = StartY; i > 0; i--)
	{
		temp.Y -= 1;
		ATile* Next = Field->TileMap[(temp)];
		if (Next->GetTileStatus() == EStatus::EMPTY)
		{
			this->Moves.Add(temp);
		}
		else if (Next->GetTileStatus() == EnemyStatus) 
		{
			this->Moves.Add(temp);
			i = 0;
		}
		else
		{
			i = 0;
		}
	}
}*/


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

