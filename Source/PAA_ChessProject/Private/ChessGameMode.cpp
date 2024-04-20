// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameMode.h"
#include "ChessPlayerController.h"
#include "HumanPlayer.h"
#include "RandomPlayer.h"
//#include "MinimaxPlayer.h"
#include "EngineUtils.h"


AChessGameMode::AChessGameMode()
{
	PlayerControllerClass = AChessPlayerController::StaticClass();
	DefaultPawnClass = AHumanPlayer::StaticClass();
}

void AChessGameMode::BeginPlay()
{
	Super::BeginPlay();

	AHumanPlayer* HumanPlayer = Cast<AHumanPlayer>(*TActorIterator<AHumanPlayer>(GetWorld()));
	
	int32 FieldSize;
	if (GameFieldClass != nullptr)
	{
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
		FieldSize = GField->Size;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}

	float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
	FVector CameraPos(CameraPosX, CameraPosX, 1000.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	// Human player = 0
	Players.Add(HumanPlayer);
	// Random Player
	auto* AI = GetWorld()->SpawnActor<ARandomPlayer>(FVector(), FRotator());

	// MiniMax Player
	//auto* AI = GetWorld()->SpawnActor<ATTT_MinimaxPlayer>(FVector(), FRotator());

	// AI player = 1
	Players.Add(AI);

	this->ChoosePlayerAndStartGame();
}

void AChessGameMode::ChoosePlayerAndStartGame()
{
	CurrentPlayer = 0;

	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
	}
	Players[CurrentPlayer]->OnTurn();
}

int32 AChessGameMode::GetNextPlayer(int32 Player)
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}

void AChessGameMode::TurnNextPlayer()
{
	if (CheckMate) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Scacco Matto"));
		Players[CurrentPlayer]->OnWin();
		for (int32 i = 0; i < Players.Num(); i++)
		{
			if (i != CurrentPlayer)
			{
				Players[i]->OnLose();
			}
		}
	}
	else if (Pair)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Patta"));
	}
	else
	{
		CurrentPlayer = GetNextPlayer(CurrentPlayer);
		Players[CurrentPlayer]->OnTurn();
	}
}

void AChessGameMode::FilterIllegals(APiece* Current)
{
	if (Current->BitColor == 0)
	{
		FVector2D KingPosition = GField->WhiteKing->PieceGridPosition;
		bool MovingKing = 0;
		if (Current->PieceGridPosition == KingPosition)
		{
			MovingKing = 1;
		}
		TArray<FVector2D> ActualMoves = Current->Moves;
		FVector2D ToNotCount(-1, -1);
		// scorro le mosse possibili della pedina che ho cliccato
		for (auto i : Current->Moves)
		{
			ATile* Start = GField->TileMap[Current->PieceGridPosition];
			//simulo la mossa
			if (MovingKing)
			{
				KingPosition = i;
			}
			Start->SetTileStatus(EStatus::EMPTY);
			ATile* End = GField->TileMap[i];
			EStatus Previous = End->GetTileStatus();
			if (Previous == EStatus::BLACKOCCUPIED)
			{
				ToNotCount = End->GetGridPosition();
			}
			End->SetTileStatus(EStatus::WHITEOCCUPIED);
			//calcolo contromosse dei pezzi nemici
			bool loopBreaker = false;
			for (auto Piece : GField->BPieceInGame)
			{
				if (loopBreaker)
				{
					break;
				}
				if (Piece->PieceGridPosition == ToNotCount)
				{
					continue;
				}
				Piece->PossibleMoves(GField);
				for (auto EnemyMove : Piece->Moves)
				{
					if (EnemyMove == KingPosition)
					{
						ActualMoves.Remove(i);
						loopBreaker = true;
						break;
					}
				}

			}
			Start->SetTileStatus(EStatus::WHITEOCCUPIED);
			End->SetTileStatus(Previous);
			ToNotCount.X = -1;
			ToNotCount.Y = -1;
		}
		Current->Moves = ActualMoves;
	}

	else
	{
		FVector2D KingPosition = GField->BlackKing->PieceGridPosition;
		bool MovingKing = 0;
		if (Current->PieceGridPosition == KingPosition)
		{
			MovingKing = 1;
		}
		TArray<FVector2D> ActualMoves = Current->Moves;
		FVector2D ToNotCount(-1, -1);
		// scorro le mosse possibili della pedina che ho cliccato
		for (auto i : Current->Moves)
		{
			ATile* Start = GField->TileMap[Current->PieceGridPosition];
			//simulo la mossa
			if (MovingKing)
			{
				KingPosition = i;
			}
			Start->SetTileStatus(EStatus::EMPTY);
			ATile* End = GField->TileMap[i];
			EStatus Previous = End->GetTileStatus();
			if (Previous == EStatus::WHITEOCCUPIED)
			{
				ToNotCount = End->GetGridPosition();
			}
			End->SetTileStatus(EStatus::BLACKOCCUPIED);
			//calcolo contromosse dei pezzi nemici
			bool loopBreaker = false;
			for (auto Piece : GField->WPieceInGame)
			{
				if (loopBreaker)
				{
					break;
				}
				if (Piece->PieceGridPosition == ToNotCount)
				{
					continue;
				}
				Piece->PossibleMoves(GField);
				for (auto EnemyMove : Piece->Moves)
				{
					if (EnemyMove == KingPosition)
					{
						ActualMoves.Remove(i);
						loopBreaker = true;
						break;
					}
				}

			}
			Start->SetTileStatus(EStatus::BLACKOCCUPIED);
			End->SetTileStatus(Previous);
			ToNotCount.X = -1;
			ToNotCount.Y = -1;
		}
		Current->Moves = ActualMoves;
	}
}

void AChessGameMode::IsCheck(APiece* Current, AChessKing* EnemyKing, TArray<APiece*> EnemyPieces)
{
	Current->PossibleMoves(GField);
	for (auto PossibleMove : Current->Moves)
	{
		if (PossibleMove == EnemyKing->PieceGridPosition)
		{
			for (auto Enemy : EnemyPieces) 
			{
				LegalMoves(Enemy);
				if (!Enemy->Moves.IsEmpty()) 
				{
					break;
				}
				if (Enemy == EnemyPieces.Last() && Enemy->Moves.IsEmpty()) 
				{
					CheckMate = true;
				}
			}		
		}
	}

}

void AChessGameMode::IsPair(TArray<APiece*> EnemyPieces)
{
	for (auto Enemy : EnemyPieces)
	{
		LegalMoves(Enemy);
		if (!Enemy->Moves.IsEmpty())
		{
			break;
		}
		if (Enemy == EnemyPieces.Last() && Enemy->Moves.IsEmpty())
		{
			Pair = true;
		}
	}
}



	

void AChessGameMode::LegalMoves(APiece* Current)
{
	Current->PossibleMoves(GField);
	FilterIllegals(Current);
	if (Current->BitColor == 0) 
	{
		for (auto i : Current->Moves)
		{
			ATile* LegalTile = GField->TileMap[i];
			FString MaterialPath = TEXT("/Game/Materials/MI_LegalMove");
			UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
			UStaticMeshComponent* Comp = LegalTile->GetStatMeshComp();
			Comp->SetMaterial(0, Material);
		}
	}
}

