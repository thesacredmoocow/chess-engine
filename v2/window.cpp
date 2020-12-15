#include "window.h"


window::window(int w, int h, int f)
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
}

window::~window()
{
    //dtor
}

void window::setFunction(void (*f)(ALLEGRO_EVENT*))
{
    frameUpdate = f;
}

bool window::update()
{
    if (!al_is_event_queue_empty(q))
    {
        al_wait_for_event(q, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            return false;
        }
        frameUpdate(&event);
    }
    return true;
}
