//
//  Hero.h
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-11-21.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GAM_1514_OSX_Game__MainBase__
#define __GAM_1514_OSX_Game__MainBase__

#include "../player.h"

class OpenGLTexture;


class MainBase : public Player
{
public:
    MainBase(Level* level, float speed,int heath,const char* textureFile);
    ~MainBase();
    
    void update(double delta);
	void paint();
    const char* getType();
    void reset();
    
    void mouseLeftClickUpEvent(float positionX, float positionY);
    void mouseMovementEvent(float deltaX, float deltaY, float PositionX, float PositionY);
    void keyUpEvent(int keyCode);
    void fireProjectile(float x, float y, const char* texture,float speed, int damage);
    
protected:
    void handlePlayerCollision(Projectile* projectile);
    void fireAtEnemies();

	OpenGLTexture* m_Texture;


};
#endif /* defined(__GAM_1514_OSX_Game__Hero__) */