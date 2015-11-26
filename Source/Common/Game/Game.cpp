//Student: Cavan MacPhail
// Creation Date: Oct 30,2013
// Course Number: GAM1514
// Professor: Brad Flood
// Purpose: Game Logic



#include "Game.h"
#include "GameObject.h"
#include "Level.h"
#include "TowerTypes/Charmander.h"
#include "../OpenGL/OpenGL.h"
#include "../Constants/Constants.h"
#include "../UI/UISideMenu.h"
#include "../Utils/Utils.h"
#include "../Screen Manager/ScreenManager.h"
#include "../Menus/LevelSlotMenu.h"
#include "../UI/UIFont.h"
#include <string>
#include <sstream>


Game::Game() :
    m_TowerMenu(NULL),
    m_GameOptionsMenu(NULL),
    m_SelectedTileIndex(-1),
    m_IsMouseDown(false),
    m_Font(NULL),
    m_FontCurrency(NULL),
    m_FontEnemy(NULL),
    m_FontEnemyCount(NULL)
{
    //Create the level object
    //Reset everything
    m_TowerMenu = new UISideMenu(this, SideMenuRight);
    m_TowerMenu->addButton(new UIToggle("MenuBulbasaur"));
    m_TowerMenu->addButton(new UIToggle("MenuSquirtle"));
    m_TowerMenu->addButton(new UIToggle("MenuCharmander"));
    
    m_GameOptionsMenu = new UISideMenu(this,SideMenuLeft);
    m_GameOptionsMenu->addButton(new UIButton("SpeedUp"));
    
	m_Level = new Level();
    
    m_Font = new UIFont("Fonties");
    m_FontCurrency = new UIFont("Fonties");

    m_FontEnemy = new UIFont("Fonties");
    m_FontEnemyCount = new UIFont("Fonties");
    
    m_SpeedUp = 1.0;
    
    
    
	reset();
}

Game::~Game()
{
    //Delete the level object
    if(m_Level != NULL)
    {
        delete m_Level;
        m_Level = NULL;
    }
    if(m_TowerMenu != NULL)
    {
        delete m_TowerMenu;
        m_TowerMenu = NULL;
    }
    if(m_GameOptionsMenu != NULL)
    {
        delete m_GameOptionsMenu;
        m_GameOptionsMenu = NULL;
    }
    if(m_Font != NULL)
    {
        delete m_Font;
        m_Font = NULL;
    }
    if(m_FontCurrency != NULL)
    {
        delete m_FontCurrency;
        m_FontCurrency = NULL;
    }
    if(m_FontEnemy != NULL)
    {
        delete m_FontEnemy;
        m_FontEnemy = NULL;
    }
    if(m_FontEnemyCount != NULL)
    {
        delete m_FontEnemyCount;
        m_FontEnemyCount = NULL;
    }


    
}

void Game::update(double delta)
{
    if(m_Level != NULL)
    {
        m_Level->update(delta * m_SpeedUp);
    }
    if(m_TowerMenu != NULL)
    {
        m_TowerMenu->update(delta);
    }
	if(m_GameOptionsMenu != NULL)
    {
        m_GameOptionsMenu->update(delta);
    }
    std::stringstream input;
    std::stringstream input2;
    std::string money = "";
    std::string enemy = "";
    input << m_Level->getCurrency();
    input >> money;
    input << "";
    m_Font->setText(money.c_str());
    
    m_FontCurrency->setText("Currency");
    
    m_FontEnemy->setText("Enemies");
    input2 << m_Level->getEnemies().size();
    input2 >> enemy;
    m_FontEnemyCount->setText(enemy.c_str());

}

void Game::paint()
{
    if(m_Level != NULL)
    {
        m_Level->paint();
    }
    if(m_TowerMenu != NULL)
    {
        m_TowerMenu->paint();
    }
	if(m_GameOptionsMenu != NULL)
    {
        m_GameOptionsMenu->paint();
    }
    
    m_Font->draw(512, 700);
    m_FontCurrency->draw(300,700);
    m_FontEnemy->draw(650, 700);
    m_FontEnemyCount->draw(900, 700);

}

