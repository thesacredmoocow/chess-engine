#include "menu.h"

menu::menu()
{
    background = nullptr;
}

menu::menu(int w, int h)
{
    width = w;
    height = h;
    background = nullptr;
}

menu::menu(int w, int h, int * m)
{
    //ctor
    //set all values to parameters
    width = w;
    height = h;
    modifier = m;
    background = nullptr;
}

bool menu::addBackground(char * b)
{
    //load the background bitmap, and return false if unable to
    background = al_load_bitmap(b);
    if(!background)
    {
        return false;
    }
    return true;
}

void menu::draw(int mx, int my, bool mpressed)
{
    //check if background has been set to determine whether to draw it or not
    if(background)
    {
        al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, width, height, 0);
    }
    for(map<string, button>::iterator i = buttons.begin(); i != buttons.end(); i++)
    {
        i->second.draw(mx, my, mpressed);
    }
}

bool menu::addButton(string name)
{
    if(buttons.find(name) != buttons.end())
    {
        return false;
    }
    buttons.insert(pair<string, button>(name, button()));
    return true;
}

bool menu::addButton(string name, int x1, int y1, int x2, int y2, ALLEGRO_BITMAP *n, ALLEGRO_BITMAP *p, int flag)
{
    if(buttons.find(name) != buttons.end())
    {
        return false;
    }
    buttons.insert(pair<string, button>(name, button(x1, y1, x2, y2)));
    buttons[name].setModifier(modifier, flag);
    buttons[name].setBitmaps(n, p);
    return true;

}

bool menu::addButton(string name, int x1, int y1, int x2, int y2, ALLEGRO_BITMAP *n, ALLEGRO_BITMAP *p, void(*btnFunction)())
{
    if(buttons.find(name) != buttons.end())
    {
        return false;
    }
    buttons.insert(pair<string, button>(name, button(x1, y1, x2, y2)));
    buttons[name].setFunction(btnFunction);
    buttons[name].setBitmaps(n, p);
    return true;
}

bool menu::addButton(string name, int x1, int y1, int x2, int y2, string text, ALLEGRO_COLOR n, ALLEGRO_COLOR p, int flag)
{
    if(buttons.find(name) != buttons.end())
    {
        return false;
    }
    buttons.insert(pair<string, button>(name, button(x1, y1, x2, y2)));
    buttons[name].setModifier(modifier, flag);
    buttons[name].setColors(n, p);
    return true;
}

bool menu::addButton(string name, int x1, int y1, int x2, int y2, string text, ALLEGRO_COLOR n, ALLEGRO_COLOR p, void(*btnFunction)())
{
    if(buttons.find(name) != buttons.end())
    {
        return false;
    }
    buttons.insert(pair<string, button>(name, button(x1, y1, x2, y2)));
    buttons[name].setFunction(btnFunction);
    buttons[name].setColors(n, p);
    return true;
}

bool menu::setButtonUseability(string name, bool usable)
{
    if(buttons.find(name) != buttons.end())
    {
        buttons[name].setUseability(usable);
        return true;
    }
    return false;
}

bool menu::setButtonVisibility(string name, bool visible)
{
    if(buttons.find(name) != buttons.end())
    {
        buttons[name].setVisibility(visible);
        return true;
    }
    return false;
}

bool menu::setButtonDimensions(string name, int x1, int y1, int x2, int y2)
{
    if(buttons.find(name) != buttons.end())
    {
        buttons[name].setDimensions(x1, y1, x2, y2);
        return true;
    }
    return false;
}
bool menu::setButtonBitmaps(string name, ALLEGRO_BITMAP *n, ALLEGRO_BITMAP *p)
{
    if(buttons.find(name) != buttons.end())
    {
        buttons[name].setBitmaps(n, p);
        return true;
    }
    return false;
}

bool menu::setButtonColors(string name, ALLEGRO_COLOR n, ALLEGRO_COLOR p)
{
    if(buttons.find(name) != buttons.end())
    {
        buttons[name].setColors(n, p);
        return true;
    }
    return false;
}

bool menu::setButtonModifiers(string name, int *m, int f)
{
    if(buttons.find(name) != buttons.end())
    {
        buttons[name].setModifier(m, f);
        return true;
    }
    return false;
}

bool menu::setButtonFunction(string name, void(*btnFunction)())
{
    if(buttons.find(name) != buttons.end())
    {
        buttons[name].setFunction(btnFunction);
        return true;
    }
    return false;
}

void menu::deleteAll()
{
    //free up background bitmap then all button bitmaps
    al_destroy_bitmap(background);
    while(!buttons.empty())
    {
        buttons.erase(buttons.begin());
    }
}
