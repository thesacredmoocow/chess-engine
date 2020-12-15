#include "AndyAllegro.h"
#include "defines.h"
using namespace std;

ALLEGRO_BITMAP *pieces;

//bitboard related variables
unsigned long long moveTables[2][7][8][8]; //1st dimension white or black, second dimension piece type, third dimension starting position
map<unsigned long long, pair<int, int>>bitboardToPair;
unsigned long long keys[8][8];

float calcVal(boardState &b)
{
    int materialPoints = b.piece[WHITE][KING].size()*200 - b.piece[BLACK][KING].size()*200;
    materialPoints += b.piece[WHITE][QUEEN].size()*9 - b.piece[BLACK][QUEEN].size()*9;
    materialPoints += b.piece[WHITE][BISHOP].size()*3 - b.piece[BLACK][BISHOP].size()*3;
    materialPoints += b.piece[WHITE][KNIGHT].size()*3 - b.piece[BLACK][KNIGHT].size()*3;
    materialPoints += b.piece[WHITE][ROOK].size()*5 - b.piece[BLACK][ROOK].size()*5;
    materialPoints += b.piece[WHITE][PAWN].size() - b.piece[BLACK][PAWN].size();
    float centerPoints = 0;
    float centerBias = 0.1;
    float mobilityPoints = 0;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(i >= 3 && i <= 4 && j >= 3 && j <= 4)
            {
                centerPoints += centerBias*2*(b.guards[WHITE][i][j] - b.guards[BLACK][i][j]);
            }
            else if(i >= 2 && i <= 5 && j >= 2 && j <= 5)
            {
                centerPoints += centerBias*(b.guards[WHITE][i][j] - b.guards[BLACK][i][j]);
            }
        }
    }
    return materialPoints + centerPoints;
}

//helper function prototypes
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

void drawSquares(unsigned long long bb, ALLEGRO_COLOR color)
{
    for(int i = 0; i < 8; i++)
    {
        for(int k = 0; k < 8; k++)
        {
            if(keys[i][k] & bb)
            {
                al_draw_filled_rectangle(k*100+10,(7-i)*100+10,k*100+90,(7-i)*100+90,color);
            }
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
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 7; j++)
        {
            for(int k = 0; k < b.piece[i][k].size(); k++)
            {
                //drawSquares(b.piece[i][j][k].second, al_map_rgb(80,200,80));
            }
        }
    }
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            for(int k = 0; k < b.piece[i][j].size(); k++)
            {
                al_draw_bitmap_region(pieces, j*100, i*100, 100, 100, b.piece[i][j][k].first.x*100, (7-b.piece[i][j][k].first.y)*100, 0);
            }
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

void populateMoveTables()
{
    ifstream rookMovement("movementTables/RookMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            unsigned long long key, val;
            rookMovement >> key >> val;
            moveTables[WHITE][ROOK][i][j] = val;
            moveTables[BLACK][ROOK][i][j] = val;
        }
    }
    rookMovement.close();

    ifstream knightMovement("movementTables/KnightMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            unsigned long long key, val;
            knightMovement >> key >> val;
            moveTables[WHITE][KNIGHT][i][j] = val;
            moveTables[BLACK][KNIGHT][i][j] = val;
        }
    }
    knightMovement.close();

    ifstream bishopMovement("movementTables/BishopMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            unsigned long long key, val;
            bishopMovement >> key >> val;
            moveTables[WHITE][BISHOP][i][j] = val;
            moveTables[BLACK][BISHOP][i][j] = val;
        }
    }
    bishopMovement.close();

    ifstream queenMovement("movementTables/QueenMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            unsigned long long key, val;
            queenMovement >> key >> val;
            //cout << val << endl;
            moveTables[WHITE][QUEEN][i][j] = val;
            moveTables[BLACK][QUEEN][i][j] = val;
        }
    }
    queenMovement.close();

    ifstream kingMovement("movementTables/KingMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            unsigned long long key, val;
            kingMovement >> key >> val;
            moveTables[WHITE][KING][i][j] = val;
            moveTables[BLACK][KING][i][j] = val;
        }
    }
    kingMovement.close();

    ifstream whitePawnMovement("movementTables/WhitePawnMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            unsigned long long key, val;
            whitePawnMovement >> key >> val;
            moveTables[WHITE][PAWN][i][j] = val;
        }
    }
    whitePawnMovement.close();

    ifstream blackPawnMovement("movementTables/BlackPawnMovement.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            unsigned long long key, val;
            blackPawnMovement >> key >> val;
            moveTables[BLACK][PAWN][i][j] = val;
        }
    }
    blackPawnMovement.close();

    ifstream whitePawnCapture("movementTables/WhitePawnCapture.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            unsigned long long key, val;
            whitePawnCapture >> key >> val;
            moveTables[WHITE][PAWN_CAPTURE][i][j] = val;
        }
    }
    whitePawnCapture.close();

    ifstream blackPawnCapture("movementTables/BlackPawnCapture.tab");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            unsigned long long key, val;
            blackPawnCapture >> key >> val;
            moveTables[BLACK][PAWN_CAPTURE][i][j] = val;
        }
    }
    blackPawnCapture.close();
}

