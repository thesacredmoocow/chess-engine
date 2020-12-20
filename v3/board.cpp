#include "board.h"

board::board()
{
    //ctor
    db.init();
}

board::~board()
{
    //dtor
}

lookupPoint board::makePoint(int c, int p, int id)
{
    lookupPoint a;
    a.c = c;
    a.p = p;
    a.id = id;
    return a;
}

void board::genMove(int color)
{
    guards[color] = 0;
    encodedMoves[color].clear();
    if(piece[color][KING].size() > 0)
    {
        moves[color][KING][0] = db.kingMoves[piece[color][KING][0].y][piece[color][KING][0].x] ^ (db.kingMoves[piece[color][KING][0].y][piece[color][KING][0].x] & allPieces[color]);
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                if(moves[color][KING][0] & (one << (j*8+k)))
                {
                    encodedMoves[color].push_back(std::pair<XY, XY>(piece[color][KING][0], XY(j, k)));
                }
            }
        }
    }
    guards[color] |= moves[color][KING][0];
    for(int i = 0; i < piece[color][ROOK].size(); i++)
    {
        moves[color][ROOK][i] = db.rookHashedMaps[piece[color][ROOK][i].y][piece[color][ROOK][i].x][(((allPieces[BLACK] | allPieces[WHITE]) & db.rookMasks[piece[color][ROOK][i].y][piece[color][ROOK][i].x])*db.rookKeys[piece[color][ROOK][i].y][piece[color][ROOK][i].x]) >> db.rookShifts[piece[color][ROOK][i].y][piece[color][ROOK][i].x]] ^ (db.rookHashedMaps[piece[color][ROOK][i].y][piece[color][ROOK][i].x][(((allPieces[BLACK] | allPieces[WHITE]) & db.rookMasks[piece[color][ROOK][i].y][piece[color][ROOK][i].x])*db.rookKeys[piece[color][ROOK][i].y][piece[color][ROOK][i].x]) >> db.rookShifts[piece[color][ROOK][i].y][piece[color][ROOK][i].x]] & allPieces[color]);
        guards[color] |= moves[color][ROOK][i];
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                if(moves[color][ROOK][i] & (one << (j*8+k)))
                {
                    encodedMoves[color].push_back(std::pair<XY, XY>(piece[color][ROOK][i], XY(j, k)));
                }
            }
        }
    }
    for(int i = 0; i < piece[color][BISHOP].size(); i++)
    {
        moves[color][BISHOP][i] = db.bishopHashedMaps[piece[color][BISHOP][i].y][piece[color][BISHOP][i].x][(((allPieces[BLACK] | allPieces[WHITE]) & db.bishopMasks[piece[color][BISHOP][i].y][piece[color][BISHOP][i].x])*db.bishopKeys[piece[color][BISHOP][i].y][piece[color][BISHOP][i].x]) >> db.bishopShifts[piece[color][BISHOP][i].y][piece[color][BISHOP][i].x]] ^ (db.bishopHashedMaps[piece[color][BISHOP][i].y][piece[color][BISHOP][i].x][(((allPieces[BLACK] | allPieces[WHITE]) & db.bishopMasks[piece[color][BISHOP][i].y][piece[color][BISHOP][i].x])*db.bishopKeys[piece[color][BISHOP][i].y][piece[color][BISHOP][i].x]) >> db.bishopShifts[piece[color][BISHOP][i].y][piece[color][BISHOP][i].x]] & allPieces[color]);
        guards[color] |= moves[color][BISHOP][i];
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                if(moves[color][BISHOP][i] & (one << (j*8+k)))
                {
                    encodedMoves[color].push_back(std::pair<XY, XY>(piece[color][BISHOP][i], XY(j, k)));
                }
            }
        }
    }
    for(int i = 0; i < piece[color][QUEEN].size(); i++)
    {
        moves[color][QUEEN][i] = (db.bishopHashedMaps[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x][(((allPieces[BLACK] | allPieces[WHITE]) & db.bishopMasks[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x])*db.bishopKeys[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x]) >> db.bishopShifts[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x]] ^ (db.bishopHashedMaps[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x][(((allPieces[BLACK] | allPieces[WHITE]) & db.bishopMasks[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x])*db.bishopKeys[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x]) >> db.bishopShifts[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x]] & allPieces[color])) | (db.rookHashedMaps[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x][(((allPieces[BLACK] | allPieces[WHITE]) & db.rookMasks[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x])*db.rookKeys[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x]) >> db.rookShifts[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x]] ^ (db.rookHashedMaps[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x][(((allPieces[BLACK] | allPieces[WHITE]) & db.rookMasks[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x])*db.rookKeys[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x]) >> db.rookShifts[piece[color][QUEEN][i].y][piece[color][QUEEN][i].x]] & allPieces[color]));
        guards[color] |= moves[color][QUEEN][i];
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                if(moves[color][QUEEN][i] & (one << (j*8+k)))
                {
                    encodedMoves[color].push_back(std::pair<XY, XY>(piece[color][QUEEN][i], XY(j, k)));
                }
            }
        }
    }
    for(int i = 0; i < piece[color][KNIGHT].size(); i++)
    {
        moves[color][KNIGHT][i] = db.knightMoves[piece[color][KNIGHT][i].y][piece[color][KNIGHT][i].x] ^ (db.knightMoves[piece[color][KNIGHT][i].y][piece[color][KNIGHT][i].x] & allPieces[color]);
        guards[color] |= moves[color][KNIGHT][i];
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                if(moves[color][KNIGHT][i] & (one << (j*8+k)))
                {
                    encodedMoves[color].push_back(std::pair<XY, XY>(piece[color][KNIGHT][i], XY(j, k)));
                }
            }
        }
    }
    for(int i = 0; i < piece[color][PAWN].size(); i++)
    {
        moves[color][PAWN][i] = (db.pawnMoves[color][piece[color][PAWN][i].y][piece[color][PAWN][i].x] ^ (db.pawnMoves[color][piece[color][PAWN][i].y][piece[color][PAWN][i].x] & (allPieces[BLACK] | allPieces[WHITE])));
        if((piece[color][PAWN][i].y == db.startRank[color]) && moves[color][PAWN][i])
        {
            moves[color][PAWN][i] |= (db.pawnStart[color][piece[color][PAWN][i].x] ^ (db.pawnStart[color][piece[color][PAWN][i].x] & (allPieces[BLACK] | allPieces[WHITE])));
        }
        moves[color][PAWN][i] |= (db.pawnCaptures[color][piece[color][PAWN][i].y][piece[color][PAWN][i].x] & allPieces[1-color]);
        guards[color] |= db.pawnCaptures[color][piece[color][PAWN][i].y][piece[color][PAWN][i].x];
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                if(moves[color][PAWN][i] & (one << (j*8+k)))
                {
                    encodedMoves[color].push_back(std::pair<XY, XY>(piece[color][PAWN][i], XY(j, k)));
                }
            }
        }
    }
}

