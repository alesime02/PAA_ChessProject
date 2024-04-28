// Fill out your copyright notice in the Description page of Project Settings.


#include "GameField.h"
#include "ChessGameMode.h"
#include "HumanPlayer.h"
#include "ChessPlayerController.h"

// Sets default values
AGameField::AGameField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// size of the field (8x8)
	Size = 8;
	// tile dimension
	TileSize = 120;
	// tile padding dimension
	CellPadding = 0;
	// pawns dimendion
	PieceSize = 100;
	// string that describe the field at the start of the game
	FieldAtStart = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	// create the field
	GenerateField();
	// create the pieces
	SpawnPieces(FieldAtStart);
	
}

// Called every frame
//void AGameField::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);

//}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//normalized tilepadding
	NormalizedCellPadding = FMath::RoundToDouble(((TileSize + CellPadding) / TileSize) * 100) / 100;
}

void AGameField::RandomBlackPromotion()
{
	// take the pointer to the pawn that has to be promoted
	FVector2D Coordinates = ReceivingPromotion->PieceGridPosition;
	// save his location ad uses it to spawn the new piece
	FVector Location = AGameField::GetPieceRelativeLocationByXYPosition(Coordinates.X, Coordinates.Y);
	BPieceInGame.Remove(ReceivingPromotion);
	ReceivingPromotion->Destroy();
	FString MaterialPath;
	UStaticMeshComponent* Comp;
	AChessQueen* NewQueen;
	AChessRook* NewRook;
	AChessBishop* NewBishop;
	AChessKnight* NewKnight;
	// choose a random promotion
	int32 RandPromotion = FMath::RandRange(1, 4);
	switch (RandPromotion) 
	{
	case 1:
		NewQueen = GetWorld()->SpawnActor<AChessQueen>(QueenClass, Location, FRotator::ZeroRotator);
		MaterialPath = TEXT("/Game/Materials/M_Bqueen");
		Comp = NewQueen->GetStatMeshComp();
		SpawnBlackPiece(NewQueen, MaterialPath, Coordinates.X, Coordinates.Y, 'q', Comp);
		break;
	case 2:
		NewRook = GetWorld()->SpawnActor<AChessRook>(RookClass, Location, FRotator::ZeroRotator);
		MaterialPath = TEXT("/Game/Materials/M_Brook");
		Comp = NewRook->GetStatMeshComp();
		SpawnBlackPiece(NewRook, MaterialPath, Coordinates.X, Coordinates.Y, 'r', Comp);
		break;
	case 3:
		NewBishop = GetWorld()->SpawnActor<AChessBishop>(BishopClass, Location, FRotator::ZeroRotator);
		MaterialPath = TEXT("/Game/Materials/M_Bbishop");
		Comp = NewBishop->GetStatMeshComp();
		SpawnBlackPiece(NewBishop, MaterialPath, Coordinates.X, Coordinates.Y, 'b', Comp);
		break;
	case 4:
		NewKnight = GetWorld()->SpawnActor<AChessKnight>(KnightClass, Location, FRotator::ZeroRotator);
		MaterialPath = TEXT("/Game/Materials/M_Bknight");
		Comp = NewKnight->GetStatMeshComp();
		SpawnBlackPiece(NewKnight, MaterialPath, Coordinates.X, Coordinates.Y, 'n', Comp);
		break;
	}
}






void AGameField::PromotePawnToQueen()
{
	// take the pointer to the pawn that has to be promoted
	FVector2D Coordinates = ReceivingPromotion->PieceGridPosition;
	// save his location ad uses it to spawn the new piece
	FVector Location = AGameField::GetPieceRelativeLocationByXYPosition(Coordinates.X, Coordinates.Y);
	WPieceInGame.Remove(ReceivingPromotion);
	ReceivingPromotion->Destroy();
	// create a new piece where there was the promoted pawn
	AChessQueen* Obj = GetWorld()->SpawnActor<AChessQueen>(QueenClass, Location, FRotator::ZeroRotator);
	SpawnWhitePiece(Obj, Coordinates.X, Coordinates.Y, 'Q');
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->DecoloringTiles();
	GameMode->CreateFieldStatus();
	// check if the new piece cause a pair or a check condition 
	GameMode->IsPair(GameMode->GField->BPieceInGame);
	GameMode->IsCheck(Obj, GameMode->GField->BlackKing, GameMode->GField->BPieceInGame);
	auto PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->SpawnButtonEvent.Broadcast();
	}
	GameMode->TurnNextPlayer();
	

}

