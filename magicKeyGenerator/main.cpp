#include <iostream>
#include <fstream>
using namespace std;

typedef std::pair<int, int> XY;
typedef unsigned long long bitboard;
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
#define y first
#define x second

bitboard hashedRookTables[8][8][4096];
bitboard hashedBishopTables[8][8][512];
void clearBoard(bool b[8][8])
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            b[i][j] = false;
        }
    }
}

void printBin(unsigned long long n)
{
    bool ans[64];
    for(int i = 63; i > -1; i--)
    {
        ans[i] = n % 2;
        n /= 2;
    }
    for(int i = 0; i < 64; i++)
    {
        cout << ans[i];
    }
    cout << endl;
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

unsigned long long encodeBitboard(bool b[8][8])
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

unsigned long long ulongRand()
{
    unsigned long long ans = 0;
    unsigned long long one = 1;
    for(int i = 0; i < 16; i++)
    {
        unsigned long long newBit = 0;
        do
        {
            newBit = rand()%63;
        }while(ans & (one << newBit));
        //cout << newBit << " ";
        //printBin(one << newBit);
        //cout << newBit << " " ;
        //printBin(ans);
        //cout << endl;
        ans |= (one << newBit);
    }
    return ans;
    //return (rand() << (64 - 15)) + (rand() << (64 - 30)) + (rand() << (64 - 45)) + (rand() << (64 - 60)) + (rand() >> 11);
}

pair<unsigned long long, int> getRookKey(XY pos)
{
    for(int i = 0; i < 4096; i++)
    {
        hashedRookTables[pos.y][pos.x][i] = 0;
    }
    bitboard verticalBlockers[64];
    bitboard verticalMoveTable[64];
    bitboard verticalAttackTable[64];
    bitboard horizontalBlockers[64];
    bitboard horizontalMoveTable[64];
    bitboard horizontalAttackTable[64];
    bitboard masks[4096];
    bitboard moveTable[4096];
    bitboard attackTable[4096];
    bool serialized[8][8];
    bool s2[8][8];
    bool s3[8][8];
    clearBoard(serialized);
    for(int i = 0; i < (1 << 6); i++)
    {
        int currPos = 6;
        int numpos = 5;
        while(currPos > 0)
        {
            if(currPos == pos.y)
            {
                currPos--;
            }
            else
            {
                serialized[currPos][pos.x] = ((1 << numpos) & i);
                currPos--;
                numpos--;
            }
        }
        clearBoard(s2);
        clearBoard(s3);
        for(int i = pos.y + 1; i < 8; i++)
        {
            if(serialized[i][pos.x])
            {
                s3[i][pos.x] = true;
                break;
            }
            else
            {
                s2[i][pos.x] = true;
                s3[i][pos.x] = true;
            }
        }
        for(int i = pos.y - 1; i > -1; i--)
        {
            if(serialized[i][pos.x])
            {
                s3[i][pos.x] = true;
                break;
            }
            else
            {
                s2[i][pos.x] = true;
                s3[i][pos.x] = true;
            }
        }
        verticalBlockers[i] = encodeBitboard(serialized);
        verticalMoveTable[i] = encodeBitboard(s2);
        verticalAttackTable[i] = encodeBitboard(s3);
        //printBitboard(verticalBlockers[i]);
        //cout << endl;
        //printBitboard(verticalAttackTable[i]);
        //cout << endl << endl;
    }
    clearBoard(serialized);
    for(int i = 0; i < (1 << 6); i++)
    {
        int currPos = 6;
        int numpos = 5;
        while(currPos > 0)
        {
            if(currPos == pos.x)
            {
                currPos--;
            }
            else
            {
                serialized[pos.y][currPos] = ((1 << numpos) & i);
                currPos--;
                numpos--;
            }
        }
        clearBoard(s2);
        clearBoard(s3);
        for(int i = pos.x + 1; i < 8; i++)
        {
            if(serialized[pos.y][i])
            {
                s3[pos.y][i] = true;
                break;
            }
            else
            {
                s2[pos.y][i] = true;
                s3[pos.y][i] = true;
            }
        }
        for(int i = pos.x - 1; i > -1; i--)
        {
            if(serialized[pos.y][i])
            {
                s3[pos.y][i] = true;
                break;
            }
            else
            {
                s2[pos.y][i] = true;
                s3[pos.y][i] = true;
            }
        }
        horizontalBlockers[i] = encodeBitboard(serialized);
        horizontalMoveTable[i] = encodeBitboard(s2);
        horizontalAttackTable[i] = encodeBitboard(s3);
        //printBitboard(horizontalBlockers[i]);
        //cout << endl;
        //printBitboard(horizontalMoveTable[i]);
        //cout << endl << endl;
    }
    for(int i = 0; i < 64; i++)
    {
        for(int j = 0; j < 64; j++)
        {
            masks[i*64+j] = verticalBlockers[i] | horizontalBlockers[j];
            //printBitboard(masks[i*64+j]);
            //cout << endl;
            moveTable[i*64+j] = verticalMoveTable[i] | horizontalMoveTable[j];
            attackTable[i*64+j] = verticalAttackTable[i] | horizontalAttackTable[j];
        }
    }
    for(int i = 0; i < 4096; i++)
    {
        bitboard currBoard = masks[i];
        bitboard currTable = attackTable[i];
        bitboard hashed = currBoard * magicmoves_r_magics[pos.y*8+pos.x];
        hashed = hashed >> magicmoves_r_shift[pos.y*8+pos.x];
        if(pos.y == 7 && pos.x == 3 && hashed == 497)
        {
            printBitboard(masks[i]);
            cout << endl;
            printBitboard(attackTable[i]);
            cout << "////////////////////////" << endl;
        }
        hashedRookTables[pos.y][pos.x][hashed] = currTable;
    }
    /*pair<unsigned long long, int> bestKey(0, 13);
    while(bestKey.second > 12)
    {
        unsigned long long newKey = ulongRand();
        int currlen = 8;
        while(currlen < min(33, bestKey.second))
        {
            cout << hex << newKey << " " << currlen << endl;
            bool hasCollision = false;
            for(int i = 0; i < 16384; i++)
            {
                for(int j = 0; j < 16384; j++)
                {
                    if(moveTable[i] != moveTable[j] && (((masks[i] * newKey) >> (64 - currlen)) == ((masks[j] * newKey) >> (64 - currlen))))
                    {
                        hasCollision = true;
                        break;
                    }
                }
            }
            if(hasCollision)
            {
                currlen++;
            }
            else
            {
                bestKey.first = newKey;
                bestKey.second = currlen;
                //cout << bestKey.first << " " << bestKey.second << endl;
            }
        }
    }*/
    //return bestKey;
}

void getBishopKey(XY pos)
{
    unsigned long long one = 1;
    bitboard prehashes[4096];
    bitboard attackTables[4096];

    for(int i = 0; i < 4096; i++)
    {
        prehashes[i] = 0;
        attackTables[i] = 0;
        int currCounter = 0;
        for(int j = 1; j < 7; j++)
        {
            for(int k = 1; k < 7; k++)
            {
                if(((pos.y != j)||(pos.x != k)) && (abs(pos.y - j) == abs(pos.x-k)))
                {
                    unsigned long long currDigit = bool(i & (one << currCounter));
                    prehashes[i] |= (currDigit << ((8*j)+k));
                    currCounter++;
                }
            }
        }
    }
    for(int i = 0; i < 512; i++)
    {
        hashedBishopTables[pos.y][pos.x][i] = 0;
    }
    for(int i = 0; i < 4096; i++)
    {
        int j = pos.y + 1;
        int k = pos.x + 1;
        while(j < 8 && k < 8)
        {
            if(prehashes[i] & (one << (j*8+k)))
            {
                attackTables[i] |= (one << (j*8+k));
                break;
            }
            attackTables[i] |= (one << (j*8+k));
            k++;
            j++;
        }
        j = pos.y - 1;
        k = pos.x - 1;
        while(j >= 0 && k >= 0)
        {
            if(prehashes[i] & (one << (j*8+k)))
            {
                attackTables[i] |= (one << (j*8+k));
                break;
            }
            attackTables[i] |= (one << (j*8+k));
            k--;
            j--;
        }
        j = pos.y + 1;
        k = pos.x - 1;
        while(j < 8 && k >= 0)
        {
            if(prehashes[i] & (one << (j*8+k)))
            {
                attackTables[i] |= (one << (j*8+k));
                break;
            }
            attackTables[i] |= (one << (j*8+k));
            k--;
            j++;
        }
        j = pos.y - 1;
        k = pos.x + 1;
        while(j >= 0 && k < 8)
        {
            if(prehashes[i] & (one << (j*8+k)))
            {
                attackTables[i] |= (one << (j*8+k));
                break;
            }
            attackTables[i] |= (one << (j*8+k));
            k++;
            j--;
        }
        /*if(i < 100)
        {
            printBitboard(prehashes[i]);
            cout << endl;
            printBitboard(attackTables[i]);
            cout << "///////////////////////////////////" << endl;
        }*/

        //cout << endl;
        prehashes[i] *= magicmoves_b_magics[pos.y*8+pos.x];
        prehashes[i] = prehashes[i] >> magicmoves_b_shift[pos.y*8+pos.x];
        hashedBishopTables[pos.y][pos.x][prehashes[i]] = attackTables[i];
        if(prehashes[i] == 5)
        {
            //cout << i  << endl;
            //printBitboard(attackTables[i]);
        }
    }
    //printBitboard(hashedBishopTables[0][2][5]);

}

int main()
{
    getRookKey(XY(2, 2));
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            getRookKey(XY(i, j));
            string outFile = "Rooks/" + to_string(i) + to_string(j) + "RookHashed.tab";
            ofstream fout(outFile);
            for(int k = 0; k < 4096; k++)
            {
                fout << hashedRookTables[i][j][k] << endl;
            }
        }
    }
