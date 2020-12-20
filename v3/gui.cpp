#include "gui.h"

gui::gui()
{
    test = new AndyAllegro();
    test->createWindow(800, 800, 60);
    test->setTitle("chess board");
    pieces = al_load_bitmap(imagePath);
}

gui::~gui()
{
    //dtor
}

void gui::drawPieces(board *b)
{

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            for(int k = 0; k < b->piece[i][j].size(); k++)
            {
                al_draw_bitmap_region(pieces, j*100, i*100, 100, 100, b->piece[i][j][k].x*100, (7-b->piece[i][j][k].y)*100, 0);
            }
        }
    }
}

void gui::drawSquares(bitboard bb, ALLEGRO_COLOR color)
{
    for(int i = 0; i < 8; i++)
    {
        for(int k = 0; k < 8; k++)
        {
            if((one << (i*8+k)) & bb)
            {
                al_draw_filled_rectangle(k*100+10,(7-i)*100+10,k*100+90,(7-i)*100+90,color);
            }
        }
    }
}

void gui::drawChecker()
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
}

void gui::drawAll(board *b)
{
    drawChecker();
    drawPieces(b);
    al_flip_display();
}
