#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

typedef unsigned long long U64;
typedef unsigned long long C64;
const unsigned int magicmoves_r_shift[64]=
{
	52, 53, 53, 53, 53, 53, 53, 52,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 53, 53, 53, 53, 53
};

const U64 magicmoves_r_magics[64]=
{
	C64(0x0080001020400080), C64(0x0040001000200040), C64(0x0080081000200080), C64(0x0080040800100080),
	C64(0x0080020400080080), C64(0x0080010200040080), C64(0x0080008001000200), C64(0x0080002040800100),
	C64(0x0000800020400080), C64(0x0000400020005000), C64(0x0000801000200080), C64(0x0000800800100080),
	C64(0x0000800400080080), C64(0x0000800200040080), C64(0x0000800100020080), C64(0x0000800040800100),
	C64(0x0000208000400080), C64(0x0000404000201000), C64(0x0000808010002000), C64(0x0000808008001000),
	C64(0x0000808004000800), C64(0x0000808002000400), C64(0x0000010100020004), C64(0x0000020000408104),
	C64(0x0000208080004000), C64(0x0000200040005000), C64(0x0000100080200080), C64(0x0000080080100080),
	C64(0x0000040080080080), C64(0x0000020080040080), C64(0x0000010080800200), C64(0x0000800080004100),
	C64(0x0000204000800080), C64(0x0000200040401000), C64(0x0000100080802000), C64(0x0000080080801000),
	C64(0x0000040080800800), C64(0x0000020080800400), C64(0x0000020001010004), C64(0x0000800040800100),
	C64(0x0000204000808000), C64(0x0000200040008080), C64(0x0000100020008080), C64(0x0000080010008080),
	C64(0x0000040008008080), C64(0x0000020004008080), C64(0x0000010002008080), C64(0x0000004081020004),
	C64(0x0000204000800080), C64(0x0000200040008080), C64(0x0000100020008080), C64(0x0000080010008080),
	C64(0x0000040008008080), C64(0x0000020004008080), C64(0x0000800100020080), C64(0x0000800041000080),
	C64(0x00FFFCDDFCED714A), C64(0x007FFCDDFCED714A), C64(0x003FFFCDFFD88096), C64(0x0000040810002101),
	C64(0x0001000204080011), C64(0x0001000204000801), C64(0x0001000082000401), C64(0x0001FFFAABFAD1A2)
};

const unsigned int magicmoves_b_shift[64]=
{
	58, 59, 59, 59, 59, 59, 59, 58,
	59, 59, 59, 59, 59, 59, 59, 59,
	59, 59, 57, 57, 57, 57, 59, 59,
	59, 59, 57, 55, 55, 57, 59, 59,
	59, 59, 57, 55, 55, 57, 59, 59,
	59, 59, 57, 57, 57, 57, 59, 59,
	59, 59, 59, 59, 59, 59, 59, 59,
	58, 59, 59, 59, 59, 59, 59, 58
};

const U64 magicmoves_b_magics[64]=
{
	C64(0x0002020202020200), C64(0x0002020202020000), C64(0x0004010202000000), C64(0x0004040080000000),
	C64(0x0001104000000000), C64(0x0000821040000000), C64(0x0000410410400000), C64(0x0000104104104000),
	C64(0x0000040404040400), C64(0x0000020202020200), C64(0x0000040102020000), C64(0x0000040400800000),
	C64(0x0000011040000000), C64(0x0000008210400000), C64(0x0000004104104000), C64(0x0000002082082000),
	C64(0x0004000808080800), C64(0x0002000404040400), C64(0x0001000202020200), C64(0x0000800802004000),
	C64(0x0000800400A00000), C64(0x0000200100884000), C64(0x0000400082082000), C64(0x0000200041041000),
	C64(0x0002080010101000), C64(0x0001040008080800), C64(0x0000208004010400), C64(0x0000404004010200),
	C64(0x0000840000802000), C64(0x0000404002011000), C64(0x0000808001041000), C64(0x0000404000820800),
	C64(0x0001041000202000), C64(0x0000820800101000), C64(0x0000104400080800), C64(0x0000020080080080),
	C64(0x0000404040040100), C64(0x0000808100020100), C64(0x0001010100020800), C64(0x0000808080010400),
	C64(0x0000820820004000), C64(0x0000410410002000), C64(0x0000082088001000), C64(0x0000002011000800),
	C64(0x0000080100400400), C64(0x0001010101000200), C64(0x0002020202000400), C64(0x0001010101000200),
	C64(0x0000410410400000), C64(0x0000208208200000), C64(0x0000002084100000), C64(0x0000000020880000),
	C64(0x0000001002020000), C64(0x0000040408020000), C64(0x0004040404040000), C64(0x0002020202020000),
	C64(0x0000104104104000), C64(0x0000002082082000), C64(0x0000000020841000), C64(0x0000000000208800),
	C64(0x0000000010020200), C64(0x0000000404080200), C64(0x0000040404040400), C64(0x0002020202020200)
};
struct coord
{
    int x;
    int y;
};

