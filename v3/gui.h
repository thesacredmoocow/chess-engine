#ifndef GUI_H
#define GUI_H

#include "board.h"
#include "AndyAllegro.h"
class gui
{
    public:
        gui();
        virtual ~gui();
        void drawPieces(board *b);
        void drawSquares(bitboard bb, ALLEGRO_COLOR color);
        void drawChecker();
        void drawAll(board *b);
    protected:

    private:
        AndyAllegro *test;
        ALLEGRO_BITMAP *pieces;
        bitboard one = 1;
};

#endif // GUI_H
