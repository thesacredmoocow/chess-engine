#include "AndyAllegro.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

//lowercase is black, uppercase is white
//board representation is inverted when written out
//first dimension 0~7 corresponds to 1~8 in chess coordinates
//second dimension 0~7 corresponds to a~h in chess coordinates

ALLEGRO_BITMAP *pieces;

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

map<char, int> piecePoints;

void initPiecePoints(map<char, int> &p)
{
    p.insert(pair<char, int>('R', 5));
    p.insert(pair<char, int>('K', 3));
    p.insert(pair<char, int>('B', 3));
    p.insert(pair<char, int>('Q', 9));
    p.insert(pair<char, int>('A', 200));
    p.insert(pair<char, int>('P', 1));
    p.insert(pair<char, int>('O', 0));
    p.insert(pair<char, int>('r', -5));
    p.insert(pair<char, int>('k', -3));
    p.insert(pair<char, int>('b', -3));
    p.insert(pair<char, int>('q', -9));
    p.insert(pair<char, int>('a', -200));
    p.insert(pair<char, int>('p', -1));
}

struct boardState
{
    int turns = 0;
    char board[8][8];
};


float aggressive_threshold = 5;
float risk_factor = 0.0;

pair<coord, coord> decode(int m)
{
    coord from, to;
    to.x = m % 8;
    m -= to.x;
    m /= 8;
    to.y = m % 8;
    m -= to.y;
    m /= 8;
    from.x = m % 8;
    m -= from.x;
    m /= 8;
    from.y = m;
    return pair<coord, coord>(from, to);
}

int encode(pair<coord, coord> val)
{
    return val.first.y*512 + val.first.x*64 + val.second.y*8 + val.second.x;
}
void printMove(int mo)
{
    pair<coord, coord> m = decode(mo);
    cout << char(m.first.x + 'a') << m.first.y+1 << " to " << char(m.second.x + 'a') << m.second.y+1 << endl;
}
bool cCoord(coord a)
{
    if((a.y < 8) && (a.y >= 0) && (a.x < 8) && (a.x >= 0))
    {
        return true;
    }
    return false;
}