void board::resetBoard()
{
    turns = 0;
    /*pieceCount[WHITE][KING] = 1;
    pieceCount[BLACK][KING] = 1;
    pieceCount[WHITE][QUEEN] = 1;
    pieceCount[BLACK][QUEEN] = 1;
    pieceCount[WHITE][BISHOP] = 2;
    pieceCount[BLACK][BISHOP] = 2;
    pieceCount[WHITE][KNIGHT] = 2;
    pieceCount[BLACK][KNIGHT] = 2;
    pieceCount[WHITE][ROOK] = 2;
    pieceCount[BLACK][ROOK] = 2;
    pieceCount[WHITE][PAWN] = 8;
    pieceCount[BLACK][PAWN] = 8;

    piece[WHITE][KING][0] = XY(0, 4);
    piece[BLACK][KING][0] = XY(7, 4);

    piece[WHITE][QUEEN][0] = XY(0, 3);
    piece[BLACK][QUEEN][0] = XY(7, 3);

    piece[WHITE][BISHOP][0] = XY(0, 2);
    piece[WHITE][BISHOP][1] = XY(0, 5);
    piece[BLACK][BISHOP][0] = XY(7, 2);
    piece[BLACK][BISHOP][1] = XY(7, 5);

    piece[WHITE][KNIGHT][0] = XY(0, 1);
    piece[WHITE][KNIGHT][1] = XY(0, 6);
    piece[BLACK][KNIGHT][0] = XY(7, 1);
    piece[BLACK][KNIGHT][1] = XY(7, 6);

    piece[WHITE][ROOK][0] = XY(0, 0);
    piece[WHITE][ROOK][1] = XY(0, 7);
    piece[BLACK][ROOK][0] = XY(7, 0);
    piece[BLACK][ROOK][1] = XY(7, 7);

    piece[WHITE][PAWN][0] = XY(1, 0);
    piece[WHITE][PAWN][1] = XY(1, 1);
    piece[WHITE][PAWN][2] = XY(1, 2);
    piece[WHITE][PAWN][3] = XY(1, 3);
    piece[WHITE][PAWN][4] = XY(1, 4);
    piece[WHITE][PAWN][5] = XY(1, 5);
    piece[WHITE][PAWN][6] = XY(1, 6);
    piece[WHITE][PAWN][7] = XY(1, 7);
    piece[BLACK][PAWN][0] = XY(6, 0);
    piece[BLACK][PAWN][1] = XY(6, 1);
    piece[BLACK][PAWN][2] = XY(6, 2);
    piece[BLACK][PAWN][3] = XY(6, 3);
    piece[BLACK][PAWN][4] = XY(6, 4);
    piece[BLACK][PAWN][5] = XY(6, 5);
    piece[BLACK][PAWN][6] = XY(6, 6);
    piece[BLACK][PAWN][7] = XY(6, 7);*/
    piece[WHITE][KING].push_back(XY(0, 4));
    piece[BLACK][KING].push_back(XY(7, 4));

    piece[WHITE][QUEEN].push_back(XY(0, 3));
    piece[BLACK][QUEEN].push_back(XY(7, 3));

    piece[WHITE][BISHOP].push_back(XY(0, 2));
    piece[WHITE][BISHOP].push_back(XY(0, 5));
    piece[BLACK][BISHOP].push_back(XY(7, 2));
    piece[BLACK][BISHOP].push_back(XY(7, 5));

    piece[WHITE][KNIGHT].push_back(XY(0, 1));
    piece[WHITE][KNIGHT].push_back(XY(0, 6));
    piece[BLACK][KNIGHT].push_back(XY(7, 1));
    piece[BLACK][KNIGHT].push_back(XY(7, 6));

    piece[WHITE][ROOK].push_back(XY(0, 0));
    piece[WHITE][ROOK].push_back(XY(0, 7));
    piece[BLACK][ROOK].push_back(XY(7, 0));
    piece[BLACK][ROOK].push_back(XY(7, 7));

    piece[WHITE][PAWN].push_back(XY(1, 0));
    piece[WHITE][PAWN].push_back(XY(1, 1));
    piece[WHITE][PAWN].push_back(XY(1, 2));
    piece[WHITE][PAWN].push_back(XY(1, 3));
    piece[WHITE][PAWN].push_back(XY(1, 4));
    piece[WHITE][PAWN].push_back(XY(1, 5));
    piece[WHITE][PAWN].push_back(XY(1, 6));
    piece[WHITE][PAWN].push_back(XY(1, 7));
    piece[BLACK][PAWN].push_back(XY(6, 0));
    piece[BLACK][PAWN].push_back(XY(6, 1));
    piece[BLACK][PAWN].push_back(XY(6, 2));
    piece[BLACK][PAWN].push_back(XY(6, 3));
    piece[BLACK][PAWN].push_back(XY(6, 4));
    piece[BLACK][PAWN].push_back(XY(6, 5));
    piece[BLACK][PAWN].push_back(XY(6, 6));
    piece[BLACK][PAWN].push_back(XY(6, 7));

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            lookupTable[i][j] = makePoint(-1,-1,-1);
        }
    }

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            for(int k = 0; k < piece[i][j].size(); k++)
            {
                for(int a = 0; a < 8; a++)
                {
                    for(int b = 0; b < 8; b++)
                    {
                        if(piece[i][j][k].y == a && piece[i][j][k].x == b)
                        {
                            lookupTable[a][b].c = i;
                            lookupTable[a][b].p = j;
                            lookupTable[a][b].id = k;
                        }
                    }
                }
            }
        }
    }
    allPieces[WHITE] = 0;
    allPieces[BLACK] = 0;
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            allPieces[WHITE] |= (one << (i*8+j));
            allPieces[BLACK] |= (one << ((7-i)*8+j));
        }
    }
    genMove(WHITE);
    genMove(BLACK);
}

