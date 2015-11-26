//
//  GameOver.cpp
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-12-12.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "GameOver.h"
#include "LevelSlotMenu.h"
#include "../UI/UIButton.h"
#include "../Game/Game.h"
#include "../Game/Level.h"
#include "../Screen Manager/ScreenManager.h"


GameOver::GameOver() : Menu("Background", NULL)
{
    addButton(new UIButton("MainMenu"));
}

GameOver::~GameOver()
{
    
}

const char* GameOver::getName()
{
    return SPLASH_SCREEN_NAME;
}

void GameOver::buttonAction(UIButton* button)
{
    int index = getIndexForButton(button);
    if(index == 0)
    {
        ScreenManager::getInstance()->switchScreen(MAIN_MENU_SCREEN_NAME);
    }
    
}