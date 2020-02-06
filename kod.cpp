#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "pomoc.h"

//GLOBALNE ZMIENNE------------------------------------
const int szer=1200;
const int wys=800;
const int NUM_BULLETS = 5;
const int NUM_COMETS = 10;
enum KEYS{UP,DOWN,LEFT,RIGHT,SPACE};
bool keys[5]={false,false,false,false,false};

//PRIMITIVE VALUES
bool redraw = true;
bool done   = false;
const int FPS = 60;
bool isGameOver= false;


//prototypes
void InitShip(SpaceShip &ship);
void DrawShip(SpaceShip &ship);
void MoveShipUp(SpaceShip &ship);
void MoveShipDown(SpaceShip &ship);
void MoveShipLeft(SpaceShip &ship);
void MoveShipRight(SpaceShip &ship);

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, SpaceShip &ship);
void UpdateBullet(Bullet bullet[], int size);

void initComet(Comet comets[], int size);
void DrawComet (Comet comets[], int size);
void StartComet(Comet comets[], int size);
void UpdateComet(Comet comets[], int size);
void CollideBullet(Bullet bullet[],int bSize, Comet comets[], int cSize,SpaceShip &ship);
void CollideComet(Comet comets[], int cSize, SpaceShip &ship);


int main()
{


//OBJECT
SpaceShip ship;
Bullet bullets[NUM_BULLETS];
Comet comets[NUM_COMETS];

//INSTALL
al_init();
al_init_primitives_addon();
al_install_keyboard();
al_init_font_addon();

//ALLEGRO VALUES
ALLEGRO_DISPLAY *display =NULL;
ALLEGRO_EVENT_QUEUE *event_queue= NULL;
ALLEGRO_TIMER *timer= NULL;
ALLEGRO_FONT    *font8 = al_create_builtin_font();


//ESTRA
display= al_create_display(szer,wys);
event_queue = al_create_event_queue();
timer = al_create_timer(1.0 /FPS);

//INICJONOWANIE
srand(time(NULL));
InitBullet(bullets,NUM_BULLETS);
InitShip(ship);
initComet(comets, NUM_COMETS);

//Rejerst
al_register_event_source(event_queue,al_get_keyboard_event_source());
al_register_event_source(event_queue,al_get_timer_event_source(timer));
al_register_event_source(event_queue,al_get_display_event_source(display));

al_start_timer(timer);
while(!done)
{
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);

    if(ev.type == ALLEGRO_EVENT_TIMER)
    {
    redraw=true;
    if(keys[UP])
        MoveShipUp(ship);
    if(keys[DOWN])
        MoveShipDown(ship);
    if(keys[RIGHT])
        MoveShipRight(ship);
    if(keys[LEFT])
        MoveShipLeft(ship);
    if(!isGameOver)
    {
        UpdateBullet(bullets, NUM_BULLETS);
        StartComet(comets, NUM_COMETS);
        UpdateComet(comets,NUM_COMETS);
        CollideBullet(bullets,NUM_BULLETS,comets, NUM_COMETS,ship);
        CollideComet(comets, NUM_COMETS,ship);

        if(ship.lives<= 0)isGameOver=true;
    }
    }
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        done=true;
    }
    else if(ev.type== ALLEGRO_EVENT_KEY_DOWN)
    {
     switch(ev.keyboard.keycode)
     {
     case ALLEGRO_KEY_ESCAPE:
        done = true;
        break;
     case ALLEGRO_KEY_UP:
        keys[UP]=true;
        break;
    case ALLEGRO_KEY_DOWN:
        keys[DOWN]=true;
        break;
    case ALLEGRO_KEY_LEFT:
        keys[LEFT]=true;
        break;
    case ALLEGRO_KEY_RIGHT:
        keys[RIGHT]=true;
        break;
     case ALLEGRO_KEY_SPACE:
        keys[SPACE]=true;
        break;
     }
    }
    else if(ev.type== ALLEGRO_EVENT_KEY_UP)
    {
     switch(ev.keyboard.keycode)
     {
     case ALLEGRO_KEY_ESCAPE:
        done = true;
        break;
     case ALLEGRO_KEY_UP:
        keys[UP]=false;
        break;
    case ALLEGRO_KEY_DOWN:
        keys[DOWN]=false;
        break;
    case ALLEGRO_KEY_LEFT:
        keys[LEFT]=false;
        break;
    case ALLEGRO_KEY_RIGHT:
        keys[RIGHT]=false;
        break;
     case ALLEGRO_KEY_SPACE:
        keys[SPACE]=false;
        FireBullet(bullets,NUM_BULLETS, ship);
        break;
     }
    }

    if(redraw && al_is_event_queue_empty(event_queue))
    {
    redraw=false;
    if(!isGameOver)
    {
    DrawShip(ship);
    DrawBullet(bullets, NUM_BULLETS);
    DrawComet(comets, NUM_COMETS);
     // al_draw_textf (font8,al_map_rgb(255,0,255),5,5,0,"Gracz ma %i zyc. Gracz rozjebal %i obiektow", ship.lives, ship.score);
    }
    else
    {
    al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_textf (font8,al_map_rgb(255,255,255),600,600,0,"KONIEC GRY. WYNIK TO %i", ship.score);
    }
    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));
    }
}
al_destroy_display(display);
return 0;
}

