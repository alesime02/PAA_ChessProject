// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameMode.h"
#include "ChessPlayerController.h"
#include "HumanPlayer.h"
#include "RandomPlayer.h"
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
		// generate the field and inizialize teh pointer to it
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
		FieldSize = GField->Size;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}

	// set the camera for the game
	float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
	FVector CameraPos(CameraPosX, CameraPosX, 1000.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	// Human player = 0
	Players.Add(HumanPlayer);
	// Random Player
	auto* AI = GetWorld()->SpawnActor<ARandomPlayer>(FVector(), FRotator());

	// AI player = 1
	Players.Add(AI) ;

	ChoosePlayerAndStartGame();
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


// check if the game is ended or the next player has to play his turn
void AChessGameMode::TurnNextPlayer()
{
	if (CheckMate) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Scacco Matto"));
		Players[CurrentPlayer]->OnWin();
	}
	else if (Pair)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Patta"));
		auto GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GameInstance)
		{
			GameInstance->SetTurnMessage(TEXT("Is Pair!"));
		}
	}
	else
	{
		CurrentPlayer = GetNextPlayer(CurrentPlayer);
		Players[CurrentPlayer]->OnTurn();
	}
}

void AChessGameMode::FilterIllegals(APiece* Current)
{
	// for white pieces
	if (Current->BitColor == 0)
	{
		FVector2D KingPosition = GField->WhiteKing->PieceGridPosition;
		bool MovingKing = false;
		if (Current->PieceGridPosition == KingPosition)
		{
			MovingKing = true;
		}
		TArray<FVector2D> ActualMoves = Current->Moves;
		FVector2D ToNotCount(-1, -1);
		// scroll the possible moves of the piece clicked
		for (auto i : Current->Moves)
		{
			ATile* Start = GField->TileMap[Current->PieceGridPosition];
			// simulate the move
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
			// compute the enemies possible after the simulated move
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
				// check if the ally king is on check after that move and it removes it
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
	// for black pieces
	else
	{
		FVector2D KingPosition = GField->BlackKing->PieceGridPosition;
		bool MovingKing = false;
		if (Current->PieceGridPosition == KingPosition)
		{
			MovingKing = true;
		}
		TArray<FVector2D> ActualMoves = Current->Moves;
		FVector2D ToNotCount(-1, -1);
		for (auto i : Current->Moves)
		{
			ATile* Start = GField->TileMap[Current->PieceGridPosition];
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
	// check if the king is on check and there are no legal moves for each of the king's ally pieces
	Current->PossibleMoves(GField);
	TArray<FVector2D> CopyOfMoves = Current->Moves;
	for (auto PossibleMove : CopyOfMoves)
	{
		if (PossibleMove == EnemyKing->PieceGridPosition)
		{
			auto GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GameInstance)
			{
				FString CurrentMessage = GameInstance->GetMessage();
				CurrentMessage.AppendChar('+');
				GameInstance->SetMessage(CurrentMessage);
			}
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
					FString CurrentMessage = GameInstance->GetMessage();
					CurrentMessage.AppendChar('+');
					GameInstance->SetMessage(CurrentMessage);
					GameInstance->SetField("CheckMate");
				}
			}		
		}
	}

}

void AChessGameMode::IsPair(TArray<APiece*> EnemyPieces)
{
	// check if the field is in one of this pair status
	// king vs king
	// king and bishop vs king
	// king and knight vs king
	auto GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GField->WPieceInGame.Num() + GField->BPieceInGame.Num() == 2)
	{
		if (GField->WPieceInGame[0]->Id == 'K' && GField->BPieceInGame[0]->Id == 'k')
		{
			Pair = true;
			if (GameInstance)
			{
				GameInstance->SetField("Pair");
			}
		}
	}
	else if (GField->WPieceInGame.Num() + GField->BPieceInGame.Num() == 3)
	{

		for (auto WhitePiece : GField->WPieceInGame)
		{
			if (WhitePiece->Id == 'B' || WhitePiece->Id == 'N')
			{
				Pair = true;
				if (GameInstance)
				{
					GameInstance->SetField("Pair");
				}

			}
		}
		for (auto BlackPiece : GField->BPieceInGame)
		{
			if (BlackPiece->Id == 'b' || BlackPiece->Id == 'n')
			{
				Pair = true;
				if (GameInstance)
				{
					GameInstance->SetField("Pair");
				}
			}
			
		}
	}
	// check if the king isn't on check but there are no legal moves for each of the king's ally pieces
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
			if (GameInstance)
			{
				GameInstance->SetField("Pair");
			}
		}
	}
}

