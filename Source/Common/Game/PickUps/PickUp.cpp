//
//  PickUp.cpp
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-11-27.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "PickUp.h"

Pickup::Pickup(PickupType pickupType) :
    m_PickupType(pickupType)
{
    
}

Pickup::~Pickup()
{
    
}

void Pickup::update(double detla)
{
    
}

void Pickup::reset()
{
    
}

PickupType Pickup::getPickupType()
{
    return m_PickupType;
}