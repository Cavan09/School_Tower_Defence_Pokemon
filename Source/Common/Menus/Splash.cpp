//
//  MainMenu.cpp
//  GAM-1514 OSX Game
//
//  Created by Bradley Flood on 2013-10-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Splash.h"
#include "LevelSlotMenu.h"
#include "../UI/UIButton.h"
#include "../Game/Game.h"
#include "../Game/Level.h"
#include "../Screen Manager/ScreenManager.h"


Splash::Splash() : Menu("Background", NULL)
{
    addButton(new UIButton("MainMenu"));
}

Splash::~Splash()
{

}

const char* Splash::getName()
{
    return SPLASH_SCREEN_NAME;
}

void Splash::buttonAction(UIButton* button)
{
    int index = getIndexForButton(button);
    if(index == 0)
    {
        ScreenManager::getInstance()->switchScreen(MAIN_MENU_SCREEN_NAME);
    }

}
