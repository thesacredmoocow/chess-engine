#include "AndyAllegro.h"

AndyAllegro::AndyAllegro()
{
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_mouse();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    mpressed = false;
}

AndyAllegro::~AndyAllegro()
{
    //dtor
}

void AndyAllegro::createWindow(int w, int h, int f)
{
    width = w;
    height = h;
    fps = f;
    timer = al_create_timer(1.0/float(f));
    q = al_create_event_queue();
    display = al_create_display(w, h);
    al_set_window_title(display, "AndyAllegro Default Window Name");
    al_register_event_source(q, al_get_display_event_source(display));
    al_register_event_source(q, al_get_timer_event_source(timer));
    al_register_event_source(q, al_get_mouse_event_source());
    al_register_event_source(q, al_get_keyboard_event_source());
    al_start_timer(timer);
    mouseUpdate = nullptr;
    keyUpdate = nullptr;
    frameUpdate = nullptr;
    everythingUpdate = nullptr;
}

void AndyAllegro::setTitle(string title)
{
    al_set_window_title(display, title.c_str());
}

void AndyAllegro::setKeyFunction(void (*f)(ALLEGRO_EVENT*))
{
    keyUpdate = f;
}
void AndyAllegro::setMouseFunction(void (*f)(ALLEGRO_EVENT*))
{
    mouseUpdate = f;
}
void AndyAllegro::setScreenFunction(void (*f)(ALLEGRO_EVENT*))
{
    frameUpdate = f;
}
void AndyAllegro::setEverythingFunction(void (*f)(ALLEGRO_EVENT*))
{
    everythingUpdate = f;
}

ALLEGRO_BITMAP* AndyAllegro::loadBitmap(string bitmap)
{
    if(bitmaps.find(bitmap) != bitmaps.end())
    {
        return nullptr;
    }
    bitmaps.insert(pair<string, ALLEGRO_BITMAP*>(bitmap, al_load_bitmap(bitmap.c_str())));
    return bitmaps[bitmap];
}

bool AndyAllegro::addMenu(string name)
{
    if(menus.find(name) != menus.end())
    {
        return false;
    }
    menus.insert(pair<string, menu>(name, menu(width, height)));
}

bool AndyAllegro::addMenu(string name, int w, int h)
{
    if(menus.find(name) != menus.end())
    {
        return false;
    }
    menus.insert(pair<string, menu>(name, menu(w, h)));
}

bool AndyAllegro::addMenu(string name, int *m)
{
    if(menus.find(name) != menus.end())
    {
        return false;
    }
    menus.insert(pair<string, menu>(name, menu(width, height, m)));
}
bool AndyAllegro::addMenu(string name, int w, int h, int *m)
{
    if(menus.find(name) != menus.end())
    {
        return false;
    }
    menus.insert(pair<string, menu>(name, menu(w, h, m)));
}

menu* AndyAllegro::getMenu(string name)
{
    if(menus.find(name) == menus.end())
    {
        return nullptr;
    }
    return &menus[name];
}

bool AndyAllegro::update()
{
    bool open = true;
    if (!al_is_event_queue_empty(q))
    {
        al_wait_for_event(q, &event);
        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            if(frameUpdate != nullptr)
            {
                frameUpdate(&event);
            }
            for(map<string, menu>::iterator i = menus.begin(); i != menus.end(); i++)
            {
                i->second.draw(mx, my, mpressed);
            }
            al_flip_display();
        }
        else if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            mx = event.mouse.x;
            my = event.mouse.y;
            if(mouseUpdate != nullptr)
            {
                mouseUpdate(&event);
            }
        }
        else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            mpressed = true;
            if(mouseUpdate != nullptr)
            {
                mouseUpdate(&event);
            }
        }
        else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            mpressed = false;
            if(mouseUpdate != nullptr)
            {
                mouseUpdate(&event);
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            return false;
        }
        if(keyUpdate != nullptr && (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP))
        {
            keyUpdate(&event);
        }
        if(everythingUpdate != nullptr)
        {
            everythingUpdate(&event);
        }
    }
    return open;
}
