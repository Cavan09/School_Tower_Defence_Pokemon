//
//  Enemy.cpp
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-11-21.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Enemy.h"
#include "Level.h"
#include "Tiles/Tile.h"
#include "Hero.h"
#include "TowerTypes/MainBase.h"
#include "../Utils/Math/MathUtils.h"
#include "../Utils/Utils.h"
#include "../Constants/Constants.h"
#include "TowerTypes/Charmander.h"
#include "TowerTypes/Squirtle.h"
#include "TowerTypes/Bulbasaur.h"

Enemy::Enemy(Level* level, float speed, const char* textureFile) : Player(level, textureFile)
{
    m_Speed = speed;
	m_DeathCount = m_Health;
}

Enemy::~Enemy()
{
    
}

const char* Enemy::getType()
{
    return ENEMY_TYPE;
}

void Enemy::update(double delta)
{
    Player::update(delta);
    //Set the destanation tile bease on the players
    setDestinationTile(m_Level->getTileForPlayer(m_Level->getMainBase()));
    
    if(m_FireRate > 0.0)
    {
        m_FireRate -= delta;
        if(m_FireRate <= 0.0)
        {
            m_FireRate = 1.0;
            fireAtPlayer();
            
        }
    }
	
}

void Enemy::paint()
{
    Player::paint();   
}
void Enemy::reset()
{
    Player::reset();

    setDestinationTile(m_Level->getTileForPlayer(m_Level->getMainBase()));
	fireAtPlayer();
	Player::setIsActive(true);
	m_DeathCount++;
}

