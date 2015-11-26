//
//  Projectile.cpp
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-11-28.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Projectile.h"
#include "../Utils/Utils.h"
#include <math.h>
#include "../OpenGL/OpenGL.h"

Projectile::Projectile(ProjectileListener* listener, int damage, float velocity, const char* texture) : GameObject(),
    m_Listener(listener),
    m_Damage(damage),
    m_Velocity(velocity),
    m_Angle(0.0f),
    m_TargetX(0.0f),
    m_TargetY(0.0f)
{
    if(texture != NULL)
	{
		m_Texture = OpenGLTextureCache::getInstance()->getTexture(texture);
	}
}

Projectile::~Projectile()
{
    
}

void Projectile::update(double delta)
{
    float x = getX() + m_Velocity * cosf(m_Angle) * delta;
    float y = getY() + m_Velocity * sinf(m_Angle) * delta;
    setPosition(x, y);
    
    //handle the player and bounds collsion
    if(m_Listener != NULL)
    {
        m_Listener->handlePlayerCollision(this);
        m_Listener->handleBoundsCollision(this);
    }
}

void Projectile::paint()
{
    if(m_Texture != NULL)
    {
		OpenGLRenderer::getInstance()->drawTexture(m_Texture, getX(), getY());
    }
}

void Projectile::reset()
{
    
}

const char* Projectile::getType()
{
    //TODO: Make a constant for this
    return "Projectile";
}

void Projectile::setTarget(float x, float y)
{
    //Set the target
    m_TargetX = x;
    m_TargetY = y;
    
    //calculate the angle
    float deltaX = m_TargetX - getX();
    float deltaY = m_TargetY - getY();
    float anngleInDegrees = atan2f(deltaY, deltaX) * 180 / M_PI;
    m_Angle = MathUtils::degressToRadians(anngleInDegrees);
}

int Projectile::getDamage()
{
    return m_Damage;
}