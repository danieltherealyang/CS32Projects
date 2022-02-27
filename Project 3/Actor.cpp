#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Peach::Peach(int startX, int startY, StudentWorld* gameWorld) :
Damageable(IID_PEACH, startX, startY, 0, 0, 1, false, gameWorld) {
}

void Peach::doSomething() {
    if (!getAlive()) //return if not alive
        return;
    
    if (m_invTicks > 0) { //decrement invincible game ticks
        m_invTicks--;
        if (m_invTicks == 0) { //set invincible to false
            m_starPower = false;
        }
    }
    
    if (m_tempInvTicks > 0) { //decrement temp invincible game ticks
        m_tempInvTicks--;
        if (m_tempInvTicks == 0) { //lose temp invincible powers
            m_shootPower = false;
            m_jumpPower = false;
        }
    }
    
    if (getRecharge() > 0) { //decrement recharge timer
        decrementRecharge();
    }
    
    getWorld()->bonkOverlap(getX(), getY()); //bonk overlapping object
    
    Actor* object;
    if (m_remainingJumpDist > 0) {
        int targetY = getY() + 4;
        if (!getWorld()->notBlocked(getX(), targetY, object)) { //bonk object blocking during a jump and stop jump
            object->bonk();
            m_remainingJumpDist = 0;
        } else { //continue moving upwards if not blocked
            moveTo(getX(), targetY);
            m_remainingJumpDist--;
        }
    } else if (m_remainingJumpDist == 0 && getWorld()->notBlocked(getX(), getY() - 4)) { //move down if not actively jumping
        moveTo(getX(), getY() - 4);
    }

    int key;
    if (getWorld()->getKey(key)) { //check pressed key
        int targetX;
        switch (key) {
            case KEY_PRESS_LEFT:
                setDirection(180);
                targetX = getX() - 4;
                if (!getWorld()->notBlocked(targetX, getY(), object)) { //bonk if blocked, move if not
                    object->bonk();
                } else {
                    moveTo(targetX, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                targetX = getX() + 4;
                if (!getWorld()->notBlocked(targetX, getY(), object)) { //bonk if blocked, move if not
                    object->bonk();
                } else {
                    moveTo(targetX, getY());
                }
                break;
            case KEY_PRESS_UP:
                if (!getWorld()->notBlocked(getX(), getY() - 1)) { //only jump if standing on block
                    m_remainingJumpDist = (m_jumpPower) ? 12 : 8;
                    getWorld()->playSound(SOUND_PLAYER_JUMP);
                }
                break;
            case KEY_PRESS_SPACE:
                if (!m_shootPower || getRecharge() > 0) //no shoot power or recharging then do nothing
                    break;
                getWorld()->playSound(SOUND_PLAYER_FIRE); //create fireball in direction of peach
                setRecharge(8);
                targetX = (getDirection() == 0) ? getX() + 4 : getX() - 4;
                getWorld()->add<Peach_Fireball>(targetX, getY(), getDirection());
            default:
                break;
        }
    }
}

void Peach::bonk() {
    if (m_starPower || m_tempInvTicks > 0) //if invincible, do nothing
        return;
    m_health--; //decrement health, set temp invincibility ticks, check if dead or just hurt
    m_tempInvTicks = 10;
    m_shootPower = false;
    m_jumpPower = false;
    if (m_health > 0)
        getWorld()->playSound(SOUND_PLAYER_HURT);
    else
        setAlive(false);
}

void Peach::damage() {
    bonk();
}

void Peach::giveStar(int ticks) {
    m_starPower = true;
    m_invTicks = ticks;
}

void Peach::giveJump() {
    m_jumpPower = true;
}

void Peach::giveShoot() {
    m_shootPower = true;
}

Block::Block(int startX, int startY, string goodie, StudentWorld* gameWorld) :
Actor(IID_BLOCK, startX, startY, 0, 2, 1, true, false, gameWorld) {
    if (goodie == "None") //set goodie with enum because works with switch
        m_goodie = m_None;
    else if (goodie == "Star")
        m_goodie = m_Star;
    else if (goodie == "Flower")
        m_goodie = m_Flower;
    else if (goodie == "Mushroom")
        m_goodie = m_Mushroom;
}

void Block::doSomething() {}

void Block::bonk() {
    if (m_goodie == m_None || m_bonked) { //bonk sound if no goodie or already released
        getWorld()->playSound(SOUND_PLAYER_BONK);
        return;
    }
    getWorld()->playSound(SOUND_POWERUP_APPEARS);
    int targetY = getY() + 8;
    switch(m_goodie) { //introduce goodie, mark block as already bonked
        case m_Star:
            getWorld()->add<Star>(getX(), targetY);
            m_bonked = true;
            break;
        case m_Flower:
            getWorld()->add<Flower>(getX(), targetY);
            m_bonked = true;
            break;
        case m_Mushroom:
            getWorld()->add<Mushroom>(getX(), targetY);
            m_bonked = true;
            break;
        default:
            break;
    }
    m_bonked = true;
}

Pipe::Pipe(int startX, int startY, StudentWorld* gameWorld) :
Actor(IID_PIPE, startX, startY, 0, 2, 1, true, false, gameWorld) {
}

void Pipe::doSomething() {}

void Pipe::bonk() {}

Flag::Flag(int startX, int startY, StudentWorld* gameWorld) :
Actor(IID_FLAG, startX, startY, 0, 1, 1, false, false, gameWorld) {
}

void Flag::doSomething() {
    if (!getAlive()) //do nothing if dead
        return;
    Peach* peach = getWorld()->getPeach();
    if(doesOverlap(peach->getX(), peach->getY())) { //check overlap with peach
        getWorld()->increaseScore(1000);
        setAlive(false);
        getWorld()->finishLevel();
    }
}

void Flag::bonk() {}

Mario::Mario(int startX, int startY, StudentWorld* gameWorld) :
Actor(IID_MARIO, startX, startY, 0, 1, 1, false, false, gameWorld) {
}

void Mario::doSomething() {
    if (!getAlive())
        return;
    Peach* peach = getWorld()->getPeach();
    if (doesOverlap(peach->getX(), peach->getY())) { //check overlap with peach
        getWorld()->increaseScore(1000);
        setAlive(false);
        getWorld()->gameOver();
    }
}

void Mario::bonk() {}

Flower::Flower(int startX, int startY, StudentWorld* gameWorld) :
Actor(IID_FLOWER, startX, startY, 0, 1, 1, false, false, gameWorld) {
}

void Flower::doSomething() {
    Peach* peach = getWorld()->getPeach();
    if (doesOverlap(peach->getX(), peach->getY())) { //check overlap with peach
        getWorld()->increaseScore(50);
        peach->giveShoot();
        peach->setHealth(2);
        setAlive(false);
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }

    if (getWorld()->notBlocked(getX(), getY() - 2)) { //move down 2 pixels if not blocked
        moveTo(getX(), getY() - 2);
    }

    int targetX = (getDirection() == 0) ? getX() + 2 : getX() - 2; //calculate target based on direction
    if (!getWorld()->notBlocked(targetX, getY())) { //check if able to move to target
        setDirection(getDirection() + 180); //reverse direction if blocked
    } else {
        moveTo(targetX, getY()); //move to target
    }
}

void Flower::bonk() {}

Mushroom::Mushroom(int startX, int startY, StudentWorld* gameWorld) :
Actor(IID_MUSHROOM, startX, startY, 0, 1, 1, false, false, gameWorld) {
}

void Mushroom::doSomething() {
    Peach* peach = getWorld()->getPeach();
    if (doesOverlap(peach->getX(), peach->getY())) { //check overlap with peach
        getWorld()->increaseScore(75);  
        peach->giveJump();
        peach->setHealth(2);
        setAlive(false);
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }

    if (getWorld()->notBlocked(getX(), getY() - 2)) { //move down 2 pixels if not blocked
        moveTo(getX(), getY() - 2);
    }

    int targetX = (getDirection() == 0) ? getX() + 2 : getX() - 2; //calculate target based on direction
    if (!getWorld()->notBlocked(targetX, getY())) { //if blocked at target, reverse direction
        setDirection(getDirection() + 180);
    } else {
        moveTo(targetX, getY()); //move if not blocked at target
    }
}

void Mushroom::bonk() {}

Star::Star(int startX, int startY, StudentWorld* gameWorld) :
Actor(IID_STAR, startX, startY, 0, 1, 1, false, false, gameWorld) {
}

void Star::doSomething() {
    Peach* peach = getWorld()->getPeach();
    if (doesOverlap(peach->getX(), peach->getY())) { //check overlap with peach
        getWorld()->increaseScore(100);  
        peach->giveStar(150);
        setAlive(false);
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }

    if (getWorld()->notBlocked(getX(), getY() - 2)) { //move down 2 pixels if not blocked
        moveTo(getX(), getY() - 2);
    }

    int targetX = (getDirection() == 0) ? getX() + 2 : getX() - 2; //calculate target based on direction
    if (!getWorld()->notBlocked(targetX, getY())) { //if blocked at target, change direction
        setDirection(getDirection() + 180);
    } else {
        moveTo(targetX, getY()); //if not blocked, move to target
    }
}

void Star::bonk() {}

Piranha_Fireball::Piranha_Fireball(int startX, int startY, int dir, StudentWorld* gameWorld) :
Actor(IID_PIRANHA_FIRE, startX, startY, dir, 1, 1, false, false, gameWorld) {
}

void Piranha_Fireball::doSomething() {
    Peach* peach = getWorld()->getPeach();
    if (doesOverlap(peach->getX(), peach->getY())) { //check overlap with peach
        peach->damage();
        setAlive(false);
        return;
    }

    if (getWorld()->notBlocked(getX(), getY() - 2)) { //move down 2 pixels if not blocked
        moveTo(getX(), getY() - 2);
    }

    int targetX = (getDirection() == 0) ? getX() + 2 : getX() - 2;
    if (!getWorld()->notBlocked(targetX, getY())) { //if blocked at target, then die
        setAlive(false);
    } else {
        moveTo(targetX, getY()); //if not blocked, move to target
    }
}

void Piranha_Fireball::bonk() {}

Peach_Fireball::Peach_Fireball(int startX, int startY, int dir, StudentWorld* gameWorld) :
Actor(IID_PEACH_FIRE, startX, startY, dir, 1, 1, false, false, gameWorld) {
}

void Peach_Fireball::doSomething() {
    Peach* peach = getWorld()->getPeach();
    Actor* object;
    if (!getWorld()->notBlockedDamageable(getX(), getY(), object)) { //check overlap with damageable object
        object->damage();
        setAlive(false);
        return;
    }

    if (getWorld()->notBlocked(getX(), getY() - 2)) { //move down 2 pixels if not blocked
        moveTo(getX(), getY() - 2);
    }

    int targetX = (getDirection() == 0) ? getX() + 2 : getX() - 2;
    if (!getWorld()->notBlocked(targetX, getY())) { //if blocked at target, then die
        setAlive(false);
    } else {
        moveTo(targetX, getY()); //if not blocked, move to target
    }
}

void Peach_Fireball::bonk() {}


Shell::Shell(int startX, int startY, int dir, StudentWorld* gameWorld) :
Actor(IID_SHELL, startX, startY, dir, 1, 1, false, false, gameWorld) {
}

void Shell::doSomething() {
    Peach* peach = getWorld()->getPeach();
    Actor* object;
    if (!getWorld()->notBlockedDamageable(getX(), getY(), object)) { //check overlap with damageable object
        object->damage();
        setAlive(false);
        return;
    }

    if (getWorld()->notBlocked(getX(), getY() - 2)) { //move down 2 pixels if not blocked
        moveTo(getX(), getY() - 2);
    }

    int targetX = (getDirection() == 0) ? getX() + 2 : getX() - 2;
    if (!getWorld()->notBlocked(targetX, getY())) { //if blocked at target, then die
        setAlive(false);
    } else {
        moveTo(targetX, getY()); //if not blocked, move to target
    }
}

void Shell::bonk() {}

Goomba::Goomba(int startX, int startY, StudentWorld* gameWorld) :
Damageable(IID_GOOMBA, startX, startY, (rand() % 2) * 180, 0, 1, false, gameWorld) {
}

void Goomba::doSomething() {
    if (!getAlive())
        return;
    Peach* peach = getWorld()->getPeach();
    if (doesOverlap(peach->getX(), peach->getY())) { //check overlap with peach
        peach->bonk();
        return;
    }
    
    int targetX = (getDirection() == 0) ? getX() + 1 : getX() - 1;
    if (!getWorld()->notBlocked(targetX, getY())) { //change direction if blocked in current direction
        setDirection(getDirection() + 180);
    } else {
        targetX = (getDirection() == 0) ? getX() + SPRITE_WIDTH : getX() - SPRITE_WIDTH;
        if (getWorld()->notBlocked(targetX, getY() - 1)) { //change direction if going to step off edge
            setDirection(getDirection() + 180);
        }
    }

    targetX = (getDirection() == 0) ? getX() + 1 : getX() - 1;
    if (getWorld()->notBlocked(targetX, getY())) { //move to target if not blocked
        moveTo(targetX, getY());
    }
}

void Goomba::bonk() {
    Peach* peach = getWorld()->getPeach();
    if (!doesOverlap(peach->getX(), peach->getY()))
        return;
    if (peach->hasStar()) { //if peach invincible, then take damage
        getWorld()->playSound(SOUND_PLAYER_KICK);
        damage();
    }
}

void Goomba::damage() {
    getWorld()->increaseScore(100);
    setAlive(false);
}

Koopa::Koopa(int startX, int startY, StudentWorld* gameWorld) :
Damageable(IID_KOOPA, startX, startY, (rand() % 2) * 180, 0, 1, false, gameWorld) {
}

void Koopa::doSomething() {
    if (!getAlive())
        return;
    Peach* peach = getWorld()->getPeach();
    if (doesOverlap(peach->getX(), peach->getY())) { //check overlap with peach
        peach->bonk();
        return;
    }
    
    int targetX = (getDirection() == 0) ? getX() + 1 : getX() - 1;
    if (!getWorld()->notBlocked(targetX, getY())) { //change direction if blocked
        setDirection(getDirection() + 180);
    } else {
        targetX = (getDirection() == 0) ? getX() + SPRITE_WIDTH : getX() - SPRITE_WIDTH;
        if (getWorld()->notBlocked(targetX, getY() - 1)) { //change direction if going to step off edge
            setDirection(getDirection() + 180);
        }
    }

    targetX = (getDirection() == 0) ? getX() + 1 : getX() - 1;
    if (getWorld()->notBlocked(targetX, getY())) { //move to target if not blocked
        moveTo(targetX, getY());
    }
}

void Koopa::bonk() {
    Peach* peach = getWorld()->getPeach();
    if (!doesOverlap(peach->getX(), peach->getY()))
        return;
    if (peach->hasStar()) { //if peach invincible, then take damage
        getWorld()->playSound(SOUND_PLAYER_KICK);
        damage();
    }
}

void Koopa::damage() {
    getWorld()->increaseScore(100);
    setAlive(false);
    getWorld()->add<Shell>(getX(), getY(), getDirection());
}

Piranha::Piranha(int startX, int startY, StudentWorld* gameWorld) :
Damageable(IID_PIRANHA, startX, startY, (rand() % 2) * 180, 0, 1, false, gameWorld) {
}

void Piranha::doSomething() {
    if (!getAlive())
        return;
    increaseAnimationNumber();
    Peach* peach = getWorld()->getPeach();
    if (doesOverlap(peach->getX(), peach->getY())) { //check overlap with peach
        peach->bonk();
        return;
    }

    if (abs(peach->getY() - getY()) > 1.5*SPRITE_HEIGHT) //return if peach y not within 1.5 sprite height
        return;
    
    if (getX() - peach->getX() < 0) //set direction based on peach x coordinate
        setDirection(0);
    else
        setDirection(180);
    
    if (getRecharge() > 0) { //decrement firing delay if there is one
        decrementRecharge();
        return;
    }

    int dist = abs(getX() - peach->getX());
    if (dist < 8*SPRITE_WIDTH) { //fire if peach less than 8 sprite widths away
        getWorld()->add<Piranha_Fireball>(getX(), getY(), getDirection());
        getWorld()->playSound(SOUND_PIRANHA_FIRE);
        setRecharge(40);
    }
}

void Piranha::bonk() {
    Peach* peach = getWorld()->getPeach();
    if (!doesOverlap(peach->getX(), peach->getY()))
        return;
    if (peach->hasStar()) { //if peach invincible, then take damage
        getWorld()->playSound(SOUND_PLAYER_KICK);
        damage();
    }
}

void Piranha::damage() {
    getWorld()->increaseScore(100);
    setAlive(false);
}