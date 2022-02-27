#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    m_finishedLevel = false; //initialize variables
    m_gameOver = false;

    Level level(assetPath()); //parse level file path
    ostringstream oss;
    oss << "level";

    if (getLevel() < 10)
        oss << 0 << getLevel() << ".txt";
    else if (getLevel() >= 10)
        oss << getLevel() << ".txt";
    else
        return GWSTATUS_LEVEL_ERROR;
    
    string level_file = oss.str();

    Level::LoadResult result = level.loadLevel(level_file); //load level data
    if (result == Level::load_fail_file_not_found) {
        cerr <<  "Could not find " << level_file << " data file" << endl;
        return GWSTATUS_LEVEL_ERROR;
    } else if (result == Level::load_fail_bad_format) {
        cerr << level_file << " is improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    } else if (result == Level::load_success) {
        cerr << "Successfully loaded level" << endl;
        Level::GridEntry ge;
        
        for (int x = 0; x < 32; x++) { //loop through all 32 x and y objects
            for (int y = 0; y < 32; y++) {
                ge = level.getContentsOf(x, y);
                switch (ge) { //populate level with objects
                    case Level::empty:
                        break;
                    case Level::peach:
                        addPeach(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                        break;
                    case Level::koopa:
                        add<Koopa>(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                        break;
                    case Level::goomba:
                        add<Goomba>(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                        break;
                    case Level::piranha:
                        add<Piranha>(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                        break;
                    case Level::block:
                        addBlock(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, "None");
                        break;
                    case Level::star_goodie_block:
                        addBlock(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, "Star");
                        break;
                    case Level::mushroom_goodie_block:
                        addBlock(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, "Mushroom");
                        break;
                    case Level::flower_goodie_block:
                        addBlock(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, "Flower");
                        break;
                    case Level::pipe:
                        add<Pipe>(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                        break;
                    case Level::flag:
                        add<Flag>(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                        break;
                    case Level::mario:
                        add<Mario>(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                        break;
                    default:
                        cerr << "GridEntry not found" << endl;
                }
            }
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

    m_Peach->doSomething(); //have all objects in the game doSomething()

    list<Actor*>::iterator it = actors.begin();
    while (it != actors.end()) {
        (*it)->doSomething();
        it++;
    }

    if (!m_Peach->getAlive()) { //if peach dies play sound and update lives
        playSound(SOUND_PLAYER_DIE);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    if (m_finishedLevel) { //return if flag overlap
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    if (m_gameOver) { //return if reach mario
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }

    it = actors.begin(); //delete all dead actors
    while (it != actors.end()) {
        if (!(*it)->getAlive()) {
            delete *it;
            it = actors.erase(it);
            continue;
        }
        it++;
    }

    ostringstream oss; //update status text
    ostringstream scorestream;
    scorestream << getScore();
    string padding(6 - scorestream.str().size(), '0'); //pad score to be 6 digits long
    oss << "Lives: " << getLives() << " Level: " << getLevel() << " Points: " << padding << getScore();
    if (m_Peach->hasStar())
        oss << " StarPower!";
    if (m_Peach->hasShoot())
        oss << " ShootPower!";
    if (m_Peach->hasJump())
        oss << " JumpPower!";
    setGameStatText(oss.str());

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it = actors.begin(); //delete all objects pointed to by actors
    while (it != actors.end()) {
        delete *it;
        it++;
    }
    actors.clear(); //clear list
    delete m_Peach;
}

bool StudentWorld::notBlocked(int x, int y, Actor*& object) { //returns actor pointer of blocking object that overlaps at x,y
    list<Actor*>::iterator it = actors.begin();
    while (it != actors.end()) {
        if ((*it)->doesOverlap(x, y) && (*it)->doesBlock()) {
            object = *it;
            return false;
        }
        it++;
    }
    return true;
}

bool StudentWorld::notBlocked(int x, int y) { //only checks if object is overlapped and blocking at x,y doesn't return pointer
    list<Actor*>::iterator it = actors.begin();
    while (it != actors.end()) {
        if ((*it)->doesOverlap(x, y) && (*it)->doesBlock()) {
            return false;
        }
        it++;
    }
    return true;
}

bool StudentWorld::notBlockedDamageable(int x, int y, Actor*& object) { //returns actor pointer of damageable object that overlaps at x,y
    list<Actor*>::iterator it = actors.begin();
    while (it != actors.end()) {
        if ((*it)->doesOverlap(x, y) && (*it)->getDamageable()) {
            object = *it;
            return false;
        }
        it++;
    }
    return true;
}

bool StudentWorld::notBlockedDamageable(int x, int y) { //only checks if object is overlapped and damageable at x,y doesn't return pointer
    list<Actor*>::iterator it = actors.begin();
    while (it != actors.end()) {
        if ((*it)->doesOverlap(x, y) && (*it)->getDamageable()) {
            return false;
        }
        it++;
    }
    return true;
}

void StudentWorld::bonkOverlap(int x, int y) { //bonk all overlapping objects at x,y regardless of blocking or damageable
    list<Actor*>::iterator it = actors.begin();
    while (it != actors.end()) {
        if ((*it)->doesOverlap(x, y)) {
            (*it)->bonk();
        }
        it++;
    }
}