//Student: Cavan MacPhail
// Creation Date: Oct 30,2013
// Course Number: GAM1514
// Professor: Brad Flood
// Purpose: Player Logic

#include "Player.h"
#include "Level.h"
#include "PathNode.h"
#include "Tiles/Tile.h"
#include "../Screen Manager/ScreenManager.h"
#include "PickUps/AmmoPickUp.h"
#include "../UI/UIFont.h"
#include "../OpenGL/OpenGL.h"
#include "../Constants/Constants.h"
#include "../Utils/Utils.h"
#include "Projectile.h"
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <sstream>
#include <string>

Player::Player(Level* aLevel,const char* textureFile) :
	m_Texture(NULL)
{
    m_Level = aLevel;

	if(textureFile != NULL)
	{
		m_Texture = OpenGLTextureCache::getInstance()->getTexture(textureFile);
	}
	m_FireRate = 3.0;
    //create the PathFinder Object
    m_PathFinder = new PathFinder(aLevel , this);
    
    //Initialize the current and destination tiles to NULL
    m_CurrentTile = NULL;
    m_DestinationTile = NULL;
    
    //Initialize the animation member variables
    m_CanAnimate = false;
    m_AbortAnimation = false;
    m_AnimationPathNodeIndex = -1;
    
    //set the player speed
    m_Speed = PLAYER_SPEED;
    
    //Initilise the ammo amount and the health
    m_Ammo = 100;
    m_Health = 3;
    
    //Initialize the player's size
    setSize(PLAYER_SIZE, PLAYER_SIZE);
}

Player::~Player()
{
    
    
    if(m_PathFinder != NULL)
    {
        delete m_PathFinder;
        m_PathFinder = NULL;
    }
    for( int i = 0; i < m_Projectiles.size(); i++)
    {
        delete m_Projectiles.at(i);
        m_Projectiles.at(i) = NULL;
    }
    if(m_Texture != NULL)
    {
        delete m_Texture;
        m_Texture = NULL;
    }
    
    //Set the current and desination tiles to NULL
	m_CurrentTile = NULL;
	m_DestinationTile = NULL;
}

void Player::update(double aDelta)
{

    
    //update the projtiles
    for ( int i =0; i < m_Projectiles.size(); i++)
    {
        if(m_Projectiles.at(i)->getIsActive() == true)
        {
            m_Projectiles.at(i)->update(aDelta);
        }
    }
    //remove any 'inactive' Projectiles
    int index = 0;
    while(index != m_Projectiles.size())
    {
        if(m_Projectiles.at(index)->getIsActive() == false)
        {
            //Log::debug("Deleting projectiles");
            
            //Delete the Projectile and remove it from the vector
            delete m_Projectiles.at(index);
            m_Projectiles.erase(m_Projectiles.begin() + index);
        }
        else
        {
            index++;
        }
    }
    
    
    //update the pathfinder
    if(m_PathFinder->isSearchingPath() == true)
    {
        m_PathFinder->update(aDelta);
    }
    
    if(isAnimating() == true && m_AnimationPathNodeIndex > -1)
    {
        PathNode* pathNode = m_PathFinder->getPathNodeAtIndex(m_AnimationPathNodeIndex);
        Tile* tile = pathNode != NULL ? pathNode->getTile() : NULL;
        
        if(tile != NULL)
        {
            float centerX = tile->getX() + (tile->getWidth() - getWidth()) / 2.0f;
            float centerY = tile->getY() + (tile->getHeight() - getHeight()) / 2.0f;
            
            //get the tile speed for the tile the player is on
            Tile* playerTile = m_Level->getTileForPosition(getX(), getY());
            float speed = playerTile->getTileSpeed();
            
            float playerX = animate(getX(), centerX, aDelta, speed);
            float playerY = animate(getY(), centerY, aDelta, speed);
            setPosition(playerX, playerY);
            
            //has the player reached the center of this tile
            if(playerX == centerX && playerY == centerY)
            {
                m_AnimationPathNodeIndex++;
                
                m_CurrentTile->setIsPath((false));
                
                setCurrentTile(tile);
                
                //does the tile have a pickup
                if(tile->getPickup() != NULL)
                {
                    handlePickup(tile->getPickup());
                    
                    //set the tiles pickup to NULL since it was picked up
                    
                    tile->setPickup(NULL);
                }
                
                if(m_AnimationPathNodeIndex >= m_PathFinder->getPathSize())
                {
                    stopAnimating();
                    m_CurrentTile->setIsPath(false);
                }
                if(m_AbortAnimation == true)
                {
                    m_AbortAnimation =false;
                    findPath();
                }
            }
        }
        else
        {
            if(m_AbortAnimation == true)
            {
                m_AbortAnimation =false;
                findPath();
            }
            
        }
    }
}

