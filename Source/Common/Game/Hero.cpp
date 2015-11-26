//
//  Hero.cpp
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-11-21.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Hero.h"
#include "Level.h"
#include "Tiles/Tile.h"
#include "../Constants/Constants.h"
#include "../Input/Input.h"
#include "../Utils/Utils.h"
#include "Enemy.h"


Hero::Hero(Level* level, float speed, const char* textureFile) : Player(level, textureFile)
{
    m_Speed = speed;
}

Hero::~Hero()
{
    
}
const char* Hero::getType()
{
    return HERO_TYPE;
}
void Hero::mouseLeftClickUpEvent(float positionX, float positionY)
{
    Tile* tile = m_Level->getTileForPosition(positionX, positionY);
    if(tile != NULL && tile->isWalkableTile() == true)
    {
        setDestinationTile(tile);
    }
}

void Hero::mouseMovementEvent(float deltaX, float deltaY, float positionX, float positionY)
{
    Tile* tile = m_Level->getTileForPosition(positionX, positionY);
    if(tile != NULL && tile->isWalkableTile() == true)
    {
        //set the level's selected tile, thius call is optional
        m_Level->setSelectedTileIndex(m_Level->getTileIndexForTile(tile));
    }
}

void Hero::keyUpEvent(int keyCode)
{
    }

void Hero::handlePlayerCollision(Projectile* projectile)
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