// those functions works the same as PromotePawnToQueen
void AGameField::PromotePawnToRook()
{
	
	FVector2D Coordinates = ReceivingPromotion->PieceGridPosition;
	FVector Location = AGameField::GetPieceRelativeLocationByXYPosition(Coordinates.X, Coordinates.Y);
	WPieceInGame.Remove(ReceivingPromotion);
	ReceivingPromotion->Destroy();
	AChessRook* Obj = GetWorld()->SpawnActor<AChessRook>(RookClass, Location, FRotator::ZeroRotator);
	SpawnWhitePiece(Obj, Coordinates.X, Coordinates.Y, 'R');
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->DecoloringTiles();
	GameMode->CreateFieldStatus();
	GameMode->IsPair(GameMode->GField->BPieceInGame);
	GameMode->IsCheck(Obj, GameMode->GField->BlackKing, GameMode->GField->BPieceInGame);
	auto PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->SpawnButtonEvent.Broadcast();
	}
	GameMode->TurnNextPlayer();
}

void AGameField::PromotePawnToBishop()
{

	FVector2D Coordinates = ReceivingPromotion->PieceGridPosition;
	FVector Location = AGameField::GetPieceRelativeLocationByXYPosition(Coordinates.X, Coordinates.Y);
	WPieceInGame.Remove(ReceivingPromotion);
	ReceivingPromotion->Destroy();
	AChessBishop* Obj = GetWorld()->SpawnActor<AChessBishop>(BishopClass, Location, FRotator::ZeroRotator);
	SpawnWhitePiece(Obj, Coordinates.X, Coordinates.Y, 'B');
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->DecoloringTiles();
	GameMode->CreateFieldStatus();
	GameMode->IsPair(GameMode->GField->BPieceInGame);
	GameMode->IsCheck(Obj, GameMode->GField->BlackKing, GameMode->GField->BPieceInGame);
	auto PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->SpawnButtonEvent.Broadcast();
	}
	GameMode->TurnNextPlayer();
}

void AGameField::PromotePawnToKnight()
{
	FVector2D Coordinates = ReceivingPromotion->PieceGridPosition;
	FVector Location = AGameField::GetPieceRelativeLocationByXYPosition(Coordinates.X, Coordinates.Y);
	WPieceInGame.Remove(ReceivingPromotion);
	ReceivingPromotion->Destroy();
	AChessKnight* Obj = GetWorld()->SpawnActor<AChessKnight>(KnightClass, Location, FRotator::ZeroRotator);
	SpawnWhitePiece(Obj, Coordinates.X, Coordinates.Y, 'N');
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->DecoloringTiles();
	GameMode->CreateFieldStatus();
	GameMode->IsPair(GameMode->GField->BPieceInGame);
	GameMode->IsCheck(Obj, GameMode->GField->BlackKing, GameMode->GField->BPieceInGame);
	auto PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->SpawnButtonEvent.Broadcast();
	}
	GameMode->TurnNextPlayer();
}





void AGameField::ResetField()
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	// check if it's the human's turn, button can'r be pressed in AI's turn
	if (GameMode->CurrentPlayer != 0) 
	{
		 UChessGameInstance* GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		 if (GameInstance)
		 {
			 GameInstance->SetTurnMessage(TEXT("Wait your turn press the button"));
		 }
	}
	else
	{
		// set al the tiles as empty and destroy all the existing pieces
		for (ATile* Obj : TileArray)
		{
			Obj->SetTileStatus(EStatus::EMPTY);
			Obj->SetOccupier(' ');
		}
		for (APiece* Obj : BPieceInGame)
		{
			Obj->Destroy();
		};
		BPieceInGame.Empty();
		for (APiece* Obj : WPieceInGame)
		{
			Obj->Destroy();
		}
		WPieceInGame.Empty();

		GameMode->CheckMate = false;
		GameMode->Pair = false;
		// recreate the start configuration and start game
		SpawnPieces(FieldAtStart);
		GameMode->ChoosePlayerAndStartGame();

	}
}

