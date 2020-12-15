#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

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
    ofstream rookMovement("movementTables/RookMovement.tab");
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
            for(int k = 0; k < 8; k++)
            {
                board[i][k] = true;
                board[k][j] = true;
            }
            unsigned long long key = pow(256, i)*pow(2, j);
            unsigned long long value = encodeBitboard(board);
            rookMovement << key << " " << value << endl;
        }
    }
    rookMovement.close();
    ofstream bishopMovement("movementTables/BishopMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            clearBoard(board);
            int cx = j;
            int cy = i;
            while(cx >= 0 && cy >= 0)
            {
                board[cy][cx] = true;
                cx--;
                cy--;
            }
            cx = j;
            cy = i;
            while(cx < 8 && cy >= 0)
            {
                board[cy][cx] = true;
                cx++;
                cy--;
            }
            cx = j;
            cy = i;
            while(cx >= 0 && cy < 8)
            {
                board[cy][cx] = true;
                cx--;
                cy++;
            }
            cx = j;
            cy = i;
            while(cx < 8 && cy < 8)
            {
                board[cy][cx] = true;
                cx++;
                cy++;
            }
            unsigned long long key = pow(256, i)*pow(2, j);
            unsigned long long value = encodeBitboard(board);
            bishopMovement << key << " " << value << endl;
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
    }
    ifstream straight("movementTables/RookMovement.tab");
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
    }
    return 0;
}