//number of pieces guarding a given square
//first int is number of white pieces
pair<int, int> getGuards(boardState &b, coord bKing)
{
    pair<int, int> ans(0, 0);
    if(bKing.y-1 >= 0)
    {
        if(bKing.x-1 >= 0 && b.board[bKing.y-1][bKing.x-1] == 'P')
        {
            ans.first += 1;
        }
        if(bKing.x+1 < 8 && b.board[bKing.y-1][bKing.x+1] == 'P')
        {
            ans.first += 1;
        }
    }
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if(i != 0 || j != 0)
            {
                for(int k = 1; k < 8; k++)
                {
                    if(cCoord(toCoord(bKing.y + i*k, bKing.x + j*k)))
                    {
                        char currentPiece = b.board[bKing.y + i*k][bKing.x + j*k];
                        if(currentPiece != 'O')
                        {
                            if(i == 0 || j == 0)
                            {
                                if(currentPiece == 'Q' || currentPiece == 'R')
                                {
                                    ans.first += 1;
                                    break;
                                }
                                else if(k == 1 && currentPiece == 'A')
                                {
                                    ans.first += 1;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            else
                            {
                                if(currentPiece == 'Q' || currentPiece == 'B')
                                {
                                    ans.first += 1;
                                    break;
                                }
                                else if(k == 1 && currentPiece == 'A')
                                {
                                    ans.first += 1;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    coord knightPos[8] = {toCoord(-1, -2),toCoord(-2, -1),toCoord(1, -2),toCoord(2, -1),toCoord(-1, 2),toCoord(-2, 1),toCoord(1, 2),toCoord(2, 1)};
    for(int i = 0; i < 8; i++)
    {
        if(cCoord(toCoord(bKing.y + knightPos[i].y, bKing.x + knightPos[i].x)) && b.board[bKing.y + knightPos[i].y][bKing.x + knightPos[i].x] == 'K')
        {
            ans.first += 1;
            break;
        }
    }



    if(bKing.y+1 < 8)
    {
        if(bKing.x-1 >= 0 && b.board[bKing.y+1][bKing.x-1] == 'p')
        {
            ans.second += 1;
        }
        if(bKing.x+1 < 8 && b.board[bKing.y+1][bKing.x+1] == 'p')
        {
            ans.second += 1;
        }
    }

    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if(i != 0 || j != 0)
            {
                for(int k = 1; k < 8; k++)
                {
                    if(cCoord(toCoord(bKing.y + i*k, bKing.x + j*k)))
                    {
                        char currentPiece = b.board[bKing.y + i*k][bKing.x + j*k];
                        if(currentPiece != 'O')
                        {
                            if(i == 0 || j == 0)
                            {
                                if(currentPiece == 'q' || currentPiece == 'r')
                                {
                                    ans.second += 1;
                                    break;
                                }
                                else if(k == 1 && currentPiece == 'a')
                                {
                                    ans.second += 1;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            else
                            {
                                if(currentPiece == 'q' || currentPiece == 'b')
                                {
                                    ans.second += 1;
                                    break;
                                }
                                else if(k == 1 && currentPiece == 'a')
                                {
                                    ans.second += 1;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    for(int i = 0; i < 8; i++)
    {
        if(cCoord(toCoord(bKing.y + knightPos[i].y, bKing.x + knightPos[i].x)) && b.board[bKing.y + knightPos[i].y][bKing.x + knightPos[i].x] == 'k')
        {
            ans.second += 1;
            break;
        }
    }
    return ans;
}

pair<int, int> isChecked(boardState &b)
{
    pair<int, int> ans(0, 0);
    coord bKing, wKing;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(b.board[i][j] == 'a')
            {
                bKing = toCoord(i, j);
            }
            else if(b.board[i][j] == 'A')
            {
                wKing = toCoord(i, j);
            }
        }
    }
    if(bKing.y-1 >= 0)
    {
        if(bKing.x-1 >= 0 && b.board[bKing.y-1][bKing.x-1] == 'P')
        {
            ans.first += 1;
        }
        else if(bKing.x+1 < 8 && b.board[bKing.y-1][bKing.x+1] == 'P')
        {
            ans.first += 1;
        }
    }
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if(i != 0 || j != 0)
            {
                for(int k = 1; k < 8; k++)
                {
                    if(cCoord(toCoord(bKing.y + i*k, bKing.x + j*k)))
                    {
                        char currentPiece = b.board[bKing.y + i*k][bKing.x + j*k];
                        if(currentPiece != 'O')
                        {
                            if(i == 0 || j == 0)
                            {
                                if(currentPiece == 'Q' || currentPiece == 'R')
                                {
                                    ans.first += 1;
                                    break;
                                }
                                else if(k == 1 && currentPiece == 'A')
                                {
                                    ans.first += 1;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            else
                            {
                                if(currentPiece == 'Q' || currentPiece == 'B')
                                {
                                    ans.first += 1;
                                    break;
                                }
                                else if(k == 1 && currentPiece == 'A')
                                {
                                    ans.first += 1;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    coord knightPos[8] = {toCoord(-1, -2),toCoord(-2, -1),toCoord(1, -2),toCoord(2, -1),toCoord(-1, 2),toCoord(-2, 1),toCoord(1, 2),toCoord(2, 1)};
    for(int i = 0; i < 8; i++)
    {
        if(cCoord(toCoord(bKing.y + knightPos[i].y, bKing.x + knightPos[i].x)) && b.board[bKing.y + knightPos[i].y][bKing.x + knightPos[i].x] == 'K')
        {
            ans.first += 1;
            break;
        }
    }



    if(wKing.y+1 < 8)
    {
        if(wKing.x-1 >= 0 && b.board[wKing.y+1][wKing.x-1] == 'p')
        {
            ans.second += 1;
        }
        else if(wKing.x+1 < 8 && b.board[wKing.y+1][wKing.x+1] == 'p')
        {
            ans.second += 1;
        }
    }

    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if(i != 0 || j != 0)
            {
                for(int k = 1; k < 8; k++)
                {
                    if(cCoord(toCoord(wKing.y + i*k, wKing.x + j*k)))
                    {
                        char currentPiece = b.board[wKing.y + i*k][wKing.x + j*k];
                        if(currentPiece != 'O')
                        {
                            if(i == 0 || j == 0)
                            {
                                if(currentPiece == 'q' || currentPiece == 'r')
                                {
                                    ans.second += 1;
                                    break;
                                }
                                else if(k == 1 && currentPiece == 'a')
                                {
                                    ans.second += 1;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            else
                            {
                                if(currentPiece == 'q' || currentPiece == 'b')
                                {
                                    ans.second += 1;
                                    break;
                                }
                                else if(k == 1 && currentPiece == 'a')
                                {
                                    ans.second += 1;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    for(int i = 0; i < 8; i++)
    {
        if(cCoord(toCoord(wKing.y + knightPos[i].y, wKing.x + knightPos[i].x)) && b.board[wKing.y + knightPos[i].y][wKing.x + knightPos[i].x] == 'k')
        {
            ans.second += 1;
            break;
        }
    }
    return ans;
}

vector<int> generateRookMoves(boardState &b, coord piece)
{
    int an = piece.y*512 + piece.x * 64;
    vector<int> ans;
    coord moves[4] = {toCoord(-1, 0), toCoord(1, 0), toCoord(0, -1), toCoord(0, 1)};
    for(int i = 0; i < 4; i++)
    {
        int j = 1;
        while(cCoord(toCoord(piece.y+moves[i].y*j, piece.x+moves[i].x*j)))
        {
            if(b.board[piece.y+moves[i].y*j][piece.x+moves[i].x*j] == 'O')
            {
                ans.push_back(an + (piece.y+moves[i].y*j)*8 + piece.x+moves[i].x*j);
            }
            else if(b.board[piece.y][piece.x] <= 'Z' && b.board[piece.y+moves[i].y*j][piece.x+moves[i].x*j] > 'Z')
            {
                ans.push_back(an + (piece.y+moves[i].y*j)*8 + piece.x+moves[i].x*j);
                break;
            }
            else if(b.board[piece.y][piece.x] >= 'a' && b.board[piece.y+moves[i].y*j][piece.x+moves[i].x*j] < 'a')
            {
                ans.push_back(an + (piece.y+moves[i].y*j)*8 + piece.x+moves[i].x*j);
                break;
            }
            else
            {
                break;
            }
            j++;
        }
    }
    return ans;
}

vector<int> generateBishopMoves(boardState &b, coord piece)
{
    int an = piece.y*512 + piece.x * 64;
    vector<int> ans;
    coord moves[4] = {toCoord(-1, -1), toCoord(1, 1), toCoord(1, -1), toCoord(-1, 1)};
    for(int i = 0; i < 4; i++)
    {
        int j = 1;
        while(cCoord(toCoord(piece.y+moves[i].y*j, piece.x+moves[i].x*j)))
        {
            if(b.board[piece.y+moves[i].y*j][piece.x+moves[i].x*j] == 'O')
            {
                ans.push_back(an + (piece.y+moves[i].y*j)*8 + piece.x+moves[i].x*j);
            }
            else if(b.board[piece.y][piece.x] <= 'Z' && b.board[piece.y+moves[i].y*j][piece.x+moves[i].x*j] > 'Z')
            {
                ans.push_back(an + (piece.y+moves[i].y*j)*8 + piece.x+moves[i].x*j);
                break;
            }
            else if(b.board[piece.y][piece.x] >= 'a' && b.board[piece.y+moves[i].y*j][piece.x+moves[i].x*j] < 'a')
            {
                ans.push_back(an + (piece.y+moves[i].y*j)*8 + piece.x+moves[i].x*j);
                break;
            }
            else
            {
                break;
            }
            j++;
        }
    }
    return ans;
}

vector<int> generateKnightMoves(boardState &b, coord piece)
{
    int an = piece.y*512 + piece.x * 64;
    vector<int> ans;
    coord knightPos[8] = {toCoord(-1, -2),toCoord(-2, -1),toCoord(1, -2),toCoord(2, -1),toCoord(-1, 2),toCoord(-2, 1),toCoord(1, 2),toCoord(2, 1)};
    for(int i = 0; i < 8; i++)
    {
        if(cCoord(toCoord(piece.y + knightPos[i].y, piece.x + knightPos[i].x)))
        {
            if(b.board[piece.y + knightPos[i].y][piece.x + knightPos[i].x] == 'O')
            {
                ans.push_back(an + (piece.y + knightPos[i].y)*8 + piece.x + knightPos[i].x);
            }
            else if(b.board[piece.y][piece.x] <= 'Z' && b.board[piece.y + knightPos[i].y][piece.x + knightPos[i].x] > 'Z')
            {
                ans.push_back(an + (piece.y + knightPos[i].y)*8 + piece.x + knightPos[i].x);
            }
            else if(b.board[piece.y][piece.x] >= 'a' && b.board[piece.y + knightPos[i].y][piece.x + knightPos[i].x] < 'a')
            {
                ans.push_back(an + (piece.y + knightPos[i].y)*8 + piece.x + knightPos[i].x);
            }
        }
    }
    return ans;
}

vector<int> generateKingMoves(boardState &b, coord piece)
{
    int an = piece.y*512 + piece.x * 64;
    vector<int> ans;
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            if(i != 0 || j != 0)
            {
                if(cCoord(toCoord(piece.y + i, piece.x + j)))
                {
                    if(b.board[piece.y + i][piece.x + j] == 'O')
                    {
                        ans.push_back(an + (piece.y + i)*8 + piece.x + j);
                    }
                    else if(b.board[piece.y][piece.x] <= 'Z' && b.board[piece.y + i][piece.x + j] > 'Z')
                    {
                        ans.push_back(an + (piece.y + i)*8 + piece.x + j);
                    }
                    else if(b.board[piece.y][piece.x] >= 'a' && b.board[piece.y + i][piece.x + j] < 'a')
                    {
                        ans.push_back(an + (piece.y + i)*8 + piece.x + j);
                    }
                }
            }
        }
    }
    return ans;
}

vector<int> generatePawnMoves(boardState &b, coord piece)
{
    int an = piece.y*512 + piece.x * 64;
    vector<int> ans;

    if(b.board[piece.y][piece.x] == 'p' && piece.y == 6 && b.board[5][piece.x] == 'O' && b.board[4][piece.x] == 'O')
    {
        ans.push_back(an + 4*8 + piece.x);
    }
    if(b.board[piece.y][piece.x] == 'p' && piece.y > 0 && b.board[piece.y-1][piece.x] == 'O')
    {
        ans.push_back(an + (piece.y-1)*8 + piece.x);
    }
    if(b.board[piece.y][piece.x] == 'p' && piece.y > 0 && piece.x > 0 && b.board[piece.y-1][piece.x-1] < 'a' && b.board[piece.y-1][piece.x-1] != 'O')
    {
        ans.push_back(an + (piece.y-1)*8 + piece.x-1);
    }
    if(b.board[piece.y][piece.x] == 'p' && piece.y > 0 && piece.x < 7 && b.board[piece.y-1][piece.x+1] < 'a' && b.board[piece.y-1][piece.x+1] != 'O')
    {
        ans.push_back(an + (piece.y-1)*8 + piece.x+1);
    }

    if(b.board[piece.y][piece.x] == 'P' && piece.y == 1 && b.board[2][piece.x] == 'O' && b.board[3][piece.x] == 'O')
    {
        ans.push_back(an + 3*8 + piece.x);
    }
    if(b.board[piece.y][piece.x] == 'P' && piece.y < 7 && b.board[piece.y+1][piece.x] == 'O')
    {
        ans.push_back(an + (piece.y+1)*8 + piece.x);
    }
    if(b.board[piece.y][piece.x] == 'P' && piece.y < 7 && piece.x > 0 && b.board[piece.y+1][piece.x-1] > 'Z')
    {
        ans.push_back(an + (piece.y+1)*8 + piece.x-1);
    }
    if(b.board[piece.y][piece.x] == 'P' && piece.y < 7 && piece.x < 7 && b.board[piece.y+1][piece.x+1] > 'Z')
    {
        ans.push_back(an + (piece.y+1)*8 + piece.x+1);
    }
    return ans;
}

vector<int> generateQueenMoves(boardState &b, coord piece)
{
    vector<int> ans = generateRookMoves(b, piece);
    vector<int> secondSet = generateBishopMoves(b, piece);
    ans.insert(ans.end(), secondSet.begin(), secondSet.end());
    return ans;
}

vector<int> generateMoves(boardState &b)
{
    vector<int> ans;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if((!(b.turns % 2) && (b.board[i][j] <= 'Z' && b.board[i][j] >= 'A' && b.board[i][j] != 'O')) || ((b.turns % 2) && b.board[i][j] <= 'z' && b.board[i][j] >= 'a'))
            {
                vector<int>temp;
                switch(tolower(b.board[i][j]))
                {
                case 'r':
                    {
                        temp = generateRookMoves(b, toCoord(i, j));
                        break;
                    }
                case 'k':
                    {
                        temp = generateKnightMoves(b, toCoord(i, j));
                        break;
                    }
                case 'b':
                    {
                        temp = generateBishopMoves(b, toCoord(i, j));
                        break;
                    }
                case 'q':
                    {
                        temp = generateQueenMoves(b, toCoord(i, j));
                        break;
                    }
                case 'a':
                    {
                        temp = generateKingMoves(b, toCoord(i, j));
                        break;
                    }
                case 'p':
                    {
                        temp = generatePawnMoves(b, toCoord(i, j));
                        break;
                    }
                default:
                    break;
                }
                 ans.insert(ans.end(), temp.begin(), temp.end());
            }
        }
    }
    return ans;
}

void initBoardState(boardState &b)
{
    char board[8][8] = {
        {'R', 'K', 'B', 'Q', 'A', 'B', 'K', 'R'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O'},
        {'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O'},
        {'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O'},
        {'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'r', 'k', 'b', 'q', 'a', 'b', 'k', 'r'}};
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            b.board[i][j] = board[i][j];
        }
    }
    b.turns = 0;
}

void initTestBoardState(boardState &b)
{
    char board[8][8] = {
        {'R', 'K', 'B', 'Q', 'A', 'B', 'K', 'R'},
        {'P', 'P', 'q', 'O', 'P', 'P', 'P', 'P'},
        {'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O'},
        {'O', 'O', 'O', 'P', 'O', 'O', 'O', 'O'},
        {'O', 'O', 'O', 'O', 'p', 'O', 'O', 'O'},
        {'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O'},
        {'p', 'p', 'p', 'p', 'o', 'p', 'p', 'p'},
        {'r', 'k', 'b', 'O', 'a', 'b', 'k', 'r'}};
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            b.board[i][j] = board[i][j];
        }
    }
    b.turns = 0;
}

boardState movePiece(boardState &b, int m)
{
    pair<coord, coord> currMove = decode(m);
    boardState newBoard = b;
    newBoard.board[currMove.second.y][currMove.second.x] = newBoard.board[currMove.first.y][currMove.first.x];
    newBoard.board[currMove.first.y][currMove.first.x] = 'O';
    newBoard.turns++;
    return newBoard;
}

float calcVal(boardState &b)
{
    float posMultiplier = 0.005;
    float ans = 0;
    float abspoints = 0;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            ans += piecePoints[b.board[i][j]];
            abspoints += abs(piecePoints[b.board[i][j]]);
            if(i >= 3 && j >= 3 && i <= 4 && j <= 4)
            {
                /*pair<int, int> guards = getGuards(b, toCoord(i, j));
                ans += posMultiplier*guards.first*2;
                ans -= posMultiplier*guards.second*2;*/
                if(b.board[i][j] <= 'Z' && b.board[i][j] >= 'A' && b.board[i][j] != 'O')
                {
                    ans += posMultiplier*2;
                }
                if(b.board[i][j] <= 'z' && b.board[i][j] >= 'a')
                {
                    ans -= posMultiplier*2;
                }
            }
            if(i >= 2 && j >= 2 && i <= 5 && j <= 5)
            {
                /*pair<int, int> guards = getGuards(b, toCoord(i, j));
                ans += posMultiplier*guards.first;
                ans -= posMultiplier*guards.second;*/
                if(b.board[i][j] <= 'Z' && b.board[i][j] >= 'A' && b.board[i][j] != 'O')
                {
                    ans += posMultiplier;
                }
                if(b.board[i][j] <= 'z' && b.board[i][j] >= 'a')
                {
                    ans -= posMultiplier;
                }

            }
        }
    }
    return ans;
}

pair<int, float> recursiveFind(int plyLeft, boardState &b)
{
    if(plyLeft == 0)
    {
        vector<int> possibleMoves = generateMoves(b);
        if(!(b.turns % 2))
        {
            pair<int, float> bestMove(0, -300);
            for(int i = 0; i < possibleMoves.size(); i++)
            {
                boardState newBoard = movePiece(b, possibleMoves[i]);
                if(!isChecked(newBoard).second)
                {
                    float newScore = calcVal(newBoard);
                    float risk = abs(1-abs(newScore/bestMove.second));
                    bool takeRisk = false;
                    if(risk < risk_factor)
                    {
                        if(rand() % 100 > (100 * risk/risk_factor))
                        {
                            takeRisk = true;
                        }
                    }
                    if(takeRisk || newScore > bestMove.second)
                    {
                        bestMove.first = possibleMoves[i];
                        bestMove.second = newScore;
                    }
                }
            }
            return bestMove;
        }
        else
        {
            pair<int, float> bestMove(0, 300);
            for(int i = 0; i < possibleMoves.size(); i++)
            {
                boardState newBoard = movePiece(b, possibleMoves[i]);
                if(!isChecked(newBoard).first)
                {
                    float newScore = calcVal(newBoard);
                    float risk = abs(1-abs(newScore/bestMove.second));
                    bool takeRisk = false;
                    if(risk < risk_factor)
                    {
                        if(rand() % 100 > (100 * risk/risk_factor))
                        {
                            takeRisk = true;
                        }
                    }
                    if(takeRisk || newScore < bestMove.second)
                    {
                        bestMove.first = possibleMoves[i];
                        bestMove.second = newScore;
                    }
                }
            }
            return bestMove;
        }
    }
    else
    {
        vector<int> possibleMoves = generateMoves(b);
        if(!(b.turns % 2))
        {
            pair<int, float> bestMove(0, -300);
            for(int i = 0; i < possibleMoves.size(); i++)
            {
                boardState newBoard = movePiece(b, possibleMoves[i]);
                if(!isChecked(newBoard).second)
                {
                    float newScore = recursiveFind(plyLeft-1, newBoard).second;
                    float aggressiveScore = calcVal(newBoard);
                    float risk = abs(1-abs(newScore/bestMove.second));
                    bool takeRisk = false;
                    if(risk < risk_factor)
                    {
                        if(rand() % 100 > (100 * risk/risk_factor))
                        {
                            takeRisk = true;
                        }
                    }
                    if(takeRisk || (newScore > bestMove.second) || ((aggressiveScore - bestMove.second)/(newScore - bestMove.second) > aggressive_threshold))
                    {
                        bestMove.first = possibleMoves[i];
                        bestMove.second = newScore;
                    }
                }
            }
            return bestMove;
        }
        else
        {
            pair<int, float> bestMove(0, 300);
            for(int i = 0; i < possibleMoves.size(); i++)
            {
                boardState newBoard = movePiece(b, possibleMoves[i]);
                if(!isChecked(newBoard).first)
                {
                    float newScore = recursiveFind(plyLeft-1, newBoard).second;
                    float aggressiveScore = calcVal(newBoard);
                    float risk = abs(1-abs(newScore/bestMove.second));
                    bool takeRisk = false;
                    if(risk < risk_factor)
                    {
                        if(rand() % 100 > (100 * risk/risk_factor))
                        {
                            takeRisk = true;
                        }
                    }
                    if(takeRisk || (newScore < bestMove.second) || ((aggressiveScore - bestMove.second)/(newScore - bestMove.second) < -aggressive_threshold))
                    {
                        bestMove.first = possibleMoves[i];
                        bestMove.second = newScore;
                    }
                }
            }
            return bestMove;
        }
    }
}

void drawBoard(boardState &b)
{
    al_clear_to_color(al_map_rgb(64,64,64));
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            bool lightSquare = bool(((j%2)+(i%2))%2);
            if(lightSquare)
            {
                al_draw_filled_rectangle((7-i)*100, j*100, (7-i)*100+100, j*100+100, al_map_rgb(196,196,196));
            }
        }
    }
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            switch(b.board[i][j])
            {
            case 'A':
                {
                    al_draw_bitmap_region(pieces, 0*100, 0*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            case 'a':
                {
                    al_draw_bitmap_region(pieces, 0*100, 1*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            case 'Q':
                {
                    al_draw_bitmap_region(pieces, 1*100, 0*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            case 'q':
                {
                    al_draw_bitmap_region(pieces, 1*100, 1*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            case 'B':
                {
                    al_draw_bitmap_region(pieces, 2*100, 0*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            case 'b':
                {
                    al_draw_bitmap_region(pieces, 2*100, 1*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            case 'K':
                {
                    al_draw_bitmap_region(pieces, 3*100, 0*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            case 'k':
                {
                    al_draw_bitmap_region(pieces, 3*100, 1*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            case 'R':
                {
                    al_draw_bitmap_region(pieces, 4*100, 0*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            case 'r':
                {
                    al_draw_bitmap_region(pieces, 4*100, 1*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            case 'P':
                {
                    al_draw_bitmap_region(pieces, 5*100, 0*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            case 'p':
                {
                    al_draw_bitmap_region(pieces, 5*100, 1*100, 100, 100, j*100, (7-i)*100, 0);
                    break;
                }
            default:
                    break;
            }
        }
    }
    //al_draw_bitmap(pieces, 0, 0, 0);
    al_flip_display();
}

string f = "a0";
string t = "a0";
int allegroMove = 0;
void mouseUpdate(ALLEGRO_EVENT *event)
{
    if(event->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
        cout << event->mouse.x << event->mouse.y << endl;
        int mousey = 7-(event->mouse.y/100);
        int mousex = event->mouse.x/100;
        if(f == "a0")
        {
            f[0] = char(mousex + 'a');
            f[1] = char(mousey + '1');
            //cout << char(mousex + 'a') << " " << char(mousey + '1') << endl;
        }
        else
        {
            t[0] = char(mousex + 'a');
            t[1] = char(mousey + '1');
            cout << char(mousex + 'a') << " " << char(mousey + '1') << endl;
        }
    }
}
boardState b;
void frameUpdate(ALLEGRO_EVENT *event)
{
    drawBoard(b);
}


int main()
{
    srand(0);
    AndyAllegro test;
    test.createWindow(800, 800, 60);
    test.setTitle("chess board");

    test.setScreenFunction(frameUpdate);
    test.setMouseFunction(mouseUpdate);

    pieces = al_load_bitmap("pieces.png");


    initBoardState(b);
    initPiecePoints(piecePoints);
    //cout << getGuards(b, toCoord(1, 3)).first << " " <<  getGuards(b, toCoord(2, 3)).second << endl;
    while(1)
    {
        drawBoard(b);

        pair<int, float> bestMove = recursiveFind(3, b);
        printMove(bestMove.first);
        //cout <<  bestMove.second << endl;
        b = movePiece(b, bestMove.first);
        drawBoard(b);
        cout << " " << calcVal(b) << endl;
        pair<int, int> check = isChecked(b);
        cout << " " << bestMove.first << " " << bestMove.second << endl;
        if ((bestMove.second < -150) || (bestMove.second > 150))
        {
            break;
        }
        vector<int> validMoves = generateMoves(b);
        f = "a0";
        t = "a0";
        while(true)
        {
            test.update();
            if((f != "a0")&&(t!="a0"))
            {
                if(find(validMoves.begin(), validMoves.end(), (f[1]-'1')*512 + (f[0]-'a')*64 + (t[1]-'1')*8 + t[0]-'a') != validMoves.end())
                {
                    break;
                }
                else
                {
                    f = "a0";
                    t = "a0";
                }
            }

        }
        b = movePiece(b, (f[1]-'1')*512 + (f[0]-'a')*64 + (t[1]-'1')*8 + t[0]-'a');
        //coord fr = toCoord(f[0]-'1', f[1]-'a');
        //coorf to = toCoord()
    }
    //al_rest(10);
    return 0;
}