// works as the resetField but it's
void AGameField::ReplayField(FString FieldToReturn, int32 Player)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	UChessGameInstance* GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameMode->CurrentPlayer != 0)
	{
		if (GameInstance)
		{
			GameInstance->SetTurnMessage(TEXT("Wait your turn press the button"));
		}
	}
	else 
	{
		if (GameInstance && FieldToReturn != "CheckMate" && FieldToReturn != "Pair")
		{
			for (ATile* Obj : TileArray)
			{
				Obj->SetTileStatus(EStatus::EMPTY);
				Obj->SetOccupier(' ');
			}
			for (APiece* Obj : BPieceInGame)
			{
				Obj->Destroy();
			};
			BPieceInGame.Empty();
			for (APiece* Obj : WPieceInGame)
			{
				Obj->Destroy();
			}
			WPieceInGame.Empty();

			GameMode->CheckMate = false;
			GameMode->Pair = false;
			SpawnPieces(FieldToReturn);
			GameMode->CurrentPlayer = Player;
			GameMode->TurnNextPlayer();
		}
	}
}



void AGameField::GenerateField()
{
	for (int32 x = 0; x < Size; x++)
	{
		for (int32 y = 0; y < Size; y++)
		{
			FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y);
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);
			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetGridPosition(x, y);
			if (((x + y) % 2) == 0)
			{
				FString MaterialPath = TEXT("/Game/Materials/MI_Black");
				UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				Comp->SetMaterial(0, Material);
			}
			TileArray.Add(Obj);
			TileMap.Add(FVector2D(x, y), Obj);
		}
	}
}