coord toCoord(int y, int x)
{
    coord t;
    t.x = x;
    t.y = y;
    return t;
}

bool cCoord(coord a)
{
    if((a.y < 8) && (a.y >= 0) && (a.x < 8) && (a.x >= 0))
    {
        return true;
    }
    return false;
}

void clearBoard(bool** b)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            b[i][j] = false;
        }
    }
}

void printBitboard(unsigned long long bitboard)
{
    bool board[8][8];
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            board[i][j] = bitboard%2;
            bitboard/=2;
        }
    }
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            cout << board[i][j];
        }
        cout << endl;
    }
}

unsigned long long encodeBitboard(bool** b)
{
    unsigned long long value;
    for(int k = 7; k >= 0; k--)
    {
        for(int a = 7; a >= 0; a--)
        {
            value *= 2;
            value += b[k][a];
        }
    }
    return value;
}
int main()
{
    ofstream rookMagics("movementTables/RookMagics.tab");
    ofstream bishopMagics("movementTables/BishopMagics.tab");
    for(int i = 0; i < 64; i++)
    {
        rookMagics << magicmoves_r_shift[i] << " " << magicmoves_r_magics[i] << endl;
        bishopMagics << magicmoves_b_shift[i] << " " << magicmoves_b_magics[i] << endl;
    }
/*    ofstream rookMovement("movementTables/RookMask.tab");
    bool **board = new bool*[8];
    for(int i = 0; i < 8; i++)
    {
        board[i] = new bool[8];
    }
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            clearBoard(board);
            for(int k = 1; k < 7; k++)
            {
                if(k != j)
                {
                    board[i][k] = true;
                }
                if(k != i)
                {
                    board[k][j] = true;
                }

            }
            unsigned long long key = pow(256, i)*pow(2, j);
            unsigned long long value = encodeBitboard(board);

            rookMovement << value << endl;
        }
    }
    rookMovement.close();
    ofstream bishopMovement("movementTables/BishopMask.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            clearBoard(board);
            int cx = j-1;
            int cy = i-1;
            while(cx > 0 && cy > 0)
            {
                board[cy][cx] = true;
                cx--;
                cy--;
            }
            cx = j+1;
            cy = i-1;
            while(cx < 7 && cy > 0)
            {
                board[cy][cx] = true;
                cx++;
                cy--;
            }
            cx = j-1;
            cy = i+1;
            while(cx > 0 && cy < 7)
            {
                board[cy][cx] = true;
                cx--;
                cy++;
            }
            cx = j+1;
            cy = i+1;
            while(cx < 7 && cy < 7)
            {
                board[cy][cx] = true;
                cx++;
                cy++;
            }
            unsigned long long key = pow(256, i)*pow(2, j);
            unsigned long long value = encodeBitboard(board);
            bishopMovement << value << endl;
            printBitboard(key);
            cout << endl;
            printBitboard(value);
            cout << "///////////////////////////////////" << endl;
            //printBitboard(value);
            //cout << endl;
        }
    }
    bishopMovement.close();

    ofstream kingMovement("movementTables/KingMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            clearBoard(board);
            for(int k = i - 1; k <= i+1; k++)
            {
                for(int a = j - 1; a <= j+1; a++)
                {
                    if(k < 8 && k >= 0 && a < 8 && a >= 0 && (i != k || j != a))
                    {
                        board[k][a] = true;
                    }
                }
            }
            unsigned long long key = pow(256, i)*pow(2, j);
            unsigned long long value = encodeBitboard(board);
            kingMovement << key << " " << value << endl;
            //printBitboard(value);
            //cout << endl;
        }
    }

    ofstream knightMovement("movementTables/KnightMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            clearBoard(board);
            coord knightPos[8] = {toCoord(-1, -2),toCoord(-2, -1),toCoord(1, -2),toCoord(2, -1),toCoord(-1, 2),toCoord(-2, 1),toCoord(1, 2),toCoord(2, 1)};
            for(int k = 0; k < 8; k++)
            {
                if(cCoord(toCoord(i + knightPos[k].y, j + knightPos[k].x)))
                {
                    board[i+knightPos[k].y][j+knightPos[k].x] = true;
                }
            }
            unsigned long long key = pow(256, i)*pow(2, j);
            unsigned long long value = encodeBitboard(board);
            knightMovement << key << " " << value << endl;
            //printBitboard(value);
            //cout << endl;
        }
    }

    ofstream whitePawnMovement("movementTables/WhitePawnMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            clearBoard(board);
            if(i < 7)
            {
                board[i+1][j] = true;
                if(i == 1)
                {
                    //board[i+2][j] = true;
                }
            }
            unsigned long long key = pow(256, i)*pow(2, j);
            unsigned long long value = encodeBitboard(board);
            whitePawnMovement << key << " " << value << endl;
            //printBitboard(value);
            //cout << endl;
        }
    }

    ofstream blackPawnMovement("movementTables/BlackPawnMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            clearBoard(board);
            if(i > 0)
            {
                board[i-1][j] = true;
                if(i == 6)
                {
                    //board[i-2][j] = true;
                }
            }
            unsigned long long key = pow(256, i)*pow(2, j);
            unsigned long long value = encodeBitboard(board);
            blackPawnMovement << key << " " << value << endl;
            //printBitboard(value);
            //cout << endl;
        }
    }

    ofstream whitePawnCapture("movementTables/WhitePawnCapture.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            clearBoard(board);
            if(i < 7)
            {
                if(j > 0)
                {
                    board[i+1][j-1] = true;
                }
                if(j < 7)
                {
                    board[i+1][j+1] = true;
                }
            }
            unsigned long long key = pow(256, i)*pow(2, j);
            unsigned long long value = encodeBitboard(board);
            whitePawnCapture << key << " " << value << endl;
            //printBitboard(value);
            //cout << endl;
        }
    }
    ofstream blackPawnCapture("movementTables/BlackPawnCapture.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            clearBoard(board);
            if(i > 0)
            {
                if(j > 0)
                {
                    board[i-1][j-1] = true;
                }
                if(j < 7)
                {
                    board[i-1][j+1] = true;
                }
            }
            unsigned long long key = pow(256, i)*pow(2, j);
            unsigned long long value = encodeBitboard(board);
            blackPawnCapture << key << " " << value << endl;
            //printBitboard(value);
            //cout << endl;
        }
    }*/
    /*ifstream straight("movementTables/RookMovement.tab");
    ifstream diag("movementTables/BishopMovement.tab");
    ofstream queenMovement("movementTables/QueenMovement.tab");
    for(int i = 0; i < 64; i++)
    {
        unsigned long long skey, sval, dkey, dval;
        straight >> skey >> sval;
        diag >> dkey >> dval;

        if(skey == dkey)
        {
            unsigned long long value = sval | dval;
            printBitboard(value);
            cout << endl;
            queenMovement << skey << " " << value << endl;
        }
    }*/
    return 0;
}

