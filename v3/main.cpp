#include "structs.h"

#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include "gui.h"
using namespace std;

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

bitboard one = 1;
gui g;
pair<pair<XY, XY>, float> recursiveGenMoves(int plyLeft, board &b)
{
    int color = b.turns%2;
    if(plyLeft == 0)
    {
        b.genMove(BLACK);
        b.genMove(WHITE);
        pair<pair<XY, XY>, float> bestMove = pair<pair<XY, XY>, float>(pair<XY, XY>(XY(0,0), XY(0,0)),600*color-300);
        vector<pair<XY, XY>> validMoves;
        for(int i = 0; i < 6; i++)
        {
            for(int j = 0; j < b.piece[color][i].size(); j++)
            {
                for(int k = 0; k < 8; k++)
                {
                    for(int a = 0; a < 8; a++)
                    {
                        if(b.moves[color][i][j] & (one << (k*8+a)))
                            validMoves.push_back(std::pair<XY, XY>(b.piece[color][i][j], XY(k, a)));
                    }
                }
            }
        }
        for(int i = 0; i < validMoves.size(); i++)
        {
            b.makeMove(validMoves[i].first, validMoves[i].second);
            b.genMove(BLACK);
            b.genMove(WHITE);
            float newScore = b.assessScore();
            b.unMove();
            if(color == WHITE && newScore > bestMove.second)
            {
                bestMove.first = validMoves[i];
                bestMove.second = newScore;
            }
            else if(color == BLACK && newScore < bestMove.second)
            {
                bestMove.first = validMoves[i];
                bestMove.second = newScore;
            }
        }
        return bestMove;
    }
    else
    {
        b.genMove(BLACK);
        b.genMove(WHITE);
        pair<pair<XY, XY>, float> bestMove = pair<pair<XY, XY>, float>(pair<XY, XY>(XY(0,0), XY(0,0)),600*color-300);
        vector<pair<XY, XY>> validMoves;
        for(int i = 0; i < 6; i++)
        {
            for(int j = 0; j < b.piece[color][i].size(); j++)
            {
                for(int k = 0; k < 8; k++)
                {
                    for(int a = 0; a < 8; a++)
                    {
                        if(b.moves[color][i][j] & (one << (k*8+a)))
                            validMoves.push_back(std::pair<XY, XY>(b.piece[color][i][j], XY(k, a)));
                    }
                }
            }
        }
        for(int i = 0; i < validMoves.size(); i++)
        {
            b.makeMove(validMoves[i].first, validMoves[i].second);
            float newScore = recursiveGenMoves(plyLeft-1, b).second;
            b.unMove();
            if(color == WHITE && newScore > bestMove.second)
            {
                bestMove.first = validMoves[i];
                bestMove.second = newScore;
            }
            else if(color == BLACK && newScore < bestMove.second)
            {
                bestMove.first = validMoves[i];
                bestMove.second = newScore;
            }
        }
        return bestMove;
    }
}

int main()
{
    board b;
    b.resetBoard();
    g.drawAll(&b);
    al_rest(1);
    while(true)
    {
        pair<pair<XY, XY>, float> bestmove = recursiveGenMoves(3, b);
        b.makeMove(bestmove.first.first, bestmove.first.second);
        g.drawChecker();
        g.drawSquares(b.allPieces[WHITE], GREEN);
        g.drawSquares(b.allPieces[BLACK], BLUE);
        g.drawPieces(&b);
        al_flip_display();
        int u;
        cin >> u;
    }

    while(true)
    {
        int fy, fx, ty, tx;
        cin >> fy >> fx >> ty >> tx;
        b.makeMove(XY(fy, fx), XY(ty, tx));
        b.genMove(b.turns%2);
        b.genMove(1-b.turns%2);
        cout << "score " << b.assessScore() << " " << b.piece[WHITE][QUEEN].size() << endl;
        g.drawChecker();
        g.drawSquares(b.guards[WHITE], GREEN);
        g.drawPieces(&b);
        al_flip_display();
    }
    al_rest(10);
    return 0;
}
