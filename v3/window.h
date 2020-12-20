#include "Header.h"

class window
{
    public:
        window(int w, int h, int f);
        ~window();

        void setFunction(void (*f)(ALLEGRO_EVENT*));
        bool update();

    private:
        void (*frameUpdate)(ALLEGRO_EVENT*);
        int width, height, fps;
        ALLEGRO_DISPLAY *display;
        ALLEGRO_EVENT_QUEUE *q;
        ALLEGRO_EVENT event;
        ALLEGRO_TIMER *timer;
};