void Game::reset()
{
    if(m_Level != NULL)
    {
        m_Level->reset();
    }
}

const char* Game::getName()
{
	return GAME_SCREEN_NAME;
}

Level* Game::getLevel()
{
    return m_Level;
}
void Game::mouseMovementEvent(float deltaX, float deltaY, float positionX, float positionY)
{
    if(m_TowerMenu != NULL)
    {
        m_TowerMenu->mouseMovementEvent(deltaX, deltaY, positionX, positionY);
    }
    if(m_GameOptionsMenu != NULL)
    {
        m_GameOptionsMenu->mouseMovementEvent(deltaX, deltaY, positionX, positionY);
    }
    
    if(m_Level != NULL)
    {
        if(m_SelectedTileIndex != -1 && m_IsMouseDown == true)
        {
        }
    }
    if(m_Level != NULL)
    {
        m_Level->mouseMovementEvent(deltaX, deltaY, positionX, positionY);
    }
}

void Game::mouseLeftClickDownEvent(float positionX, float positionY)
{
    m_IsMouseDown = true;
}
void Game::mouseLeftClickUpEvent(float positionX, float positionY)
{
    //Set the mouse up flag
    m_IsMouseDown = false;
    
    //Notify the tiles menu of the mouse event
    if(m_TowerMenu != NULL)
    {
        m_TowerMenu->mouseLeftClickUpEvent(positionX, positionY);
    }
    if(m_GameOptionsMenu != NULL)
    {
        m_GameOptionsMenu->mouseLeftClickUpEvent(positionX, positionY);
        m_GameOptionsMenu->hide();
        m_TowerMenu->hide();
    }
    
    if(m_Level != NULL)
    {
        m_Level->mouseLeftClickUpEvent(positionX, positionY);
    }
}
void Game::mouseRightClickUpEvent(float positionX, float positionY)
{
    if(m_Level != NULL)
    {
        m_Level->mouseRightClickUpEvent(positionX, positionY);
    }
}

void Game::keyUpEvent(int keyCode)
{
    if(m_Level != NULL)
    {
        m_Level->keyUpEvent(keyCode);
    }
    if(keyCode == KEYCODE_TAB)
    {
        if(m_GameOptionsMenu != NULL)
        {
            m_GameOptionsMenu->isShowing() == true ? m_GameOptionsMenu->hide() : m_GameOptionsMenu->show();
        }
		if(m_TowerMenu != NULL)
		{
			m_TowerMenu->isShowing() == true ? m_TowerMenu->hide() : m_TowerMenu->show();
		}
    }
}
void Game::setLevel(Level* level)
{
	m_Level = level;
}

void Game::sideMenuButtonAction(UISideMenu* sideMenu, UIButton* button, int buttonIndex)
{
    if(sideMenu == m_GameOptionsMenu)
    {
        if(m_GameOptionsMenu->getIndexForButton(button) == 0)
        {
            if(m_SpeedUp != 2.0)
            {
                m_SpeedUp = 2.0;
            }
            else
            {
                m_SpeedUp = 1.0;
            }
        }
        
    }
}
void Game::sideMenuToggleAction(UISideMenu* sideMenu, UIToggle* toggle, int toggleIndex)
{
    if( sideMenu == m_TowerMenu)
    {
        UIToggle* previousToggle = (UIToggle*)m_TowerMenu->getButtonForIndex(m_SelectedTileIndex);
        if(previousToggle != NULL)
        {
            previousToggle->setIsToggled(false);
        }
        
        //Set the selected tile index
        m_SelectedTileIndex = toggle->isToggled() == true ? toggleIndex : -1;
        
        //Hide the options and tiles menus
        m_TowerMenu->hide();
		m_TowerMenu->hide();
    }
}
int Game::getSelectedToggle()
{
    return m_SelectedTileIndex;
}