void board::makeMove(XY from, XY to)
{
    int sColor = bool(allPieces[BLACK] & (one << (from.y*8 + from.x)));
    lookupPoint currSource;
    currSource.c = sColor;
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < piece[sColor][i].size(); j++)
        {
            if(from == piece[sColor][i][j])
            {
                currSource.p = i;
                currSource.id = j;
                break;
            }
        }
    }
    lastFrom.push(from);
    lastTo.push(to);
    allPieces[sColor] ^= ((one << (from.y*8+from.x)) | (one << (to.y*8+to.x)));
    piece[sColor][currSource.p][currSource.id] = to;
    if(allPieces[1-sColor] & (one << (to.y*8 + to.x)))
    {
        for(int i = 0; i < 6; i++)
        {
            for(int j = 0; j < piece[1-sColor][i].size(); j++)
            {
                if(to == piece[1-sColor][i][j])
                {
                    lastCap.push(i);
                    piece[1-sColor][i].erase(piece[1-sColor][i].begin() + j);
                    break;
                }
            }
        }
        allPieces[1-sColor] ^= (one << (to.y*8+to.x));
    }
    else
    {
        lastCap.push(-1);
    }
    turns++;
}

void board::unMove()
{
    int sColor = bool(allPieces[BLACK] & (one << (lastTo.top().y*8 + lastTo.top().x)));
    allPieces[sColor] ^= (one << (lastFrom.top().y*8+lastFrom.top().x));
    allPieces[sColor] ^= (one << (lastTo.top().y*8+lastTo.top().x));
    lookupPoint currSource;
    currSource.c = sColor;
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < piece[sColor][i].size(); j++)
        {
            if(lastTo.top() == piece[sColor][i][j])
            {
                currSource.p = i;
                currSource.id = j;
                break;
            }
        }
    }
    piece[sColor][currSource.p][currSource.id] = lastFrom.top();
    if(lastCap.top() != -1)
    {
        piece[1-sColor][lastCap.top()].push_back(lastTo.top());
        allPieces[1-sColor] ^= (one << (lastTo.top().y*8+lastTo.top().x));
    }
    lastFrom.pop();
    lastTo.pop();
    lastCap.pop();
    turns--;
}

