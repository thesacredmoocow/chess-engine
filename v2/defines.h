#include "structs.h"
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

//color IDs
#define WHITE 0
#define BLACK 1

//piece IDs
#define ROOK 4
#define KNIGHT 3
#define BISHOP 2
#define QUEEN 1
#define KING 0
#define PAWN 5
#define PAWN_CAPTURE 6

//directionality
#define D0 0
#define D45 1
#define D90 2
#define D135 3
#define D180 4
#define D225 5
#define D270 6
#define D315 7

#define GREEN al_map_rgb(80,200,80)
