//
//  Enemy.h
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-11-21.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GAM_1514_OSX_Game__Enemy__
#define __GAM_1514_OSX_Game__Enemy__

#include "Player.h"

class Enemy : public Player
{
public:
    Enemy(Level* level, float speed, const char* textureFile);
    virtual ~Enemy();
    
    const char* getType();
    
    void update(double delta);
    void paint();
    void reset();
    
protected:
    void handlePlayerCollision(Projectile* projectile);
    void fireProjectile(float x, float y, const char* texture,float speed, int damage);
    void fireAtPlayer();
	int m_DeathCount;
    
private:
    
};

#endif /* defined(__GAM_1514_OSX_Game__Enemy__) */
