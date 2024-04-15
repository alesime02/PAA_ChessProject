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

	/*RookGridPosition = FVector2D(0, 0); */
}

//aggiunto per i test
UStaticMeshComponent* AChessRook::GetStatMeshComp()
{
	return StaticMeshComponent;
}
void AChessRook::PossibleMoves(AGameField* Field)
{
	double StartX = this->PieceGridPosition[0];
	double StartY = this->PieceGridPosition[1];
	ETileStatus Status;
	FVector2D temp(StartX, StartY);
	if (this->BitColor == 0) 
	{
		Status = ETileStatus::BLACKOCCUPIED;
	}
	else 
	{
		Status = ETileStatus::WHITEOCCUPIED;
	}
	// Checking the possible moves on X+
	for (double i = StartX; i < 7; i++) 
	{
		temp.X +=  1;
		ATile* Next = Field->TileMap[(temp)];
		if (Next->GetTileStatus() == ETileStatus::EMPTY) 
		{
			this->Moves.Add(temp);
		}
		else if (Next->GetTileStatus() == Status) 
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
		if (Next->GetTileStatus() == ETileStatus::EMPTY || Next->GetTileStatus() == Status)
		{
			this->Moves.Add(temp);
		}
		else if (Next->GetTileStatus() == Status)
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

	// Checking teh possible moves on Y+
	for (double i = StartY; i < 7; i++)
	{
		temp.Y += 1;
		ATile* Next = Field->TileMap[(temp)];
		if (Next->GetTileStatus() == ETileStatus::EMPTY || Next->GetTileStatus() == Status)
		{
			this->Moves.Add(temp);
		}
		else if (Next->GetTileStatus() == Status)
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
		if (Next->GetTileStatus() == ETileStatus::EMPTY || Next->GetTileStatus() == Status)
		{
			this->Moves.Add(temp);
		}
		else if (Next->GetTileStatus() == Status)
		{
			this->Moves.Add(temp);
			i = 0;
		}
		else
		{
			i = 0;
		}
	}
	temp.Y = StartY;
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

