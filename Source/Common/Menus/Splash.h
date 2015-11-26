//
//  MainMenu.h
//  GAM-1514 OSX Game
//
//  Created by Bradley Flood on 2013-10-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GAM_1514_OSX_Game__Splash__
#define __GAM_1514_OSX_Game__Splash__

#include "Menu.h"


class Splash : public Menu
{
public:
    Splash();
    ~Splash();
    
    const char* getName();

private:
    void buttonAction(UIButton* button);
};

#endif /* defined(__GAM_1514_OSX_Game__Splash__) */