void AGameField::SpawnPieces(FString Field)
{
	int32 NewX = 7;
	int32 NewY = 0;
	//const float PieceScale = PieceSize / 100;
	for (int32 i = 0; i < Field.Len(); ++i)
	//for (auto i : Field)
	{
		TCHAR Character = Field[i];
		if (Character == '/')
		{
			NewX -= 1;
			NewY = 0;
		}
		if (Character >= '1' && Character <= '8')
		{
			int32 NumericValue = Character - '0';
			NewY += NumericValue;
		}
		if (Character >= 'B' && Character <= 'R')
		{
			FVector Location = AGameField::GetPieceRelativeLocationByXYPosition(NewX, NewY);

			if (Character == 'R')
			{
				AChessRook* Obj = GetWorld()->SpawnActor<AChessRook>(RookClass, Location, FRotator::ZeroRotator);
				SpawnWhitePiece(Obj, NewX, NewY, Character);
				NewY += 1;
			}
			if (Character == 'N')
			{
				AChessKnight* Obj = GetWorld()->SpawnActor<AChessKnight>(KnightClass, Location, FRotator::ZeroRotator);
				SpawnWhitePiece(Obj, NewX, NewY, Character);
				NewY += 1;
			}
			if (Character == 'B')
			{
				AChessBishop* Obj = GetWorld()->SpawnActor<AChessBishop>(BishopClass, Location, FRotator::ZeroRotator);
				SpawnWhitePiece(Obj, NewX, NewY, Character);
				NewY += 1;
			}
			if (Character == 'Q')
			{
				AChessQueen* Obj = GetWorld()->SpawnActor<AChessQueen>(QueenClass, Location, FRotator::ZeroRotator);
				SpawnWhitePiece(Obj, NewX, NewY, Character);
				NewY += 1;
			}
			if (Character == 'K')
			{
				WhiteKing = GetWorld()->SpawnActor<AChessKing>(KingClass, Location, FRotator::ZeroRotator);
				SpawnWhitePiece(WhiteKing, NewX, NewY, Character);
				NewY += 1;
			}
			if (Character == 'P')
			{
				AChessPawn* Obj = GetWorld()->SpawnActor<AChessPawn>(PawnClass, Location, FRotator::ZeroRotator);
				SpawnWhitePiece(Obj, NewX, NewY, Character);
				NewY += 1;
			}


		}
		if (Character >= 'b' && Character <= 'r')
		{
			FVector Location = AGameField::GetPieceRelativeLocationByXYPosition(NewX, NewY);

			if (Character == 'r')
			{
				AChessRook* Obj = GetWorld()->SpawnActor<AChessRook>(RookClass, Location, FRotator::ZeroRotator);
				FString MaterialPath = TEXT("/Game/Materials/M_Brook");
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				SpawnBlackPiece(Obj, MaterialPath, NewX, NewY, Character, Comp);
				NewY += 1;
			}
			if (Character == 'n')
			{
				AChessKnight* Obj = GetWorld()->SpawnActor<AChessKnight>(KnightClass, Location, FRotator::ZeroRotator);
				FString MaterialPath = TEXT("/Game/Materials/M_BKnight");
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				SpawnBlackPiece(Obj, MaterialPath, NewX, NewY, Character, Comp);
				NewY += 1;
			}
			if (Character == 'b')
			{
				AChessBishop* Obj = GetWorld()->SpawnActor<AChessBishop>(BishopClass, Location, FRotator::ZeroRotator);
				FString MaterialPath = TEXT("/Game/Materials/M_Bbishop");
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				SpawnBlackPiece(Obj, MaterialPath, NewX, NewY, Character, Comp);
				NewY += 1;
			}
			if (Character == 'q')
			{
				AChessQueen* Obj = GetWorld()->SpawnActor<AChessQueen>(QueenClass, Location, FRotator::ZeroRotator);
				FString MaterialPath = TEXT("/Game/Materials/M_Bqueen");
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				SpawnBlackPiece(Obj, MaterialPath, NewX, NewY, Character, Comp);
				NewY += 1;
			}
			if (Character == 'k')
			{
				BlackKing = GetWorld()->SpawnActor<AChessKing>(KingClass, Location, FRotator::ZeroRotator);
				FString MaterialPath = TEXT("/Game/Materials/M_Bking");
				UStaticMeshComponent* Comp = BlackKing->GetStatMeshComp();
				SpawnBlackPiece(BlackKing, MaterialPath, NewX, NewY, Character, Comp);
				NewY += 1;
			}
			if (Character == 'p')
			{
				AChessPawn* Obj = GetWorld()->SpawnActor<AChessPawn>(PawnClass, Location, FRotator::ZeroRotator);
				FString MaterialPath = TEXT("/Game/Materials/M_Bpawn");
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				SpawnBlackPiece(Obj, MaterialPath, NewX, NewY, Character, Comp);
				NewY += 1;
			}

		}
	}
}

FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AGameField::GetTileArray()
{
	return TileArray;
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}

FVector AGameField::GetPieceRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * FVector(InX, InY, 0.1);
}


FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const
{
	const double x = Location[0] / (TileSize * NormalizedCellPadding);
	const double y = Location[1] / (TileSize * NormalizedCellPadding);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}

void AGameField::SpawnBlackPiece(APiece* ToSpawn, FString MaterialPath, int32 DestX, int32 DestY, TCHAR id, UStaticMeshComponent* Component)
{
	const float PieceScale = PieceSize / 100;
	ToSpawn->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
	ToSpawn->PieceGridPosition.X = DestX;
	ToSpawn->PieceGridPosition.Y = DestY;
	ToSpawn->BitColor = 1;
	ToSpawn->Id = id;
	UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
	Component->SetMaterial(0, Material);
	ATile* Position = TileMap[FVector2D(DestX, DestY)];
	Position->SetTileStatus(EStatus::BLACKOCCUPIED);
	Position->SetOccupier(id);
	BPieceInGame.Add(ToSpawn);
}

void AGameField::SpawnWhitePiece(APiece* ToSpawn, int32 DestX, int32 DestY, TCHAR id)
{
	const float PieceScale = PieceSize / 100;
	ToSpawn->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
	ToSpawn->PieceGridPosition.X = DestX;
	ToSpawn->PieceGridPosition.Y = DestY;
	ToSpawn->Id = id;
	ATile* Position = TileMap[FVector2D(DestX, DestY)];
	Position->SetTileStatus(EStatus::WHITEOCCUPIED);
	Position->SetOccupier(id);
	WPieceInGame.Add(ToSpawn);
}