void populateTranslators()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            unsigned long long key = pow(256, i)*pow(2, j);
            pair<int, int> c(i,j);
            keys[i][j] = key;
            bitboardToPair.insert(pair<unsigned long long, pair<int, int>>(key, c));
        }
    }
}

pair<unsigned long long, unsigned long long> generateBishopMovements(boardState &b, coord bishop, int color)
{
    unsigned long long ans = 0;
    unsigned long long gd = 0;
    int maxDist;
    //-,-
    maxDist = min(bishop.y, bishop.x);
    for(int i = 1; i <= maxDist; i++)
    {
        if(keys[bishop.y-i][bishop.x-i] & b.allPieces[color])
        {
            gd |= keys[bishop.y-i][bishop.x-i];
            break;
        }
        else if(keys[bishop.y-i][bishop.x-i] & b.allPieces[1 - color])
        {
            ans |= keys[bishop.y-i][bishop.x-i];
            gd |= keys[bishop.y-i][bishop.x-i];
            break;
        }
        else
        {
            ans |= keys[bishop.y-i][bishop.x-i];
            gd |= keys[bishop.y-i][bishop.x-i];
        }
    }

    //-,+
    maxDist = min(bishop.y, 7-bishop.x);
    for(int i = 1; i <= maxDist; i++)
    {
        if(keys[bishop.y-i][bishop.x+i] & b.allPieces[color])
        {
            gd |= keys[bishop.y-i][bishop.x+i];
            break;
        }
        else if(keys[bishop.y-i][bishop.x+i] & b.allPieces[1 - color])
        {
            ans |= keys[bishop.y-i][bishop.x+i];
            gd |= keys[bishop.y-i][bishop.x+i];
            break;
        }
        else
        {
            ans |= keys[bishop.y-i][bishop.x+i];
            gd |= keys[bishop.y-i][bishop.x+i];
        }
    }

    //-,-
    maxDist = min(7-bishop.y, bishop.x);
    for(int i = 1; i <= maxDist; i++)
    {
        if(keys[bishop.y+i][bishop.x-i] & b.allPieces[color])
        {
            gd |= keys[bishop.y+i][bishop.x-i];
            break;
        }
        else if(keys[bishop.y+i][bishop.x-i] & b.allPieces[1 - color])
        {
            ans |= keys[bishop.y+i][bishop.x-i];
            gd |= keys[bishop.y+i][bishop.x-i];
            break;
        }
        else
        {
            gd |= keys[bishop.y+i][bishop.x-i];
            ans |= keys[bishop.y+i][bishop.x-i];
        }
    }

    maxDist = min(7-bishop.y, 7-bishop.x);
    for(int i = 1; i <= maxDist; i++)
    {
        if(keys[bishop.y+i][bishop.x+i] & b.allPieces[color])
        {
            gd |= keys[bishop.y+i][bishop.x+i];
            break;
        }
        else if(keys[bishop.y+i][bishop.x+i] & b.allPieces[1 - color])
        {
            ans |= keys[bishop.y+i][bishop.x+i];
            gd |= keys[bishop.y+i][bishop.x+i];
            break;
        }
        else
        {
            gd |= keys[bishop.y+i][bishop.x+i];
            ans |= keys[bishop.y+i][bishop.x+i];
        }
    }
    return pair<unsigned long long, unsigned long long>(ans, gd);
}

