#ifndef GAME_H
#define GAME_H

#include <vector>
#include "../Screen Manager/Screen.h"
#include "../UI/UISideMenu.h"

class GameObject;
class Level;
class UISideMenu;
class UIFont;

class Game : public Screen, public UISideMenuListener
{
public:
  Game();
  ~Game();

  //Game lifecycle methods
  void update(double delta);
  void paint();
  void reset();
  void setLevel(Level* level);
  //Screen name, must be implemented, it's a pure
  //virtual method in the Screen class
  const char* getName();
  
  Level* getLevel();
    int getSelectedToggle();

private:
  //Mouse Events
    void mouseMovementEvent(float deltaX, float deltaY, float positionX, float positionY);
    void mouseLeftClickDownEvent(float positionX, float positionY);
    void mouseLeftClickUpEvent(float positionX, float positionY);
    void mouseRightClickUpEvent(float positionX, float positionY);
    void keyUpEvent(int keyCode);
    void sideMenuButtonAction(UISideMenu* sideMenu, UIButton* button, int buttonIndex);
    void sideMenuToggleAction(UISideMenu* sideMenu, UIToggle* toggle, int toggleIndex);
    void nextLevel();

  //Level pointer
  Level* m_Level;
    UIFont* m_FontCurrency;
    UIFont* m_Font;
    UIFont* m_FontEnemy;
    UIFont* m_FontEnemyCount;
    UISideMenu* m_TowerMenu;
    UISideMenu* m_GameOptionsMenu;
    int m_SelectedTileIndex;
    bool m_IsMouseDown;
    int m_SelectedToggle;
    double m_SpeedUp;
};

#endif