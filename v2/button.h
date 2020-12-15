#ifndef BUTTON_H
#define BUTTON_H

#include "Header.h"

extern ALLEGRO_FONT *smallfont;
//buttons structure that sets a predetermined variable to a predetermined value when pressed
class button
{
    public:
        button();
        button(int left, int top, int right, int bot);

        void setText(string t);
        void setBitmaps(ALLEGRO_BITMAP *n, ALLEGRO_BITMAP *p);
        void setColors(ALLEGRO_COLOR n, ALLEGRO_COLOR p);
        void setModifier(int *m, int f);
        void setFunction(void (*f)());
        void setCornerRadius(int px);
        void draw(int x, int y, bool mstate);
        void setVisibility(bool v);
        void setUseability(bool u);
        void setDimensions(int left, int top, int right, int bot);
    private:
        string bText;
        void (*btnFunction)();
        bool visible;
        bool useable;
        int x1, x2, y1, y2;

        int * modifier;
        int flag;

        bool prevstate = false;

        ALLEGRO_BITMAP * normal;
        ALLEGRO_BITMAP * pressed;

        ALLEGRO_COLOR nc;
        ALLEGRO_COLOR pc;

        int rounding;
};

#endif // BUTTON_H
