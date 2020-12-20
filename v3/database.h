#ifndef DATABASE_H
#define DATABASE_H
#include "defines.h"
#include <fstream>
class database
{
    public:
        database();
        virtual ~database();

        bitboard rookMasks[8][8];
        bitboard rookKeys[8][8];
        int rookShifts[8][8];
        bitboard ***rookHashedMaps;

        bitboard bishopMasks[8][8];
        bitboard bishopKeys[8][8];
        int bishopShifts[8][8];
        bitboard ***bishopHashedMaps;

        bitboard kingMoves[8][8];
        bitboard knightMoves[8][8];

        bitboard pawnMoves[2][8][8];
        bitboard pawnCaptures[2][8][8];

        int startRank[2];
        bitboard pawnStart[2][8];
        void init();
    protected:

    private:
};

#endif // DATABASE_H