pair<unsigned long long, unsigned long long> generateRookMovements(boardState &b, coord rook, int color)
{
    unsigned long long ans = 0;
    unsigned long long gd = 0;
    for(int i = rook.y-1; i >= 0; i--)
    {
        if(keys[i][rook.x] & b.allPieces[color])
        {
            gd |= keys[i][rook.x];
            break;
        }
        else if(keys[i][rook.x] & b.allPieces[1 - color])
        {
            ans |= keys[i][rook.x];
            gd |= keys[i][rook.x];
            break;
        }
        else
        {
            ans |= keys[i][rook.x];
            gd |= keys[i][rook.x];
        }
    }

    for(int i = rook.y+1; i < 8; i++)
    {
        if(keys[i][rook.x] & b.allPieces[color])
        {
            gd |= keys[i][rook.x];
            break;
        }
        else if(keys[i][rook.x] & b.allPieces[1 - color])
        {
            ans |= keys[i][rook.x];
            gd |= keys[i][rook.x];
            break;
        }
        else
        {
            ans |= keys[i][rook.x];
            gd |= keys[i][rook.x];
        }
    }

    for(int i = rook.x-1; i >= 0; i--)
    {
        if(keys[rook.y][i] & b.allPieces[color])
        {
            gd |= keys[rook.y][i];
            break;
        }
        else if(keys[rook.y][i] & b.allPieces[1 - color])
        {
            ans |= keys[rook.y][i];
            gd |= keys[rook.y][i];
            break;
        }
        else
        {
            gd |= keys[rook.y][i];
            ans |= keys[rook.y][i];
        }
    }

    for(int i = rook.x+1; i < 8; i++)
    {
        if(keys[rook.y][i] & b.allPieces[color])
        {
            gd |= keys[rook.y][i];
            break;
        }
        else if(keys[rook.y][i] & b.allPieces[1 - color])
        {
            ans |= keys[rook.y][i];
            gd |= keys[rook.y][i];
            break;
        }
        else
        {
            gd |= keys[rook.y][i];
            ans |= keys[rook.y][i];
        }
    }
    return pair<unsigned long long, unsigned long long>(ans, gd);

}
void generateMovements(boardState &b)
{
    //drawBoard(b);

    for(int i = 0; i < b.piece[BLACK][KNIGHT].size(); i++)
    {
        b.piece[BLACK][KNIGHT][i].second = moveTables[BLACK][KNIGHT][b.piece[BLACK][KNIGHT][i].first.y][b.piece[BLACK][KNIGHT][i].first.x] ^ (moveTables[BLACK][KNIGHT][b.piece[BLACK][KNIGHT][i].first.y][b.piece[BLACK][KNIGHT][i].first.x] & b.allPieces[BLACK]);
        b.guardTable[BLACK][KNIGHT][i] |= moveTables[BLACK][KNIGHT][b.piece[BLACK][KNIGHT][i].first.y][b.piece[BLACK][KNIGHT][i].first.x];
    }
    for(int i = 0; i < b.piece[WHITE][KNIGHT].size(); i++)
    {
        b.piece[WHITE][KNIGHT][i].second = moveTables[WHITE][KNIGHT][b.piece[WHITE][KNIGHT][i].first.y][b.piece[WHITE][KNIGHT][i].first.x] ^ (moveTables[WHITE][KNIGHT][b.piece[WHITE][KNIGHT][i].first.y][b.piece[WHITE][KNIGHT][i].first.x] & b.allPieces[WHITE]);
        b.guardTable[WHITE][KNIGHT][i] |= moveTables[WHITE][KNIGHT][b.piece[WHITE][KNIGHT][i].first.y][b.piece[WHITE][KNIGHT][i].first.x];
    }
    for(int i = 0; i < b.piece[WHITE][PAWN].size(); i++)
    {
        b.piece[WHITE][PAWN][i].second = moveTables[WHITE][PAWN][b.piece[WHITE][PAWN][i].first.y][b.piece[WHITE][PAWN][i].first.x] ^ (moveTables[WHITE][PAWN][b.piece[WHITE][PAWN][i].first.y][b.piece[WHITE][PAWN][i].first.x] & (b.allPieces[WHITE] | b.allPieces[BLACK]));
        if(b.piece[WHITE][PAWN][i].first.y == 1 && b.piece[WHITE][PAWN][i].second && !(keys[b.piece[WHITE][PAWN][i].first.y+2][b.piece[WHITE][PAWN][i].first.x] & (b.allPieces[WHITE] | b.allPieces[BLACK])))
        {
            b.piece[WHITE][PAWN][i].second |= keys[b.piece[WHITE][PAWN][i].first.y+2][b.piece[WHITE][PAWN][i].first.x];
        }
    }
    for(int i = 0; i < b.piece[BLACK][PAWN].size(); i++)
    {
        b.piece[BLACK][PAWN][i].second = moveTables[BLACK][PAWN][b.piece[BLACK][PAWN][i].first.y][b.piece[BLACK][PAWN][i].first.x] ^ (moveTables[BLACK][PAWN][b.piece[BLACK][PAWN][i].first.y][b.piece[BLACK][PAWN][i].first.x] & (b.allPieces[BLACK] | b.allPieces[BLACK]));
        if(b.piece[BLACK][PAWN][i].first.y == 6 && b.piece[BLACK][PAWN][i].second && !(keys[b.piece[BLACK][PAWN][i].first.y-2][b.piece[BLACK][PAWN][i].first.x] & (b.allPieces[BLACK] | b.allPieces[BLACK])))
        {
            b.piece[BLACK][PAWN][i].second |= keys[b.piece[BLACK][PAWN][i].first.y-2][b.piece[BLACK][PAWN][i].first.x];
        }
    }

    for(int i = 0; i < b.piece[WHITE][PAWN].size(); i++)
    {
        b.piece[WHITE][PAWN_CAPTURE][i].first = b.piece[WHITE][PAWN][i].first;
        b.piece[WHITE][PAWN_CAPTURE][i].second |= (moveTables[WHITE][PAWN_CAPTURE][b.piece[WHITE][PAWN][i].first.y][b.piece[WHITE][PAWN][i].first.x] & b.allPieces[BLACK]);
        b.guardTable[WHITE][PAWN][i] |= moveTables[WHITE][PAWN_CAPTURE][b.piece[WHITE][PAWN][i].first.y][b.piece[WHITE][PAWN][i].first.x];
    }
    for(int i = 0; i < b.piece[BLACK][PAWN].size(); i++)
    {
        b.piece[BLACK][PAWN_CAPTURE][i].first = b.piece[BLACK][PAWN][i].first;
        b.piece[BLACK][PAWN_CAPTURE][i].second |= (moveTables[BLACK][PAWN_CAPTURE][b.piece[BLACK][PAWN][i].first.y][b.piece[BLACK][PAWN][i].first.x] & b.allPieces[WHITE]);
        b.guardTable[BLACK][PAWN][i] |= moveTables[BLACK][PAWN_CAPTURE][b.piece[BLACK][PAWN][i].first.y][b.piece[BLACK][PAWN][i].first.x];
    }

    b.piece[WHITE][KING][0].second = moveTables[BLACK][KING][b.piece[WHITE][KING][0].first.y][b.piece[WHITE][KING][0].first.x] ^ (moveTables[WHITE][KING][b.piece[WHITE][KING][0].first.y][b.piece[WHITE][KING][0].first.x] & b.allPieces[WHITE]);
    b.piece[BLACK][KING][0].second = moveTables[BLACK][KING][b.piece[BLACK][KING][0].first.y][b.piece[BLACK][KING][0].first.x] ^ (moveTables[BLACK][KING][b.piece[BLACK][KING][0].first.y][b.piece[BLACK][KING][0].first.x] & b.allPieces[BLACK]);
    b.guardTable[WHITE][KING][0] |= moveTables[WHITE][KING][b.piece[WHITE][KING][0].first.y][b.piece[WHITE][KING][0].first.x];
    b.guardTable[BLACK][KING][0] |= moveTables[BLACK][KING][b.piece[BLACK][KING][0].first.y][b.piece[BLACK][KING][0].first.x];
    for(int i = 0; i < b.piece[BLACK][BISHOP].size(); i++)
    {
        pair<unsigned long long, unsigned long long>p = generateBishopMovements(b, b.piece[BLACK][BISHOP][i].first, BLACK);
        b.piece[BLACK][BISHOP][i].second = p.first;
        b.guardTable[BLACK][BISHOP][i] = p.second;
    }
    for(int i = 0; i < b.piece[WHITE][BISHOP].size(); i++)
    {
        pair<unsigned long long, unsigned long long>p = generateBishopMovements(b, b.piece[WHITE][BISHOP][i].first, WHITE);
        b.piece[WHITE][BISHOP][i].second = p.first;
        b.guardTable[WHITE][BISHOP][i] = p.second;
    }

    for(int i = 0; i < b.piece[BLACK][ROOK].size(); i++)
    {
        pair<unsigned long long, unsigned long long>p = generateRookMovements(b, b.piece[BLACK][ROOK][i].first, BLACK);
        b.piece[BLACK][ROOK][i].second = p.first;
        b.guardTable[BLACK][ROOK][i] = p.second;
    }
    for(int i = 0; i < b.piece[WHITE][ROOK].size(); i++)
    {
        pair<unsigned long long, unsigned long long>p = generateRookMovements(b, b.piece[WHITE][ROOK][i].first, WHITE);
        b.piece[WHITE][ROOK][i].second = p.first;
        b.guardTable[WHITE][ROOK][i] = p.second;
    }

    for(int i = 0; i < b.piece[BLACK][QUEEN].size(); i++)
    {
        pair<unsigned long long, unsigned long long>p = generateRookMovements(b, b.piece[BLACK][QUEEN][i].first, BLACK);
        pair<unsigned long long, unsigned long long>q = generateBishopMovements(b, b.piece[BLACK][QUEEN][i].first, BLACK);
        b.piece[BLACK][QUEEN][i].second = p.first | q.first;
        b.guardTable[BLACK][QUEEN][i] = p.second | q.second;
    }
    for(int i = 0; i < b.piece[WHITE][QUEEN].size(); i++)
    {
        pair<unsigned long long, unsigned long long>p = generateRookMovements(b, b.piece[WHITE][QUEEN][i].first, WHITE);
        pair<unsigned long long, unsigned long long>q = generateBishopMovements(b, b.piece[WHITE][QUEEN][i].first, WHITE);
        b.piece[WHITE][QUEEN][i].second = p.first | q.first;
        b.guardTable[WHITE][QUEEN][i] = p.second | q.second;
    }
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            b.guards[0][i][j] = 0;
            b.guards[1][i][j] = 0;
            for(int k = 0; k < 2; k++)
            {
                for(int a = 0; a < 6; a++)
                {
                    for(int d = 0; d < b.piece[k][a].size(); d++)
                    {
                        b.guards[k][i][j] += bool(b.guardTable[k][a][d] & keys[i][j]);
                    }
                }
            }
        }
    }
    //al_flip_display();
}

