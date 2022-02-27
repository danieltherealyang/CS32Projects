#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath); //never call these three functions
        ~StudentWorld();
        virtual int init(); //initializes each level: game start, new level, restart level
        virtual int move(); //20x per second, updating game actors, introduce new actors, delete actors
        virtual void cleanUp(); //current level or loses a life, free all actors, 

        void addPeach(int x, int y) {m_Peach = new Peach(x, y, this);} //x and y take in pixel coords

        template <typename T>
        void add(int x, int y) {actors.push_front(new T(x, y, this));}
        template <typename T>
        void add(int x, int y, int dir) {actors.push_front(new T(x, y, dir, this));}

        void addBlock(int x, int y, string goodie) {actors.push_front(new Block(x, y, goodie, this));};

        Peach* getPeach() {return m_Peach;}

        bool notBlocked(int x, int y, Actor*& object); //x and y are pixel coords
        bool notBlocked(int x, int y);
        bool notBlockedDamageable(int x, int y, Actor*& object);
        bool notBlockedDamageable(int x, int y);
        void bonkOverlap(int x, int y);

        void finishLevel() {m_finishedLevel = true;}
        void gameOver() {m_gameOver = true;}
    private:
        list<Actor*> actors;
        Peach* m_Peach;
        bool m_finishedLevel = false;
        bool m_gameOver = false;
};

#endif // STUDENTWORLD_H_
