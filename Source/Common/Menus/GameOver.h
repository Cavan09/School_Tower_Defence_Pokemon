//
//  GameOver.h
//  GAM-1514 OSX Game
//
//  Created by Cavan Macphail on 2013-12-12.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GAM_1514_OSX_Game__GameOver__
#define __GAM_1514_OSX_Game__GameOver__

#include "Menu.h"

class GameOver : public Menu
{
public:
    GameOver();
    ~GameOver();
    
    const char* getName();
    
private:
    void buttonAction(UIButton* button);
};

#endif /* defined(__GAM_1514_OSX_Game__GameOver__) */
