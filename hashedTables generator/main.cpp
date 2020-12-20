#include <iostream>
#include <vector>
using namespace std;

typedef std::pair<int, int> XY;
typedef unsigned long long bitboard;


#define y first
#define x second

vector<bitboard> rookPrehashes[8][8];
unsigned long long one = 1;
void recursiveGenerateRookPrehashes(int location, int upto, bitboard board, bool on)
{
    if(on && location != upto)
    {
        board |= (one << (upto.y*8+upto.x));
    }

    if(upto == 54)
    {
        rookPrehashes[location/8][location%8].push_back(board);
    }
    else
    {
        for(int i = upto+1, i < 55; i++)
        {
            if((location%8 == i%8) || (location/8 == i/8))
            {
                recursiveGenerateRookPrehashes(location, i, board, true);
                recursiveGenerateRookPrehashes(location, i, board, false);
            }
        }
    }
}

int main()
{
    recursiveGenerateRookPrehashes(0, )
    return 0;
}
