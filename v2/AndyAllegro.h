#include "Header.h"

#include "menu.h"
#include "button.h"

class AndyAllegro
{
    public:
        AndyAllegro();
        ~AndyAllegro();

        void createWindow(int w, int h, int f);
        void setTitle(string title);
        void setKeyFunction(void (*f)(ALLEGRO_EVENT*));
        void setMouseFunction(void (*f)(ALLEGRO_EVENT*));
        void setScreenFunction(void (*f)(ALLEGRO_EVENT*));
        void setEverythingFunction(void (*f)(ALLEGRO_EVENT*));

        bool addMenu(string name);
        bool addMenu(string name, int w, int h);
        bool addMenu(string name, int *m);
        bool addMenu(string name, int w, int h, int *m);

        menu* getMenu(string name);

        ALLEGRO_BITMAP* loadBitmap(string bitmap);

        bool update();

    private:
        int width, height, fps;
        int mx, my;
        bool mpressed;
        ALLEGRO_DISPLAY *display;
        ALLEGRO_EVENT_QUEUE *q;
        ALLEGRO_EVENT event;
        ALLEGRO_TIMER *timer;

        void (*keyUpdate)(ALLEGRO_EVENT*);
        void (*mouseUpdate)(ALLEGRO_EVENT*);
        void (*frameUpdate)(ALLEGRO_EVENT*);
        void (*everythingUpdate)(ALLEGRO_EVENT*);

        map<string, menu> menus;
        map<string, ALLEGRO_BITMAP*> bitmaps;
};