void initBoardState(boardState &b)
{

    b.piece[WHITE][ROOK].push_back(pair<coord, unsigned long long>(toCoord(0, 0), 0));
    b.piece[WHITE][ROOK].push_back(pair<coord, unsigned long long>(toCoord(0, 7), 0));
    b.piece[WHITE][KNIGHT].push_back(pair<coord, unsigned long long>(toCoord(0, 1), 0));
    b.piece[WHITE][KNIGHT].push_back(pair<coord, unsigned long long>(toCoord(0, 6), 0));
    b.piece[WHITE][BISHOP].push_back(pair<coord, unsigned long long>(toCoord(0, 2), 0));
    b.piece[WHITE][BISHOP].push_back(pair<coord, unsigned long long>(toCoord(0, 5), 0));
    b.piece[WHITE][QUEEN].push_back(pair<coord, unsigned long long>(toCoord(0, 3), 0));
    b.piece[WHITE][KING].push_back(pair<coord, unsigned long long>(toCoord(0, 4), 0));

    b.guardTable[WHITE][ROOK].push_back(0);
    b.guardTable[WHITE][ROOK].push_back(0);
    b.guardTable[WHITE][BISHOP].push_back(0);
    b.guardTable[WHITE][BISHOP].push_back(0);
    b.guardTable[WHITE][KNIGHT].push_back(0);
    b.guardTable[WHITE][KNIGHT].push_back(0);
    b.guardTable[WHITE][QUEEN].push_back(0);
    b.guardTable[WHITE][KING].push_back(0);

    b.piece[WHITE][PAWN].push_back(pair<coord, unsigned long long>(toCoord(1, 0), 0));
    b.piece[WHITE][PAWN].push_back(pair<coord, unsigned long long>(toCoord(1, 1), 0));
    b.piece[WHITE][PAWN].push_back(pair<coord, unsigned long long>(toCoord(1, 2), 0));
    b.piece[WHITE][PAWN].push_back(pair<coord, unsigned long long>(toCoord(1, 3), 0));
    b.piece[WHITE][PAWN].push_back(pair<coord, unsigned long long>(toCoord(1, 4), 0));
    b.piece[WHITE][PAWN].push_back(pair<coord, unsigned long long>(toCoord(1, 5), 0));
    b.piece[WHITE][PAWN].push_back(pair<coord, unsigned long long>(toCoord(1, 6), 0));
    b.piece[WHITE][PAWN].push_back(pair<coord, unsigned long long>(toCoord(1, 7), 0));

    b.guardTable[WHITE][PAWN].push_back(0);
    b.guardTable[WHITE][PAWN].push_back(0);
    b.guardTable[WHITE][PAWN].push_back(0);
    b.guardTable[WHITE][PAWN].push_back(0);
    b.guardTable[WHITE][PAWN].push_back(0);
    b.guardTable[WHITE][PAWN].push_back(0);
    b.guardTable[WHITE][PAWN].push_back(0);
    b.guardTable[WHITE][PAWN].push_back(0);

    b.piece[WHITE][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(1, 0), 0));
    b.piece[WHITE][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(1, 1), 0));
    b.piece[WHITE][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(1, 2), 0));
    b.piece[WHITE][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(1, 3), 0));
    b.piece[WHITE][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(1, 4), 0));
    b.piece[WHITE][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(1, 5), 0));
    b.piece[WHITE][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(1, 6), 0));
    b.piece[WHITE][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(1, 7), 0));

    b.piece[BLACK][ROOK].push_back(pair<coord, unsigned long long>(toCoord(7, 0), 0));
    b.piece[BLACK][ROOK].push_back(pair<coord, unsigned long long>(toCoord(7, 7), 0));
    b.piece[BLACK][KNIGHT].push_back(pair<coord, unsigned long long>(toCoord(7, 1), 0));
    b.piece[BLACK][KNIGHT].push_back(pair<coord, unsigned long long>(toCoord(7, 6), 0));
    b.piece[BLACK][BISHOP].push_back(pair<coord, unsigned long long>(toCoord(7, 2), 0));
    b.piece[BLACK][BISHOP].push_back(pair<coord, unsigned long long>(toCoord(7, 5), 0));
    b.piece[BLACK][QUEEN].push_back(pair<coord, unsigned long long>(toCoord(7, 3), 0));
    b.piece[BLACK][KING].push_back(pair<coord, unsigned long long>(toCoord(7, 4), 0));

    b.guardTable[BLACK][ROOK].push_back(0);
    b.guardTable[BLACK][ROOK].push_back(0);
    b.guardTable[BLACK][BISHOP].push_back(0);
    b.guardTable[BLACK][BISHOP].push_back(0);
    b.guardTable[BLACK][KNIGHT].push_back(0);
    b.guardTable[BLACK][KNIGHT].push_back(0);
    b.guardTable[BLACK][QUEEN].push_back(0);
    b.guardTable[BLACK][KING].push_back(0);

    b.piece[BLACK][PAWN].push_back(pair<coord, unsigned long long>(toCoord(6, 0), 0));
    b.piece[BLACK][PAWN].push_back(pair<coord, unsigned long long>(toCoord(6, 1), 0));
    b.piece[BLACK][PAWN].push_back(pair<coord, unsigned long long>(toCoord(6, 2), 0));
    b.piece[BLACK][PAWN].push_back(pair<coord, unsigned long long>(toCoord(6, 3), 0));
    b.piece[BLACK][PAWN].push_back(pair<coord, unsigned long long>(toCoord(6, 4), 0));
    b.piece[BLACK][PAWN].push_back(pair<coord, unsigned long long>(toCoord(6, 5), 0));
    b.piece[BLACK][PAWN].push_back(pair<coord, unsigned long long>(toCoord(6, 6), 0));
    b.piece[BLACK][PAWN].push_back(pair<coord, unsigned long long>(toCoord(6, 7), 0));

    b.guardTable[BLACK][PAWN].push_back(0);
    b.guardTable[BLACK][PAWN].push_back(0);
    b.guardTable[BLACK][PAWN].push_back(0);
    b.guardTable[BLACK][PAWN].push_back(0);
    b.guardTable[BLACK][PAWN].push_back(0);
    b.guardTable[BLACK][PAWN].push_back(0);
    b.guardTable[BLACK][PAWN].push_back(0);
    b.guardTable[BLACK][PAWN].push_back(0);

    b.piece[BLACK][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(6, 0), 0));
    b.piece[BLACK][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(6, 1), 0));
    b.piece[BLACK][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(6, 2), 0));
    b.piece[BLACK][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(6, 3), 0));
    b.piece[BLACK][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(6, 4), 0));
    b.piece[BLACK][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(6, 5), 0));
    b.piece[BLACK][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(6, 6), 0));
    b.piece[BLACK][PAWN_CAPTURE].push_back(pair<coord, unsigned long long>(toCoord(6, 7), 0));

    b.allPieces[WHITE] = 0;
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            b.allPieces[WHITE] |= keys[i][j];
        }
    }

    b.allPieces[BLACK] = 0;
    for(int i = 6; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            b.allPieces[BLACK] |= keys[i][j];
        }
    }
    b.turn = 0;

}

