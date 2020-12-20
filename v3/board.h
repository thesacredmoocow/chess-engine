#ifndef BOARD_H
#define BOARD_H

#include "defines.h"
#include "database.h"
#include <stack>
#include <vector>

void printBitboard(unsigned long long bitboard);
struct lookupPoint
{
    int c, p, id;
};
class board
{
    public:
        board();
        virtual ~board();
        std::vector<XY> piece[2][6]; //first dimension color, second dimension piece ID, third dimension each piece
        bitboard moves[2][6][8];
        //int pieceCount[2][6];
        lookupPoint lookupTable[8][8];
        bitboard allPieces[2];
        bitboard guards[2];
        database db;
        std::vector<std::pair<XY, XY>> encodedMoves[2];
        int turns;

        void resetBoard();
        void makeMove(XY from, XY to);
        void unMove();
        void unMove(XY from, XY to);
        void genMove(int color);
        float assessScore();
        float assessCenterBias();
    private:
        lookupPoint makePoint(int c, int p, int id);
        bitboard one = 1;
        std::stack<XY> lastFrom;
        std::stack<XY> lastTo;
        std::stack<int> lastCap;
};

#endif // BOARD_H