void InitShip(SpaceShip &ship)
{
    ship.x=20;
    ship.y=wys/2;
    ship.ID= PLAYER;
    ship.lives=3;
    ship.speed=15;
    ship.boundx= 6;
    ship.boundy=7;
    ship.score=0;
}
void DrawShip(SpaceShip &ship)
{
    al_draw_filled_rectangle(ship.x,ship.y-9, ship.x+10, ship.y-7,al_map_rgb(0,255,0));
    al_draw_filled_rectangle(ship.x,ship.y+9, ship.x+10, ship.y+7,al_map_rgb(0,255,0));

    al_draw_filled_triangle(ship.x-12, ship.y-17, ship.x+12, ship.y, ship.x-12, ship.y+17,al_map_rgb(255,0,0));
    al_draw_filled_rectangle(ship.x-12, ship.y-2, ship.x+15, ship.y+2, al_map_rgb(0,0,255));
}
void MoveShipUp(SpaceShip &ship)
{
    ship.y-=ship.speed;
    if(ship.y<20)
        ship.y=20;
}
void MoveShipDown(SpaceShip &ship)
{
    ship.y+=ship.speed;
    if(ship.y>wys-20)
        ship.y=wys-20;
}
void MoveShipLeft(SpaceShip &ship)
{
    ship.x-=ship.speed;
    if(ship.x<15)
        ship.x=15;
}
void MoveShipRight(SpaceShip &ship)
{
    ship.x+=ship.speed;
    if(ship.x>300)
        ship.x=300;
}




void InitBullet(Bullet bullet[], int size)
{
    for(int i=0; i<size; i++)
    {
        bullet[i].ID = BULLET;
        bullet[i].speed = 10;
        bullet[i].live = false;
    }
}
void DrawBullet(Bullet bullet[], int size)
{
    for(int i=0; i<size; i++)
    {
        if(bullet[i].live)
            al_draw_filled_circle(bullet[i].x, bullet[i].y, 2, al_map_rgb(255,0,0));
    }
}
void FireBullet(Bullet bullet[], int size, SpaceShip &ship)
{
    for(int i=0; i<size; i++)
    {
        if(!bullet[i].live)
        {
            bullet[i].x=ship.x+17;
            bullet[i].y=ship.y;
            bullet[i].live= true;
            break;
        }
    }
}
void UpdateBullet(Bullet bullet[], int size)
{
    for(int i=0; i<size; i++)
    {
        if(bullet[i].live)
        {
            bullet[i].x+=bullet[i].speed;
            if(bullet[i].x>szer)
                bullet[i].live=false;
        }
    }
}
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize,SpaceShip &ship)
{
    for(int i=0; i<bSize; i++)
    {
        if(bullet[i].live)
        {
            for(int j=0; j<cSize; j++)
                if(comets[j].live)
                {
                if(bullet[i].x> (comets[j].x -comets[j].boundx) &&
                   bullet[i].x <(comets[j].x +comets[j].boundx) &&
                   bullet[i].y > (comets[j].y - comets[j].boundy) &&
                   bullet[i].y < (comets[j].y + comets[j].boundy))
                    {
                            bullet[i].live=false;
                            comets[j].live=false;
                            ship.score ++;
                    }

                }
        }
    }
}
void initComet(Comet comets[], int size)
{
    for(int i=0; i<size; i++)
    {
        comets[i].ID= ENEMY;
        comets[i].live= false;
        comets[i].speed= 1;
        comets[i].boundx=18;
        comets[i].boundy=18;
    }
}
void DrawComet (Comet comets[], int size)
{
    for(int i=0; i<size; i++)
    {
        if(comets[i].live)
            al_draw_filled_circle(comets[i].x,comets[i].y, 20, al_map_rgb(255,0,0));
    }
}
void StartComet(Comet comets[], int size)
{
    for(int i=0; i<size; i++)
    {
        if(!comets[i].live)
        {
            if(rand()%500==0)
            {
                comets[i].live=true;
                comets[i].x=szer;
                comets[i].y=30+rand()%(wys-60);

                break;
            }
        }
    }
}
void UpdateComet(Comet comets[], int size)
{
    for(int i=0; i<size; i++)
    {
        if(comets[i].live)
        {
            comets[i].x-= comets[i].speed;
        }
    }
}
void CollideComet(Comet comets[], int cSize, SpaceShip &ship)
{
    for(int i=0; i<cSize; i++)
        if(comets[i].live)
        {
            if(comets[i].x - comets[i].boundx <ship.x+ship.boundx &&
               comets[i].x + comets[i].boundx >ship.x-ship.boundx &&
               comets[i].y - comets[i].boundx <ship.y+ship.boundy &&
               comets[i].y + comets[i].boundx >ship.y-ship.boundx)
                {
                ship.lives --;
                comets[i].live = false;
                }
            else if(comets[i].x<0)
            {
                comets[i].live=false;
                ship.lives --;
            }
        }
}
