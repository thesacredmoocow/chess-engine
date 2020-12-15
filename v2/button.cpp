#include "button.h"

button::button()
{
    x1 = 0;
    x2 = 100;
    y1 = 0;
    y2 = 100;
    normal = nullptr;
    pressed = nullptr;
    nc = al_map_rgb(255,255,255);
    pc = al_map_rgb(0,0,0);
    bText = "text";
    modifier = nullptr;
    flag = 0;
    btnFunction = nullptr;
    rounding = 5;
    visible = true;
    useable = true;
}
button::button(int left, int top, int right, int bot)
{
    x1 = left;
    x2 = right;
    y1 = top;
    y2 = bot;
    normal = nullptr;
    pressed = nullptr;
    nc = al_map_rgb(255,255,255);
    pc = al_map_rgb(0,0,0);
    bText = "text";
    modifier = nullptr;
    flag = 0;
    btnFunction = nullptr;
    rounding = 5;
    visible = true;
    useable = true;
}

void button::setText(string t)
{
    bText = t;
}

void button::setBitmaps(ALLEGRO_BITMAP *n, ALLEGRO_BITMAP *p)
{
    normal = n;
    pressed = p;
}

void button::setColors(ALLEGRO_COLOR n, ALLEGRO_COLOR p)
{
    nc = n;
    pc = p;
}

void button::setModifier(int *m, int f)
{
    modifier = m;
    flag = f;
}

void button::setFunction(void (*f)())
{
    btnFunction = f;
}

void button::setCornerRadius(int px)
{
    rounding = px;
}

void button::setDimensions(int left, int top, int right, int bot)
{
    x1 = left;
    x2 = right;
    y1 = top;
    y2 = bot;
}

void button::draw(int x, int y, bool mstate)
{
    //check if button is supposed to be visible
    if(visible)
    {
        if(useable)
        {
            if(mstate && x1 < x && x2 > x && y1 < y && y2 > y)
            {
                al_draw_filled_rounded_rectangle(x1, y1, x2, y2, rounding, rounding, pc);
            }
            else if(prevstate && !mstate && x1 < x && x2 > x && y1 < y && y2 > y)
            {
                 al_draw_filled_rounded_rectangle(x1, y1, x2, y2, rounding, rounding, pc);
                 if(modifier != nullptr)
                 {
                     *modifier = flag;
                 }
                 if(btnFunction != nullptr)
                 {
                     btnFunction();
                 }
            }
            else
            {
                al_draw_filled_rounded_rectangle(x1, y1, x2, y2, rounding, rounding, pc);
                al_draw_filled_rounded_rectangle(x1, y1, x2-2, y2-2, rounding, rounding, nc);
            }
        }
        else
        {
            al_draw_filled_rounded_rectangle(x1, y1, x2, y2, rounding, rounding, pc);
        }
        prevstate = mstate;
        //al_draw_text(smallfont, al_map_rgb(255,255,255), (x2 + x1)/2 - (bText.length()*SMALLTEXT*WIDTH)/2, (y2+y1)/2 - SMALLTEXT/2, 0, bText.c_str());
    }
}

void button::setVisibility(bool v)
{
    visible = v;
}

void button::setUseability(bool u)
{
    useable = u;
}
