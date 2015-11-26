//
//  AmmoPickup.cpp
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-11-27.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "AmmoPickup.h"

AmmoPickup::AmmoPickup(int ammoCount) : Pickup(PickupTypeAmmo),
    m_AmmoCount(ammoCount)
{
    //TODO: the size should be that if the texture
    //you use fir the ammo ouck ( or any Pickup)
    setSize(24.0f,24.0f);
}

AmmoPickup::~AmmoPickup()
{
    
}
void AmmoPickup::paint()
{
    OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorYellow());
    OpenGLRenderer::getInstance()->drawRectangle(getX(), getY(), getWidth(), getHeight());
}

const char* AmmoPickup::getType()
{
    return AMMO_PICKUP_TYPE;
}

int AmmoPickup::getAmmoCount()
{
    return m_AmmoCount;
}




