//
//  Bulbasaur.h
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-12-12.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GAM_1514_OSX_Game__Bulbasaur__
#define __GAM_1514_OSX_Game__Bulbasaur__

#include "../player.h"

class OpenGLTexture;


class Bulbasaur : public Player
{
public:
    Bulbasaur(Level* level,Tile* tile, float speed,int heath,const char* textureFile);
    ~Bulbasaur();
    
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
#endif /* defined(__GAM_1514_OSX_Game__Bulbasaur__) */
