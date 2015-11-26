//
//  MainBase.cpp
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-11-21.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "MainBase.h"
#include "../Level.h"
#include "../Tiles/Tile.h"
#include "../../Constants/Constants.h"
#include "../../Input/Input.h"
#include "../../Utils/Utils.h"
#include "../../OpenGL/OpenGL.h"
#include "../Enemy.h"



MainBase::MainBase(Level* level, float speed,int health, const char* textureFile) : Player(level, textureFile),
	m_Texture(NULL)
{
    m_Speed = speed;
    m_Health = health;
}

MainBase::~MainBase()
{

}
void MainBase::update(double delta)
{
    Player::update(delta);
    if(m_FireRate > 0.0)
    {
        m_FireRate -= delta;
        if(m_FireRate <= 0.0)
        {
            m_FireRate = 1.0;
            
            fireAtEnemies();
            
        }
    }

}
void MainBase::paint()
{
	Player::paint();
}
void MainBase::reset()
{
    fireAtEnemies();
    
}
const char* MainBase::getType()
{
    return MAIN_BASE_TYPE;
}


void MainBase::handlePlayerCollision(Projectile* projectile)
{
    Tile* tile = m_Level->getTileForPosition(projectile->getX(), projectile->getY());
    
    //cycle through all the enemesn  and ceck for the collision with the projectile
    for(int i = 0; i < m_Level->getEnemies().size(); i ++)
    {
        Enemy* enemy = m_Level->getEnemies().at(i);
        if(enemy != NULL && enemy->getIsActive() == true)
        {
            //get the tile the enemy is on
            Tile* enemyTile = m_Level->getTileForPlayer(enemy);
            
            //is the projectile on the same tile as the emnemy?
            if(tile == enemyTile)
            {
               // Log::debug("Hero projectile hit an enemy");
                
                // apply damage tot he enemt and set the projectile
                enemy->applyDamage(projectile->getDamage());
                projectile->setIsActive(false);
            }
        }
    }
}
void MainBase::fireAtEnemies()
{
    int enemyCount = 0;
	for( int i = 0; i < m_Level->getEnemies().size(); i++)
    {
        Enemy* enemy = m_Level->getEnemies().at(i);
	
        if(enemy != NULL && enemy->getIsActive() == true)
        {
            if(MathUtils::withinRange(enemy->getX(),getX(),250.0f) == true || MathUtils::withinRange(enemy->getY(),getY(),250.0f == true))
            {
                enemyCount++;
                Tile* targetTile = m_Level->getTileForPlayer(enemy);
                float centerX = targetTile->getX() + (targetTile->getWidth() / 2.0f);
                float centerY = targetTile->getY() + (targetTile->getHeight() / 2.0f);
                
                if(enemyCount == 1)
                {
                    if(enemy->getIsActive() == true && getIsActive() == true)
                    {
                        Player::fireProjectile(centerX, centerY,"FireBall",500.0f,3);
                    }
                }
                else
                {
                    break;
                }
        
            }   
        }
    }
}
void MainBase::fireProjectile(float x, float y, const char* texture,float speed, int damage)
{
    Projectile* projectile = new Projectile(this, damage, speed, texture);
    projectile->setPosition(getX() + (getWidth() / 2.0f), getY() + getHeight() / 2.0f);
    projectile->setTarget(x, y);
    m_Projectiles.push_back(projectile);
}
