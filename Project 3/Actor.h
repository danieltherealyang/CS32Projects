#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include <string>

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
// add public/private functions and only private data members
// must NOT also have x or y member variables, but instead use these functions and
// moveTo() from the GraphObject base class.
// MUST NOT store an imageId value or any value somehow
// related/derived from the imageID value in any way

/*
Actor
    Damageable
        Peach

        Enemy
            Shell
            Koopa
            Goomba
    Block
    Pipe
    Flag
    Mario
    Flower
    Mushroom
    Star
    Piranha_Fireball
    Peach_Fireball
    Shell
*/

class StudentWorld;

class Actor : public GraphObject {
    public:
        Actor(int imageID, int startX, int startY, int dir, int depth, double size, bool doesBlock, bool damageable,  StudentWorld* gameWorld) : 
            GraphObject(imageID, startX, startY, dir, depth, size), m_doesBlock(doesBlock), m_damageable(damageable), m_gameWorld(gameWorld) {};
        virtual void doSomething() = 0;
        virtual void bonk() = 0;
        bool doesOverlap(int x, int y) {
            if (getX() > x + SPRITE_WIDTH - 1 ||
            getX() + SPRITE_WIDTH - 1 < x ||
            getY() > y + SPRITE_HEIGHT - 1 ||
            getY() + SPRITE_HEIGHT - 1 < y)
                return false;
            return true;
        }
        void setAlive(bool alive) {m_alive = alive;}
        bool getAlive() {return m_alive;}
        bool getDamageable() {return m_damageable;}
        virtual void damage() {};
        bool doesBlock() {return m_doesBlock;}
        StudentWorld* getWorld() {return m_gameWorld;}
    private:
        StudentWorld* m_gameWorld;
        bool m_alive = true;
        bool m_damageable;
        bool m_doesBlock;
};

class Damageable : public Actor {
    public:
        Damageable(int imageID, int startX, int startY, int dir, int depth, double size, bool doesBlock, StudentWorld* gameWorld) :
            Actor(imageID, startX, startY, dir, depth, size, doesBlock, true,  gameWorld) {};
        virtual void damage() = 0;
    private:
};

class Fireable {
    public:
        int getRecharge() {return m_rechargeTicks;}
        void setRecharge(int ticks) {m_rechargeTicks = ticks;}
        void decrementRecharge() {m_rechargeTicks--;}
    private:
        int m_rechargeTicks = 0;
};

class Peach : public Damageable, private Fireable {
    public:
        Peach(int startX, int startY, StudentWorld* gameWorld);
        void doSomething();
        void bonk();
        void damage();
        void setHealth(int health) {m_health = health;}
        void giveStar(int ticks);
        void giveJump();
        void giveShoot();
        bool hasStar() {return m_starPower;}
        bool hasJump() {return m_jumpPower;}
        bool hasShoot() {return m_shootPower;}
    private:
        bool m_starPower = false; bool m_shootPower = false; bool m_jumpPower = false;
        int m_health = 1;
        int m_invTicks = 0; int m_tempInvTicks = 0;
        int m_remainingJumpDist = 0;
};

class Block : public Actor {
    public:
        Block(int startX, int startY, string goodie, StudentWorld* gameWorld);
        void doSomething();
        void bonk();
    private:
        //Types: None, Star, Flower, Mushroom
        enum goodie {m_None, m_Star, m_Flower, m_Mushroom};
        goodie m_goodie;
        bool m_bonked = false;
};

class Pipe : public Actor {
    public:
        Pipe(int startX, int startY, StudentWorld* gameWorld);
        void doSomething();
        void bonk();
    private:
};

class Flag : public Actor {
    public:
        Flag(int startX, int startY, StudentWorld* gameWorld);
        void doSomething();
        void bonk();
    private:
};

class Mario : public Actor {
    public:
        Mario(int startX, int startY, StudentWorld* gameWorld);
        void doSomething();
        void bonk();
    private:
};

class Flower : public Actor {
    public:
        Flower(int startX, int startY, StudentWorld* gameWorld);
        void doSomething();
        void bonk();
    private:
};

class Mushroom : public Actor {
    public:
        Mushroom(int startX, int startY, StudentWorld* gameWorld);
        void doSomething();
        void bonk();
    private:
};

class Star : public Actor {
    public:
        Star(int startX, int startY, StudentWorld* gameWorld);
        void doSomething();
        void bonk();
    private:
};

class Piranha_Fireball : public Actor {
    public:
        Piranha_Fireball(int startX, int startY, int dir, StudentWorld* gameWorld);
        void doSomething();
        void bonk();
    private:
};

class Peach_Fireball : public Actor {
    public:
        Peach_Fireball(int startX, int startY, int dir, StudentWorld* gameWorld);
        void doSomething();
        void bonk();
    private:
};

class Shell : public Actor {
    public:
        Shell(int startX, int startY, int dir, StudentWorld* gameWorld);
        void doSomething();
        void bonk();
    private:
};

class Goomba : public Damageable {
    public:
        Goomba(int startX, int startY, StudentWorld* gameWorld);
        void doSomething();
        void damage();
        void bonk();
    private:
};

class Koopa : public Damageable {
    public:
        Koopa(int startX, int startY, StudentWorld* gameWorld);
        void doSomething();
        void damage();
        void bonk();
    private:
};

class Piranha : public Damageable, private Fireable {
    public:
        Piranha(int startX, int startY, StudentWorld* gameWorld);
        void doSomething();
        void damage();
        void bonk();
    private:
};

#endif // ACTOR_H_