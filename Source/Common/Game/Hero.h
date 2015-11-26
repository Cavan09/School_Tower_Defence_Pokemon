//
//  Hero.h
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-11-21.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GAM_1514_OSX_Game__Hero__
#define __GAM_1514_OSX_Game__Hero__

#include "player.h"


class Hero : public Player
{
public:
    Hero(Level* level, float speed, const char* textureFile);
    ~Hero();
    
    const char* getType();
    
    void mouseLeftClickUpEvent(float positionX, float positionY);
    void mouseMovementEvent(float deltaX, float deltaY, float PositionX, float PositionY);
    void keyUpEvent(int keyCode);
    
protected:
    void handlePlayerCollision(Projectile* projectile);

};
#endif /* defined(__GAM_1514_OSX_Game__Hero__) */