float board::assessScore()
{
    float score = 0;

    score += int(300*piece[WHITE][KING].size() - 300*piece[BLACK][KING].size());
    score += int(9*piece[WHITE][QUEEN].size() - 9*piece[BLACK][QUEEN].size());
    score += int(3*piece[WHITE][BISHOP].size() - 3*piece[BLACK][BISHOP].size());
    score += int(3*piece[WHITE][KNIGHT].size() - 3*piece[BLACK][KNIGHT].size());
    score += int(5*piece[WHITE][ROOK].size() - 5*piece[BLACK][ROOK].size());
    score += int(1*piece[WHITE][PAWN].size() - 1*piece[BLACK][PAWN].size());
    float centerBias = 0.1;
    for(int i = 2; i < 6; i++)
    {
        for(int j = 2; j < 6; j++)
        {
            if(guards[WHITE] & (one << (8*i+j)))
            {
                score += centerBias;
            }
            if(guards[BLACK] & (one << (8*i+j)))
            {
                score -= centerBias;
            }
        }
    }
    for(int i = 3; i < 5; i++)
    {
        for(int j = 3; j < 5; j++)
        {
            if(guards[WHITE] & (one << (8*i+j)))
            {
                score += centerBias;
            }
            if(guards[BLACK] & (one << (8*i+j)))
            {
                score -= centerBias;
            }
        }
    }
    float mobilityBias = 0.05;
    score += mobilityBias*encodedMoves[WHITE].size();
    score -= mobilityBias*encodedMoves[BLACK].size();
    return score;

}
