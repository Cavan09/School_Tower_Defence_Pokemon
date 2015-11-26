//
//  Player.h
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-03-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "PathFinder.h"
#include "Projectile.h"
#include <vector>

class Level;
class Tile;
class Pickup;
class UIFont;
class OpenGLTexture;

class Player : public GameObject, public PathFinderListener, public ProjectileListener
{
public:
	Player(Level* level,const char* textureFile);
	virtual ~Player();

  //Update, paint and reset methods
	void update(double delta);
	void paint();
  void reset();
  
  //Implementing GameObject's pure virtual method
  virtual const char* getType() = 0;
  
  //Setter methods for the current and destination tiles
	void setCurrentTile(Tile* tile);
	void setDestinationTile(Tile* tile);
    
    void spawnPoints();
    
    //Projectile Methods
    virtual void fireProjectile(float x, float y, const char* texture,float speed, int damage);
    void applyDamage(int damage);

protected:
    //projectyile listerner methods
    virtual void handlePlayerCollision(Projectile* projectile) = 0;
    virtual void handleBoundsCollision(Projectile* projectile);
    
    //pickup method
    void handlePickup(Pickup* pickup);
    
    // PathFinder listerner method
    void pathFinderFinishedSearching(PathFinder* pathFinder, bool pathWasFound);
    //PathFinder methods
    PathFinder* getPathFinder();
    void findPath();
    
  //Animation methods
    float animate(float current, float target, double delta, float speed);
	void startAnimating();
	void stopAnimating();
    bool isAnimating();
    
    //friend class level so that is can access the protected meber
    friend class Level;
    
	OpenGLTexture* m_Texture;
    Level* m_Level;
    PathFinder* m_PathFinder;
	Tile* m_CurrentTile;
	Tile* m_DestinationTile;
	bool m_CanAnimate;
    bool m_AbortAnimation;
	int m_AnimationPathNodeIndex;
    int m_Speed;
    int m_Ammo;
    double m_FireRate;
    int m_Health;
    std::vector<Projectile*> m_Projectiles;
};

#endif