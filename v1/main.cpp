#include "AndyAllegro.h"
#include <iostream>
#include <vector>
#include <map>
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
    /*vector<coord> wRooks;
    vector<coord> wKnights;
    vector<coord> wBishops;
    vector<coord> wQueens;
    vector<coord> wPawns;
    coord wKing;

    vector<coord> bRooks;
    vector<coord> bKnights;
    vector<coord> bBishops;
    vector<coord> bQueens;
    vector<coord> bPawns;
    coord bKing;*/
};




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
                        //cout << bKing.y + i*k << " " << bKing.x + j*k << endl;
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

//first bool whether black is checked, second bool whether white is checked
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
                        //cout << wKing.y + i*k << " " << wKing.x + j*k << endl;
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
    //cout << piece.y << " " << piece.x << endl;
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
                //cout << piece.y << " " << piece.x << " " << (piece.y + knightPos[i].y) << " " << piece.x + knightPos[i].x << endl;
                ans.push_back(an + (piece.y + knightPos[i].y)*8 + piece.x + knightPos[i].x);
            }
            else if(b.board[piece.y][piece.x] <= 'Z' && b.board[piece.y + knightPos[i].y][piece.x + knightPos[i].x] > 'Z')
            {
                //cout << piece.y << " " << piece.x << " " << (piece.y + knightPos[i].y) << " " << piece.x + knightPos[i].x << endl;
                ans.push_back(an + (piece.y + knightPos[i].y)*8 + piece.x + knightPos[i].x);
            }
            else if(b.board[piece.y][piece.x] >= 'a' && b.board[piece.y + knightPos[i].y][piece.x + knightPos[i].x] < 'a')
            {
                //cout << piece.y << " " << piece.x << " " << (piece.y + knightPos[i].y) << " " << piece.x + knightPos[i].x << endl;
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
                /*cout << b.board[i][j] << endl;
                //cout << ans.size() << endl;
                for(int k = 0; k < temp.size(); k++)
                {
                    //cout << temp[k] << " ";
                    pair<coord, coord> m = decode(temp[k]);
                    cout << "  " << char(m.first.x + 'a') << m.first.y+1 << " to " << char(m.second.x + 'a') << m.second.y+1 << endl;
                }
                cout << endl;*/
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
/*
    b.wRooks.push_back(toCoord(0, 0));
    b.wRooks.push_back(toCoord(0, 7));
    b.wKnights.push_back(toCoord(0, 1));
    b.wKnights.push_back(toCoord(0, 6));
    b.wBishops.push_back(toCoord(0, 2));
    b.wBishops.push_back(toCoord(0, 5));
    b.wQueens.push_back(toCoord(0, 3));
    b.wKing = toCoord(0, 4);
    b.wPawns.push_back(toCoord(1, 0));
    b.wPawns.push_back(toCoord(1, 1));
    b.wPawns.push_back(toCoord(1, 2));
    b.wPawns.push_back(toCoord(1, 3));
    b.wPawns.push_back(toCoord(1, 4));
    b.wPawns.push_back(toCoord(1, 5));
    b.wPawns.push_back(toCoord(1, 6));
    b.wPawns.push_back(toCoord(1, 7));

    b.bRooks.push_back(toCoord(7, 0));
    b.bRooks.push_back(toCoord(7, 7));
    b.bKnights.push_back(toCoord(7, 1));
    b.bKnights.push_back(toCoord(7, 6));
    b.bBishops.push_back(toCoord(7, 2));
    b.bBishops.push_back(toCoord(7, 5));
    b.bQueens.push_back(toCoord(7, 3));
    b.bKing = toCoord(7, 4);
    b.bPawns.push_back(toCoord(6, 0));
    b.bPawns.push_back(toCoord(6, 1));
    b.bPawns.push_back(toCoord(6, 2));
    b.bPawns.push_back(toCoord(6, 3));
    b.bPawns.push_back(toCoord(6, 4));
    b.bPawns.push_back(toCoord(6, 5));
    b.bPawns.push_back(toCoord(6, 6));
    b.bPawns.push_back(toCoord(6, 7));
*/
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
/*
    b.wRooks.push_back(toCoord(0, 0));
    b.wRooks.push_back(toCoord(0, 7));
    b.wKnights.push_back(toCoord(0, 1));
    b.wKnights.push_back(toCoord(0, 6));
    b.wBishops.push_back(toCoord(0, 2));
    b.wBishops.push_back(toCoord(0, 5));
    b.wQueens.push_back(toCoord(0, 3));
    wKing = toCoord(0, 4);
    b.wPawns.push_back(toCoord(1, 0));
    b.wPawns.push_back(toCoord(1, 1));
    b.wPawns.push_back(toCoord(1, 2));
    b.wPawns.push_back(toCoord(1, 3));
    b.wPawns.push_back(toCoord(1, 4));
    b.wPawns.push_back(toCoord(1, 5));
    b.wPawns.push_back(toCoord(1, 6));
    b.wPawns.push_back(toCoord(1, 7));

    b.bRooks.push_back(toCoord(3, 4));
    b.bRooks.push_back(toCoord(7, 7));
    b.bKnights.push_back(toCoord(7, 1));
    b.bKnights.push_back(toCoord(7, 6));
    b.bBishops.push_back(toCoord(7, 2));
    b.bBishops.push_back(toCoord(7, 5));
    b.bQueens.push_back(toCoord(7, 3));
    b.bKing = toCoord(7, 4);
    b.bPawns.push_back(toCoord(6, 0));
    b.bPawns.push_back(toCoord(6, 1));
    b.bPawns.push_back(toCoord(6, 2));
    b.bPawns.push_back(toCoord(6, 3));
    b.bPawns.push_back(toCoord(6, 4));
    b.bPawns.push_back(toCoord(6, 5));
    b.bPawns.push_back(toCoord(6, 6));
    b.bPawns.push_back(toCoord(6, 7));
*/
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
    float posMultiplier = 0.01;
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
                pair<int, int> guards = getGuards(b, toCoord(i, j));
                ans += posMultiplier*guards.first*2;
                ans -= posMultiplier*guards.second*2;
            }
            if(i >= 2 && j >= 2 && i <= 5 && j <= 5)
            {
                pair<int, int> guards = getGuards(b, toCoord(i, j));
                ans += posMultiplier*guards.first;
                ans -= posMultiplier*guards.second;

            }
        }
    }
    return ans / (abspoints/50);
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
                //printMove(possibleMoves[i]);
                boardState newBoard = movePiece(b, possibleMoves[i]);
                /*for(int i = 0; i < 8; i++)
                {
                    for(int j = 0; j < 8; j++)
                    {
                        cout << b.board[i][j];
                    }
                    cout << " ";
                    for(int j = 0; j < 8; j++)
                    {
                        cout << newBoard.board[i][j];
                    }
                    cout << endl;
                }*/
                float newScore = calcVal(newBoard);
                //cout << newScore << endl;
                if(newScore > bestMove.second)
                {
                    bestMove.first = possibleMoves[i];
                    bestMove.second = newScore;
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
                float newScore = calcVal(newBoard);
                if(newScore < bestMove.second)
                {
                    bestMove.first = possibleMoves[i];
                    bestMove.second = newScore;
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
                float newScore = recursiveFind(plyLeft-1, newBoard).second;
                if(newScore > bestMove.second)
                {
                    bestMove.first = possibleMoves[i];
                    bestMove.second = newScore;
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
                float newScore = recursiveFind(plyLeft-1, newBoard).second;
                if(newScore < bestMove.second)
                {
                    bestMove.first = possibleMoves[i];
                    bestMove.second = newScore;
                }
            }
            return bestMove;
        }
    }
}

