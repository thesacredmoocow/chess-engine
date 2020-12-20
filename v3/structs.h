#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include <vector>
struct coord
{
    int x;
    int y;
};

struct boardState
{
    //unsigned long long pieces[2][6]; //first dimension white or black, second dimension piece type, third dimension 1
    std::vector<std::pair<coord, unsigned long long>> piece[2][7];
    std::vector<unsigned long long> guardTable[2][6];
    unsigned long long allPieces[2];
    int turn;
    //std::vector<unsigned long long> possibleMovements[2][6];
    int guards[2][8][8];
};

struct mov
{
    int color;
    int piece;
    int index;
    coord destination;
};
#endif // STRUCTS_H_INCLUDED
