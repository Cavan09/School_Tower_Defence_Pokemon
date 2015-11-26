//
//  Charmander.h
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-12-12.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GAM_1514_OSX_Game__Charmander__
#define __GAM_1514_OSX_Game__Charmander__

#include "../player.h"

class OpenGLTexture;


class Charmander : public Player
{
public:
    Charmander(Level* level,Tile* tile, float speed,int heath,const char* textureFile);
    ~Charmander();
    
    void update(double delta);
	void paint();
    const char* getType();
    void reset();
    
    void mouseLeftClickUpEvent(float positionX, float positionY);
    void mouseMovementEvent(float deltaX, float deltaY, float PositionX, float PositionY);
    void keyUpEvent(int keyCode);
    void fireProjectile(float x, float y, const char* texture,float speed, int damage);
    void dropTower(int x, int y);
    
protected:
    void handlePlayerCollision(Projectile* projectile);
    void fireAtEnemies();
    
	OpenGLTexture* m_Texture;

};

#endif /* defined(__GAM_1514_OSX_Game__Charmander__) */