void keyUpdate(ALLEGRO_EVENT *event)
{
    //handle keyboard presses here
    //cout << "Keyboard Event" << endl;
}

void mouseUpdate(ALLEGRO_EVENT *event)
{
    //handle mouse events here
    //cout << "Mouse Event" << endl;
}

void frameUpdate(ALLEGRO_EVENT *event)
{
    //draw each frame here

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
void btnFunction()
{
    //cout << "Button Pressed" << endl;
}

int main()
{
    AndyAllegro test;
    test.createWindow(800, 800, 60);
    test.setTitle("chess board");

    test.setScreenFunction(frameUpdate);
    test.setKeyFunction(keyUpdate);
    test.setMouseFunction(mouseUpdate);

    pieces = al_load_bitmap("pieces.png");

    boardState b;
    initBoardState(b);
    initPiecePoints(piecePoints);
    cout << getGuards(b, toCoord(1, 3)).first << " " <<  getGuards(b, toCoord(2, 3)).second << endl;
    //cout << calcVal(b) << endl;
    //pair<bool, bool> test = isChecked(b);
    //cout << test.first << " " << test.second << endl;
    //vector<int> moves = generateMoves(b);
    //cout << moves.size() << endl;
    //cout << calcVal(b) << endl;
    //pair<int, float> bestMove = recursiveFind(3, b);
    //printMove(bestMove.first);
   // cout <<  bestMove.second << endl;
    //b = movePiece(b, bestMove.first);
    /*while(1)
    {
        string from, to;
        cin >> from;
        cin >> to;
        if(from == "q")
        {
            break;
        }
        else
        {
            pair<coord, coord> userMove(toCoord(from[1]-'1', from[0]-'a'), toCoord(to[1]-'1', to[0]-'a'));
            b = movePiece(b, encode(userMove));
            pair<int, float> bestMove = recursiveFind(3, b);
            printMove(bestMove.first);
            //cout <<  bestMove.second << endl;
            b = movePiece(b, bestMove.first);
        }


    }*/
    while(1)
    {
        drawBoard(b);

        pair<int, float> bestMove = recursiveFind(2, b);
        string a;
        //cin >> a;
        printMove(bestMove.first);
        //cout <<  bestMove.second << endl;
        b = movePiece(b, bestMove.first);
        cout << " " << calcVal(b) << endl;
        pair<int, int> check = isChecked(b);
        cout << " " << check.first << " " << check.second << endl;
        if ((bestMove.second < -150) || (bestMove.second > 150))
        {
            break;
        }
    }
    return 0;
}