void AChessGameMode::DecoloringTiles()
{
	// restore the originale material of the tiles
	for (auto i : GField->TileArray)
	{
		FVector2D Coordinates = i->GetGridPosition();
		int32 x = Coordinates.X;
		int32 y = Coordinates.Y;
		if (((x + y) % 2) == 0)
		{
			FString MaterialPath = TEXT("/Game/Materials/MI_Black");
			UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
			UStaticMeshComponent* Comp = i->GetStatMeshComp();
			Comp->SetMaterial(0, Material);
		}
		else
		{
			FString MaterialPath = TEXT("/Game/Materials/MI_White");
			UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
			UStaticMeshComponent* Comp = i->GetStatMeshComp();
			Comp->SetMaterial(0, Material);
		}
	}
}

void AChessGameMode::CreateCurrentMove(ATile* Start, ATile* End, APiece* Moving, TCHAR Case)
{
	// create the string that describe tha move just happened
	TArray<TCHAR> JustHappened_Array;
	TArray<TCHAR> Columns = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

	int32 Xs_number = Start->GetGridPosition().X + 1;
	FString StartXstring = FString::Printf(TEXT("%d"), Xs_number);
	TCHAR Xstart = StartXstring[0];
	TCHAR Ystart = Columns[Start->GetGridPosition().Y];

	int32 Xe_number = End->GetGridPosition().X + 1;
	FString EndXstring = FString::Printf(TEXT("%d"), Xe_number);
	TCHAR Xend = EndXstring[0];
	TCHAR Yend =Columns[End->GetGridPosition().Y];

	JustHappened_Array = {Moving->Id, Ystart, Xstart, Case, Yend, Xend};
	JustHappened_Array.Add('\0');
	FString JustHappened(JustHappened_Array.GetData());

	auto GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance) 
	{
		GameInstance->SetMessage(JustHappened);
	}
	UE_LOG(LogTemp, Error, TEXT("%s"), *GameInstance->GetMessage());
}

void AChessGameMode::CreateFieldStatus()
{
	// create the string that describe the field status using the FEN notation
	FString Status;
	for (int32 x = 7; x >= 0; x--)
	{
		int32 CounterFreeTiles = 0;
		for (int32 y = 0; y < GField->Size; y++)
		{
			ATile* temp = GField->TileMap[FVector2D(x, y)];
			switch (temp->GetOccupier()) 
			{
				case ' ':
					CounterFreeTiles += 1;
					break;
				default:
					if (CounterFreeTiles != 0) 
					{
						FString StringaDaNumero = FString::Printf(TEXT("%d"), CounterFreeTiles);
						TCHAR FreeTiles = StringaDaNumero[0];
						Status.AppendChar(FreeTiles);
						CounterFreeTiles = 0;
					}
					Status.AppendChar(temp->GetOccupier());
					break;
			}
		}
		if (CounterFreeTiles != 0)
		{
			FString StringaDaNumero = FString::Printf(TEXT("%d"), CounterFreeTiles);
			TCHAR FreeTiles = StringaDaNumero[0];
			Status.AppendChar(FreeTiles);
		}

		if (x > 0) 
		{
			Status.AppendChar('/');
		}
	}
	auto GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->SetField(Status);
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


