//Object IDS
enum IDS{PLAYER,BULLET,ENEMY};

//STATEK
struct SpaceShip
{
    int ID;
    int x;
    int y;
    int lives;
    int speed;
    int boundx;
    int boundy;
    int score;

};

//KULLA
struct Bullet
{
    int ID;
    int x;
    int y;
    int speed;
    bool live;
};

struct Comet
{
    int ID;
    int x;
    int y;
    int live;
    int speed;
    int boundx;
    int boundy;
};
