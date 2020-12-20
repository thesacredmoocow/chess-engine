#include "database.h"

database::database()
{
    //ctor
}

database::~database()
{
    //dtor
}


void database::init()
{
    std::ifstream rookMaskFile("movementTables/RookMask.tab");
    std::ifstream rookMagicFile("movementTables/RookMagics.tab");
    std::ifstream bishopMaskFile("movementTables/BishopMask.tab");
    std::ifstream bishopMagicFile("movementTables/BishopMagics.tab");
    std::ifstream kingMoveFile("movementTables/KingMovement.tab");
    std::ifstream knightMoveFile("movementTables/KnightMovement.tab");
    std::ifstream blackPawnMoveFile("movementTables/BlackPawnMovement.tab");
    std::ifstream whitePawnMoveFile("movementTables/WhitePawnMovement.tab");
    std::ifstream blackPawnCapFile("movementTables/BlackPawnCapture.tab");
    std::ifstream whitePawnCapFile("movementTables/WhitePawnCapture.tab");
    rookHashedMaps = new bitboard**[8];
    bishopHashedMaps = new bitboard**[8];
    for(int i = 0; i < 8; i++)
    {
        rookHashedMaps[i] = new bitboard*[8];
        bishopHashedMaps[i] = new bitboard*[8];
        for(int j = 0; j < 8; j++)
        {
            rookMaskFile >> rookMasks[i][j];
            rookMagicFile >> rookShifts[i][j] >> rookKeys[i][j];
            rookHashedMaps[i][j] = new bitboard[4096];
            std::string fileLoc = "movementTables/Rooks/" + std::to_string(i) + std::to_string(j) + "RookHashed.tab";
            std::ifstream hashedRookTable(fileLoc);
            for(int k = 0; k < 4096; k++)
            {
                hashedRookTable >> rookHashedMaps[i][j][k];
            }

            bishopMaskFile >> bishopMasks[i][j];
            bishopMagicFile >> bishopShifts[i][j] >> bishopKeys[i][j];
            bishopHashedMaps[i][j] = new bitboard[512];
            std::string bfileLoc = "movementTables/Bishops/" + std::to_string(i) + std::to_string(j) + "BishopHashed.tab";
            std::ifstream hashedBishopTable(bfileLoc);
            for(int k = 0; k < 512; k++)
            {
                hashedBishopTable >> bishopHashedMaps[i][j][k];
            }

            bitboard useless;
            kingMoveFile >> useless >> kingMoves[i][j];
            knightMoveFile >> useless >> knightMoves[i][j];
            whitePawnMoveFile >> useless >> pawnMoves[WHITE][i][j];
            blackPawnMoveFile >> useless >> pawnMoves[BLACK][i][j];
            whitePawnCapFile >> useless >> pawnCaptures[WHITE][i][j];
            blackPawnCapFile >> useless >> pawnCaptures[BLACK][i][j];
        }
        pawnStart[WHITE][i] = pawnMoves[WHITE][2][i];
    }
    for(int i = 0; i < 8; i++)
    {
        pawnStart[WHITE][i] = pawnMoves[WHITE][2][i];
        pawnStart[BLACK][i] = pawnMoves[BLACK][5][i];
    }
    rookMaskFile.close();
    rookMagicFile.close();
    bishopMaskFile.close();
    bishopMagicFile.close();
    kingMoveFile.close();
    knightMoveFile.close();
    blackPawnMoveFile.close();
    whitePawnMoveFile.close();
    blackPawnCapFile.close();
    whitePawnCapFile.close();

    startRank[WHITE] = 1;
    startRank[BLACK] = 6;

}

