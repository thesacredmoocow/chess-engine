//displays a menu with buttons. Give a width, height, and runmode pointer in order to initialize
//add a background by calling addBackground with a char array with the location/name of the bitmap
//add a button by calling addButton with the x, y, W, H, two strings containing the normal image as well as the pressed image, and the value to change runmode to when pressed
//update every frame by calling draw with mouse x and y values, as well as whether the mouse button is pressed
//show/hide button by calling the two respective functions along with the index of the button in the vector

#ifndef MENU_H
#define MENU_H

#include "Header.h"
#include "button.h"
class menu
{
    public:
        menu();
        menu(int w, int h);
        menu(int w, int h, int * m);

        bool addBackground(char * b);

        //add a button given the position, two bitmap names as char arrays, as well as the flag
        bool addButton(string name);
        bool addButton(string name, int x1, int y1, int x2, int y2, ALLEGRO_BITMAP *n, ALLEGRO_BITMAP *p, int flag);
        bool addButton(string name, int x1, int y1, int x2, int y2, ALLEGRO_BITMAP *n, ALLEGRO_BITMAP *p, void(*btnFunction)());
        bool addButton(string name, int x1, int y1, int x2, int y2, string text, ALLEGRO_COLOR n, ALLEGRO_COLOR p, int flag);
        bool addButton(string name, int x1, int y1, int x2, int y2, string text, ALLEGRO_COLOR n, ALLEGRO_COLOR p, void(*btnFunction)());

        bool setButtonUseability(string name, bool usable);
        bool setButtonVisibility(string name, bool visible);
        bool setButtonDimensions(string name, int x1, int y1, int x2, int y2);
        bool setButtonBitmaps(string name, ALLEGRO_BITMAP *n, ALLEGRO_BITMAP *p);
        bool setButtonColors(string name, ALLEGRO_COLOR n, ALLEGRO_COLOR p);
        bool setButtonModifiers(string name, int *m, int f);
        bool setButtonFunction(string name, void(*btnFunction)());
        bool setButtonText(string name, string text);

        //draw the menu(pass mouse info for click detection)
        void draw(int mx, int my, bool mpressed);

        //free up all bitmaps from memory
        void deleteAll();
    private:
        int width;
        int height;
        int *modifier;
        map<string, button> buttons;
        ALLEGRO_BITMAP * background;
};

#endif // MENU_H