/*
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            getBishopKey(XY(i, j));
            string boutFile = "Bishops/" + to_string(i) + to_string(j) + "BishopHashed.tab";
            ofstream bfout(boutFile);
            for(int k = 0; k < 512; k++)
            {
                bfout << hashedBishopTables[i][j][k] << endl;
                if(i == 0 && j == 2 && k == 5)
                {
                    cout << hashedBishopTables[0][2][5] << endl;
                }
            }
        }
    }*/
    //getBishopKey(XY(0,1));
    //getBishopKey(XY(0,2));
    //cout << hashedBishopTables[0][2][5] << endl;
    //printBitboard(hashedBishopTables[0][2][5]);
    //cout << hashedBishopTables[0][2][5] << endl;
    /*cout << "Hello world!" << endl;
    srand(5353);
    getRookKey(XY(0, 1));
    ofstream rookKeys("rookKeys.txt");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            pair<unsigned long long, int> currKey = getRookKey(XY(i, j));
            cout << "good key" << i << " " << j << " " << currKey.first << " " << currKey.second << endl;
            rookKeys << i << " " << j << " " << currKey.first << " " << currKey.second << endl;
        }
    }
    //ulongRand();
    //cout << hex << ulongRand() << endl;*/
    //printBitboard(0x000101010101017E);
    return 0;
}