mov createMov(int color, int piece, int index, coord destination)
{
    mov temp;
    temp.color = color;
    temp.piece = piece;
    temp.index = index;
    temp.destination = destination;
    return temp;
}

void movePiece(boardState &b, mov currMove)
{
    unsigned long long bitSwitcher = keys[b.piece[currMove.color][currMove.piece][currMove.index].first.y][b.piece[currMove.color][currMove.piece][currMove.index].first.x] + keys[currMove.destination.y][currMove.destination.x];
    b.allPieces[currMove.color] ^= bitSwitcher;
    b.piece[currMove.color][currMove.piece][currMove.index].first = currMove.destination;
    for(int i = 0; i < 7; i++)
    {
        for(int j = b.piece[1-currMove.color][i].size()-1; j >= 0; j--)
        {
            if((b.piece[1-currMove.color][i][j].first.x == currMove.destination.x) && (b.piece[1-currMove.color][i][j].first.y == currMove.destination.y))
            {
                b.piece[1-currMove.color][i].erase(b.piece[1-currMove.color][i].begin() + j);
                if(i != 6)
                {
                    b.guardTable[1-currMove.color][i].erase(b.guardTable[1-currMove.color][i].begin() + j);
                }
            }
        }
    }
    b.turn++;
}


pair<mov, float> recursiveFind(int plyLeft, boardState &b)
{
    int currColor = b.turn%2;
    if(plyLeft == 0)
    {
        pair<mov, float> bestMove(createMov(0, 0, 0, toCoord(0, 0)), 600*currColor-300);
        vector<mov> possibleMoves;
        generateMovements(b);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                for(int k = 0; k < 7; k++)
                {
                    for(int a = 0; a < b.piece[currColor][k].size(); a++)
                    {
                        if(keys[i][j] & b.piece[currColor][k][a].second)
                        {
                            possibleMoves.push_back(createMov(currColor, max(5, k), a, toCoord(i,j)));
                        }
                    }
                }
            }
        }
        for(int i = 0; i < possibleMoves.size(); i++)
        {
            cout << b.piece[possibleMoves[i].color][possibleMoves[i].piece][possibleMoves[i].index].first.y << " " << b.piece[possibleMoves[i].color][possibleMoves[i].piece][possibleMoves[i].index].first.x << " " << possibleMoves[i].destination.y << " " << possibleMoves[i].destination.x << endl;
        }
        for(int i = 0; i < possibleMoves.size(); i++)
        {
            boardState newBoard = b;
            movePiece(newBoard, possibleMoves[i]);
            float newScore = calcVal(newBoard);
            if(newScore > bestMove.second && currColor == WHITE)
            {
                bestMove.second = newScore;
                bestMove.first = possibleMoves[i];
            }
            else if(newScore < bestMove.second && currColor == BLACK)
            {
                bestMove.second = newScore;
                bestMove.first = possibleMoves[i];
            }

        }
        return bestMove;
    }
    else
    {
        pair<mov, float> bestMove(createMov(0, 0, 0, toCoord(0, 0)), 600*currColor-300);
        vector<mov> possibleMoves;
        generateMovements(b);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                for(int k = 0; k < 7; k++)
                {
                    for(int a = 0; a < b.piece[currColor][k].size(); a++)
                    {
                        if(keys[i][j] & b.piece[currColor][k][a].second)
                        {
                            possibleMoves.push_back(createMov(currColor, max(5, k), a, toCoord(i,j)));
                        }
                    }
                }
            }
        }
        for(int i = 0; i < possibleMoves.size(); i++)
        {
            boardState newBoard = b;
            movePiece(newBoard, possibleMoves[i]);
            float newScore = recursiveFind(plyLeft-1, newBoard).second;
            if(newScore > bestMove.second && currColor == WHITE)
            {
                bestMove.second = newScore;
                bestMove.first = possibleMoves[i];
            }
            else if(newScore < bestMove.second && currColor == BLACK)
            {
                bestMove.second = newScore;
                bestMove.first = possibleMoves[i];
            }

        }
        return bestMove;
    }
}


int main()
{
    AndyAllegro test;
    test.createWindow(800, 800, 60);
    test.setTitle("chess board");
    pieces = al_load_bitmap("pieces.png");

    populateMoveTables();
    populateTranslators();
    boardState b;
    initBoardState(b);
    generateMovements(b);
    while(1)
    {
        drawBoard(b);
        al_flip_display();
        mov bestMove = recursiveFind(0, b).first;
        movePiece(b, bestMove);
    }
    /*
    drawBoard(b);
    al_flip_display();
    cout << calcVal(b) << endl;
    int s;
    cin >> s;
    movePiece(b, createMov(WHITE, PAWN, 3, toCoord(3, 3)));
    generateMovements(b);
    drawBoard(b);
    al_flip_display();
    cout << calcVal(b) << endl;
    while
    al_rest(3);*/
    return 0;
}