void Enemy::handlePlayerCollision(Projectile* projectile)
{
     Tile* tile = m_Level->getTileForPosition(projectile->getX(), projectile->getY());
     MainBase* mainBase = m_Level->getMainBase();
        if(mainBase != NULL && mainBase->getIsActive() == true)
        {
            //get the tile the enemy is on
            Tile* heroTile = m_Level->getTileForPlayer(mainBase);
            
            //is the projectile on the same tile as the emnemy?
            if(tile == heroTile)
            {
               // Log::debug("Hero projectile hit an enemy");
                
                // apply damage tot he enemt and set the projectile
                mainBase->applyDamage(projectile->getDamage());
                projectile->setIsActive(false);
            }
        }
    for(int i = 0; i < m_Level->getCharmander().size(); i ++)
    {
        Charmander* charmander = m_Level->getCharmander().at(i);
        if(charmander != NULL && charmander->getIsActive() == true)
        {
            //get the tile the enemy is on
            Tile* enemyTile = m_Level->getTileForPlayer(charmander);
            
            //is the projectile on the same tile as the emnemy?
            if(tile == enemyTile)
            {
                // Log::debug("Hero projectile hit an enemy");
                
                // apply damage tot he enemt and set the projectile
                charmander->applyDamage(projectile->getDamage());
                projectile->setIsActive(false);
            }
        }
    }
    for(int i = 0; i < m_Level->getSquirtle().size(); i ++)
    {
        Squirtle* squirtle = m_Level->getSquirtle().at(i);
        if(squirtle != NULL && squirtle->getIsActive() == true)
        {
            //get the tile the enemy is on
            Tile* enemyTile = m_Level->getTileForPlayer(squirtle);
            
            //is the projectile on the same tile as the emnemy?
            if(tile == enemyTile)
            {
                // Log::debug("Hero projectile hit an enemy");
                
                // apply damage tot he enemt and set the projectile
                squirtle->applyDamage(projectile->getDamage());
                projectile->setIsActive(false);
            }
        }
    }
    for(int i = 0; i < m_Level->getBulbasaur().size(); i ++)
    {
        Bulbasaur* bulbasaur = m_Level->getBulbasaur().at(i);
        if(bulbasaur != NULL && bulbasaur->getIsActive() == true)
        {
            //get the tile the enemy is on
            Tile* enemyTile = m_Level->getTileForPlayer(bulbasaur);
            
            //is the projectile on the same tile as the emnemy?
            if(tile == enemyTile)
            {
                // Log::debug("Hero projectile hit an enemy");
                
                // apply damage tot he enemt and set the projectile
                bulbasaur->applyDamage(projectile->getDamage());
                projectile->setIsActive(false);
            }
        }
    }



}
void Enemy::fireAtPlayer()
{
	
	MainBase* mainBase = m_Level->getMainBase();

    //Squirtle* squirtle = m_Level->getSquirtle();
    //Bulbasaur* bulbasaur = m_Level->getBulbasaur();

		if(MathUtils::withinRange(mainBase->getX(),getX(),250.0f) == true || MathUtils::withinRange(mainBase->getY(),getY(),250.0f == true))
		{
            Tile* targetTile = m_Level->getTileForPlayer(m_Level->getMainBase());
			float centerX = targetTile->getX() + (targetTile->getWidth() / 2.0f);
			float centerY = targetTile->getY() + (targetTile->getHeight() / 2.0f);
        
            if(mainBase->getIsActive() == true && getIsActive() == true)
            {
                m_Speed = 0;
                fireProjectile(centerX, centerY,"Zubat",500.0f,1);
            }
            

		}
    int enemyCount = 0;
	for( int i = 0; i < m_Level->getCharmander().size(); i++)
    {
        Charmander* charmander = m_Level->getCharmander().at(i);
        
        if(charmander != NULL && charmander->getIsActive() == true)
        {
            if(MathUtils::withinRange(charmander->getX(),getX(),250.0f) == true || MathUtils::withinRange(charmander->getY(),getY(),250.0f == true))
            {
                enemyCount++;
                Tile* targetTile = m_Level->getTileForPlayer(charmander);
                float centerX = targetTile->getX() + (targetTile->getWidth() / 2.0f);
                float centerY = targetTile->getY() + (targetTile->getHeight() / 2.0f);
                
                if(enemyCount == 1)
                {
                    if(charmander->getIsActive() == true && getIsActive() == true)
                    {
                        fireProjectile(centerX, centerY,"Zubat",500.0f,1);
                    }
                }
                else
                {
                    break;
                }
                
            }
        }
    }
    
    for( int i = 0; i < m_Level->getSquirtle().size(); i++)
    {
        Squirtle* squirtle = m_Level->getSquirtle().at(i);
        
        if(squirtle != NULL && squirtle->getIsActive() == true)
        {
            if(MathUtils::withinRange(squirtle->getX(),getX(),250.0f) == true || MathUtils::withinRange(squirtle->getY(),getY(),250.0f == true))
            {
                enemyCount++;
                Tile* targetTile = m_Level->getTileForPlayer(squirtle);
                float centerX = targetTile->getX() + (targetTile->getWidth() / 2.0f);
                float centerY = targetTile->getY() + (targetTile->getHeight() / 2.0f);
                
                if(enemyCount == 1)
                {
                    if(squirtle->getIsActive() == true && getIsActive() == true)
                    {
                        fireProjectile(centerX, centerY,"Zubat",500.0f,1);
                    }
                }
                else
                {
                    break;
                }
                
            }
        }
    }
    
    for( int i = 0; i < m_Level->getBulbasaur().size(); i++)
    {
        Bulbasaur* bulbasaur = m_Level->getBulbasaur().at(i);
        
        if(bulbasaur != NULL && bulbasaur->getIsActive() == true)
        {
            if(MathUtils::withinRange(bulbasaur->getX(),getX(),250.0f) == true || MathUtils::withinRange(bulbasaur->getY(),getY(),250.0f == true))
            {
                enemyCount++;
                Tile* targetTile = m_Level->getTileForPlayer(bulbasaur);
                float centerX = targetTile->getX() + (targetTile->getWidth() / 2.0f);
                float centerY = targetTile->getY() + (targetTile->getHeight() / 2.0f);
                
                if(enemyCount == 1)
                {
                    if(bulbasaur->getIsActive() == true && getIsActive() == true)
                    {
                        fireProjectile(centerX, centerY,"Zubat",500.0f,1);
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
void Enemy::fireProjectile(float x, float y, const char* texture,float speed, int damage)
{
    Projectile* projectile = new Projectile(this, damage, speed, texture);
    projectile->setPosition(getX() + (getWidth() / 2.0f), getY() + getHeight() / 2.0f);
    projectile->setTarget(x, y);
    m_Projectiles.push_back(projectile);
}