void Player::paint()
{
    //cycle throught and paint all the 'active' projectiles
    for ( int i = 0; i < m_Projectiles.size(); i++)
    {
        if(m_Projectiles.at(i)->getIsActive() == true)
        {
            m_Projectiles.at(i)->paint();
        }
    }
     if(m_Texture != NULL)
    {
		OpenGLRenderer::getInstance()->drawTexture(m_Texture, getX(), getY(), getWidth(), getHeight());
    }
    
}

void Player::reset()
{
    //Stop animating the player
    stopAnimating();
    
    //reset the path finder
    m_PathFinder->reset();
    
    //set the destination tile to NULL
    m_DestinationTile = NULL;
    
    //set the player's state to active
    setIsActive(true);
    
    //reset players health back to default
    m_Health = 3;
}

void Player::fireProjectile(float x, float y, const char* texture,float speed, int damage)
{
    //saftey check that there is asome ammo l;eft

        
        //create a new projectile object
        Projectile* projectile = new Projectile(this, damage, speed, texture);
        projectile->setPosition(getX() + (getWidth() / 2.0f), getY() + getHeight() / 2.0f);
        projectile->setTarget(x, y);
        m_Projectiles.push_back(projectile);
        
        //Log::debug("Fired projectile, %i ammo left" , m_Ammo);

}
void Player::applyDamage(int damage)
{
    m_Health -= damage;
    if(m_Health <= 0)
    {
        m_Health = 0;
        setIsActive(false);
        m_Level->addCurency();
        
        //Log::debug("Player is dead");
    }
    else
    {
        //Log::debug("Player is hit %i health remaning", m_Health);
    }
}


void Player::setCurrentTile(Tile* tile)
{
	//Set the current tile pointer
	m_CurrentTile = tile;
    
	//Center the player's position on the tile
	setPosition(tile->getX() + ((tile->getWidth() - getWidth()) / 2), tile->getY() + ((tile->getHeight() - getHeight()) / 2));
}

void Player::setDestinationTile(Tile* tile)
{
	//Set the destination tile pointer
	m_DestinationTile = tile;
    
    //start pathFinding
    if(isAnimating() == false)
    {
        findPath();
    }
    else
    {
        m_AbortAnimation = true;
    }
}

void Player::handlePickup(Pickup* pickup)
{
    switch (pickup->getPickupType())
    {
        case PickupTypeAmmo:
            m_Ammo += ((AmmoPickup*)pickup)->getAmmoCount();
            break;
            
        default:
            break;
    }
}

void Player::handleBoundsCollision(Projectile* projectile)
{
    Tile* tile = m_Level->getTileForPosition(projectile->getX(), projectile->getY());
    if(tile == NULL)
    {
        //if the tile object is NULL, it means thje projecxtile is now no longert in the level
        projectile->setIsActive(false);
        
        //Log::debug("Projectile went off the screen");
    }
}

void Player::pathFinderFinishedSearching(PathFinder* pathFinder, bool pathWasFound)
{
    if(pathFinder == m_PathFinder)
    {
        if(pathWasFound == true)
        {
            startAnimating();
        }
        else
        {
            m_PathFinder->reset();
        }
    }
    
}

PathFinder* Player::getPathFinder()
{
    return m_PathFinder;
}
void Player::findPath()
{
    m_PathFinder->reset();
    m_PathFinder->findPath(m_CurrentTile, m_DestinationTile);
}

float Player::animate(float aCurrent, float aTarget, double aDelta, float speed)
{
    float increment = aDelta * m_Speed * (aTarget < aCurrent ? -1 : 1) * speed;
    if(fabs(increment) > fabs(aTarget - aCurrent))
    {
        return aTarget;
    }
    else
    {
        aCurrent += increment;
    }
    return aCurrent;
}

void Player::startAnimating()
{
	m_CanAnimate = true;
	m_AnimationPathNodeIndex = 0;
}

void Player::stopAnimating()
{
	m_CanAnimate = false;
	m_AnimationPathNodeIndex = -1;
}

bool Player::isAnimating()
{
    return m_CanAnimate;
}
void Player::spawnPoints()
{
    OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorBlue());
	OpenGLRenderer::getInstance()->drawCircle(getX() + (getWidth() / 2), getY() + (getHeight() / 2), getWidth() / 2, false);
}
