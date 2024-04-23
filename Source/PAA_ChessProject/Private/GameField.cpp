// Fill out your copyright notice in the Description page of Project Settings.


#include "GameField.h"
#include "ChessGameMode.h"
#include "HumanPlayer.h"

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

	FieldStatus = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	//FieldStatus = "8/2Q5/8/5K1k/8/8/8/8";
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
	SpawnPawns();
	
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

// DA SISTEMARE!!!
void AGameField::ResetField()
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode->CurrentPlayer != 0) 
	{
		 UChessGameInstance* GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		 GameInstance->SetTurnMessage(TEXT("Wait your turn to reset"));
	}
	else
	{
		for (ATile* Obj : TileArray)
		{
			Obj->SetTileStatus(EStatus::EMPTY);
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
		SpawnPawns();
		GameMode->ChoosePlayerAndStartGame();

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

//spawn per test
void AGameField::SpawnPawns()
{
	int32 NewX = 7;
	int32 NewY = 0;
	const float PieceScale = PieceSize / 100;
	for (int32 i = 0; i < FieldStatus.Len(); ++i)
	{
		TCHAR Character = FieldStatus[i];
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
				/*Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				Obj->ChangeBitColor();
				FString MaterialPath = TEXT("/Game/Materials/M_Bqueen");
				UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				Comp->SetMaterial(0, Material);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::BLACKOCCUPIED);
				Position->SetOccupier(Character);
				BPieceInGame.Add(Obj);*/
			}
			if (Character == 'k')
			{
				BlackKing = GetWorld()->SpawnActor<AChessKing>(KingClass, Location, FRotator::ZeroRotator);
				FString MaterialPath = TEXT("/Game/Materials/M_Bking");
				UStaticMeshComponent* Comp = BlackKing->GetStatMeshComp();
				SpawnBlackPiece(BlackKing, MaterialPath, NewX, NewY, Character, Comp);
				NewY += 1;
				/*BlackKing->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				BlackKing->SetGridPosition(NewX, NewY);
				BlackKing->ChangeBitColor();
				FString MaterialPath = TEXT("/Game/Materials/M_Bking");
				UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
				UStaticMeshComponent* Comp = BlackKing->GetStatMeshComp();
				Comp->SetMaterial(0, Material);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::BLACKOCCUPIED);
				Position->SetOccupier(Character);*/
				//BPieceInGame.Add(BlackKing);
			}
			if (Character == 'p')
			{
				AChessPawn* Obj = GetWorld()->SpawnActor<AChessPawn>(PawnClass, Location, FRotator::ZeroRotator);
				FString MaterialPath = TEXT("/Game/Materials/M_Bpawn");
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				SpawnBlackPiece(Obj, MaterialPath, NewX, NewY, Character, Comp);
				NewY += 1;
				/*Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				Obj->ChangeBitColor();
				FString MaterialPath = TEXT("/Game/Materials/M_Bpawn");
				UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				Comp->SetMaterial(0, Material);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::BLACKOCCUPIED);*/
				//Position->SetOccupier(Character);
				//BPieceInGame.Add(Obj);
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

FString& AGameField::GetStatus()
{
	return FieldStatus;
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
	ATile* Position = TileMap[FVector2D(DestX, DestY)];
	Position->SetTileStatus(EStatus::WHITEOCCUPIED);
	Position->SetOccupier(id);
	WPieceInGame.Add(ToSpawn);
}
